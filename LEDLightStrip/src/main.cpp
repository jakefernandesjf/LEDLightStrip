/*
    File:           main.cpp
    Author:         Jake Fernandes
    Description:    Main code to control ESP32 handling for LED strip
    
    Change Log:
    22 OCT 2022     Reorganized main with a header file.
    23 OCT 2022     Updated to use wave animation.
    23 OCT 2022     Updates for externally powered LEDs.
    26 OCT 2022     Updated name of wave function.
    30 OCT 2022     Updated to use new wave function.
    01 NOV 2022     Updated to use wave_palette().
*/

#include <header.h>

#pragma region Fields
// Pins for OLED display
#define OLED_CLOCK  15
#define OLED_DATA   4
#define OLED_RESET  16

// FastLED definitions
#define LED_PIN     5
#define NUM_LEDS    75

// Frame buffer for FastLED
CRGB g_LEDs[NUM_LEDS] = {0};
int g_BRIGHTNESS = 64;
int g_POWER_LIMIT = 2000;

// Constructor for OLED display (Hardware mode)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);

// Line height of the OLED display
int g_OLED_LINEHEIGHT = 0;
#pragma endregion


#pragma region Methods
/// @brief Draws LED data to display on microcontroller OLED
/// @param FastLED FastLED controller object
void DrawOledData(CFastLED FastLED)
{
  g_OLED.clearBuffer();
  g_OLED.setCursor(0, g_OLED_LINEHEIGHT);
  g_OLED.printf("FPS: %6u Hz", FastLED.getFPS());
  g_OLED.setCursor(0, g_OLED_LINEHEIGHT * 2);
  g_OLED.printf("PWR: %6u mW", calculate_unscaled_power_mW(FastLED.leds(), NUM_LEDS));
  g_OLED.setCursor(0, g_OLED_LINEHEIGHT * 3);
  g_OLED.printf("BRT: %6u bt", calculate_max_brightness_for_power_mW(g_BRIGHTNESS, g_POWER_LIMIT));
  g_OLED.sendBuffer();
}
#pragma endregion


#pragma region Main
// Define Gradiant
DEFINE_GRADIENT_PALETTE( water_gp ) 
  {
    0,      10,   151,  255,
    46,     0,    255,  235,
    179,    80,    92,  252,
    255,    10,   151,  255,
  };

DEFINE_GRADIENT_PALETTE( sand_gp ) 
  {
    0,      200,  200,   49,
    255,    255,  255,  0,
  };


void setup() 
{
  // Set Pin Modes
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Serial Log
  Serial.begin(115200);
  while (!Serial) {};
  Serial.println("ESP32 Startup");

  // Configure OLED
  g_OLED.begin();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_OLED_LINEHEIGHT = g_OLED.getFontAscent() - g_OLED.getFontDescent();

  // Configure FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);
  FastLED.setBrightness(g_BRIGHTNESS);
  FastLED.setMaxPowerInMilliWatts(g_POWER_LIMIT);
  set_max_power_indicator_LED(LED_BUILTIN);
}


void loop() 
{
  CRGBPalette16 waterPalette = water_gp;
  CRGBPalette16 sandPalette = sand_gp;
  while (true)
  {
    // OLED drawing handler
    EVERY_N_MILLISECONDS(100)
    {
      DrawOledData(FastLED);
    }

    // LED strip handler
    wave_palette(g_LEDs, NUM_LEDS-1, waterPalette, sandPalette, 7, 10, 0.5);
    // static int i = 0;
    // fill_palette(g_LEDs, 50, i, 255/50, waterPalette, 255, TBlendType::LINEARBLEND);
    // i--;
    // FastLED.delay(10);
  }
}
#pragma endregion