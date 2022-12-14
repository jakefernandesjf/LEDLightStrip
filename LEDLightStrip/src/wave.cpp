/*
    File:           wave.cpp
    Author:         Jake Fernandes
    Description:    Wave animation for FastLED
    
    Change Log:
    22 OCT 2022     Created.
    23 OCT 2022     Created first implementation of wave animation.
    23 OCT 2022     Bug fixes and added Serial Debug logging.
    26 OCT 2022     Created new wave function skeleton definition. Made file into .cpp and moved declerations to animations.h.
    26 OCT 2022     Created initial test of custom wave equation. Needs DrawFractionalPixels() implemented.
    30 OCT 2022     Implemented wave function.
    01 NOV 2022     Implemented wave_palette().
    09 NOV 2022     Fixed Flow/Ebb lineup for wave_palette().
    09 NOV 2022     Implemented wave_palette() reset animation.
*/
#include <animations.h>

void wave_palette( struct CRGB * pFirstLED, int numToFill,
                CRGBPalette16 waterColorPalette,
                CRGBPalette16 sandColorPalette,
                double flowLength,
                double ebbLength,
                double waveSpeed)
{
    const int DELAY = 10;                                                           // Delay for each iteration
    const CRGB WAVE_COLOR = CRGB::White;                                            // Color of wave
    const int WAVE_MULTIPLIER = 15;                                                 // Multiplier for wave size
    static double waveSize = 1;                                                     // Wave size in the animation
    double wavePosition = 0.0;                                                      // Current wave position on LED strip (fractional positions allowed)
    static double lastWavePosition = 0.0;                                           // Last iteration wave position on LED strip
    static double animationDirection = 1.0;                                         // Current direction of animation (-1.0 for reset sequence)
    static double initialTime = millis();                                           // Initial start time of function
    double time;                                                                    // Current time for animation


    #pragma region Wave Position calculation
    time = (millis() - initialTime) / 1000.0;
    time *= animationDirection;

    double waveSqrt = flowLength * sqrt(time);
    double waveSin = ebbLength * sin(time / waveSpeed);
    wavePosition = max( 0.0, min(waveSin + waveSqrt, (double)numToFill));           // 0 <= wavePosition <= numToFill
    int wavePixel = (int)floor(wavePosition);
    int paletteIndex = wavePixel - floor(2 * waveSin + waveSqrt);


    // Determine if we need to change animation direction
    bool forwardDirectionEndBool = (animationDirection == 1.0) && (wavePosition == (double)numToFill);
    bool backwardDirectionEndBool = (animationDirection == -1.0) && (wavePosition == 0.0);
    if(forwardDirectionEndBool || backwardDirectionEndBool)
    {
        animationDirection *= -1.0;
        initialTime = millis();
    }
    if(forwardDirectionEndBool)
    {
        initialTime *= 2;
    }
    #pragma endregion


    #pragma region Draw LEDs
    FastLED.clear();


    // Draw Water
    fill_palette(pFirstLED, wavePixel, paletteIndex, 255 / (numToFill * 2), waterColorPalette, 255, TBlendType::LINEARBLEND);

    // Draw sand
    uint8_t currentIndex = paletteIndex;
    for (int j = wavePixel; j <= numToFill; j++)
    {
        pFirstLED[j] = ColorFromPalette( sandColorPalette, currentIndex, 255, TBlendType::LINEARBLEND);
        currentIndex += 255 / (numToFill * 2);
    }

    // Draw Wave
    EVERY_N_MILLISECONDS(100)
    {
        waveSize = max( WAVE_MULTIPLIER * abs(wavePosition - lastWavePosition), 1.0);
    }
    DrawFractionalPixels(pFirstLED, numToFill, wavePosition, waveSize, WAVE_COLOR);


    lastWavePosition = wavePosition;
    FastLED.delay(DELAY);
    #pragma endregion
}


