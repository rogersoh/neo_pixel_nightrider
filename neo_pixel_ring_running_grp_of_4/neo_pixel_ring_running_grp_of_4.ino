#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NumberPixel 12
#define GROUP 4
#define readVoltPin 12
#define powerOnPin 4
#define sensingPin A2
#define lowBattVolt 3.8
#define BRIGTNESS 20
float batteryVolt = 0.0;
int printInterval = 2000;
unsigned long printTime = 0;
const float OnTime = 0.5 ; // OnTime in hours
bool ON = true;

// Parameter 1 = number of pixels
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NumberPixel, PIN, NEO_GRB + NEO_KHZ800);

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

  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  pinMode(readVoltPin, OUTPUT);
  pinMode(powerOnPin, OUTPUT);
  digitalWrite(readVoltPin, HIGH);
  digitalWrite(powerOnPin, HIGH);
  delay(50);
  batteryVolt = analogRead(sensingPin);
  batteryVolt = batteryVolt * 5 / 1024.0;
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
    //    if (batteryVolt > lowBattVolt) {
    //      ON = true;
    //    } else {
    //      ON = false;
    //    }
  }
  if (ON) {
    for (int j = 0; j < GROUP; j++) {
      for (int i = 0; i < NumberPixel / GROUP; i++) {
        for (int k = 0; k < 3; k++) {
          int m = j + i * GROUP + k;
//          Serial.print("j ");
//          Serial.print(j);
//          Serial.print("   i ");
//          Serial.print(i);
//          Serial.print("   k ");
//          Serial.print(k);
//          Serial.print("   m ");
//          Serial.print(m);
          if (m > (NumberPixel - 1)) {
            m = m - NumberPixel;
          }
//          Serial.print("   m(cal) ");
//          Serial.println(m);
          pixels.setPixelColor(m, pixels.Color(BRIGTNESS, 0, 0));
        }
      }
      pixels.show();
//      Serial.println("delay");
//      Serial.println("");
      delay(100);
      pixels.clear();
    }
  }
}
