#include <Arduino.h> // standard lib
#include <unity.h>   // test framework
#include "MHZ19.h"   // the co2 sensor
#include <FastLED.h> // for the LEDs
#include <Tone32.h>  // for the buzzer


// Define the sensor connection
#define RX_PIN 32                                          // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 26                                          // Tx pin which the MHZ19 Rx pin is attached to
#define MHZ19_BAUDRATE 9600

// Define the RGB LED connection here
#define LED_PIN 23
#define LED_CHIPSET WS2812B
#define LED_COLOR_ORDER GRB
#define LED_BRIGHTNESS 42 // 0-255
#define NUM_LEDS 2

// Define the beeper connection here
#define BUZZER_PIN 19


// calc test, no interaction with connected stuff
void test_led_builtin_pin_number(void) {
    TEST_ASSERT_EQUAL(2, LED_BUILTIN);
}

// Connects to the dev-board
void test_can_read_devboard_ID(void){
  uint64_t chipid=0;
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length:6bytes)
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32)); //print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  TEST_ASSERT_NOT_EQUAL(chipid, 0);
  delay(3000);
}

// connects to the attached sensor
void test_can_read_mhz19_version(void) {
  MHZ19 co2sensor;
  HardwareSerial co2Serial(1);
  co2Serial.begin(MHZ19_BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN); //baudrate is fix at 9600
  co2sensor.begin(co2Serial);
  // check version of the sensor to ensure we're not talking to a zombie
  char mhz19bVersion[4];
  co2sensor.getVersion(mhz19bVersion);
  TEST_ASSERT_EQUAL(co2sensor.errorCode, RESULT_OK);
  TEST_ASSERT_NOT_NULL(mhz19bVersion);
  Serial.printf("[DEBUG] Firmware version: %s\n", mhz19bVersion);
}

// connects to the attached LEDs:
void test_flicker_the_LEDs(void) {
  CRGB leds[NUM_LEDS];
  FastLED.addLeds<LED_CHIPSET, LED_PIN, LED_COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(50);

  // LED testing: this should flicker in colors!
  for (uint8_t  b=0; b<63; b++) {
    for (int l=0; l<NUM_LEDS; l++) {
      leds[l].setHue((4-2*l)*b); FastLED.show();
      delay(60);
      leds[l] = CRGB::Black; FastLED.show();
      delay(30);
    }
  }

  Serial.println("Check the red/yellow/green colors we'll be using!");
  leds[0] = CRGB::DarkRed; FastLED.show();
  delay(1000);
  leds[0] = CRGB::Black; FastLED.show();
  delay(30);
  leds[0] = CRGB::Yellow; FastLED.show();
  delay(1000);
  leds[0] = CRGB::Black; FastLED.show();
  delay(30);
  leds[0] = CRGB::DarkGreen; FastLED.show();
  delay(1000);
  leds[0] = CRGB::Black; FastLED.show();
  delay(30);

}

void test_tone32(void){
  // taken from examples at https://github.com/lbernstone/Tone32/blob/master/examples/Simple_Tone.ino
  tone(BUZZER_PIN, NOTE_C4, 500, 0);
  noTone(BUZZER_PIN, 0);
  tone(BUZZER_PIN, NOTE_D4, 500, 0);
  noTone(BUZZER_PIN, 0);
  tone(BUZZER_PIN, NOTE_E4, 500, 0);
  noTone(BUZZER_PIN, 0);
  tone(BUZZER_PIN, NOTE_F4, 500, 0);
  noTone(BUZZER_PIN, 0);
  tone(BUZZER_PIN, NOTE_G4, 500, 0);
  noTone(BUZZER_PIN, 0);
  tone(BUZZER_PIN, NOTE_A4, 500, 0);
  noTone(BUZZER_PIN, 0);
  tone(BUZZER_PIN, NOTE_B4, 500, 0);
  noTone(BUZZER_PIN, 0);
  tone(BUZZER_PIN, NOTE_C5, 500, 0);
  noTone(BUZZER_PIN, 0);
}

void setup() {

  Serial.begin(115200);
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
  RUN_TEST(test_led_builtin_pin_number);
  RUN_TEST(test_can_read_mhz19_version);
  RUN_TEST(test_flicker_the_LEDs); // this test will likely not fail automatically, but check if the LEDs go crazy.
  UNITY_END(); // stop unit testing
  Serial.println("That's all folks");
}


void loop(){
    return;
}
