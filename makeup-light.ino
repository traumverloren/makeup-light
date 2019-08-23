#include <DmxSimple.h>
#include <FastLED.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

//--------------Blinkytiles Wire guide:-------------
// For reference, the connections on the side of the BlinkyTile are:
// [gnd] [power] [data] [power] [gnd]
//--------------------------------------------------

#define ACTIVATED   LOW      // button is active low (HIGH = 1, LOW = 0)
#define BUTTON_PIN  9        // Connect the button to GND and one of the pins. 

#define LED_PIN     4
#define LAST_LED    11
#define START_LED   7

#define LED_TYPE    DMXSIMPLE
#define COLOR_ORDER BGR

CRGB leds[LAST_LED];

uint8_t brightness = 255;
#define TEMPERATURE_1 Tungsten40W

//----------initial button setup--------------------
boolean areLightsOn = true;

long buttonTimer = 0;
long longPressTime = 250;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {
  delay(4000);
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, LAST_LED).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
  set_max_power_in_volts_and_milliamps(5, 500);  // FastLED Power management set at 5V, 500mA.

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.setTemperature( TEMPERATURE_1 ); 
  for ( uint16_t i = START_LED; i < LAST_LED; i++) {
    leds[i] = TEMPERATURE_1;
  }
  FastLED.show();

}

void loop() {
  byte currKeyState = digitalRead(BUTTON_PIN);

  // button pressed
  if (currKeyState == ACTIVATED) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      longButtonPress();
    }
  // button not pressed
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        Serial.println("Short press!!!");
        shortButtonPress();
      }
      buttonActive = false;
    }
  }

 FastLED.show();
}

//------------------Button Press Setup------------------
void shortButtonPress() {
  if (areLightsOn == false) {
    return;
  }
  if (brightness == 192) {
    FastLED.setBrightness(255); 
    for ( uint16_t i = START_LED; i < LAST_LED; i++) {
      leds[i] = TEMPERATURE_1;
    }
    brightness = 64;
  } else {
    brightness += 64;
    FastLED.setBrightness(brightness); 
    for ( uint16_t i = START_LED; i < LAST_LED; i++) {
      leds[i] = TEMPERATURE_1;
    }
  }
}

void longButtonPress() {
  areLightsOn = !areLightsOn;
  if (areLightsOn == false) {
    setDarkMode();
  } else {
    FastLED.setBrightness(brightness); 
    for ( uint16_t i = START_LED; i < LAST_LED; i++) {
      leds[i] = TEMPERATURE_1;
    }
  }
}

void setDarkMode() {
  for ( uint16_t i = START_LED; i < LAST_LED; i++) {
    leds[i] = CRGB::Black;
  }
}
