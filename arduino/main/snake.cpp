#include "snake.h"
#include <Arduino.h>

//Initialize initial snake positions and clear the grid.

void initSnake(int16_t snakeX[], int16_t snakeY[], uint16_t &snakeLength, uint8_t grid[ROWS][COLS]) {
  // 1. Clear the entire grid
  for (uint8_t r = 0; r < ROWS; r++)
    for (uint8_t c = 0; c < COLS; c++)
      grid[r][c] = EMPTY;

  // Set initial length and position
  snakeLength = 3;
  
  int16_t centerX = COLS / 2;
  int16_t centerY = ROWS / 2;

  snakeX[0] = centerX;
  snakeY[0] = centerY;
  snakeX[1] = centerX - 1;
  snakeY[1] = centerY;
  snakeX[2] = centerX - 2;
  snakeY[2] = centerY;

  // Mark snake positions in the grid
  for (uint16_t i = 0; i < snakeLength; i++) {
    if (snakeX[i] >= 0 && snakeX[i] < COLS && snakeY[i] >= 0 && snakeY[i] < ROWS) {
      grid[snakeY[i]][snakeX[i]] = SNAKE;
    }
  }
}

/*Handles snake movement, clears the old tail and draws the new head. */
void moveSnake(int16_t snakeX[], int16_t snakeY[], uint16_t &snakeLength,
               uint16_t oldLength, int16_t &dirX, int16_t &dirY,
               uint8_t grid[ROWS][COLS], bool wrapMode,
               int16_t &foodX, int16_t &foodY) { // ADDED foodX/foodY
  
  //Clear the old tail (if length hasn't increased)
  // If the snake ate, the length increased, and the old tail remains (it's now part of the body)
  if (snakeLength == oldLength) {
    int16_t tailX = snakeX[snakeLength - 1];
    int16_t tailY = snakeY[snakeLength - 1];
    
    // Ensure the tail is within bounds before clearing
    if (tailX >= 0 && tailX < COLS && tailY >= 0 && tailY < ROWS) {
      grid[tailY][tailX] = EMPTY;
    }
  }

  // Move the body (shift all segments)
  for (uint16_t i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  //Calculate the new head
  snakeX[0] += dirX;
  snakeY[0] += dirY;

  // Handle wrapping (wrap mode)
  if (wrapMode) {
    if (snakeX[0] < 0) snakeX[0] = COLS - 1;
    if (snakeX[0] >= COLS) snakeX[0] = 0;
    if (snakeY[0] < 0) snakeY[0] = ROWS - 1;
    if (snakeY[0] >= ROWS) snakeY[0] = 0;
  }
  
  //  Draw the new head in the grid
  if (snakeX[0] >= 0 && snakeX[0] < COLS && snakeY[0] >= 0 && snakeY[0] < ROWS) {
    grid[snakeY[0]][snakeX[0]] = SNAKE;
  }
  
  // If the food is within bounds, we mark it again in the grid.
  if (foodX >= 0 && foodX < COLS && foodY >= 0 && foodY < ROWS) {
      grid[foodY][foodX] = FOOD;
  }
}

//Checks for collisions with walls or with the body.

bool checkCollision(const int16_t snakeX[], const int16_t snakeY[], uint16_t snakeLength, bool wallCollision) {
  
  // Collision with walls
  if (wallCollision) {
    if (snakeX[0] < 0 || snakeX[0] >= COLS || snakeY[0] < 0 || snakeY[0] >= ROWS) {
      return true;
    }
  }
  
  // Collision with body (start at i=1)
  for (uint16_t i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }
  
  return false;
}

bool checkFood(int16_t snakeX[], int16_t snakeY[], uint16_t &snakeLength,
               int16_t &foodX, int16_t &foodY, uint8_t grid[ROWS][COLS]) {
  
  // If head is on the food
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    if (snakeLength > MAX_SNAKE_LENGTH) {
      snakeLength = MAX_SNAKE_LENGTH;
    }
    grid[foodY][foodX] = SNAKE; 

    generateFood(foodX, foodY, grid); 

    return true;
  }

  return false;
}

//Places new food in an empty position on the grid.
 
void generateFood(int16_t &foodX, int16_t &foodY, uint8_t grid[ROWS][COLS]) {
  int16_t newFoodX, newFoodY;
  bool found = false;
  uint8_t attempts = 0;
  const uint8_t maxAttempts = 50;

  // Fast random method
  while (!found && attempts < maxAttempts) {
    newFoodX = random(0, COLS);
    newFoodY = random(0, ROWS);

    if (grid[newFoodY][newFoodX] == EMPTY) {
      found = true;
    }
    attempts++;
  }

  // Fallback: systematic search (if grid is almost full)
  if (!found) {
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        if (grid[r][c] == EMPTY) {
          newFoodX = c;
          newFoodY = r;
          found = true;
          break;
        }
      }
      if (found) break;
    }
  }

  // If an empty position is found, update grid and global coordinates
  if (found) {
    foodX = newFoodX;
    foodY = newFoodY;
    
    // *** CRUCIAL POINT: Mark the food in the grid ***
    grid[foodY][foodX] = FOOD; 
  } else {
    // If the grid is full, the game is won (checkWin should handle it)
    foodX = -1; // Mark food as non-existent
    foodY = -1;
  }
}

// Checks if the win condition is met (full grid).
bool checkWin(uint16_t snakeLength) {
    return (snakeLength >= WIN_LENGTH);
}