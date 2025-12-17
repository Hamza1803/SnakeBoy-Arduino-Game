# 🐍 SnakeBoy – An Arduino-Powered Snake Game Console

![IMG_2565](https://github.com/user-attachments/assets/c807cfd7-ba09-4555-bc9e-a67e660109d3)


A handheld, retro-inspired Snake game console built with Arduino and open-source hardware. SnakeBoy combines embedded programming, hardware optimization, and mechanical design into an accessible and highly optimized gaming device.

**Developed as part of the Creative Electronics course in the Electronics Engineering program at the University of Málaga.**

---

## 📋 Overview

SnakeBoy is a handheld console featuring the classic Snake game displayed on a 32×8 LED matrix. Built around an Arduino Leonardo-compatible board (Save Maker I), this project demonstrates practical embedded systems design with real-time gameplay, persistent high score storage, and optimized resource utilization.

### 🎯 Design Goals
- Demonstrate embedded programming and hardware optimization techniques
- Create a fully functional gaming device using accessible components
- Utilize open-source tools and methodologies
- Achieve minimal resource footprint (<15 KB Flash, ~2. 5 KB RAM)

---

## 🕹️ Key Features

- **Real-time Gameplay**:  Responsive joystick and directional button controls
- **Audio Feedback**: 6 distinct melodies via PWM buzzer
- **Persistent Storage**:  High scores saved in EEPROM memory
- **Efficient Architecture**: State-machine-driven game logic with zero blocking delays
- **Compact Footprint**: <15 KB Flash / ~2.5 KB RAM usage — highly optimized
- **Custom Graphics**: 7×5 pixel font and smooth menu animations
- **Custom Enclosure**: 3D-printed case with precision-engineered component cutouts
- **Status Indicator**: Visual feedback LED for game states
- **USB Powered**: Connected operation via 5V/1A USB power supply

---

## 📦 Hardware Specifications

### Core Components

| Component | Specification |
|-----------|---------------|
| **Microcontroller** | Save Maker I (Arduino Leonardo-compatible) |
| **Display** | 4× cascaded MAX7219 8×8 LED matrices (32×8 total resolution) |
| **Input:  Analog** | PS4-style analog joystick with integrated L3 push button |
| **Input: Digital** | 4× tactile push buttons (Up, Down, Left, Right) |
| **Audio Output** | Passive buzzer with PWM modulation |
| **Visual Indicator** | Red 5mm status LED |
| **Power Supply** | External 5V/1A USB power supply (wired connection) |

### Important Notes
⚠️ **LED Matrix Power**:  The LED matrix **must be powered externally via 5V** with a **shared ground connection** to prevent flickering and signal instability.

---

## 🔧 Pinout Configuration

| Component | Arduino Pin | Function |
|-----------|------------|----------|
| MAX7219 Data | D13 | Serial data input |
| MAX7219 Clock | D11 | Clock signal |
| MAX7219 Load | D10 | Latch/load signal |
| Buzzer | D12 | PWM audio output |
| Status LED | D7 | Visual state indicator |
| Joystick X-Axis | A0 | Analog X input |
| Joystick Y-Axis | A1 | Analog Y input |
| Joystick Button (L3) | D9 | Push button input |
| Up Button | D8 | Directional input |
| Down Button | D4 | Directional input |
| Left Button | D5 | Directional input |
| Right Button | D6 | Directional input |

**Note**: All digital buttons utilize the internal `INPUT_PULLUP` feature, eliminating the need for external pull-up resistors.

---

## 📁 Repository Structure

```
SnakeBoy-Arduino-Game/
├── src/
│   └── main/
│   │   ├── snake. cpp           # Main game logic and state machine
│   │   ├── display.cpp         # LED matrix driver and rendering
│   │   ├── inputs.cpp          # Joystick and button input handling
│   │   ├── sound.cpp           # Audio synthesis and melody management
│   │   ├── led. cpp             # Status LED control
│   │   └── config.h            # Hardware configuration and pin mappings
│   └── test/
│   │   └── test_display.txt
│   │   └── test_input.txt
│   │   └── test_led_buzzer.txt
├── enclosure/
│   ├── case/               # OpenSCAD 3D models (parametric design)
│   │   ├── front_panel.scad
│   │   ├── back_panel.scad
│   │   └── button. scad
│   │   └── button. scad
│   │   └── button. scad
│   └── diagrams/           # Wiring and schematic diagrams
├── README.md               # This file
└── LICENSE                 # CC BY-NC-SA 4.0 License
```

---

## 🛠️ Getting Started

### Prerequisites
- Arduino IDE 1.8.0 or later
- Arduino Leonardo board or compatible (Save Maker I)
- MAX7219 LED matrix module (32×8)
- Joystick and push buttons
- Buzzer and status LED
- 5V/1A USB power supply

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/Hamza1803/SnakeBoy-Arduino-Game. git
   ```

2. **Install dependencies**
   - Download the `LedControl` library in Arduino IDE:  `Sketch → Include Library → Manage Libraries` → Search "LedControl"

3. **Configure hardware**
   - Review `config.h` and verify pin mappings match your setup
   - Ensure external 5V power is connected to the LED matrix

4. **Upload firmware**
   - Connect your Arduino Leonardo board via USB
   - Open `snake.cpp` in Arduino IDE
   - Select Board: Arduino Leonardo
   - Click Upload

5. **Connect power**
   - Connect the 5V/1A USB power supply to the designated connector
   - The device should power on automatically

---

## 🎮 Gameplay Instructions

- **Move**: Use joystick or directional buttons (Up, Down, Left, Right)
- **Start Game**: Press any button or move joystick
- **Pause**: Press L3 button (joystick button)
- **Game Over**:  Collision with wall or self triggers game over
- **High Score**:  Automatically saved to EEPROM

---

## 👥 Contributors

**Project Team**:  
- **Hamza Lotfi** – Hardware design & firmware development
- **Andreea Simion** – Mechanical design & testing

**Educational Institution**: 
Degree in Electronic Systems Engineering
University of Málaga, School of Telecommunications

**Special Thanks**:
- Course Instructors: Luis, Arcadio, and Paco
- Inspiration and mentorship:  Max Nelson, Luke Van Scherrenburg, and Sean Nussdorfer (California State University Maritime Academy)

---

## 📖 Additional Resources

- **Full Tutorial**: [SnakeBoy – An Arduino-Powered Snake Game Console](https://www.instructables.com/SnakeBoy-an-Arduino-Powered-Snake-Game-Console/) on Instructables
- **Arduino Leonardo Documentation**: [arduino. cc/reference](https://www.arduino.cc/reference/en/)
- **MAX7219 Datasheet**: Available from component suppliers

---

## ⚖️ License

This project is licensed under the **Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)** license.

**You are free to**:
- Share and adapt the material
- Use for educational purposes

**Under the terms that you**:
- Provide attribution to the original creators
- Do not use commercially
- Share derivative works under the same license

For complete license details, see the [LICENSE](./LICENSE) file or visit [creativecommons.org](https://creativecommons.org/licenses/by-nc-sa/4.0/).

---

## 🐛 Troubleshooting

### LED Matrix not displaying
- Verify external 5V power is connected with shared ground
- Check MAX7219 pin connections (DATA, CLK, LOAD)
- Test with the LedControl example sketch

### Joystick not responding
- Calibrate analog input in `inputs.cpp`
- Verify A0 and A1 connections
- Check for loose wiring

### Sound not working
- Verify buzzer polarity and D12 connection
- Check that `sound.cpp` is compiled

For more detailed troubleshooting, see `docs/troubleshooting.md`.

---

**Made with ❤️ by the SnakeBoy team**