void wave( struct CRGB * pFirstLED, int numToFill,
                CRGB waterColor,
                CRGB sandColor,
                double flowLength,
                double ebbLength,
                double waveSpeed)
{
    double wavePosition;                                                            // Current wave position on LED strip (fractional positions allowed)
    static float lastWavePosition = 0;                                              // Last iteration wave position on LED strip
    static double initialTime = millis();                                           // Initial start time of function
    const CRGB waveColor = CRGB::White;                                             // Color of wave
    const int delay = 10;                                                           // Delay for each iteration
    const float waveSize = (float)numToFill / 15;                                   // Wave Size dependent on LED strip length


    double time = (millis() - initialTime) / 1000.0;
    wavePosition = ebbLength * sin(time / waveSpeed) + flowLength * sqrt(time);     // Calculate wave position


    FastLED.clear();
    DrawFractionalPixels(pFirstLED, numToFill, wavePosition, waveSize, waveColor);
    int wavePixel = min((int)floor(wavePosition), numToFill);
    // Draw Water
    for(int i = 0; i < wavePixel; i++)
    {
        pFirstLED[i] += waterColor;
    }
    // Draw sand
    for (int j = wavePixel; j <= numToFill; j++)
    {
        pFirstLED[j] += sandColor;
    }
    FastLED.delay(delay);
}


void wave_v1( struct CRGB * pFirstLED, int numToFill,
                  uint8_t flowHue,
                  uint8_t ebbHue,
                  int flowLength,
                  int ebbLengthFactor)
{
    int ebbLength = flowLength / ebbLengthFactor;

    const int FADE_FLOW_FACTOR = 255 / flowLength;
    const int FADE_EBB_FACTOR = 255 / ebbLength;
    const int FLOW_DIRECTION = 1;
    const int EBB_DIRECTION = -1;
    
    static int iPos = 0;                        // Current wave position on LED strip
    static int iDirection = FLOW_DIRECTION;     // Current direction of wave (FLOW and EBB)
    static int waveStartPos = 0;                // Current wave start position

    // Serial Debug Logging
    Serial.println("iDirection = " + (String)iDirection);
    Serial.println("iPos = " + (String)iPos);
    Serial.println("waveStartPos = " + (String)waveStartPos);
    Serial.println("");

    // Set strip to ebb hue if we are at the start of the animation
    if (iPos == 0)
    {
        for (int i = 0; i < numToFill; i++)
        {
            pFirstLED[i].setHue(ebbHue);
        }
    }

    // Draw the wave
    switch (iDirection)
    {
        case (FLOW_DIRECTION):
            // Set first LED in flow white
            pFirstLED[iPos].setHSV(flowHue, 0, 255);
            // Set start of strip to iPos to flowHue
            for (int i = 0; i < iPos; i++)
            {
                pFirstLED[i].setHue(flowHue);
            }
            // Set flow randomly decreasingly saturated
            for (int i = waveStartPos; i < iPos; i++)
            {
                int iSaturation = (iPos - i) * FADE_FLOW_FACTOR;
                if(random(10)>3)
                {
                    pFirstLED[i].setHSV(flowHue, iSaturation, 255);
                }
            }
            // Switch to EBB_DIRECTION if we get to the end of the flow
            if (iPos == waveStartPos + flowLength)
            {
                iDirection = EBB_DIRECTION;
                waveStartPos = iPos;
            }
            break;

        case (EBB_DIRECTION):
            // Set first LED in ebb white
            pFirstLED[iPos].setHSV(ebbHue, 0, 255);
            // Set iPos to end of strip to ebbHue
            for (int i = numToFill - 1; i > iPos; i--)
            {
                pFirstLED[i].setHue(ebbHue);
            }
            // Set ebb randomly decreasingly saturated
            for (int i = iPos; i < waveStartPos; i++)
            {
                int iSaturation = (iPos - i) * FADE_EBB_FACTOR;
                if(random(10)>3)
                {
                    pFirstLED[i].setHSV(ebbHue, iSaturation, 255);
                }
            }
            // Switch to FLOW_DIRECTION if we get to the end of the ebb
            if (iPos == waveStartPos - ebbLength)
            {
                iDirection = FLOW_DIRECTION;
                waveStartPos = iPos;
            }
            break;
    }

    // If wave is at the end of the strip, run ebb to the start of the strip to reset
    if (iPos == numToFill)
    {
        iDirection = EBB_DIRECTION;
        waveStartPos = ebbLength;
    }
    
    // Increment iPos in iDirection
    iPos += iDirection;
}