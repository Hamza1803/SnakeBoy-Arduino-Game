// display.cpp
#include "display.h"
#include "SPI.h"
#include <string.h>
#include <Arduino.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

extern uint8_t currentSelectedMode;

// Buffer local for save state dof matrices (4x8)
uint8_t displayBuffer[MAX_DEVICES][8] = {0};

// number 1 and 2 (8x8, MSB à gauche)
const uint8_t chiffreUn[8] = {
  0b00000000,
  0b00001000,
  0b00011000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00011100,
  0b00000000
};

const uint8_t chiffreDeux[8] = {
  0b00000000,
  0b00011100,
  0b00100010,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00111110,
  0b00000000
};

// Police 7x5 of number 0-9
const uint8_t fontDigits[10][7] = {
  { B01110000, B10001000, B10011000, B10101000, B11001000, B10001000, B01110000 }, // 0
  { B01000000, B11000000, B01000000, B01000000, B01000000, B01000000, B11100000 }, // 1
  { B01110000, B10001000, B00001000, B00010000, B00100000, B01000000, B11111000 }, // 2
  { B11111000, B00010000, B00100000, B00010000, B00001000, B10001000, B01110000 }, // 3
  { B00010000, B00110000, B01010000, B10010000, B11111000, B00010000, B00010000 }, // 4
  { B11111000, B10000000, B11110000, B00001000, B00001000, B10001000, B01110000 }, // 5
  { B00110000, B01000000, B10000000, B11110000, B10001000, B10001000, B01110000 }, // 6
  { B11111000, B10001000, B00001000, B00010000, B00100000, B00100000, B00100000 }, // 7
  { B01110000, B10001000, B10001000, B01110000, B10001000, B10001000, B01110000 }, // 8
  { B01110000, B10001000, B10001000, B01111000, B00001000, B00010000, B01100000 }  // 9
};

// STAR (BEST SCORE)
const uint8_t star[8] = {
  0b00000000,
  0b00101000, //   █ █
  0b00010000, //    █
  0b01111100, //  █████
  0b00010000, //    █
  0b00101000, //   █ █
  0b00000000,
  0b00000000
};


void clearDisplay() {
  memset(displayBuffer, 0, sizeof(displayBuffer));
  mx.clear();
}

void initDisplay() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 8);
  clearDisplay();
}

void updateMatrix(uint8_t device) {
  for (uint8_t r = 0; r < 8; r++) {
    mx.setRow(device, r, displayBuffer[device][r]);
  }
}

void setPixel(uint8_t device, uint8_t row, uint8_t col, bool state) {
  if (device >= MAX_DEVICES || row >= 8 || col >= 8) return;
  if (state) {
    displayBuffer[device][row] |= (1 << (7 - col));
  } else {
    displayBuffer[device][row] &= ~(1 << (7 - col));
  }
}


void updateDisplay(uint8_t grid[ROWS][COLS]) {
  mx.clear();
  
  static uint32_t lastFoodBlink = 0;
  static bool foodVisible = true;
  
  if (millis() - lastFoodBlink >= 300) {
    lastFoodBlink = millis();
    foodVisible = !foodVisible;
  }

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      if (grid[r][c] == SNAKE) {
        uint8_t device = 3 - (c / 8);
        uint8_t col = c % 8;
        mx.setPoint(r, (device * 8) + (7 - col), true);
      } else if (grid[r][c] == FOOD && foodVisible) {
        uint8_t device = 3 - (c / 8);
        uint8_t col = c % 8;
        mx.setPoint(r, (device * 8) + (7 - col), true);
      }
    }
  }
}


void displayWelcomeScreen() {
  static bool initialized = false;
  if (!initialized) {
    initialized = true;
    clearDisplay();

    // === SNAKE LOGO ===
    displayBuffer[3][4] = 0b00011000;
    displayBuffer[3][5] = 0b00111100;
    displayBuffer[3][6] = 0b00111100;
    
    displayBuffer[2][3] = 0b00011000;
    displayBuffer[2][4] = 0b00111100;
    displayBuffer[2][5] = 0b01111110;
    displayBuffer[2][6] = 0b01111110;
    displayBuffer[2][7] = 0b00111100;
    
    displayBuffer[1][2] = 0b00011000;
    displayBuffer[1][3] = 0b00111100;
    displayBuffer[1][4] = 0b01111110;
    displayBuffer[1][5] = 0b01111110;
    displayBuffer[1][6] = 0b01111110;
    displayBuffer[1][7] = 0b00111100;
    
    displayBuffer[0][1] = 0b00011000;
    displayBuffer[0][2] = 0b00111100;
    displayBuffer[0][3] = 0b01111110;
    displayBuffer[0][4] = 0b01100110;
    displayBuffer[0][5] = 0b01111110;
    displayBuffer[0][6] = 0b01111110;
    displayBuffer[0][7] = 0b00111100;

    for (int i = 0; i < 4; i++) updateMatrix(i);
  }
}

