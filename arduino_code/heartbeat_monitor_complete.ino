/***********************************************************
 *  Project: Heartbeat Monitor with Alert System
 *  File: heartbeat_monitor_complete.ino
 *  Description: Fully integrated main program
 *  Author: [Your Name]
 *  Date: [Current Date]
 *  Version: 1.0
 ***********************************************************/

// -------------------- Includes --------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------------------- Configuration --------------------
#include "config.ino"

// -------------------- Pin Definitions --------------------
#define PIN_PULSE_SENSOR A0
#define PIN_BUZZER 3
#define PIN_LED_GREEN 4
#define PIN_LED_RED 5
#define PIN_BUTTON 2

// -------------------- Constants --------------------
const int LOW_BPM_THRESHOLD = 50;
const int HIGH_BPM_THRESHOLD = 100;
const unsigned long SAMPLE_INTERVAL = 10;
const unsigned long DISPLAY_INTERVAL = 500;
const unsigned long DEBOUNCE_INTERVAL = 200;
const unsigned long ALERT_TIMEOUT = 5000;
const int PULSE_THRESHOLD = 530;
const int FILTER_WINDOW = 5;

// -------------------- Global Variables --------------------
// Sensor
int sensorValue = 0;
int filteredValue = 0;
bool isBeatDetected = false;

// Timing
unsigned long lastSampleTime = 0;
unsigned long lastDisplayTime = 0;
unsigned long lastBeatTime = 0;
unsigned long previousBeatTime = 0;
unsigned long alertStartTime = 0;

// BPM
int currentBPM = 0;
int stableBPM = 0;
int bpmHistory[FILTER_WINDOW] = {0};
int bpmIndex = 0;
int bpmCount = 0;

// Status
String statusString = "STARTING...";
bool isAlertActive = false;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);
  Serial.println("=====================================");
  Serial.println("HEARTBEAT MONITOR WITH ALERT SYSTEM");
  Serial.println("Version: 1.0");
  Serial.println("=====================================");

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Heartbeat Mon");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  // Initialize pins
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Initial state
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // Wait and show ready
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place Finger");
  lcd.setCursor(0, 1);
  lcd.print("on Sensor...");
  delay(2000);
  lcd.clear();

  // Initialize timing
  previousBeatTime = millis();
  
  Serial.println("System Ready!");
  Serial.println("=====================================");
}

// -------------------- Main Loop --------------------
void loop() {
  unsigned long currentMillis = millis();

  // Read sensor at fixed interval
  if (currentMillis - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = currentMillis;
    processSensorData();
  }

  // Update display at fixed interval
  if (currentMillis - lastDisplayTime >= DISPLAY_INTERVAL) {
    lastDisplayTime = currentMillis;
    updateDisplay();
  }

  // Handle alert timeout
  if (isAlertActive && (currentMillis - alertStartTime > ALERT_TIMEOUT)) {
    isAlertActive = false;
    digitalWrite(PIN_BUZZER, LOW);
    digitalWrite(PIN_LED_RED, LOW);
  }

  // Update alert pattern
  if (isAlertActive) {
    updateAlertPattern();
  }
}

// -------------------- Sensor Processing --------------------
void processSensorData() {
  sensorValue = analogRead(PIN_PULSE_SENSOR);
  
  // Simple moving average filter
  static int filterBuffer[5] = {0};
  static int filterIndex = 0;
  filterBuffer[filterIndex] = sensorValue;
  filterIndex = (filterIndex + 1) % 5;
  
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += filterBuffer[i];
  }
  filteredValue = sum / 5;

  // Detect heartbeat
  if (filteredValue > PULSE_THRESHOLD && !isBeatDetected) {
    isBeatDetected = true;
    if (millis() - lastBeatTime >= DEBOUNCE_INTERVAL) {
      handleHeartbeat();
    }
  }
  if (filteredValue < PULSE_THRESHOLD) {
    isBeatDetected = false;
  }
}

// -------------------- Heartbeat Handler --------------------
void handleHeartbeat() {
  lastBeatTime = millis();
  calculateBPM();
  updateAlertSystem();
}

// -------------------- BPM Calculation --------------------
void calculateBPM() {
  unsigned long interval = millis() - previousBeatTime;
  previousBeatTime = millis();
  
  if (interval > 100 && interval < 3000) {
    currentBPM = 60000 / interval;
    
    bpmHistory[bpmIndex] = currentBPM;
    bpmIndex = (bpmIndex + 1) % FILTER_WINDOW;
    if (bpmCount < FILTER_WINDOW) bpmCount++;
    
    long sum = 0;
    for (int i = 0; i < bpmCount; i++) {
      sum += bpmHistory[i];
    }
    stableBPM = sum / bpmCount;
  }
}

// -------------------- Alert System --------------------
void updateAlertSystem() {
  if (stableBPM == 0) {
    statusString = "NO SIGNAL";
  } else if (stableBPM < LOW_BPM_THRESHOLD) {
    statusString = "LOW";
  } else if (stableBPM >= LOW_BPM_THRESHOLD && stableBPM <= HIGH_BPM_THRESHOLD) {
    statusString = "NORMAL";
  } else {
    statusString = "HIGH";
  }

  if (statusString == "NORMAL") {
    digitalWrite(PIN_LED_GREEN, HIGH);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    isAlertActive = false;
  } else if (statusString == "LOW" || statusString == "HIGH") {
    digitalWrite(PIN_LED_GREEN, LOW);
    isAlertActive = true;
    alertStartTime = millis();
  }
}

// -------------------- Alert Pattern --------------------
void updateAlertPattern() {
  static unsigned long lastToggle = 0;
  static bool state = false;
  
  if (millis() - lastToggle > 500) {
    lastToggle = millis();
    state = !state;
    digitalWrite(PIN_LED_RED, state);
    digitalWrite(PIN_BUZZER, state);
  }
}

// -------------------- Display Update --------------------
void updateDisplay() {
  // Serial Monitor
  Serial.print("BPM: ");
  Serial.print(stableBPM);
  Serial.print(" | Raw: ");
  Serial.print(sensorValue);
  Serial.print(" | Status: ");
  Serial.println(statusString);

  // LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  if (stableBPM > 0) {
    lcd.print(stableBPM);
  } else {
    lcd.print("---");
  }
  
  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  lcd.print(statusString);
  
  // Alert indicator
  if (isAlertActive) {
    lcd.setCursor(15, 1);
    lcd.print("!");
  }
}