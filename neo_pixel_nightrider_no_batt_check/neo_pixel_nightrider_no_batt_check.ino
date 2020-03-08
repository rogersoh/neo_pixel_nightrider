#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NumberPixel 60
#define GROUP 15
#define OFFSET 30
#define BRIGHTNESS 12
int printInterval = 2000;
unsigned long printTime = 0;
const float OnTime = 0.17 ; // OnTime in hours
bool ON = true;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumberPixel, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop() {
  if (ON && millis() > OnTime * 3600000) {
    ON = false;
  }
  if (ON) {
    // Some example procedures showing how to display to the pixels:
    nightrider(strip.Color(100, 0, 0), 40); // Red
    nightrider(strip.Color(0, 10, 0), 40); // Green
    nightrider(strip.Color(0, 0, 10), 40); // Blue
    //    colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    //Send a theater pixel chase in...
    theaterChase(strip.Color(10, 10, 10), 50); // White
    theaterChase(strip.Color(40, 0, 0), 100); // Red
    theaterChase(strip.Color(10, 10, 0), 100); // Red n Green
    rainbow(10);
    theaterChase(strip.Color(10, 00, 10), 100); // Red n Blue
    rainbowCycle(10);
  } else {
//    for (int i = 0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, 0);
//    }
//    strip.show();
    strip.clear();
    delay(50);
  }
}

// nightrider
void nightrider(uint32_t c, uint8_t wait) {
  for (uint8_t k = 0; k < 5; k++) {


    for (uint16_t i = 0; i < NumberPixel; i++) {
      for (uint16_t j = 0; j < NumberPixel / GROUP; j++) {

        strip.setPixelColor(range(i - 2 + j * GROUP), c / 10);
        strip.setPixelColor(range(i  - 1 + j * GROUP), c / 2);
        strip.setPixelColor(range(i + j * GROUP), c);
        strip.setPixelColor(range(i + 1 + j * GROUP), c / 2);
        strip.setPixelColor(range(i + 2 + j * GROUP), c / 10);
      }
      strip.show();
      Serial.println(i);
      delay(wait);
      strip.clear();
    }//forward
    for (int i = NumberPixel - 1; i > -1; i--) {
      for (int j = NumberPixel / GROUP - 1; j > -1; j--) {
        strip.setPixelColor(range(i - 2  + j * GROUP) , c / 10);
        strip.setPixelColor(range(i - 1 + j * GROUP), c / 2);
        strip.setPixelColor(range(i + j * GROUP), c);
        strip.setPixelColor(range(i  + 1 + j * GROUP), c / 2);
        strip.setPixelColor(range(i  + 2 + j * GROUP), c / 10);
      }
      strip.show();
      Serial.println(i);
      delay(wait);
      strip.clear();
    }
  }
}



// nightriderFwd
void nightriderFwd(uint32_t c, uint8_t wait) {
  for (uint16_t k = 0; k < 2 * NumberPixel / GROUP; k++) {
    for (uint16_t j = 0; j < GROUP; j++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + GROUP) {
        strip.setPixelColor(i + j, 0);
        if (j + 1 >= GROUP) {
          strip.setPixelColor(i + j + 1 - GROUP, c / 10);
        } else {
          strip.setPixelColor(i + j + 1, c / 10);
        }
        if (j + 2 >= GROUP) {
          strip.setPixelColor(i + j + 2 - GROUP, c / 2);
        } else {
          strip.setPixelColor(i + j + 2, c / 2);
        }
        if (j + 3 >= GROUP) {
          strip.setPixelColor(i + j + 3 - GROUP, c);
        } else {
          strip.setPixelColor(i + j + 3, c);
        }
        if (j + 4 >= GROUP) {
          strip.setPixelColor(i + j + 4 - GROUP, c / 2);
        } else {
          strip.setPixelColor(i + j + 4, c / 2);
        }
        if (j + 5 >= GROUP) {
          strip.setPixelColor(i + j + 5 - GROUP, c / 10);
        } else {
          strip.setPixelColor(i + j + 5, c / 10);
        }
      }
      strip.show();
      delay(wait);
    }//forward
  }//number of times
}


void rainbow(uint8_t wait) {
  strip.setBrightness(BRIGHTNESS);
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  strip.setBrightness(255);
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  strip.setBrightness(BRIGHTNESS);
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  strip.setBrightness(255);
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int range(int m) {
  if (m < 0) {
    m = m + NumberPixel - 1;
  }
  if (m > NumberPixel - 1) {
    m = m - NumberPixel + 1 ;
  }
  return m;
}
