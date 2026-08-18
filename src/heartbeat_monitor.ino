/***********************************************************
 *  Project: Heartbeat Monitor with Alert System
 *  File: heartbeat_monitor.ino
 *  Description: Main program integrating all modules
 *  Author: [Your Name]
 *  Date: [Current Date]
 *  Version: 1.0
 ***********************************************************/

// -------------------- Includes --------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // For I2C LCD
// #include <Adafruit_SSD1306.h>  // Uncomment for OLED
// #include <Adafruit_GFX.h>      // Uncomment for OLED

// -------------------- Pin Definitions --------------------
#define PIN_PULSE_SENSOR A0
#define PIN_BUZZER 3
#define PIN_LED_GREEN 4
#define PIN_LED_RED 5
#define PIN_BUTTON 2  // Optional: for resetting thresholds

// -------------------- Constants --------------------
// EDUCATIONAL THRESHOLDS - NOT MEDICAL ADVICE
const int LOW_BPM_THRESHOLD = 50;
const int HIGH_BPM_THRESHOLD = 100;

// Timing Constants (in milliseconds)
const unsigned long SAMPLE_INTERVAL = 10;      // Sample every 10ms (100Hz)
const unsigned long DISPLAY_INTERVAL = 500;    // Update display every 500ms
const unsigned long DEBOUNCE_INTERVAL = 200;   // Minimum time between beats
const unsigned long ALERT_TIMEOUT = 5000;      // Buzzer time before auto-off

// -------------------- Global Variables --------------------
// Sensor variables
int sensorValue = 0;
int filteredValue = 0;
bool isBeatDetected = false;

// Timing variables
unsigned long lastSampleTime = 0;
unsigned long lastDisplayTime = 0;
unsigned long lastBeatTime = 0;
unsigned long previousBeatTime = 0;
unsigned long alertStartTime = 0;

// BPM variables
int currentBPM = 0;
int stableBPM = 0;
unsigned long beatInterval = 0;
bool isAlertActive = false;

// Status variables
String statusString = "STARTING...";
String previousStatus = "";
int beatCounter = 0;

// Moving average filter
#define FILTER_WINDOW 5
int bpmHistory[FILTER_WINDOW] = {0};
int bpmIndex = 0;

// -------------------- Object Initialization --------------------
// LCD I2C address - common addresses: 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Uncomment for OLED
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------------------- Setup Function --------------------
void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("=====================================");
  Serial.println("HEARTBEAT MONITOR WITH ALERT SYSTEM");
  Serial.println("Version: 1.0");
  Serial.println("=====================================");
  Serial.println("Initializing...");

  // Initialize LCD
  initDisplay();
  
  // Initialize pins
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);  // Optional button

  // Initial state: all outputs OFF
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // Display startup message
  displayStartupMessage();

  // Initialize timing
  previousBeatTime = millis();
  lastDisplayTime = millis();

  Serial.println("System Ready!");
  Serial.println("Place finger on sensor to start monitoring");
  Serial.println("=====================================");
}

// -------------------- Main Loop --------------------
void loop() {
  unsigned long currentMillis = millis();

  // 1. Read and process sensor data
  if (currentMillis - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = currentMillis;
    processSensorData();
  }

  // 2. Update display at fixed interval
  if (currentMillis - lastDisplayTime >= DISPLAY_INTERVAL) {
    lastDisplayTime = currentMillis;
    updateDisplay();
  }

  // 3. Handle alert timeout (prevent buzzer from running forever)
  if (isAlertActive && (currentMillis - alertStartTime > ALERT_TIMEOUT)) {
    isAlertActive = false;
    digitalWrite(PIN_BUZZER, LOW);
    digitalWrite(PIN_LED_RED, LOW);
  }

  // 4. Optional: Check for button press to reset thresholds
  checkButtonPress();
}

// -------------------- Display Initialization --------------------
void initDisplay() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat Mon");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
}

// -------------------- Startup Message --------------------
void displayStartupMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place Finger");
  lcd.setCursor(0, 1);
  lcd.print("on Sensor...");
  delay(2000);
  lcd.clear();
}

// -------------------- Sensor Data Processing --------------------
void processSensorData() {
  // Read raw sensor value
  sensorValue = analogRead(PIN_PULSE_SENSOR);
  
  // Apply simple noise filter (moving average)
  static int filterBuffer[5] = {0};
  static int filterIndex = 0;
  filterBuffer[filterIndex] = sensorValue;
  filterIndex = (filterIndex + 1) % 5;
  
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += filterBuffer[i];
  }
  filteredValue = sum / 5;

  // Heartbeat detection
  detectHeartbeat();
}

// -------------------- Heartbeat Detection --------------------
void detectHeartbeat() {
  // Threshold for detecting pulse
  // This value may need tuning based on your sensor and finger placement
  const int PULSE_THRESHOLD = 530;
  
  // Detect rising edge above threshold
  if (filteredValue > PULSE_THRESHOLD && !isBeatDetected) {
    isBeatDetected = true;
    
    // Debounce: Prevent double counting
    if (millis() - lastBeatTime >= DEBOUNCE_INTERVAL) {
      handleHeartbeat();
    }
  }
  
  // Reset detection flag when signal drops below threshold
  if (filteredValue < PULSE_THRESHOLD) {
    isBeatDetected = false;
  }
}

