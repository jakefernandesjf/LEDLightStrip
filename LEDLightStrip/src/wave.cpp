/*
    File:           wave.cpp
    Author:         Jake Fernandes
    Description:    Wave animation for FastLED
    
    Change Log:
    22 OCT 2022     Created.
    23 OCT 2022     Created first implementation of wave animation.
    23 OCT 2022     Bug fixes and added Serial Debug logging.
    26 OCT 2022     Created new wave function skeleton definition. Made file into .cpp and moved declerations to animations.h.
*/
#include <animations.h>


void wave( struct CRGB * pFirstLED, int numToFill,
                uint8_t flowHue,
                uint8_t ebbHue,
                double flowLength,
                double ebbLength,
                double waveSpeed)
{
    // TODO
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