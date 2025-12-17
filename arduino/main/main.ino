#include <Arduino.h>
#include <EEPROM.h>
#include "snake.h"
#include "display.h"
#include "inputs.h"
#include "sound.h"
#include "led.h"
#include <avr/wdt.h>

// --- Game Constants ---
#define SNAKE_SPEED_EASY 400
#define SNAKE_SPEED_HARD 200

// --- SNAKE DATA ---
int16_t snakeX[MAX_SNAKE_LENGTH];
int16_t snakeY[MAX_SNAKE_LENGTH];
uint16_t snakeLength = 0;
uint8_t grid[ROWS][COLS];

int16_t dirX = 1, dirY = 0;
int16_t newDirX = 1, newDirY = 0;

int16_t foodX = 0, foodY = 0;
uint16_t score = 0;
bool isNewRecord = false;

// Scores - CHANGED to uint16_t
uint8_t bestScoreEasy = 0;
uint8_t bestScoreHard = 0;

// --- Game States ---
enum GameState {
  STATE_WELCOME,
  STATE_PLAY_BLINK,
  STATE_MENU,
  STATE_PLAYING,
  STATE_PAUSE,
  STATE_GAME_OVER_ANIMATION,
  STATE_WIN_ANIMATION,
  STATE_FINAL_SCORE
};

GameState state = STATE_WELCOME;

// --- Global Variables ---
bool l3PressedLast = false;
unsigned long lastMoveTime = 0;
bool soundPlayed = false;
bool welcomeAnimationStarted = false;

unsigned long l3PressStartTime = 0;
const unsigned long L3_HOLD_TIME = 2000;
uint8_t currentSelectedMode = 1;

// Game variables
bool wrapMode = true;
uint16_t currentGameSpeed = SNAKE_SPEED_EASY;

// Input buffering variables
int16_t bufferedDirX = 0, bufferedDirY = 0;
bool hasBufferedInput = false;

// Anti-freeze variables
unsigned long lastGameActivity = 0;
const unsigned long GAME_TIMEOUT = 3000; // 3 seconds max without movement

// Animation variables
unsigned long gameOverAnimationStart = 0;
unsigned long winAnimationStart = 0;
unsigned long scoreDisplayStart = 0;
bool animationCompleted = false;
bool soundTriggered = false;

// --- Soft reset ---
void softReset() {
  wdt_enable(WDTO_15MS);
  while (1);
}

// --- Reset ---
void resetGame() {
  // Reset all game variables
  snakeLength = 0;
  score = 0;
  
  // Call initialization functions
  initSnake(snakeX, snakeY, snakeLength, grid);
  generateFood(foodX, foodY, grid);
  
  dirX = newDirX = 1;
  dirY = newDirY = 0;
  
  soundPlayed = false;
  soundTriggered = false;
  lastMoveTime = millis();
  lastGameActivity = millis();
  
  setLEDState(LED_OFF_STATE);
  
  // Reset buffered inputs
  hasBufferedInput = false;
  bufferedDirX = 0;
  bufferedDirY = 0;
  
  // Update display
  updateDisplay(grid);
}

// --- Emergency check ---
void emergencyResetIfStuck() {
  static unsigned long lastEmergencyCheck = 0;
  static int stuckCounter = 0;
  
  if (millis() - lastEmergencyCheck > 500) { // Every 500ms
    lastEmergencyCheck = millis();
    
    // If playing for more than 2 seconds without movement
    if (state == STATE_PLAYING && (millis() - lastMoveTime > 2000)) {
      stuckCounter++;
      
      if (stuckCounter >= 3) { // Stuck for 1.5 seconds
        stuckCounter = 0;
        // Force a soft reset
        lastMoveTime = millis() - currentGameSpeed;
        
        // Check if the snake is initialized
        if (snakeLength == 0) {
          // Fully reset
          resetGame();
        }
      }
    } else {
      stuckCounter = 0; // Reset if everything is fine
    }
  }
}

// --- Setup ---
void setup() {
  //wdt_disable();
  initDisplay();
  initInputs();
  initSound();
  initLED();

  // Read best scores
  bestScoreEasy = EEPROM.read(0);
  bestScoreHard = EEPROM.read(1);

  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  setLEDState(LED_SLOW_BLINK_STATE);
  state = STATE_WELCOME;
  welcomeAnimationStarted = false;
  
  // Random initialization
  randomSeed(analogRead(A2) + analogRead(A3));
}

