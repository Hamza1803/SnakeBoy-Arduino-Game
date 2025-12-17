#ifndef SNAKE_H
#define SNAKE_H

#include "display.h"

#define SNAKE_SPEED_EASY 250
#define SNAKE_SPEED_HARD 150
#define GRID_TOTAL_CELLS (ROWS * COLS)
#define WIN_LENGTH (GRID_TOTAL_CELLS - 1)
extern int16_t foodX, foodY;
void initSnake(int16_t snakeX[], int16_t snakeY[], uint16_t &snakeLength, uint8_t grid[ROWS][COLS]);
void moveSnake(int16_t snakeX[], int16_t snakeY[], uint16_t &snakeLength,
               uint16_t oldLength, int16_t &dirX, int16_t &dirY,
               uint8_t grid[ROWS][COLS], bool wrapMode,
               int16_t &foodX, int16_t &foodY); // AJOUTÉ
bool checkCollision(const int16_t snakeX[], const int16_t snakeY[], uint16_t snakeLength, bool wallCollision);
bool checkFood(int16_t snakeX[], int16_t snakeY[], uint16_t &snakeLength,
               int16_t &foodX, int16_t &foodY, uint8_t grid[ROWS][COLS]);
void generateFood(int16_t &foodX, int16_t &foodY, uint8_t grid[ROWS][COLS]);
bool checkWin(uint16_t snakeLength);

#endif