/***********************************************************
 *  Module: Alert System
 *  Description: Handles LEDs, Buzzer, and alert logic
 *  File: alert_system.ino
 ***********************************************************/

// -------------------- Pin Definitions --------------------
#define PIN_LED_GREEN 4
#define PIN_LED_RED 5
#define PIN_BUZZER 3

// -------------------- Constants --------------------
#define ALERT_INTERVAL 500      // Blink/beep interval (ms)
#define BUZZER_FREQUENCY 1000   // Buzzer frequency (Hz)
#define ALERT_TIMEOUT 5000      // Auto-off after 5 seconds

// Alert patterns
enum AlertPattern {
  PATTERN_NONE,
  PATTERN_CONTINUOUS,
  PATTERN_INTERMITTENT,
  PATTERN_SOS
};

// -------------------- Variables --------------------
bool isAlertActive = false;
unsigned long alertStartTime = 0;
unsigned long lastAlertToggle = 0;
bool alertToggleState = false;
AlertPattern currentPattern = PATTERN_NONE;

// -------------------- Functions --------------------

/**
 * Initialize alert system
 */
void initAlertSystem() {
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  // Set initial state
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  
  Serial.println("Alert System Initialized");
}

/**
 * Set normal state (green LED ON, everything else OFF)
 */
void setNormalState() {
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  isAlertActive = false;
  currentPattern = PATTERN_NONE;
  alertToggleState = false;
}

/**
 * Set alert state with specific pattern
 * @param pattern: Alert pattern to use
 */
void setAlertState(AlertPattern pattern = PATTERN_INTERMITTENT) {
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, HIGH);  // Red LED ON as base
  
  isAlertActive = true;
  alertStartTime = millis();
  currentPattern = pattern;
}

/**
 * Update alert system (call in loop)
 */
void updateAlertSystem() {
  if (!isAlertActive) {
    return;
  }
  
  unsigned long currentTime = millis();
  
  // Check timeout
  if (currentTime - alertStartTime > ALERT_TIMEOUT) {
    deactivateAlert();
    return;
  }
  
  // Update based on pattern
  switch (currentPattern) {
    case PATTERN_CONTINUOUS:
      // Continuous beep and LED
      digitalWrite(PIN_BUZZER, HIGH);
      digitalWrite(PIN_LED_RED, HIGH);
      break;
      
    case PATTERN_INTERMITTENT:
      // Intermittent beep and flashing
      if (currentTime - lastAlertToggle > ALERT_INTERVAL) {
        lastAlertToggle = currentTime;
        alertToggleState = !alertToggleState;
        digitalWrite(PIN_BUZZER, alertToggleState ? HIGH : LOW);
        digitalWrite(PIN_LED_RED, alertToggleState ? HIGH : LOW);
      }
      break;
      
    case PATTERN_SOS:
      // SOS pattern: 3 short, 3 long, 3 short
      handleSOSPattern(currentTime);
      break;
      
    default:
      break;
  }
}

/**
 * Handle SOS pattern
 * @param currentTime: Current millis() value
 */
void handleSOSPattern(unsigned long currentTime) {
  static int patternStep = 0;
  static unsigned long stepStartTime = 0;
  
  // SOS: 3 short, 3 long, 3 short
  // Short = 200ms, Long = 500ms, Gap = 200ms
  int durations[] = {200, 200, 200, 500, 500, 500, 200, 200, 200};
  int totalSteps = sizeof(durations) / sizeof(durations[0]);
  
  if (currentTime - stepStartTime > durations[patternStep]) {
    stepStartTime = currentTime;
    patternStep = (patternStep + 1) % totalSteps;
    
    // Toggle buzzer and LED
    bool state = (patternStep % 2 == 0) ? HIGH : LOW;
    digitalWrite(PIN_BUZZER, state);
    digitalWrite(PIN_LED_RED, state);
  }
}

/**
 * Deactivate alert
 */
void deactivateAlert() {
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  isAlertActive = false;
  currentPattern = PATTERN_NONE;
  alertToggleState = false;
  Serial.println("Alert Deactivated");
}

/**
 * Set buzzer frequency (for PWM buzzer)
 * @param frequency: Frequency in Hz
 */
void setBuzzerFrequency(int frequency) {
  if (frequency > 0) {
    tone(PIN_BUZZER, frequency);
  } else {
    noTone(PIN_BUZZER);
  }
}

/**
 * Activate alert with specific type
 * @param type: Alert type (0=low, 1=high)
 * @param pattern: Pattern to use
 */
void triggerAlert(int type, AlertPattern pattern = PATTERN_INTERMITTENT) {
  if (!isAlertActive) {
    Serial.print("⚠️ Alert triggered! Type: ");
    Serial.println(type == 0 ? "LOW" : "HIGH");
    setAlertState(pattern);
  }
}

/**
 * Check if alert is currently active
 * @return bool: True if alert active
 */
bool isAlertActiveState() {
  return isAlertActive;
}

/**
 * Get remaining alert time
 * @return unsigned long: Milliseconds remaining (0 if not active)
 */
unsigned long getAlertTimeRemaining() {
  if (!isAlertActive) return 0;
  unsigned long elapsed = millis() - alertStartTime;
  return (elapsed < ALERT_TIMEOUT) ? (ALERT_TIMEOUT - elapsed) : 0;
}