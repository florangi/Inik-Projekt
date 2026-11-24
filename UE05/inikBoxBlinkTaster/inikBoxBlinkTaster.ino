/*
  IniK-Box - Name auf dem Display, RGB-LED blinkt, Taster steuern den Takt
  Informatik im Kontext, WiSe 26/27

  Hardware: Carenuity C3 Mini auf der Platine IniK-Box v1.3
  In der Arduino IDE als Board "LOLIN C3 MINI" auswaehlen.

  Pin         Funktion
  ----------  ------------------------------------------
  GPIO 4      schwarzer Taster, gegen GND
  GPIO 5      roter Taster, gegen GND
  GPIO 7      RGB-LED (WS2812B) auf dem C3 Mini
  GPIO 8      SDA (I2C)
  GPIO 10     SCL (I2C)

  Benoetigte Bibliotheken (Bibliotheksverwalter der Arduino IDE):
  U8g2, Adafruit NeoPixel

  Der Aufbau im loop ist derselbe wie bei "Blink without Delay" aus
  Vorlesung 03: previousMillis merkt sich, wann zuletzt umgeschaltet
  wurde, interval ist die Wartezeit dazwischen.
*/

#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>

U8G2_SSD1306_64X48_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int PIN_SCHW = 4;  // schwarz
const int PIN_ROT  = 5;  // rot
const int PIN_LED  = 7;  // RGB-LED

Adafruit_NeoPixel led(1, PIN_LED, NEO_GRB + NEO_KHZ800);

unsigned long previousMillis = 0;
unsigned long interval = 300;   // Millisekunden je Phase
bool ledState = false;          // leuchtet die LED gerade?
int  farbe = 0;                 // 0 rot, 1 gruen, 2 blau

void zeigeFarbe() {
  if (!ledState) {
    led.clear();
  } else if (farbe == 0) {
    led.setPixelColor(0, led.Color(255, 0, 0));
  } else if (farbe == 1) {
    led.setPixelColor(0, led.Color(0, 255, 0));
  } else {
    led.setPixelColor(0, led.Color(0, 0, 255));
  }
  led.show();
}

void setup() {
  Wire.begin(8, 10);

  pinMode(PIN_SCHW, INPUT_PULLUP);
  pinMode(PIN_ROT,  INPUT_PULLUP);

  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.setCursor(10, 28);
  display.print("Angelika");
  display.sendBuffer();

  led.begin();
  led.setBrightness(40);   // volle Helligkeit blendet
  led.clear();
  led.show();
}

void loop() {
  unsigned long currentMillis = millis();

  // schwarzer Taster: schneller blinken, interval wird kleiner
  if (digitalRead(PIN_SCHW) == LOW) {
    if (interval > 100) {
      interval -= 100;
    }
    delay(200);
  }
  // roter Taster:     langsamer blinken, interval wird groesser
  if (digitalRead(PIN_ROT) == LOW) {
    if (interval < 2000) {
      interval += 100;
    }
    delay(200);
  }
  

  // Zusatz: Am langsamen Ende soll das Blinken ganz aufhoeren,
  // und der schwarze Taster soll es wieder starten.

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    ledState = !ledState;
    if (ledState) {
      farbe = (farbe + 1) % 3;
    }
    zeigeFarbe();
  }
}
