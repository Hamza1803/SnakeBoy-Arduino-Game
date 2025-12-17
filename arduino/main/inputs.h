#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>

// Pins
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_L3_PIN 9

#define BTN_UP_PIN    8
#define BTN_DOWN_PIN  4
#define BTN_LEFT_PIN  5
#define BTN_RIGHT_PIN 6

// Joystick rest values 
#define JOYSTICK_HOME_X 365  
#define JOYSTICK_HOME_Y 350   

// Increased thresholds for better detection
#define JOYSTICK_THRESHOLD 60  
#define JOYSTICK_DEADZONE 60   
#define DEBOUNCE_DELAY 10

// Input state structure
struct InputState {
  int8_t dirX;
  int8_t dirY;
  bool l3Pressed;
  bool upPressed;
  bool downPressed;
  bool leftPressed;
  bool rightPressed;
  bool anyPressed;
};

// Functions
void initInputs();
InputState readInputs();
int8_t getDirectionFromAnalog(int analogValue, int homeValue, int threshold);

#endif