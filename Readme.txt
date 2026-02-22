# Zuzu - Personal Desktop Companion 🤖

**Zuzu** is an interactive, emotive desktop robot engineered as a custom gift. It uses a "personality-driven" state machine to bridge the gap between simple electronics and social robotics.

---

## 🌟 Project Scope + Core Features
Zuzu is designed to inhabit a workspace, providing emotional interaction through high-fidelity feedback loops.

* **👁️ Active Tracking:** Pan-Tilt ultrasonic array to follow people moving across the desk.
* **😊 Emotive Vision:** 0.96" OLED "Face" rendering real-time eye animations (Happy, Sad, Angry, Sleepy).
* **🖐️ Haptic Intelligence:** Multi-zone capacitive touch sensors (Head Pat = Happy | Side Poke = Angry).
* **🎵 Sonic Identity:** DFPlayer Mini for high-quality chirps, sighs, and custom voice clips.
* **❓ Attention Seeking:** Initiates "Seeker" protocols if an object is detected but remains out of reach.

---

## 🧠 Technical Architecture (The "Brain")
Built on a modular hardware stack optimized for the ATmega328P.

| Logic Layer | Component | Technical Role |
| :--- | :--- | :--- |
| **Microcontroller** | **Arduino Nano V3.0** | Main processing unit; manages state logic. |
| **Visuals** | **SSD1306 OLED** | 128x64 I2C display (Powered by **U8g2 Library**). |
| **Spatial Sensing** | **HC-SR04** | Ultrasonic "Radar" for distance and tracking. |
| **Audio** | **DFPlayer Mini** | MP3-TF-16P Module with 1kΩ resistor on RX line. |

---

## 🔌 Pin Mapping & Wiring
| Component | Arduino Pin | Function |
| :--- | :--- | :--- |
| **OLED SDA** | A4 | I2C Data |
| **OLED SCL** | A5 | I2C Clock |
| **Servo 1 (Pan)** | D3 | PWM Head Rotation |
| **Servo 2 (Tilt)** | D5 | PWM Head Pitch |
| **Servo 3 (L-Arm)** | D6 | PWM Left Arm |
| **Servo 4 (R-Arm)** | D9 | PWM Right Arm |
| **Ultrasonic Trig**| D7 | Trigger Pulse |
| **Ultrasonic Echo**| D8 | Echo Return |
| **Touch (Head)** | D2 | Interrupt Trigger (Happy) |
| **Touch (Side)** | D4 | Grumpy Trigger |

---

## ⚡ Power Architecture (Single Cable Mode)
To prevent "Brown-outs" and ensure stability, Zuzu uses a **Common Ground** split-power design:
1. **Source:** Single 5V 2A USB wall adapter.
2. **Logic Rail:** Powers Arduino Nano, OLED, and Sensors.
3. **Power Rail:** Powers 4x SG90 Servos directly (bypass Arduino regulator).
4. **Common Ground:** All GND pins connected to a single point.

---

## 💰 Bill of Materials (Sri Lanka Market - Feb 2026)
*Estimated local prices from vendors like Tronic.lk, Senith, and Nilambara.*

| ✅ | Component Item | Qty | Unit Price (LKR) | Total (LKR) |
| :--- | :--- | :---: | :---: | :---: |
| 🔲 | Arduino Nano (CH340 Clone) | 1 | 1,050 | 1,050 |
| 🔲 | 0.96" OLED I2C Display | 1 | 590 | 590 |
| 🔲 | SG90 Micro Servo | 4 | 350 | 1,400 |
| 🔲 | Pan-Tilt Mechanical Bracket | 1 | 350 | 350 |
| 🔲 | HC-SR04 Ultrasonic Sensor | 1 | 300 | 300 |
| 🔲 | TTP223 Touch Module | 2 | 100 | 200 |
| 🔲 | DFPlayer Mini Sound Board | 1 | 450 | 450 |
| 🔲 | MB102 Power Module (for testing) | 1 | 250 | 250 |
| 🔲 | Jumper Wires + 830pt Breadboard | 1 | 900 | 900 |
| 🔲 | Small Speaker + 1kΩ Resistor | 1 | 160 | 160 |
| | | | | |
| 🚀 | **ESTIMATED TOTAL** | | | **Rs. 5,650** |

---

## 📂 Repository Structure
* `/src`: Main Arduino `.ino` files and logic.
* `/assets`: MP3 sound files and eye bitmaps.
* `/docs`: Wiring diagrams and assembly photos.
* `/lib`: External library references (U8g2, Servo, DFRobot).