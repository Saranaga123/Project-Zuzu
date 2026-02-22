# Zuzu - Personal Desktop Companion

## 🌟 Project Scope + Core Features
Zuzu is a customized, interactive desktop companion robot engineered as a high-tech gift. It is designed to inhabit a workspace, providing emotional interaction through a "personality-driven" state machine.

* **👁️ Active Tracking:** Uses a Pan-Tilt ultrasonic array to "look" at and follow people as they move across the desk.
* **😊 Emotive Vision:** A 0.96" OLED "Face" capable of rendering real-time eye animations based on internal "mood" variables.
* **🖐️ Haptic Intelligence:** Multi-zone capacitive touch sensors allow Zuzu to differentiate between a gentle head pat (Happy) and a poke (Angry).
* **🎵 Sonic Identity:** Integrated MP3 module for high-fidelity audio feedback (chirps, sighs, and custom voice clips).
* **❓ Attention Seeking:** If an object is detected but remains out of reach, Zuzu will initiate "Seeker" protocols to get noticed.

---

## 🧠 The "Brain" and Components
Zuzu’s architecture is built on a Modular Hardware Stack, ensuring each sensory input is handled with minimal latency.

| Logic Layer | Component | Technical Role |
| :--- | :--- | :--- |
| **Microcontroller** | **Arduino Nano V3.0** | Main processing unit; manages state logic and I/O. |
| **Visual Interface** | **SSD1306 OLED** | 128x64 I2C display for rendering eye animations. |
| **Spatial Sensing** | **HC-SR04** | Ultrasonic "Radar" for distance and object tracking. |
| **Tactile Input** | **TTP223 Sensor** | Capacitive touch triggers for haptic interaction. |

---

## 🛒 Shopping List (High-Quality Prototype)
| Component Category | Item Description | Specification |
| :--- | :--- | :--- |
| **Core Logic** | Arduino Nano V3.0 | ATmega328P with USB Mini |
| **Face/Display** | 0.96" I2C OLED | SSD1306 (Blue/White) |
| **Motion/Arms** | 4x SG90 Servos | 9g Micro Servos |
| **Sensors** | HC-SR04 + 2x TTP223 | Proximity + Capacitive Touch |
| **Audio** | DFPlayer Mini + Speaker | MP3 Module + 8Ω 0.5W Speaker |
| **Structure** | Pan-Tilt Bracket | FPV Nylon mini bracket |
| **Electrical** | 1k Ohm Resistor | For DFPlayer RX noise reduction |

---

## 💰 Hardware Specifications (Sri Lanka Market - 2026)
*Estimated prices based on local vendors (Tronic.lk, Alphatronic, Nilambara).*

| ✅ | Component Item | Qty | Price (LKR) | Total (LKR) |
| :--- | :--- | :---: | :---: | :---: |
| 🔲 | **Arduino Nano (Compatible)** | 1 | 1,050 | 1,050 |
| 🔲 | **0.96" OLED I2C Display** | 1 | 590 | 590 |
| 🔲 | **SG90 Micro Servo** | 4 | 350 | 1,400 |
| 🔲 | **Pan-Tilt Mechanical Bracket** | 1 | 350 | 350 |
| 🔲 | **HC-SR04 Ultrasonic Sensor** | 1 | 300 | 300 |
| 🔲 | **TTP223 Touch Module** | 2 | 100 | 200 |
| 🔲 | **DFPlayer Mini Sound Board** | 1 | 510 | 510 |
| 🔲 | **8-Ohm Internal Speaker** | 1 | 150 | 150 |
| 🔲 | **Jumper Wires + Breadboard** | 1 | 700 | 700 |
| | | | | |
| 🚀 | **ESTIMATED PROJECT TOTAL** | | | **Rs. 5,250** |