void displayPlayBlinkScreen() {
  static bool visible = true;
  static uint32_t lastBlink = 0;
  if (millis() - lastBlink > 500) {
    lastBlink = millis();
    visible = !visible;
    if (visible) {
      displayBuffer[3][0] = B01111000;
      displayBuffer[3][1] = B01000100;
      displayBuffer[3][2] = B01000100;
      displayBuffer[3][3] = B01111000;
      displayBuffer[3][4] = B01000000;
      displayBuffer[3][5] = B01000000;
      displayBuffer[3][6] = B01000000;
      displayBuffer[3][7] = B00000000;

      displayBuffer[2][0] = B10000000;
      displayBuffer[2][1] = B10000000;
      displayBuffer[2][2] = B10000000;
      displayBuffer[2][3] = B10000000;
      displayBuffer[2][4] = B10000000;
      displayBuffer[2][5] = B10000000;
      displayBuffer[2][6] = B11111000;
      displayBuffer[2][7] = B00000000;

      displayBuffer[1][0] = B01110000;
      displayBuffer[1][1] = B10001000;
      displayBuffer[1][2] = B10001000;
      displayBuffer[1][3] = B10001000;
      displayBuffer[1][4] = B11111000;
      displayBuffer[1][5] = B10001000;
      displayBuffer[1][6] = B10001000;
      displayBuffer[1][7] = B00000000;

      displayBuffer[0][0] = B10001000;
      displayBuffer[0][1] = B10001000;
      displayBuffer[0][2] = B10001000;
      displayBuffer[0][3] = B01010000;
      displayBuffer[0][4] = B00100000;
      displayBuffer[0][5] = B00100000;
      displayBuffer[0][6] = B00100000;
      displayBuffer[0][7] = B00000000;

      for (int i = 0; i < 4; i++) updateMatrix(i);
    } else {
      clearDisplay();
    }
  }
}

void updatePlayBlink() {
  displayPlayBlinkScreen();
}

// ------------------ FONCTIONS SCORE ------------------

void drawDigit(uint8_t digit, uint8_t matrix) {
  if (digit > 9 || matrix >= MAX_DEVICES) return;
  for (int r = 0; r < 7; r++) {
    displayBuffer[matrix][r] = fontDigits[digit][r];
  }
  displayBuffer[matrix][7] = 0;
}

void drawScore(uint16_t score) {
  uint8_t h = score / 100;
  uint8_t t = (score % 100) / 10;
  uint8_t u = score % 10;

  if (h > 0) {
    drawDigit(h, 2);
  } else {
    memset(displayBuffer[2], 0, 8);
  }

  if (h > 0 || t > 0) {
    drawDigit(t, 1);
  } else {
    memset(displayBuffer[1], 0, 8);
  }

  drawDigit(u, 0);
}

void drawStarIfRecord(uint16_t score, uint16_t bestScore) {
  if (score >= bestScore && score > 0) {
    memcpy(displayBuffer[3], star, 8);
  } else {
    memset(displayBuffer[3], 0, 8);
  }
}

// ------------------ ANIMATIONS WIN/LOST ------------------

void displayWinAnimation(int bestScore) {
  static bool visible = true;
  static uint32_t lastBlink = 0;
  
  if (millis() - lastBlink > 300) { // 300ms of blinking
    lastBlink = millis();
    visible = !visible;
    
    clearDisplay();
    
    if (visible) {
      // WIN visible
      // W (matrice 3)
      displayBuffer[3][2] = 0b10001000;
      displayBuffer[3][3] = 0b10001000;
      displayBuffer[3][4] = 0b10101000;
      displayBuffer[3][5] = 0b10101000;
      displayBuffer[3][6] = 0b01010000;
      
      // I (matrice 2)
      displayBuffer[2][2] = 0b11111000;
      displayBuffer[2][3] = 0b00100000;
      displayBuffer[2][4] = 0b00100000;
      displayBuffer[2][5] = 0b00100000;
      displayBuffer[2][6] = 0b11111000;
      
      // N (matrice 1)
      displayBuffer[1][2] = 0b10001000;
      displayBuffer[1][3] = 0b11001000;
      displayBuffer[1][4] = 0b10101000;
      displayBuffer[1][5] = 0b10011000;
      displayBuffer[1][6] = 0b10001000;
    }
    
    for (int i = 0; i < 4; i++) {
      updateMatrix(i);
    }
  }
}

