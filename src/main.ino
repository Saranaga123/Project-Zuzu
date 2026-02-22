/*
 * PROJECT: Zuzu - Personal Desktop Companion
 * Simulation + Real Hardware Compatible Version
 */

// ===============================
// 🔹 Uncomment for Wokwi simulation
// ===============================
#define SIMULATION_MODE

#include <Arduino.h>
#include <U8g2lib.h>
#include <Servo.h>
#include <math.h>

#ifndef SIMULATION_MODE
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#endif

// ---------------- PIN DEFINITIONS ----------------
#define PIN_SERVO_PAN   3
#define PIN_SERVO_TILT  5
#define PIN_SERVO_L_ARM 6
#define PIN_SERVO_R_ARM 9
#define PIN_US_TRIG     7
#define PIN_US_ECHO     8
#define PIN_TOUCH_HEAD  2
#define PIN_TOUCH_SIDE  4

#ifndef SIMULATION_MODE
#define PIN_DF_RX       11
#define PIN_DF_TX       10
SoftwareSerial dfSerial(PIN_DF_TX, PIN_DF_RX);
DFRobotDFPlayerMini myDFPlayer;
#endif

// ---------------- OBJECTS ----------------
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Servo panServo, tiltServo, leftArm, rightArm;

// ---------------- ROBOT STATE ----------------
enum Mood { BORED, HAPPY, ANGRY, TRACKING, SLEEP };
Mood currentMood = BORED;

// ---------------- TIMING ----------------
unsigned long lastBlinkTime = 0;
unsigned long blinkDuration = 150;
unsigned long nextBlinkInterval = 3000;
bool isBlinking = false;

unsigned long moodStartTime = 0;
const unsigned long moodDuration = 5000;

// ---------------- SERVO ----------------
int panCurrent = 90, panTarget = 90;
int tiltCurrent = 90, tiltTarget = 90;
unsigned long lastServoStep = 0;
const int servoSpeed = 20;

// =================================================
// SETUP
// =================================================
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(25, 35, "Zuzu Booting...");
  } while (u8g2.nextPage());

#ifndef SIMULATION_MODE
  dfSerial.begin(9600);
  if (myDFPlayer.begin(dfSerial)) {
    myDFPlayer.volume(20);
    myDFPlayer.play(4);
  }
#endif

  panServo.attach(PIN_SERVO_PAN);
  tiltServo.attach(PIN_SERVO_TILT);
  leftArm.attach(PIN_SERVO_L_ARM);
  rightArm.attach(PIN_SERVO_R_ARM);

  panServo.write(90);
  tiltServo.write(90);
  leftArm.write(0);
  rightArm.write(0);

  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  pinMode(PIN_TOUCH_HEAD, INPUT);
  pinMode(PIN_TOUCH_SIDE, INPUT);

  Serial.println("Zuzu Online");
}

// =================================================
// LOOP
// =================================================
void loop() {
  unsigned long now = millis();

  handleBlink(now);
  checkTouch();
  checkDistance();
  handleMoodTimeout();
  executeMoodActions(now);
  updateDisplay();
}

// =================================================
// BLINK
// =================================================
void handleBlink(unsigned long now) {
  if (!isBlinking && now - lastBlinkTime >= nextBlinkInterval) {
    isBlinking = true;
    lastBlinkTime = now;
    nextBlinkInterval = random(2000, 6000);
  }

  if (isBlinking && now - lastBlinkTime >= blinkDuration) {
    isBlinking = false;
  }
}

// =================================================
// DISPLAY
// =================================================
void updateDisplay() {
  u8g2.firstPage();
  do {

    if (isBlinking || currentMood == SLEEP) {
      u8g2.drawHLine(32, 35, 20);
      u8g2.drawHLine(76, 35, 20);
    }
    else if (currentMood == HAPPY) {
      u8g2.drawCircle(42, 40, 12,
        U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);
      u8g2.drawCircle(86, 40, 12,
        U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);
    }
    else if (currentMood == ANGRY) {
      u8g2.drawTriangle(30, 30, 52, 45, 30, 50);
      u8g2.drawTriangle(98, 30, 76, 45, 98, 50);
    }
    else {
      u8g2.drawFilledEllipse(42, 35, 10, 15);
      u8g2.drawFilledEllipse(86, 35, 10, 15);
    }

  } while (u8g2.nextPage());
}

// =================================================
// TOUCH
// =================================================
void checkTouch() {
  static bool lastHeadState = LOW;
  static bool lastSideState = LOW;

  bool headState = digitalRead(PIN_TOUCH_HEAD);
  bool sideState = digitalRead(PIN_TOUCH_SIDE);

  // HEAD button pressed (edge detection)
  if (headState == HIGH && lastHeadState == LOW) {
    currentMood = HAPPY;
    moodStartTime = millis();
#ifndef SIMULATION_MODE
    myDFPlayer.play(1);
#endif
    Serial.println("Mood: HAPPY");
  }

  // SIDE button pressed (edge detection)
  if (sideState == HIGH && lastSideState == LOW) {
    currentMood = ANGRY;
    moodStartTime = millis();
#ifndef SIMULATION_MODE
    myDFPlayer.play(2);
#endif
    Serial.println("Mood: ANGRY");
  }

  lastHeadState = headState;
  lastSideState = sideState;
}

// =================================================
// ULTRASONIC
// =================================================
void checkDistance() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 200) return;
  lastCheck = millis();

  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);

  long duration = pulseIn(PIN_US_ECHO, HIGH, 20000);
  if (duration == 0) return;

  float distance = (duration * 0.0343) / 2;

  if (distance > 0 && distance < 30) {
    currentMood = TRACKING;
  }
  else if (distance > 60 && currentMood == TRACKING) {
    currentMood = BORED;
  }
}

// =================================================
// MOOD TIMEOUT
// =================================================
void handleMoodTimeout() {
  if ((currentMood == HAPPY || currentMood == ANGRY) &&
      millis() - moodStartTime > moodDuration) {
    currentMood = BORED;
  }
}

// =================================================
// SERVO + ARMS
// =================================================
void executeMoodActions(unsigned long now) {

  if (now - lastServoStep < servoSpeed) return;
  lastServoStep = now;

  static int scanDir = 1;

  switch (currentMood) {

    case TRACKING:
      tiltTarget = 110;
      panTarget += scanDir;
      if (panTarget > 120 || panTarget < 60) scanDir *= -1;
      leftArm.write(0);
      rightArm.write(0);
      break;

    case HAPPY:
      panTarget = 90 + 5 * sin(now * 0.002);
      tiltTarget = 90;
      leftArm.write(30 + 20 * sin(now * 0.005));
      rightArm.write(30 + 20 * sin(now * 0.005));
      break;

    case ANGRY:
      panTarget = 150;
      tiltTarget = 80;
      leftArm.write(90);
      rightArm.write(90);
      break;

    default:
      panTarget = 90;
      tiltTarget = 90;
      leftArm.write(0);
      rightArm.write(0);
      break;
  }

  panTarget = constrain(panTarget, 0, 180);
  tiltTarget = constrain(tiltTarget, 0, 180);

  if (panCurrent < panTarget) panCurrent++;
  if (panCurrent > panTarget) panCurrent--;
  if (tiltCurrent < tiltTarget) tiltCurrent++;
  if (tiltCurrent > tiltTarget) tiltCurrent--;

  panServo.write(panCurrent);
  tiltServo.write(tiltCurrent);
}