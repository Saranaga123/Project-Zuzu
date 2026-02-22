# Zuzu - Personal Desktop Companion 🤖

**Zuzu** is a personality-driven desktop robot engineered as a customized gift. It bridges the gap between simple electronics and social robotics by using an asynchronous state-machine to react to its environment in real-time.

---

## 🌟 Project Scope + Core Features
* **👁️ Active Tracking:** Uses a **RB0036 Pan-Tilt** ultrasonic array to "look" at and follow objects.
* **😊 Emotive Vision:** 0.96" OLED "Face" rendering real-time expressions (Happy, Bored, Angry, Sleepy).
* **🖐️ Haptic Intelligence:** Capacitive touch sensors for head-pats and side-pokes.
* **🎵 Sonic Identity:** DFPlayer Mini providing high-fidelity emotional audio feedback.
* **🧠 Non-Blocking Logic:** Built on `millis()` timing to allow simultaneous movement, blinking, and sensing.

---

## 🧠 Technical Architecture
| Logic Layer | Component | Technical Role |
| :--- | :--- | :--- |
| **Microcontroller** | **Arduino Nano V3.0** | Main processing unit; manages state logic and I/O. |
| **Visual Interface** | **SSD1306 OLED** | 128x64 I2C display using **U8g2 (Page Mode)**. |
| **Mechanics** | **RB0036 Bracket** | 2-Axis Nylon Pan-Tilt for anatomical "head" movement. |
| **Audio Processing** | **DFPlayer Mini** | MP3-TF-16P Module for localized audio storage. |

---

## 🔌 Pin Mapping & Connectivity
| Component | Arduino Pin | Function |
| :--- | :--- | :--- |
| **OLED SDA** | A4 | I2C Data Lane |
| **OLED SCL** | A5 | I2C Clock Lane |
| **Servo 1 (Pan)** | D3 | PWM Horizontal Rotation (Bracket Base) |
| **Servo 2 (Tilt)** | D5 | PWM Vertical Pitch (Bracket Top) |
| **Servo 3 (L-Arm)** | D6 | PWM Left Arm Actuator |
| **Servo 4 (R-Arm)** | D9 | PWM Right Arm Actuator |
| **Ultrasonic Trig** | D7 | Sonar Pulse Trigger |
| **Ultrasonic Echo** | D8 | Sonar Echo Return |
| **Touch (Head)** | D2 | Interrupt: Happy Interaction |
| **Touch (Side)** | D4 | Grumpy Interaction |
| **DFPlayer RX** | D11 | Serial Transmit (via 1kΩ Resistor) |
| **DFPlayer TX** | D10 | Serial Receive |

---

## ⚡ Power Architecture (Single Cable Mode)
To ensure stability during simultaneous 4-servo movement and prevent MCU resets:
1. **Single 5V 2A Source:** Powered via a standard USB wall adapter.
2. **External Power Rail:** Servos are powered directly via the **MB102 Module** (bypassing the Nano's onboard regulator).
3. **Common Ground:** All GND pins (Nano, Servos, Sensors, Player) are tied to a unified ground bus.

---

## 💰 Bill of Materials (Tronic.lk - Feb 2026)
| ✅ | Item Code | Component Description | Qty | Price (LKR) | Total (LKR) |
| :--- | :--- | :--- | :---: | :---: | :---: |
| 🔲 | **AR0022** | Arduino Nano V3.0 + USB Cable | 1 | 1,050 | 1,050 |
| 🔲 | **DI0025** | 0.96" OLED I2C Display (Blue) | 1 | 590 | 590 |
| 🔲 | **RB0002** | SG90 9g Micro Servo | 4 | 350 | 1,400 |
| 🔲 | **RB0036** | 2-Axis Servo Bracket PT Pan/Tilt | 1 | 270 | 270 |
| 🔲 | **SN0003** | HC-SR04 Ultrasonic Sensor | 1 | 300 | 300 |
| 🔲 | **SN0038** | TTP223 Touch Module | 2 | 100 | 200 |
| 🔲 | **RB0060** | DFPlayer Mini MP3 Player | 1 | 450 | 450 |
| 🔲 | **PW0014** | MB102 Breadboard Power Module | 1 | 250 | 250 |
| 🔲 | - | Jumper Wires + 830pt Breadboard | 1 | 900 | 900 |
| 🔲 | - | Small Speaker + 1kΩ Resistor | 1 | 160 | 160 |
| | | **ESTIMATED TOTAL** | | | **Rs. 5,570** |

---

## 🛠️ Assembly & Software Guidelines
* **Calibration:** Execute the "Center Servo" sketch (90°) before attaching the **RB0036** plastic horns to avoid mechanical binding.
* **Audio Safety:** A **1kΩ resistor** must be used on the D11 line to the DFPlayer RX to prevent signal noise and protect the module.
* **Display:** Use the `u8g2.firstPage()` loop to keep SRAM usage under 40% on the Nano.

---

## 📂 Repository Structure
* `/src`: Main Arduino `.ino` source code.
* `/assets`: MP3 sound library (0001.mp3 to 0005.mp3).
* `/docs`: RB0036 Assembly Guide & Wiring Schematics.
* `/lib`: Required libraries (U8g2, Servo, DFRobotDFPlayerMini).