// sound.h
// Declarations for managing the buzzer

#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>

// Hardware configuration for buzzer
#define BUZZER_PIN 12

// --- Constants for musical notes (in Hz) ---
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988

// Note durations reduced by half
#define DURATION_SHORT 80    
#define DURATION_MEDIUM 150  
#define DURATION_LONG 200    

// Sound function declarations
void initSound();                  // Initialize buzzer
void playEatSound();               // Sound when snake eats fruit
void playStartSound();             // Sound at game start
void playLoseSound();              // Game over sound
void playWinSound();               // Easy mode victory sound
void playHardWinSound();           // Hard mode victory sound
void setSoundVolume(uint8_t volume); // Optional: change volume
void playSnakeIntroTheme(); 
#endif // SOUND_H