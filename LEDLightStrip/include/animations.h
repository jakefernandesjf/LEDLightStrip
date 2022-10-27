/*
    File:           animations.h
    Author:         Jake Fernandes
    Description:    Collection of all user-created FastLED animations.
    Last Updated:   26 OCT 2022
*/

// Animation dependencies
#include <FastLED.h>
#include <sys/time.h>
#include <ledgfx.h>


#pragma region Animations
/// @brief  A wave animation based on the following equation:
///             f(x) = k * sin(x/s) + ( c * x^(1/2) )
///         where
///             x       is time in milliseconds,
///             f(x)    is the wave endpoint position on LED strip,
///             c       is the flow length constant,
///             k       is the ebb length constant, and
///             s       is the wave speed constant.
void wave( struct CRGB * pFirstLED, int numToFill,
                uint8_t flowHue,
                uint8_t ebbHue,
                double flowLength,
                double ebbLength,
                double waveSpeed);

/// @brief A wave animation that lights the LEDs with a choice of flow length and hue forwards, and then a choice of ebb length and hue backwards.
void wave_v1( struct CRGB * pFirstLED, int numToFill,
                  uint8_t flowHue,
                  uint8_t ebbHue,
                  int flowLength,
                  int ebbLengthFactor = 2);
#pragma endregion