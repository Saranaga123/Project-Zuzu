/*
 * PROJECT: Zuzu - Personal Desktop Companion
 * AUTHOR: [Your Name]
 * DESCRIPTION: Emotive robot with tracking, touch, and audio feedback.
 * HARDWARE: Arduino Nano, OLED SSD1306 (U8g2), 4x Servos, HC-SR04, DFPlayer.
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini myDFPlayer;

// --- PIN DEFINITIONS ---
#define PIN_EYES_SDA    A4
#define PIN_EYES_SCL    A5
#define PIN_SERVO_PAN   3
#define PIN_SERVO_TILT  5
#define PIN_SERVO_L_ARM 6
#define PIN_SERVO_R_ARM 9
#define PIN_US_TRIG     7
#define PIN_US_ECHO     8
#define PIN_TOUCH_HEAD  2
#define PIN_TOUCH_SIDE  4
#define PIN_DF_RX       11
#define PIN_DF_TX       10

// --- OBJECT INITIALIZATION ---
// Using U8g2 Page Mode to save SRAM for Zuzu's logic
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

Servo panServo, tiltServo, leftArm, rightArm;
SoftwareSerial dfSerial(PIN_DF_TX, PIN_DF_RX); // RX, TX

// --- ROBOT STATE ---
enum Mood { BORED, HAPPY, ANGRY, TRACKING, SLEEP };
Mood currentMood = BORED;

void setup() {
  Serial.begin(9600);
  
  // Initialize Display
  u8g2.begin();
  
  // Initialize Servos (Center them)
  panServo.attach(PIN_SERVO_PAN);
  tiltServo.attach(PIN_SERVO_TILT);
  leftArm.attach(PIN_SERVO_L_ARM);
  rightArm.attach(PIN_SERVO_R_ARM);
  
  panServo.write(90);   // Center
  tiltServo.write(90);  // Center
  leftArm.write(0);     // Arms down
  rightArm.write(0);    // Arms down

  // Initialize Sensors
  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  pinMode(PIN_TOUCH_HEAD, INPUT);
  pinMode(PIN_TOUCH_SIDE, INPUT);

  // Initialize Audio
  dfSerial.begin(9600);
  
  Serial.println("Zuzu System Online...");
  dfSerial.begin(9600);
  
  if (!myDFPlayer.begin(dfSerial)) {
    Serial.println(F("DFPlayer Error: Check SD Card/Wiring"));
  } else {
    myDFPlayer.volume(20); // Set volume (0 to 30)
    myDFPlayer.play(4);   // Play "Hello!" on startup
  }
}

// --- TIMING VARIABLES ---
unsigned long lastBlinkTime = 0;
unsigned long blinkDuration = 150;     // How long eyes stay shut (ms)
unsigned long nextBlinkInterval = 3000; // Time between blinks (ms)
bool isBlinking = false;

void loop() {
  unsigned long currentMillis = millis();

  // 1. Check if it's time to START a blink
  if (!isBlinking && (currentMillis - lastBlinkTime >= nextBlinkInterval)) {
    isBlinking = true;
    lastBlinkTime = currentMillis;
    nextBlinkInterval = random(2000, 6000); // Randomize next blink for "life"
  }

  // 2. Check if it's time to END the blink
  if (isBlinking && (currentMillis - lastBlinkTime >= blinkDuration)) {
    isBlinking = false;
  }

  // 3. Regular Sensor/Servo Tasks (These never stop!)
  checkTouch();
  checkDistance();
  updateDisplay(); // Now draws either OPEN or CLOSED eyes
  executeMoodActions();
}

void updateDisplay() {
  u8g2.firstPage();
  do {
    if (isBlinking) {
      // Draw CLOSED eyes (just two flat lines)
      u8g2.drawHLine(30, 35, 25); 
      u8g2.drawHLine(73, 35, 25);
    } else {
      // Draw eyes based on Mood (e.g., HAPPY)
      drawHappyEyes(); 
    }
  } while (u8g2.nextPage());
}

void checkTouch() { /* Logic for TTP223 */ }
void checkDistance() { /* Logic for HC-SR04 */ }
void executeMoodActions() { /* Logic for Servos/Audio */ }

