#include <Arduino.h>
#include <U8g2lib.h>
#include <FastLED.h>

// Pins for OLED display
#define OLED_CLOCK  15
#define OLED_DATA   4
#define OLED_RESET  16

// FastLED definitions
#define LED_PIN     5
#define NUM_LEDS    10

// Frame buffer for FastLED
CRGB LEDs[NUM_LEDS] = {0};


#pragma region Fields
// Constructor for OLED display (Hardware mode)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);

// Line height of the OLED display
int OLED_LINEHEIGHT = 0;
#pragma endregion


#pragma region Methods
/// @brief Tracks a weighted average of the frames per second
/// @param seconds
/// @return Weighted average of the current frames per second
double FramesPerSecond(double seconds)
{
  static double framesPerSecond;
  framesPerSecond = (framesPerSecond * 0.9) + (1.0 / seconds * 0.1);
  return framesPerSecond;
}
#pragma endregion


#pragma region Main
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Serial Log
  Serial.begin(115200);
  while (!Serial) {};
  Serial.println("ESP32 Startup");

  // Configure OLED
  OLED.begin();
  OLED.clear();
  OLED.setFont(u8g2_font_profont15_tf);
  OLED_LINEHEIGHT = OLED.getFontAscent() - OLED.getFontDescent();

  // Configure FastLED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(LEDs, NUM_LEDS);
  FastLED.setBrightness(16);
}

void loop() 
{
  double fps = 0;

  for (;;)
  {
    // Start counter for fps calculation
    double stopwatchStart = millis() / 1000.0;

    // OLED drawing handler
    OLED.clearBuffer();
    OLED.setCursor(0, OLED_LINEHEIGHT);
    OLED.printf("FPS: %.1f", fps);
    OLED.sendBuffer();

    // LED strip handler
    LEDs[0] = CRGB::Red;
    FastLED.show();

    // End counter and calculate fps
    double stopwatchEnd = millis() / 1000.0;
    fps = FramesPerSecond(stopwatchEnd - stopwatchStart);
  }
}
#pragma endregion