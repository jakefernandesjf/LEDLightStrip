/*
    @file:          ledgfx.h
    Author:         Jake Fernandes
    Description:    LED graphic helper functions for user-created FastLED animations.
    Last Updated:   26 OCT 2022
*/
#include <FastLED.h>

/// @brief Convert an RGB color value into an RGB color value that has a saturation value decreased by a factor determined by fraction.
/// @return CRGB color value that is fraction less saturated than color.
CRGB ColorFraction(CRGB color, float fraction);

/// @brief Draw pixel(s) at a floating point on the LED strip.
void DrawFractionalPixels(struct CRGB * pFirstLED, float fPos, float pixelWidth, CRGB color);