// --- Main Loop ---
void loop() {
  InputState input = readInputs();
  bool l3PressedNow = input.l3Pressed;

  // Anti-freeze check
  emergencyResetIfStuck();

  // Handle long L3 press (power off/on)
  if (l3PressedNow && !l3PressedLast) {
    l3PressStartTime = millis();
  }
  if (!l3PressedNow && l3PressedLast) {
    l3PressStartTime = 0;
  }

  if (l3PressedNow && l3PressStartTime > 0 && (millis() - l3PressStartTime >= L3_HOLD_TIME)) {
    displayOffMessage();
    delay(3200);
    clearDisplay();
    tone(BUZZER_PIN, 262, 500);
    delay(500);
    noTone(BUZZER_PIN);
    setLEDState(LED_OFF_STATE);
    clearDisplay();

    while (digitalRead(JOYSTICK_L3_PIN) == LOW) delay(10);

    while (true) {
      input = readInputs();
      if (input.l3Pressed) {
        l3PressStartTime = millis();
        while (input.l3Pressed && (millis() - l3PressStartTime < L3_HOLD_TIME)) {
          input = readInputs();
          delay(10);
        }
        if (input.l3Pressed) {
          clearDisplay();
          displayOnMessage();
          delay(1200);
          clearDisplay();
          tone(BUZZER_PIN, 659, 300);
          delay(300);
          noTone(BUZZER_PIN);
          delay(100);
          softReset();
        }
      }
      delay(50);
    }
  }

  updateLEDPattern();

  // Short L3 click
  if (l3PressedNow && !l3PressedLast) {
    if (state == STATE_PLAYING) {
      state = STATE_PAUSE;
      displayPauseScreen();
      setLEDState(LED_FAST_BLINK_STATE);
    } else if (state == STATE_PAUSE) {
      state = STATE_PLAYING;
      clearDisplay();
      updateDisplay(grid);
      setLEDState(LED_OFF_STATE);
      lastMoveTime = millis(); // Reset timer
    } else if (state == STATE_MENU) {
      wrapMode = (currentSelectedMode == 1);
      currentGameSpeed = wrapMode ? SNAKE_SPEED_EASY : SNAKE_SPEED_HARD;
      state = STATE_PLAYING;
      resetGame();
      playStartSound();
      setLEDState(LED_OFF_STATE);
    } else if (state == STATE_PLAY_BLINK || state == STATE_WELCOME) {
      state = STATE_MENU;
      clearDisplay();
      displayModeSelectScreenWithSelection(currentSelectedMode);
      setLEDState(LED_ON_STATE);
    }
  }

  l3PressedLast = l3PressedNow;

  // State machine
  switch (state) {

    case STATE_WELCOME: {
      if (!welcomeAnimationStarted) {
        displayWelcomeScreen();
        playSnakeIntroTheme();
        welcomeAnimationStarted = true;
      }
      if (input.anyPressed && !input.l3Pressed) {
        state = STATE_MENU;
        clearDisplay();
        displayModeSelectScreenWithSelection(currentSelectedMode);
        setLEDState(LED_ON_STATE);
      } else {
        static unsigned long welcomeStartTime = 0;
        if (welcomeStartTime == 0) welcomeStartTime = millis();
        if (millis() - welcomeStartTime > 3000) {
          state = STATE_PLAY_BLINK;
          clearDisplay();
          displayPlayBlinkScreen();
          setLEDState(LED_SLOW_BLINK_STATE);
          welcomeStartTime = 0;
        }
      }
      break;
    }

    case STATE_PLAY_BLINK: {
      updatePlayBlink();
      if (input.anyPressed && !input.l3Pressed) {
        state = STATE_MENU;
        clearDisplay();
        displayModeSelectScreenWithSelection(currentSelectedMode);
        setLEDState(LED_ON_STATE);
      }
      break;
    }

    case STATE_MENU: {
      static unsigned long lastChange = 0;
      static unsigned long lastActivity = millis();
      if (input.anyPressed || input.dirX != 0 || input.dirY != 0) {
        lastActivity = millis();
      }
      if (millis() - lastActivity > 15000) {
        state = STATE_PLAY_BLINK;
        clearDisplay();
        displayPlayBlinkScreen();
        setLEDState(LED_SLOW_BLINK_STATE);
        lastActivity = millis();
        break;
      }
      if ((input.leftPressed || input.dirX == -1) && (millis() - lastChange > 200)) {
        currentSelectedMode = 1;
        lastChange = millis();
        displayModeSelectScreenWithSelection(currentSelectedMode);
        lastActivity = millis();
      }
      if ((input.rightPressed || input.dirX == 1) && (millis() - lastChange > 200)) {
        currentSelectedMode = 2;
        lastChange = millis();
        displayModeSelectScreenWithSelection(currentSelectedMode);
        lastActivity = millis();
      }
      updateModeSelectBlink();
      break;
    }

    case STATE_PLAYING: {
      // CRITICAL FIX: Always allow movement even without input
      bool shouldMove = (millis() - lastMoveTime >= currentGameSpeed);
      
      if (shouldMove) {
        lastMoveTime = millis();
        lastGameActivity = millis();

        // Input buffering
        if ((input.leftPressed || input.dirX == -1) && dirX == 0) {
          bufferedDirX = -1; bufferedDirY = 0;
          hasBufferedInput = true;
        }
        if ((input.rightPressed || input.dirX == 1) && dirX == 0) {
          bufferedDirX = 1; bufferedDirY = 0;
          hasBufferedInput = true;
        }
        if ((input.upPressed || input.dirY == -1) && dirY == 0) {
          bufferedDirX = 0; bufferedDirY = -1;
          hasBufferedInput = true;
        }
        if ((input.downPressed || input.dirY == 1) && dirY == 0) {
          bufferedDirX = 0; bufferedDirY = 1;
          hasBufferedInput = true;
        }

        // Apply buffered input if available
        if (hasBufferedInput) {
          newDirX = bufferedDirX;
          newDirY = bufferedDirY;
          dirX = newDirX;
          dirY = newDirY;
          hasBufferedInput = false;
        }

        uint16_t oldLength = snakeLength;
        
        // SAFETY CHECK: Ensure snake is valid
        if (snakeLength < 3 || snakeLength > MAX_SNAKE_LENGTH) {
          resetGame();
          break;
        }
        
        // Move snake
        moveSnake(snakeX, snakeY, snakeLength, oldLength, dirX, dirY, grid, wrapMode, foodX, foodY);
        
        // Check if food is eaten
        if (checkFood(snakeX, snakeY, snakeLength, foodX, foodY, grid)) {
          score++;
          playEatSound();
          setLEDState(LED_FAST_BLINK_STATE);
          delay(100);
          setLEDState(LED_OFF_STATE);

          // Update best scores
          if (wrapMode) {
            if (score > bestScoreEasy) {
              bestScoreEasy = score;
              EEPROM.update(0, bestScoreEasy);
            }
          } else {
            if (score > bestScoreHard) {
              bestScoreHard = score;
              EEPROM.update(1, bestScoreHard);
            }
          }
        }

        // Check collisions
        if (checkCollision(snakeX, snakeY, snakeLength, !wrapMode)) {
          setLEDState(LED_ON_STATE);
          state = STATE_GAME_OVER_ANIMATION;
          gameOverAnimationStart = millis();
          soundTriggered = false;
          break;
        }

        // Check win
        if (checkWin(snakeLength)) {
          setLEDState(LED_VICTORY_STATE);
          state = STATE_WIN_ANIMATION;
          winAnimationStart = millis();
          soundTriggered = false;
          break;
        }

        // Update display
        updateDisplay(grid);
      }
      
      // Global timeout check
      if (millis() - lastGameActivity > GAME_TIMEOUT) {
        state = STATE_MENU;
        clearDisplay();
        displayModeSelectScreenWithSelection(currentSelectedMode);
        setLEDState(LED_ON_STATE);
      }
      
      break;
    }

    case STATE_PAUSE:
      // Do nothing, wait for input
      break;

    case STATE_GAME_OVER_ANIMATION: {
      // Play sound ONLY ONCE
      if (!soundTriggered) {
        playLoseSound();
        soundTriggered = true;
      }
      
      // Display blinking LOST for 3 seconds
      displayLostAnimation(wrapMode ? bestScoreEasy : bestScoreHard);
      
      if (millis() - gameOverAnimationStart > 3000) {
        state = STATE_FINAL_SCORE;
        scoreDisplayStart = millis();
        animationCompleted = false;
      }
      break;
    }

    case STATE_WIN_ANIMATION: {
      // Play sound ONLY ONCE
      if (!soundTriggered) {
        if (wrapMode) playWinSound(); else playHardWinSound();
        soundTriggered = true;
      }
      
      // Display blinking WIN for 3 seconds
      displayWinAnimation(wrapMode ? bestScoreEasy : bestScoreHard);
      
      if (millis() - winAnimationStart > 3000) {
        state = STATE_FINAL_SCORE;
        scoreDisplayStart = millis();
        animationCompleted = false;
      }
      break;
    }

    case STATE_FINAL_SCORE: {
      // Display final score with star if record
      if (!animationCompleted) {
        bool isNewRecord = false;
        if (wrapMode) {
          isNewRecord = (score >= bestScoreEasy);
        } else {
          isNewRecord = (score >= bestScoreHard);
        }
        displayFinalScore(score, wrapMode ? bestScoreEasy : bestScoreHard, isNewRecord);
        animationCompleted = true;
      }
      
      // Stay on this screen for 2 seconds
      if (millis() - scoreDisplayStart > 2000) {
        state = STATE_MENU;
        clearDisplay();
        displayModeSelectScreenWithSelection(currentSelectedMode);
        setLEDState(LED_ON_STATE);
        
        // Reset variables
        soundPlayed = false;
        soundTriggered = false;
        animationCompleted = false;
        gameOverAnimationStart = 0;
        winAnimationStart = 0;
        scoreDisplayStart = 0;
      }
      break;
    }
  }

  delay(10);
}
