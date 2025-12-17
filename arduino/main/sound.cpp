// sound.cpp
// Definitions for managing the buzzer

#include "sound.h"

// Variable for volume (PWM)
uint8_t currentVolume = 128; // 50% volume (128/255)

// Helper function to play a note with controlled volume
void playNote(uint16_t frequency, uint16_t duration) {
  analogWrite(BUZZER_PIN, currentVolume); // Apply volume
  tone(BUZZER_PIN, frequency, duration);
  delay(duration);
  noTone(BUZZER_PIN);
  analogWrite(BUZZER_PIN, 0); // Cut sound
}

// Helper function to play a note without delay (for sequences)
void playNoteNoDelay(uint16_t frequency, uint16_t duration) {
  analogWrite(BUZZER_PIN, currentVolume);
  tone(BUZZER_PIN, frequency, duration);
}

void initSound() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  // No need for analogWrite here, we do it in each function
}

void playEatSound() {
  // Short sound when snake eats fruit
  playNote(NOTE_E5, 60);  // Short high note
  delay(30);
  playNote(NOTE_G5, 40);  // Second even higher note
}

void playStartSound() {
  // 3 ascending tones with reduced volume
  playNote(NOTE_C4, 80);  // Reduced duration
  delay(90);               // Reduced delay
  
  playNote(NOTE_E4, 80);
  delay(90);
  
  playNote(NOTE_G4, 80);
}

void playLoseSound() {
  // Descending tones with reduced volume
  playNote(NOTE_G4, 150);
  delay(160);
  
  playNote(NOTE_E4, 150);
  delay(160);
  
  playNote(NOTE_C4, 200);
}

void playWinSound() {
  // Easy melody with reduced volume
  playNote(NOTE_C4, 80);
  delay(90);
  
  playNote(NOTE_E4, 80);
  delay(90);
  
  playNote(NOTE_G4, 80);
  delay(90);
  
  playNote(NOTE_C5, 150);
  delay(160);
  
  playNote(NOTE_G4, 80);
  delay(90);
  
  playNote(NOTE_C5, 200);
}

void playHardWinSound() {
  // Hard melody with reduced volume
  playNote(NOTE_E5, 60);
  delay(70);
  
  playNote(NOTE_G5, 60);
  delay(70);
  
  playNote(NOTE_B5, 60);
  delay(70);
  
  playNote(NOTE_E5, 60);
  delay(70);
  
  playNote(NOTE_G5, 60);
  delay(70);
  
  playNote(NOTE_B5, 150);
}

// Optional: Function to change volume if needed
void setSoundVolume(uint8_t volume) {
  currentVolume = volume; // 0-255, 128 = 50%
}

// Add this new function to sound.cpp
void playSnakeIntroTheme() {
  // Melody more adapted to Snake with your functions
  playNote(NOTE_C4, 150);
  delay(50);
  playNote(NOTE_E4, 150);
  delay(50);
  playNote(NOTE_G4, 100);
  delay(30);
  playNote(NOTE_A4, 200);
  delay(100);
  playNote(NOTE_G4, 80);
  delay(30);
  playNote(NOTE_E4, 80);
  delay(30);
  playNote(NOTE_C4, 250);
}