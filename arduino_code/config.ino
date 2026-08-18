/***********************************************************
 *  Module: Configuration
 *  Description: Central configuration for the system
 *  File: config.ino
 ***********************************************************/

#ifndef CONFIG_H
#define CONFIG_H

// -------------------- Hardware Configuration --------------------

// Pin definitions
#define PIN_PULSE_SENSOR A0
#define PIN_BUZZER 3
#define PIN_LED_GREEN 4
#define PIN_LED_RED 5
#define PIN_BUTTON 2

// -------------------- Threshold Configuration --------------------
// EDUCATIONAL THRESHOLDS - NOT MEDICAL ADVICE

// BPM thresholds (adjust as needed)
#define LOW_BPM_THRESHOLD 50
#define HIGH_BPM_THRESHOLD 100

// -------------------- Timing Configuration --------------------

// Sensor sampling
#define SAMPLE_INTERVAL 10      // 10ms = 100Hz sampling rate

// Display update
#define DISPLAY_INTERVAL 500    // Update display every 500ms

// Beat detection
#define DEBOUNCE_INTERVAL 200   // Minimum time between beats (ms)

// Alert system
#define ALERT_TIMEOUT 5000      // Auto-off after 5 seconds
#define ALERT_INTERVAL 500      // Blink/beep interval (ms)

// -------------------- Sensor Configuration --------------------

// Pulse detection threshold (adjust based on sensor)
#define PULSE_THRESHOLD 530

// Filter settings
#define FILTER_SIZE 5          // Moving average filter size

// BPM calculation
#define BPM_HISTORY_SIZE 5     // Number of beats to average
#define MIN_VALID_INTERVAL 100 // Min interval for valid beat (ms)
#define MAX_VALID_INTERVAL 3000 // Max interval for valid beat (ms)

// -------------------- Display Configuration --------------------

// LCD
#define LCD_ADDRESS 0x27       // I2C address (0x27 or 0x3F)
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// OLED (uncomment to use OLED instead of LCD)
// #define USE_OLED
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64

// -------------------- Debug Configuration --------------------

// Serial communication
#define BAUD_RATE 9600
#define DEBUG_ENABLED true

// Logging
#define LOG_BPM true
#define LOG_RAW_DATA false
#define LOG_ALERTS true

// -------------------- Feature Flags --------------------

// Enable/disable features
#define FEATURE_BUTTON_RESET false
#define FEATURE_DATA_LOGGING false
#define FEATURE_BLINKING_CURSOR true

#endif // CONFIG_H