/***********************************************************
 *  Module: BPM Calculation
 *  Description: Calculates Beats Per Minute from pulse data
 *  File: bpm_calculation.ino
 ***********************************************************/

// -------------------- Constants --------------------
#define BPM_HISTORY_SIZE 5
#define MIN_VALID_INTERVAL 100     // Minimum 100ms between beats
#define MAX_VALID_INTERVAL 3000    // Maximum 3000ms between beats

// -------------------- Variables --------------------
unsigned long previousBeatTime = 0;
unsigned long lastBeatTime = 0;
unsigned long beatInterval = 0;
int currentBPM = 0;
int stableBPM = 0;

// BPM history for averaging
int bpmHistory[BPM_HISTORY_SIZE] = {0};
int bpmIndex = 0;
int bpmCount = 0;

// Signal quality metrics
int signalQuality = 100;  // 0-100, 100 is perfect

// -------------------- Functions --------------------

/**
 * Initialize BPM calculation module
 */
void initBPMCalculator() {
  previousBeatTime = millis();
  for (int i = 0; i < BPM_HISTORY_SIZE; i++) {
    bpmHistory[i] = 0;
  }
  bpmIndex = 0;
  bpmCount = 0;
  Serial.println("BPM Calculator Initialized");
}

/**
 * Calculate BPM from beat interval
 * @param interval: Time between beats in milliseconds
 * @return int: Calculated BPM
 */
int calculateBPMFromInterval(unsigned long interval) {
  if (interval < MIN_VALID_INTERVAL || interval > MAX_VALID_INTERVAL) {
    return 0;  // Invalid interval
  }
  return 60000 / interval;
}

/**
 * Process a new heartbeat
 * @return int: New BPM value
 */
int processHeartbeat() {
  unsigned long currentTime = millis();
  
  // Calculate interval
  beatInterval = currentTime - previousBeatTime;
  previousBeatTime = currentTime;
  
  // Validate interval
  if (beatInterval < MIN_VALID_INTERVAL) {
    // Too fast - likely noise or double counting
    return 0;
  }
  
  // Calculate BPM
  currentBPM = calculateBPMFromInterval(beatInterval);
  
  if (currentBPM > 0) {
    // Add to history
    bpmHistory[bpmIndex] = currentBPM;
    bpmIndex = (bpmIndex + 1) % BPM_HISTORY_SIZE;
    if (bpmCount < BPM_HISTORY_SIZE) {
      bpmCount++;
    }
    
    // Calculate stable BPM (average)
    stableBPM = calculateAverageBPM();
    
    // Update signal quality
    updateSignalQuality();
  }
  
  return stableBPM;
}

/**
 * Calculate average BPM from history
 * @return int: Average BPM
 */
int calculateAverageBPM() {
  if (bpmCount == 0) return 0;
  
  long sum = 0;
  for (int i = 0; i < bpmCount; i++) {
    sum += bpmHistory[i];
  }
  return sum / bpmCount;
}

/**
 * Update signal quality metric
 */
void updateSignalQuality() {
  // Calculate variance to assess signal quality
  if (bpmCount < 2) {
    signalQuality = 50;  // Not enough data
    return;
  }
  
  float avg = stableBPM;
  float variance = 0;
  for (int i = 0; i < bpmCount; i++) {
    float diff = bpmHistory[i] - avg;
    variance += diff * diff;
  }
  variance /= bpmCount;
  
  // Convert variance to quality score (0-100)
  // Lower variance = higher quality
  if (variance < 10) signalQuality = 95;
  else if (variance < 25) signalQuality = 85;
  else if (variance < 50) signalQuality = 70;
  else if (variance < 100) signalQuality = 50;
  else signalQuality = 30;
}

/**
 * Reset BPM history
 */
void resetBPMHistory() {
  for (int i = 0; i < BPM_HISTORY_SIZE; i++) {
    bpmHistory[i] = 0;
  }
  bpmIndex = 0;
  bpmCount = 0;
  stableBPM = 0;
  currentBPM = 0;
  Serial.println("BPM History Reset");
}

/**
 * Get current BPM with smoothing
 * @return int: Smoothed BPM value
 */
int getSmoothedBPM() {
  return stableBPM;
}

/**
 * Get raw current BPM (last beat)
 * @return int: Raw BPM
 */
int getRawBPM() {
  return currentBPM;
}

/**
 * Check if BPM is stable (low variance)
 * @return bool: True if stable
 */
bool isBPMStable() {
  return (signalQuality > 70);
}

/**
 * Get signal quality as percentage
 * @return int: Quality percentage (0-100)
 */
int getSignalQuality() {
  return signalQuality;
}