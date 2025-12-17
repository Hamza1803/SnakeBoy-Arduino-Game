#include "inputs.h"

// Debounce and filtering

// Variables for filtering
static int lastFilteredX = JOYSTICK_HOME_X;
static int lastFilteredY = JOYSTICK_HOME_Y;
static bool lastL3State = HIGH;
static unsigned long lastL3Debounce = 0;

void initInputs() {
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(JOYSTICK_L3_PIN, INPUT_PULLUP);

  pinMode(BTN_UP_PIN, INPUT_PULLUP);
  pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
  pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
  pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);

  // Initial reading
  analogRead(JOYSTICK_X_PIN);
  analogRead(JOYSTICK_Y_PIN);
  
  // Initialize states
  lastL3State = digitalRead(JOYSTICK_L3_PIN);
}

InputState readInputs() {
  InputState state = {0};

  // Joystick reading and filtering
  int rawX = analogRead(JOYSTICK_X_PIN);
  int rawY = analogRead(JOYSTICK_Y_PIN);
  
  // Simple low-pass filter (reduces noise)
  int filteredX = (rawX + lastFilteredX) / 2;
  int filteredY = (rawY + lastFilteredY) / 2;
  lastFilteredX = filteredX;
  lastFilteredY = filteredY;

  // Convert to directions
  state.dirX = getDirectionFromAnalog(filteredX, JOYSTICK_HOME_X, JOYSTICK_THRESHOLD);
  state.dirY = getDirectionFromAnalog(filteredY, JOYSTICK_HOME_Y, JOYSTICK_THRESHOLD);

  // Debounced reading of L3 button
  bool currentL3State = digitalRead(JOYSTICK_L3_PIN);
  unsigned long currentTime = millis();
  
  if (currentL3State != lastL3State) {
    lastL3Debounce = currentTime;
  }
  
  if ((currentTime - lastL3Debounce) > DEBOUNCE_DELAY) {
    state.l3Pressed = (currentL3State == LOW);
  }
  
  lastL3State = currentL3State;

  // Button reading
  state.upPressed = (digitalRead(BTN_UP_PIN) == LOW);
  state.downPressed = (digitalRead(BTN_DOWN_PIN) == LOW);
  state.leftPressed = (digitalRead(BTN_LEFT_PIN) == LOW);
  state.rightPressed = (digitalRead(BTN_RIGHT_PIN) == LOW);

  // Determine if any input is active
  state.anyPressed = (state.dirX != 0 || state.dirY != 0 ||
                      state.l3Pressed ||
                      state.upPressed || state.downPressed || 
                      state.leftPressed || state.rightPressed);

  return state;
}

int8_t getDirectionFromAnalog(int analogValue, int homeValue, int threshold) {
  int difference = analogValue - homeValue;
  
  // Dead zone
  if (abs(difference) < JOYSTICK_DEADZONE) {
    return 0;
  }
  
  // Direction thresholds
  if (difference < -threshold) {
    return -1; // Left or Up
  }
  else if (difference > threshold) {
    return 1;  // Right or Down
  }
  
  return 0;  // Intermediate zone
}