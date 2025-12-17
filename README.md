🐍 SnakeBoy – An Arduino-Powered Snake Game Console
This repository contains the source code, 3D models, wiring diagrams, and documentation for SnakeBoy, a handheld Snake game console developed as part of the Creative Electronics course in the Electronic Systems Engineering degree at the University of Málaga (UMA).

The project was developed to demonstrate embedded programming, hardware optimization, and mechanical integration using accessible components and open-source tools.

📁 Project Structure

![IMG_2565](https://github.com/user-attachments/assets/375ce58e-87b6-4d63-b930-4513596db9c1)


🕹️ Main Project: SnakeBoy
Core Concept: A retro-inspired, battery-free handheld console that plays the classic Snake game on a 32×8 LED matrix, using an Arduino Leonardo-compatible board (Save Maker I).

🔧 Key Features
Real-time gameplay with joystick + 4 directional buttons
Sound feedback via PWM buzzer (6 melodies)
Persistent high score stored in EEPROM
State-machine-driven game logic (no delay())
<15 KB Flash / ~2.5 KB RAM usage — highly optimized
Custom 7×5 pixel font and menu animations
3D-printed enclosure with precise cutouts for all components

📦 Hardware Used
Save Maker I (Arduino Leonardo-compatible)
4× cascaded MAX7219 8×8 LED matrices (32×8 total)
PS4-style analog joystick (with L3 button)
4× tactile push buttons
Red 5mm status LED
Passive buzzer
External 5V/1A USB power supply
⚠️ Important: The LED matrix requires external 5V power with shared ground to avoid flickering and instability.

🔧 Pinout Configuration
|-------------------|-------------|------------------------------|
| Component         | Arduino Pin | Function                     |
|-------------------|-------------|------------------------------|
| MAX7219 DATA      | D13         | Serial data input            |
| MAX7219 CLK       | D11         | Clock signal                 |
| MAX7219 LOAD      | D10         | Latch/load signal            |
| Buzzer            | D12         | PWM audio output             |
| Status LED        | D7          | Visual indicator             |
| Joystick X        | A0          | X-axis analog input          |
| Joystick Y        | A1          | Y-axis analog input          |
| L3 Button         | D9          | Joystick push button         |
| Up Button         | D8          | Directional input            |
| Down Button       | D4          | Directional input            |
| Left Button       | D5          | Directional input            |
| Right Button      | D6          | Directional input            |
|-------------------|-------------|------------------------------|
Note: All digital buttons use the internal INPUT_PULLUP feature, eliminating the need for external resistors.

📚 Contents
This repository includes:

Modular Arduino firmware (snake.cpp, display.cpp, inputs.cpp, sound.cpp, led.cpp)
OpenSCAD parametric case design (front + back panels + button)
Diagrams
Tests
Project documentation and photos

👥 Contributors
SnakeBoy was created by:

Hamza Lotfi
Andreea Simion
Both are students in the Degree in Electronic Systems Engineering at the University of Málaga, School of Telecommunications.

Special thanks to instructors Luis, Arcadio, and Paco, and to Max Nelson, Luke Van Scherrenburg, and Sean Nussdorfer (California State University Maritime Academy) for their inspiring Instructables project.

🔗 External Links

- 📖 Full Instructables Tutorial: [SnakeBoy – An Arduino-Powered Snake Game Console](https://www.instructables.com/SnakeBoy-an-Arduino-Powered-Snake-Game-Console/)
  
⭐️ License
Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