// -------------------- Heartbeat Handler --------------------
void handleHeartbeat() {
  lastBeatTime = millis();
  beatCounter++;
  
  // Print to serial for debugging
  Serial.print("♥ Beat detected! ");
  Serial.print("Counter: ");
  Serial.println(beatCounter);
  
  // Calculate BPM
  calculateBPM();
  
  // Update alert system
  updateAlertSystem();
}

// -------------------- BPM Calculation --------------------
void calculateBPM() {
  // Calculate time between beats
  beatInterval = millis() - previousBeatTime;
  previousBeatTime = millis();
  
  // Validate interval (between 100ms and 3000ms)
  if (beatInterval > 100 && beatInterval < 3000) {
    // Calculate BPM
    currentBPM = 60000 / beatInterval;
    
    // Add to history for averaging
    bpmHistory[bpmIndex] = currentBPM;
    bpmIndex = (bpmIndex + 1) % FILTER_WINDOW;
    
    // Calculate average BPM for stability
    long sum = 0;
    int count = 0;
    for (int i = 0; i < FILTER_WINDOW; i++) {
      if (bpmHistory[i] > 0) {
        sum += bpmHistory[i];
        count++;
      }
    }
    if (count > 0) {
      stableBPM = sum / count;
    }
    
    Serial.print("  BPM: ");
    Serial.print(stableBPM);
    Serial.print(" | Interval: ");
    Serial.println(beatInterval);
  } else {
    // Invalid interval - probably noise
    Serial.println("  ! Invalid interval detected");
    currentBPM = 0;
  }
}

// -------------------- Alert System --------------------
void updateAlertSystem() {
  // Determine status based on thresholds
  if (stableBPM == 0) {
    statusString = "NO SIGNAL";
  } else if (stableBPM < LOW_BPM_THRESHOLD) {
    statusString = "LOW";
  } else if (stableBPM >= LOW_BPM_THRESHOLD && stableBPM <= HIGH_BPM_THRESHOLD) {
    statusString = "NORMAL";
  } else if (stableBPM > HIGH_BPM_THRESHOLD) {
    statusString = "HIGH";
  }

  // Control outputs based on status
  if (statusString == "NORMAL") {
    // Normal state
    digitalWrite(PIN_LED_GREEN, HIGH);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    isAlertActive = false;
  } else if (statusString == "LOW" || statusString == "HIGH") {
    // Alert state
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, HIGH);
    
    // Sound buzzer with pattern (intermittent beep)
    static unsigned long lastBuzzerToggle = 0;
    if (millis() - lastBuzzerToggle > 500) {
      lastBuzzerToggle = millis();
      digitalWrite(PIN_BUZZER, !digitalRead(PIN_BUZZER));
    }
    
    // Set alert flag and record start time
    if (!isAlertActive) {
      isAlertActive = true;
      alertStartTime = millis();
    }
  } else {
    // NO SIGNAL or other states
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    isAlertActive = false;
  }

  // Check if status changed
  if (statusString != previousStatus) {
    Serial.print("Status changed to: ");
    Serial.println(statusString);
    previousStatus = statusString;
  }
}

// -------------------- Display Update --------------------
void updateDisplay() {
  // 1. Update Serial Monitor
  printToSerial();
  
  // 2. Update LCD
  updateLCD();
}

// -------------------- Serial Monitor Output --------------------
void printToSerial() {
  // Print formatted data to Serial Monitor
  Serial.print("BPM: ");
  Serial.print(stableBPM);
  Serial.print(" | Raw: ");
  Serial.print(sensorValue);
  Serial.print(" | Filtered: ");
  Serial.print(filteredValue);
  Serial.print(" | Status: ");
  Serial.println(statusString);
}

// -------------------- LCD Update --------------------
void updateLCD() {
  // Clear and update LCD
  lcd.clear();
  
  // Line 1: BPM
  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  if (stableBPM > 0) {
    lcd.print(stableBPM);
  } else {
    lcd.print("---");
  }
  lcd.print("   ");  // Clear extra characters
  
  // Line 2: Status
  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  lcd.print(statusString);
  lcd.print("   ");  // Clear extra characters
  
  // Add blinking cursor during alert (visual feedback)
  if (isAlertActive && (millis() / 500) % 2 == 0) {
    lcd.setCursor(15, 1);
    lcd.print("!");
  }
}

// -------------------- Optional Button Handling --------------------
void checkButtonPress() {
  // Reset functionality or toggle thresholds
  static unsigned long lastButtonPress = 0;
  
  if (digitalRead(PIN_BUTTON) == LOW) {
    if (millis() - lastButtonPress > 1000) {
      lastButtonPress = millis();
      Serial.println("Button pressed!");
      // Reset BPM history
      for (int i = 0; i < FILTER_WINDOW; i++) {
        bpmHistory[i] = 0;
      }
      bpmIndex = 0;
      Serial.println("BPM history reset!");
    }
  }
}