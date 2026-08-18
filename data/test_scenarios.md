# Test Scenarios - Heartbeat Monitor

## Scenario 1: Normal Operation
**Procedure:**
1. Power on the system
2. Adjust potentiometer to simulate ~75 BPM
3. Wait for 10 seconds

**Expected Results:**
- LCD shows "BPM: 75" and "Status: NORMAL"
- Green LED ON
- Red LED OFF
- Buzzer OFF

**Actual Results:** PASS ✓

---

## Scenario 2: Low BPM Alert
**Procedure:**
1. Power on the system
2. Adjust potentiometer to simulate ~30 BPM
3. Wait for alert trigger

**Expected Results:**
- LCD shows "BPM: 30" and "Status: LOW"
- Green LED OFF
- Red LED ON
- Buzzer ON (intermittent)

**Actual Results:** PASS ✓

---

## Scenario 3: High BPM Alert
**Procedure:**
1. Power on the system
2. Adjust potentiometer to simulate ~120 BPM
3. Wait for alert trigger

**Expected Results:**
- LCD shows "BPM: 120" and "Status: HIGH"
- Green LED OFF
- Red LED ON
- Buzzer ON (intermittent)

**Actual Results:** PASS ✓

---

## Scenario 4: No Signal
**Procedure:**
1. Power on the system
2. Set potentiometer to 0

**Expected Results:**
- LCD shows "BPM: ---" and "Status: NO SIGNAL"
- All LEDs OFF
- Buzzer OFF

**Actual Results:** PASS ✓

---

## Scenario 5: State Transitions
**Procedure:**
1. Start with NO SIGNAL
2. Increase to NORMAL (70 BPM)
3. Decrease to LOW (30 BPM)
4. Increase to NORMAL (80 BPM)
5. Increase to HIGH (120 BPM)
6. Return to NORMAL (75 BPM)

**Expected Results:**
- Status changes correctly
- LEDs update accordingly
- Buzzer activates/deactivates

**Actual Results:** PASS ✓