void displayLostAnimation(int bestScore) {
  static bool visible = true;
  static uint32_t lastBlink = 0;
  
  if (millis() - lastBlink > 300) { // 300ms of blinking
    lastBlink = millis();
    visible = !visible;
    
    clearDisplay();
    
    if (visible) {
      // LOST visible
      // L (matrice 3)
      displayBuffer[3][2] = 0b10000000;
      displayBuffer[3][3] = 0b10000000;
      displayBuffer[3][4] = 0b10000000;
      displayBuffer[3][5] = 0b11110000;
      
      // O (matrice 2)
      displayBuffer[2][2] = 0b01110000;
      displayBuffer[2][3] = 0b10001000;
      displayBuffer[2][4] = 0b10001000;
      displayBuffer[2][5] = 0b01110000;
      
      // S (matrice 1)
      displayBuffer[1][2] = 0b01110000;
      displayBuffer[1][3] = 0b10000000;
      displayBuffer[1][4] = 0b01110000;
      displayBuffer[1][5] = 0b00001000;
      displayBuffer[1][6] = 0b11110000;
      
      // T (matrice 0)
      displayBuffer[0][1] = 0b11111110;
      displayBuffer[0][2] = 0b00010000;
      displayBuffer[0][3] = 0b00010000;
      displayBuffer[0][4] = 0b00010000;
      displayBuffer[0][5] = 0b00010000;
      displayBuffer[0][6] = 0b00010000;
    }
    
    for (int i = 0; i < 4; i++) {
      updateMatrix(i);
    }
  }
}

void displayFinalScore(int score, int bestScore, bool isNewRecord) {
  clearDisplay();
  drawScore(score);
  
  if (isNewRecord && score > 0) {
    memcpy(displayBuffer[3], star, 8);
  } else {
    memset(displayBuffer[3], 0, 8);
  }
  
  for (int i = 0; i < 4; i++) {
    updateMatrix(i);
  }
}

// ------------------ MENU ------------------

void displayModeSelectScreenWithSelection(uint8_t selectedMode) {
  currentSelectedMode = selectedMode;
  clearDisplay();
  memcpy(displayBuffer[2], chiffreUn, 8);
  memcpy(displayBuffer[1], chiffreDeux, 8);

  if (selectedMode == 1) {
    setPixel(2, 0, 0, true); setPixel(2, 0, 7, true);
    setPixel(2, 7, 0, true); setPixel(2, 7, 7, true);
  } else {
    setPixel(1, 0, 0, true); setPixel(1, 0, 7, true);
    setPixel(1, 7, 0, true); setPixel(1, 7, 7, true);
  }
  updateMatrix(1);
  updateMatrix(2);
}

void updateModeSelectBlink() {
  static uint32_t lastBlink = 0;
  static bool blinkOn = true;
  if (millis() - lastBlink > 500) {
    lastBlink = millis();
    blinkOn = !blinkOn;

    int devices[2] = {1, 2};
    for (int i = 0; i < 2; i++) {
      int dev = devices[i];
      setPixel(dev, 0, 0, false); setPixel(dev, 0, 7, false);
      setPixel(dev, 7, 0, false); setPixel(dev, 7, 7, false);
    }

    if (blinkOn) {
      if (currentSelectedMode == 1) {
        setPixel(2, 0, 0, true); setPixel(2, 0, 7, true);
        setPixel(2, 7, 0, true); setPixel(2, 7, 7, true);
      } else {
        setPixel(1, 0, 0, true); setPixel(1, 0, 7, true);
        setPixel(1, 7, 0, true); setPixel(1, 7, 7, true);
      }
    }
    updateMatrix(1);
    updateMatrix(2);
  }
}


