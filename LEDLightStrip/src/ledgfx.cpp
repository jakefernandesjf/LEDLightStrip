/*
    File:           ledgfx.cpp
    Author:         Jake Fernandes
    Description:    LED graphic helper functions for user-created FastLED animations.
    
    Change Log:
    26 OCT 2022     Created skeleton definitions.
    30 OCT 2022     Implemented ColorFraction() and DrawFractionalPixels().
*/
#include <ledgfx.h>
using namespace std;


CRGB ColorFraction(CRGB color, float fraction)
{
    fraction = min(1.0f, fraction);
    return CRGB(color).fadeToBlackBy((uint8_t)(255 * (1.0f - fraction)));
}


void DrawFractionalPixels(struct CRGB * pFirstLED, int maxLEDs, float fPos, float pixelWidth, CRGB color)
{
    #pragma region Error Handling
    if (pixelWidth < 1)
    {
        throw invalid_argument("pixelWidth was less than 1.");
    }
    else if (fPos - pixelWidth > (float)maxLEDs)
    {
        throw out_of_range("The fractional pixel is out of range of the LED strip.");
    }
    #pragma endregion

    /*
        Let the following diagram represent the DrawFractionalPixels() logic:

        (1)                   [        pixelWidth        ]
        (2)                (  C  ][  B  ][  B  ][  B  ][  A  )

        where line (1) represents the fractional pixel we are attempting to draw,
        and line (2) represents the actual LED strip pixel boundaries.

        In this diagram, the rightmost endpoint on line (1) represents fPos.
        Let A represent the first fractional pixel we are attempting to draw.
        Let B pixels represent the full pixels we are drawing.
        Let C represent the last fractional pixel we are attempting to draw.
    */

    int currentPixel = min((int)floor(fPos) , maxLEDs);         // The current pixel index
    float remainingWidth = pixelWidth;                          // Current amount of pixelWidth we have left to draw

    #pragma region Fractional Pixel A
    float pixelAWidth = fPos - floor(fPos);
    // If the current pixel is at maxLEDs then there is no pixel A
    if(currentPixel != maxLEDs)
    {
        pFirstLED[currentPixel] += ColorFraction(color, pixelAWidth);
        remainingWidth -= fPos - (float)currentPixel;
        currentPixel--;
    }
    else
    {
        remainingWidth -= pixelAWidth;
        currentPixel--;
    }
    #pragma endregion

    
    #pragma region Full Pixels B
    // Check if there are any full pixels to draw
    while (remainingWidth >= 1.0 && currentPixel >= 0)
    {
        pFirstLED[currentPixel] += color;
        currentPixel--;
        remainingWidth--;
    }
    #pragma endregion


    #pragma region Fractional Pixel C
    // Check if there is a fractional part to draw and that we are within LED strip boundaries
    if (remainingWidth > 0.0 && currentPixel >= 0)
    {
        pFirstLED[currentPixel] += ColorFraction(color, remainingWidth);
    }
    #pragma endregion
}