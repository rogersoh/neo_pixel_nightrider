#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define BRIGHTNESS 30
#define OFFSET 30
#define readVoltPin 12
#define sensingPin A2
#define lowBattVolt 3.0
float batteryVolt = 0.0;
int printInterval = 2000;
unsigned long printTime = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

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

  pinMode(readVoltPin, OUTPUT);
  digitalWrite(readVoltPin, LOW);
  Serial.begin(9600);
}

void loop() {
  if (millis() - printTime > printInterval) {
    printTime = millis();
    digitalWrite(readVoltPin, HIGH);
    delay(50);
    batteryVolt = analogRead(sensingPin);
    batteryVolt = batteryVolt * 5 / 1024.0;
    Serial.print("Battery Volt:  ");
    Serial.println(batteryVolt, 3);
    digitalWrite(readVoltPin, LOW);
  }
  if (batteryVolt > lowBattVolt) {
    // Some example procedures showing how to display to the pixels:
    colorWipe(strip.Color(100, 0, 0), 20); // Red
    colorWipe(strip.Color(0, 40, 0), 20); // Green
    colorWipe(strip.Color(40, 0, 40), 20); // Blue
    //  colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    // Send a theater pixel chase in...
    //  theaterChase(strip.Color(127, 127, 127), 50); // White
    theaterChase(strip.Color(40, 0, 0), 100); // Red
    theaterChase(strip.Color(10, 10, 0), 100); // Red n Green
    theaterChase(strip.Color(20, 00, 20), 100); // Red n Blue
    //theaterChase(strip.Color(0, 0, 127), 50); // Blue
    //theaterChaseRainbow(50);
    //rainbow(20);
    //rainbowCycle(20);
  } else {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i - OFFSET - 3, c * 0);
    strip.setPixelColor(i - OFFSET  - 2, c / 5);
    strip.setPixelColor(i - OFFSET  - 1, c / 2);
    strip.setPixelColor(i - OFFSET , c);
    strip.setPixelColor(i - OFFSET  + 1, c / 2);
    strip.setPixelColor(i - OFFSET  + 2, c / 5);

    strip.setPixelColor(i - 3, c * 0);
    strip.setPixelColor(i - 2, c / 5);
    strip.setPixelColor(i - 1, c / 2);
    strip.setPixelColor(i, c);
    strip.setPixelColor(i + 1, c / 2);
    strip.setPixelColor(i + 2, c / 5);

    strip.setPixelColor(i + OFFSET - 3, c * 0);
    strip.setPixelColor(i + OFFSET  - 2, c / 5);
    strip.setPixelColor(i + OFFSET  - 1, c / 2);
    strip.setPixelColor(i + OFFSET , c);
    strip.setPixelColor(i + OFFSET  + 1, c / 2);
    strip.setPixelColor(i + OFFSET  + 2, c / 5);

    strip.show();
    delay(wait);
  }
  for (uint16_t i = strip.numPixels(); i > 0; i--) {
    strip.setPixelColor(i + OFFSET + 2, c * 0);
    strip.setPixelColor(i + OFFSET + 1, c / 5);
    strip.setPixelColor(i + OFFSET + 1, c / 2);
    strip.setPixelColor(i + OFFSET, c);
    strip.setPixelColor(i + OFFSET - 1, c / 2);
    strip.setPixelColor(i + OFFSET - 2, c / 5);

    strip.setPixelColor(i + 2, c * 0);
    strip.setPixelColor(i + 1, c / 5);
    strip.setPixelColor(i + 1, c / 2);
    strip.setPixelColor(i, c);
    strip.setPixelColor(i - 1, c / 2);
    strip.setPixelColor(i - 2, c / 5);

    strip.setPixelColor(i - OFFSET + 2, c * 0);
    strip.setPixelColor(i - OFFSET + 1, c / 5);
    strip.setPixelColor(i - OFFSET + 1, c / 2);
    strip.setPixelColor(i - OFFSET, c);
    strip.setPixelColor(i - OFFSET - 1, c / 2);
    strip.setPixelColor(i - OFFSET - 2, c / 5);

    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
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

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
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