void displayGameOverScreen(int score, int bestScore) {
  clearDisplay();
  
  // LOST
  displayBuffer[3][2] = 0b10000000;
  displayBuffer[3][3] = 0b10000000;
  displayBuffer[3][4] = 0b10000000;
  displayBuffer[3][5] = 0b11110000;
  
  displayBuffer[2][2] = 0b01110000;
  displayBuffer[2][3] = 0b10001000;
  displayBuffer[2][4] = 0b10001000;
  displayBuffer[2][5] = 0b01110000;
  
  displayBuffer[1][2] = 0b01110000;
  displayBuffer[1][3] = 0b10000000;
  displayBuffer[1][4] = 0b01110000;
  displayBuffer[1][5] = 0b00001000;
  displayBuffer[1][6] = 0b11110000;
  
  displayBuffer[0][1] = 0b11111110;
  displayBuffer[0][2] = 0b00010000;
  displayBuffer[0][3] = 0b00010000;
  displayBuffer[0][4] = 0b00010000;
  displayBuffer[0][5] = 0b00010000;
  displayBuffer[0][6] = 0b00010000;

  // Score
  drawScore(score);
  drawStarIfRecord(score, bestScore);

  for (int i = 0; i < 4; i++) {
    updateMatrix(i);
  }
}

void displayWinScreen(int score, int bestScore) {

  clearDisplay();
  
  // WIN
  displayBuffer[3][2] = 0b10001000;
  displayBuffer[3][3] = 0b10001000;
  displayBuffer[3][4] = 0b10101000;
  displayBuffer[3][5] = 0b10101000;
  displayBuffer[3][6] = 0b01010000;
  
  displayBuffer[2][2] = 0b11111000;
  displayBuffer[2][3] = 0b00100000;
  displayBuffer[2][4] = 0b00100000;
  displayBuffer[2][5] = 0b00100000;
  displayBuffer[2][6] = 0b11111000;
  
  displayBuffer[1][2] = 0b10001000;
  displayBuffer[1][3] = 0b11001000;
  displayBuffer[1][4] = 0b10101000;
  displayBuffer[1][5] = 0b10011000;
  displayBuffer[1][6] = 0b10001000;

  // Score
  drawScore(score);
  drawStarIfRecord(score, bestScore);

  for (int i = 0; i < 4; i++) {
    updateMatrix(i);
  }
}



void displayPauseScreen() {
  clearDisplay();
  displayBuffer[2][1] = 0b00011100;
  displayBuffer[2][2] = 0b00011100;
  displayBuffer[2][3] = 0b00011100;
  displayBuffer[2][4] = 0b00011100;
  displayBuffer[2][5] = 0b00011100;
  displayBuffer[2][6] = 0b00011100;

  displayBuffer[1][1] = 0b00111000;
  displayBuffer[1][2] = 0b00111000;
  displayBuffer[1][3] = 0b00111000;
  displayBuffer[1][4] = 0b00111000;
  displayBuffer[1][5] = 0b00111000;
  displayBuffer[1][6] = 0b00111000;

  updateMatrix(1);
  updateMatrix(2);
}

void displayOnMessage() {
  for (int i = 0; i < 2; i++) {
    clearDisplay();
    displayBuffer[2][0] = B01110000;
    displayBuffer[2][1] = B10001000;
    displayBuffer[2][2] = B10001000;
    displayBuffer[2][3] = B10001000;
    displayBuffer[2][4] = B10001000;
    displayBuffer[2][5] = B10001000;
    displayBuffer[2][6] = B01110000;

    displayBuffer[1][0] = B10001000;
    displayBuffer[1][1] = B10001000;
    displayBuffer[1][2] = B11001000;
    displayBuffer[1][3] = B10101000;
    displayBuffer[1][4] = B10011000;
    displayBuffer[1][5] = B10001000;
    displayBuffer[1][6] = B10001000;

    updateMatrix(1);
    updateMatrix(2);
    delay(250);
    clearDisplay();
    delay(250);
  }
}

void displayOffMessage() {
  for (int i = 0; i < 2; i++) {
    clearDisplay();
    displayBuffer[3][1] = 0b00111100;
    displayBuffer[3][2] = 0b01000010;
    displayBuffer[3][3] = 0b01000010;
    displayBuffer[3][4] = 0b01000010;
    displayBuffer[3][5] = 0b01000010;
    displayBuffer[3][6] = 0b00111100;

    displayBuffer[2][1] = 0b11111000;
    displayBuffer[2][2] = 0b10000000;
    displayBuffer[2][3] = 0b10000000;
    displayBuffer[2][4] = 0b11111000;
    displayBuffer[2][5] = 0b10000000;
    displayBuffer[2][6] = 0b10000000;

    displayBuffer[1][1] = 0b11111000;
    displayBuffer[1][2] = 0b10000000;
    displayBuffer[1][3] = 0b10000000;
    displayBuffer[1][4] = 0b11111000;
    displayBuffer[1][5] = 0b10000000;
    displayBuffer[1][6] = 0b10000000;

    updateMatrix(1);
    updateMatrix(2);
    updateMatrix(3);
    delay(250);
    clearDisplay();
    delay(250);
  }
}