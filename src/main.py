'''------------------------------------------------------
main.py

The main controller for the LEDLightStrip program.
---------------------------------------------------------'''
import board
import neopixel

#region Global Variables
# Pin of the raspberry pi that the NeoPixels are connected to
PIXEL_PIN = board.D18

# The number of pixels the connected NeoPixel strip has
NUM_OF_PIXELS = 150

# Order of pixel colors the NeoPixel strip (RGB or GRB)
ORDER = neopixel.GRB

# Starting brightness level (0.0 to 1.0 where 1.0 is full brightness)
BRIGHTNESS = 0.2
#endregion

pixels = neopixel.NeoPixel( PIXEL_PIN, NUM_OF_PIXELS, BRIGHTNESS, auto_write = False, pixel_order = ORDER )

while(true):
    for i in range(len(pixels)):
        pixels[i] = RED