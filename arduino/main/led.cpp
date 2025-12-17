// led.cpp
// Definitions for managing the red status LED

#include "led.h"

// Global variables for managing non-blocking blinking
LEDState currentLEDState = LED_OFF_STATE;
unsigned long lastLEDToggle = 0;
bool ledCurrentlyOn = false;
uint8_t victoryBlinkCount = 0;

// LED function definitions

void initLED() {
  pinMode(LED_PIN, OUTPUT);
  ledOff(); // Ensure LED is off at startup
}

void ledOn() {
  digitalWrite(LED_PIN, HIGH);
  ledCurrentlyOn = true;
}

void ledOff() {
  digitalWrite(LED_PIN, LOW);
  ledCurrentlyOn = false;
}

void setLEDState(LEDState newState) {
  currentLEDState = newState;
  lastLEDToggle = millis();
  victoryBlinkCount = 0;
  
  // Initialize LED state based on the new pattern
  switch (newState) {
    case LED_OFF_STATE:
      ledOff();
      break;
    case LED_ON_STATE:
      ledOn();
      break;
    case LED_SLOW_BLINK_STATE:
    case LED_FAST_BLINK_STATE:
      ledOn(); // Start with LED on
      break;
    case LED_VICTORY_STATE:
      ledOn(); // Start victory pattern with LED on
      break;
  }
}

void updateLEDPattern() {
  unsigned long currentTime = millis();
  
  switch (currentLEDState) {
    case LED_OFF_STATE:
      // LED off
      break;
      
    case LED_ON_STATE:
      // LED permanently on
      break;
      
    case LED_SLOW_BLINK_STATE:
      // Slow blink (1 second ON, 1 second OFF)
      if (currentTime - lastLEDToggle >= BLINK_SLOW) {
        if (ledCurrentlyOn) {
          ledOff();
        } else {
          ledOn();
        }
        lastLEDToggle = currentTime;
      }
      break;
      
    case LED_FAST_BLINK_STATE:
      // Fast blink (200ms ON, 200ms OFF)
      if (currentTime - lastLEDToggle >= BLINK_FAST) {
        if (ledCurrentlyOn) {
          ledOff();
        } else {
          ledOn();
        }
        lastLEDToggle = currentTime;
      }
      break;
      
    case LED_VICTORY_STATE:
      // Victory pattern: 3 fast blinks
      if (victoryBlinkCount < 6) { // 6 = 3 ON + 3 OFF
        if (currentTime - lastLEDToggle >= 150) { // 150ms per blink
          if (ledCurrentlyOn) {
            ledOff();
          } else {
            ledOn();
          }
          victoryBlinkCount++;
          lastLEDToggle = currentTime;
        }
      } else {
        // After the 3 blinks, turn off the LED
        ledOff();
        currentLEDState = LED_OFF_STATE;
      }
      break;
  }
}

void ledSlowBlink() {
  setLEDState(LED_SLOW_BLINK_STATE);
}

void ledFastBlink() {
  setLEDState(LED_FAST_BLINK_STATE);
}

void ledConstantOn() {
  setLEDState(LED_ON_STATE);
}

void ledGameOverPattern() {
  // Turn LED on for 2 seconds
  ledOn();
  delay(BLINK_ON_DURATION);
  ledOff();
}

void ledVictoryPattern() {
  setLEDState(LED_VICTORY_STATE);
}