void updateDisplay() {
  u8g2.firstPage();
  do {
    if (currentMood == HAPPY) {
      // Draw "Happy" Eyes (Upside down U-shapes / Arches)
      u8g2.drawFrame(30, 30, 20, 10); // Left Eye Base
      u8g2.drawFrame(78, 30, 20, 10); // Right Eye Base
      // We can add "Cheek" lines or heart icons here!
      u8g2.drawDisc(64, 50, 5);       // A little 'mouth' or nose
    } 
    else if (currentMood == BORED) {
      // Draw "Bored" Eyes (Simple flat horizontal rectangles)
      u8g2.drawBox(30, 35, 25, 8);  // Left Eye
      u8g2.drawBox(73, 35, 25, 8);  // Right Eye
    }
    // Add more moods like ANGRY or SLEEP here...
  } while (u8g2.nextPage());
}

// --- SENSOR LOGIC ---

void checkTouch() {
  // Check the Head sensor (Pin D2)
  if (digitalRead(PIN_TOUCH_HEAD) == HIGH) {
    if (currentMood != HAPPY) {
       currentMood = HAPPY;
       myDFPlayer.play(1); // Play Happy Chirp
    }
  }

  if (digitalRead(PIN_TOUCH_SIDE) == HIGH) {
    if (currentMood != ANGRY) {
       currentMood = ANGRY;
       myDFPlayer.play(2); // Play Grumpy Sigh
    }
  }
}

void checkDistance() {
  // We use a non-blocking check every 200ms to save CPU power
  static unsigned long lastUSCheck = 0;
  if (millis() - lastUSCheck < 200) return; 
  lastUSCheck = millis();

  long duration;
  float distance;

  // Trigger the sonic pulse
  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);

  // Read the bounce back (Echo)
  duration = pulseIn(PIN_US_ECHO, HIGH, 20000); // 20ms timeout
  distance = (duration * 0.0343) / 2;

  if (distance > 0 && distance < 20) {
    currentMood = TRACKING; 
    Serial.print(F("Object detected at: "));
    Serial.println(distance);
  } else if (distance > 50) {
    currentMood = BORED; // Person walked away
  }
}
// --- SERVO VARIABLES ---
int panCurrent = 90;
int panTarget = 90;
int tiltCurrent = 90;
int tiltTarget = 90;
unsigned long lastServoStep = 0;
const int servoSpeed = 25; // Lower is faster (ms per degree)

void executeMoodActions() {
  unsigned long currentMillis = millis();

  // Only move the servos every 'servoSpeed' milliseconds
  if (currentMillis - lastServoStep >= servoSpeed) {
    lastServoStep = currentMillis;

    // --- TRACKING LOGIC ---
    if (currentMood == TRACKING) {
      // If object is close, look slightly down
      tiltTarget = 110; 
      // Zuzu can "scan" left/right slightly while tracking
      static int scanDir = 1;
      panTarget += scanDir;
      if (panTarget > 120 || panTarget < 60) scanDir *= -1;
    } 
    else if (currentMood == HAPPY) {
      // Wiggle head when happy
      panTarget = 90 + (5 * sin(currentMillis / 100)); 
    }
    else {
      // Default: Return to center
      panTarget = 90;
      tiltTarget = 90;
    }

    // --- APPLY SMOOTH MOVEMENT ---
    if (panCurrent < panTarget) panCurrent++;
    if (panCurrent > panTarget) panCurrent--;
    if (tiltCurrent < tiltTarget) tiltCurrent++;
    if (tiltCurrent > tiltTarget) tiltCurrent--;

    panServo.write(panCurrent);
    tiltServo.write(tiltCurrent);
  }
}