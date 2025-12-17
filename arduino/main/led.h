// led.h
// Declarations for managing the red status LED

#ifndef LED_H
#define LED_H

#include <Arduino.h>

// Hardware configuration for LED
#define LED_PIN 7

// Constants for blinking patterns
#define BLINK_SLOW 1000      // Slow blink (1 second) - Welcome Screen
#define BLINK_FAST 200       // Fast blink (200ms) - Pause
#define BLINK_ON_DURATION 2000 // LED on for 2 seconds - Game Over
// led.h - add this constant
#define BLINK_PLAY 500      // PLAY blink (500ms) - Same rhythm as display

// LED function declarations
void initLED();                    // Initialize LED
void ledOn();                      // Turn LED on
void ledOff();                     // Turn LED off
void ledSlowBlink();               // Slow blink (Welcome Screen)
void ledFastBlink();               // Fast blink (Pause)
void ledConstantOn();              // LED constantly on (Mode Selection)
void ledGameOverPattern();         // LED on for 2 seconds (Game Over)
void ledVictoryPattern();          // 3 fast blinks (Victory)
void updateLEDPattern();           // Update blinking pattern (call in loop)

// Variables for managing non-blocking blinking
enum LEDState {
  LED_OFF_STATE,          // LED off
  LED_ON_STATE,           // LED constantly on
  LED_SLOW_BLINK_STATE,   // Slow blink
  LED_FAST_BLINK_STATE,   // Fast blink
  LED_VICTORY_STATE       // Victory pattern (3 blinks)
};

// External variables for pattern management
extern LEDState currentLEDState;
extern unsigned long lastLEDToggle;
extern bool ledCurrentlyOn;
extern uint8_t victoryBlinkCount;

void setLEDState(LEDState newState); // Change LED state

#endif // LED_H