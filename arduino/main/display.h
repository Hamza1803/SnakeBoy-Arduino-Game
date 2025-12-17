// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "MD_MAX72xx.h"

#define ROWS 8
#define COLS 32
#define SNAKE 1
#define FOOD 2
#define EMPTY 0
#define MAX_SNAKE_LENGTH 256

#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
#define BLINK_SPEED 500

extern uint8_t currentSelectedMode;
extern MD_MAX72XX mx;
extern bool isNewRecord;

// --- fonctions ---
void initDisplay();
void clearDisplay();
void updateDisplay(uint8_t grid[ROWS][COLS]);
void displayWelcomeScreen();
void displayPlayBlinkScreen();
void updatePlayBlink();
void displayModeSelectScreenWithSelection(uint8_t selectedMode);
void updateModeSelectBlink();
void displayPauseScreen();
void displayOnMessage();
void displayOffMessage();

// --- Fonctions score ---
void displayGameOverScreen(int score, int bestScore);
void displayWinScreen(int score, int bestScore);
void drawScore(uint16_t score);
void drawStarIfRecord(uint16_t score, uint16_t bestScore);

// --- New fonctions animation ---
void displayWinAnimation(int bestScore);
void displayLostAnimation(int bestScore);
void displayFinalScore(int score, int bestScore, bool isNewRecord);

#endif