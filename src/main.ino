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

// --- PIN DEFINITIONS ---
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
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Servo panServo, tiltServo, leftArm, rightArm;
SoftwareSerial dfSerial(PIN_DF_TX, PIN_DF_RX); 
DFRobotDFPlayerMini myDFPlayer;

// --- ROBOT STATE ---
enum Mood { BORED, HAPPY, ANGRY, TRACKING, SLEEP };
Mood currentMood = BORED;

// --- TIMING & SERVO VARIABLES ---
unsigned long lastBlinkTime = 0;
unsigned long blinkDuration = 150;
unsigned long nextBlinkInterval = 3000;
bool isBlinking = false;

int panCurrent = 90, panTarget = 90;
int tiltCurrent = 90, tiltTarget = 90;
unsigned long lastServoStep = 0;
const int servoSpeed = 25; 

void setup() {
  Serial.begin(9600);
  
  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(30, 35, "Zuzu Booting...");
  } while (u8g2.nextPage());

  dfSerial.begin(9600);
  if (!myDFPlayer.begin(dfSerial)) {
    Serial.println(F("DFPlayer Error: Check SD Card"));
  } else {
    myDFPlayer.volume(20); 
    myDFPlayer.play(4); // Startup Sound
  }

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

  Serial.println(F("Zuzu Online"));
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Blink Logic
  if (!isBlinking && (currentMillis - lastBlinkTime >= nextBlinkInterval)) {
    isBlinking = true;
    lastBlinkTime = currentMillis;
    nextBlinkInterval = random(2000, 6000);
  }
  if (isBlinking && (currentMillis - lastBlinkTime >= blinkDuration)) {
    isBlinking = false;
  }

  // 2. Task Execution
  checkTouch();
  checkDistance();
  updateDisplay(); 
  executeMoodActions();
}

void updateDisplay() {
  u8g2.firstPage();
  do {
    if (isBlinking || currentMood == SLEEP) {
      u8g2.drawHLine(35, 35, 20); // Closed Left
      u8g2.drawHLine(73, 35, 20); // Closed Right
    } 
    else if (currentMood == HAPPY) {
      u8g2.drawFrame(30, 30, 20, 10); 
      u8g2.drawFrame(78, 30, 20, 10);
      u8g2.drawDisc(64, 50, 3); 
    } 
    else if (currentMood == ANGRY) {
      u8g2.drawTriangle(30,30, 50,35, 30,40); // Angry Left
      u8g2.drawTriangle(98,30, 78,35, 98,40); // Angry Right
    }
    else { // BORED or TRACKING
      u8g2.drawBox(30, 35, 20, 8); 
      u8g2.drawBox(78, 35, 20, 8);
    }
  } while (u8g2.nextPage());
}

void checkTouch() {
  if (digitalRead(PIN_TOUCH_HEAD) == HIGH) {
    if (currentMood != HAPPY) {
       currentMood = HAPPY;
       myDFPlayer.play(1); 
    }
  }
  if (digitalRead(PIN_TOUCH_SIDE) == HIGH) {
    if (currentMood != ANGRY) {
       currentMood = ANGRY;
       myDFPlayer.play(2);
    }
  }
}

void checkDistance() {
  static unsigned long lastUSCheck = 0;
  if (millis() - lastUSCheck < 200) return; 
  lastUSCheck = millis();

  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_US_TRIG, LOW);

  long duration = pulseIn(PIN_US_ECHO, HIGH, 20000);
  float distance = (duration * 0.0343) / 2;

  if (distance > 0 && distance < 30) {
    currentMood = TRACKING; 
  } else if (distance > 60 && currentMood == TRACKING) {
    currentMood = BORED; 
  }
}

void executeMoodActions() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastServoStep >= servoSpeed) {
    lastServoStep = currentMillis;

    if (currentMood == TRACKING) {
      tiltTarget = 110; 
      static int scanDir = 1;
      panTarget += scanDir;
      if (panTarget > 120 || panTarget < 60) scanDir *= -1;
    } 
    else if (currentMood == HAPPY) {
      panTarget = 90 + (5 * sin(currentMillis / 100)); 
      tiltTarget = 90;
    }
    else if (currentMood == ANGRY) {
      panTarget = 150; // Turn away
      tiltTarget = 80;
    }
    else {
      panTarget = 90;
      tiltTarget = 90;
    }

    if (panCurrent < panTarget) panCurrent++;
    if (panCurrent > panTarget) panCurrent--;
    if (tiltCurrent < tiltTarget) tiltCurrent++;
    if (tiltCurrent > tiltTarget) tiltCurrent--;

    panServo.write(panCurrent);
    tiltServo.write(tiltCurrent);
  }
}