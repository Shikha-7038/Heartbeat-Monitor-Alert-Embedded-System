# Heartbeat Monitor with Alert System

![Project Status](https://img.shields.io/badge/status-stable-brightgreen)
![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20ESP32-blue)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

An embedded systems prototype for real-time heart rate monitoring, BPM calculation, and threshold-based alert generation using a pulse sensor.


## 📖 Overview

This project demonstrates the development of an **Embedded Systems** application that interfaces with a **Pulse Sensor** to monitor heart rate. The system reads the analog signal from the sensor, processes it to detect individual heartbeats, and calculates the **Beats Per Minute (BPM)**. The BPM is then displayed on an **LCD/OLED screen**. If the BPM falls outside a pre-defined normal range, the system triggers visual (Red LED) and audible (Buzzer) alerts.

The project is designed as a proof-of-work for students, covering aspects of sensor integration, signal processing, real-time decision-making, and hardware interfacing. It is structured to be beginner-friendly while demonstrating industry-relevant concepts.

## 🎯 Problem Statement

Manual pulse counting is prone to human error, impractical for continuous monitoring, and not scalable. There is a need for automated, non-invasive systems that can continuously track heart rate and provide immediate alerts in case of abnormalities to ensure timely intervention, especially for individuals with heart conditions or those in high-risk activities.

## ⚠️ Educational Disclaimer

**This project is for educational and demonstration purposes only.** It is not a medical diagnostic device. The readings obtained from hobby-grade sensors like the Pulse Sensor can be approximate and are not a substitute for professional medical advice, diagnosis, or treatment. The BPM thresholds used are arbitrary and for illustration. Always consult a healthcare professional for health-related matters.

## 🏭 Industry Relevance

This project is highly relevant to the **HealthTech and Wearable IoT** industries, mirroring the core technology used in products from companies like **Fitbit, Apple, Philips Healthcare, and Medtronic**.

- **Wearable Fitness**: Continuous HR monitoring for activity tracking.
- **Patient Monitoring**: Prototype for low-cost, remote vital sign monitoring.
- **Sports & Rehab**: Biofeedback for athletes and patients.

## ✨ Features

- **Real-Time Heartbeat Detection**: Accurately detects pulses via an optical sensor.
- **BPM Calculation**: Calculates and displays Beats Per Minute.
- **Threshold-Based Alert System**: Triggers alerts when BPM is too low or too high.
- **User-Friendly Interface**:
    - **LCD/OLED Display**: Shows current BPM and status (NORMAL, LOW, HIGH).
    - **Visual Alert**: Red LED for alarms, Green LED for normal status.
    - **Audible Alert**: Buzzer sounds during alarm conditions.
- **Modular Code**: Well-commented, easy-to-understand, and structured in functions for modification.
- **Virtual Simulation Support**: Can be simulated in Wokwi without any hardware.

## 🧩 Embedded Concepts Used

- **Microcontroller Programming**
- **GPIO (General Purpose Input/Output)**
- **ADC (Analog-to-Digital Converter)**
- **Sensor Interfacing (I2C, Analog)**
- **Signal Processing & Peak Detection**
- **BPM Calculation Logic**
- **Non-Blocking Code (Timer/millis)**
- **Display Interfacing (LCD/OLED)**
- **Actuator Control (LEDs, Buzzer)**

## 🛠️ Hardware Components (Recommended)

| Component | Quantity | Purpose |
| :--- | :---: | :--- |
| Arduino UNO / ESP32 | 1 | Microcontroller |
| Pulse Sensor (or MAX30102) | 1 | Detect heartbeats |
| 16x2 I2C LCD / OLED (SSD1306) | 1 | Display BPM and status |
| Buzzer | 1 | Audible alert |
| LED (Green) | 1 | Visual indicator: NORMAL |
| LED (Red) | 1 | Visual indicator: ALERT |
| 220Ω Resistors | 2 | Current limiting for LEDs |
| Breadboard & Jumper Wires | As needed | Circuit construction |

## 🔌 Circuit Connections

*(Place your circuit diagram image here. Save it as `circuit_diagram.png` inside the `circuit_diagram/` folder)*

> **Important:** A detailed wiring table is provided below for easy reference.

| Component | Connection |
| :--- | :--- |
| **Pulse Sensor** | VCC→5V, GND→GND, SIG→A0 |
| **LCD I2C** | VCC→5V, GND→GND, SDA→A4, SCL→A5 |
| **Buzzer** | +→D3, -→GND |
| **Green LED** | Anode→D4, Cathode→GND (via 220Ω) |
| **Red LED** | Anode→D5, Cathode→GND (via 220Ω) |

## 🏗️ Project Architecture
```
Heartbeat-Monitor-Alert-Embedded-System/
│
├── arduino_code/
│   ├── alert_system.ino
│   ├── bpm_calculation.ino
│   ├── config.ino
│   ├── display.ino
│   ├── heartbeat_monitor_complete.ino
│   └── pulse_sensor.ino
│
├── circuit_diagram/
│   └── circuit_complete.png
│
├── data/
│
├── screenshots/
│
├── simulation/
│   ├── heartbeat_wokwi.json
│   └── wokwi_link.txt
│
├── src/
│   └── heartbeat_monitor.ino
│
├── .gitignore
└── README.md
```

## 🚀 Getting Started

### Hardware Setup
1.  **Install Arduino IDE**.
2.  **Select Board & Port**.
3.  **Install Required Libraries**:
    - `LiquidCrystal I2C` (for I2C LCD)
    - `Adafruit SSD1306` & `Adafruit GFX` (for OLED)
4.  **Wire the Components**: Follow the circuit diagram.
5.  **Upload Code**: Open the `.ino` file and upload it to the board.
6.  **Open Serial Monitor**: Set baud rate to 9600 to view debug output.
7.  **Place Finger**: Gently place your fingertip on the pulse sensor.

### Virtual Simulation (Wokwi)
1.  **Open Wokwi**.
2.  **Create a New Project** for Arduino UNO.
3.  **Build Circuit**: Add a potentiometer (simulated sensor), I2C LCD, LEDs, and a Buzzer.
4.  **Wire Components**: Refer to the circuit diagram.
5.  **Paste Code**: Copy the code from `src/` into the editor.
6.  **Start Simulation**: Click the green "Start" button.
7.  **Simulate Sensor**: Adjust the potentiometer to change the "BPM" and test the alert system.


## 🧪 Test Results

| Test Case | Description | Result |
| :--- | :--- | :--- |
| Normal BPM | Simulated BPM of 70-80 BPM. | Green LED ON. Display shows "NORMAL". |
| Low BPM | Simulated BPM < 50 BPM. | Red LED ON, Buzzer ON. Display shows "LOW". |
| High BPM | Simulated BPM > 100 BPM. | Red LED ON, Buzzer ON. Display shows "HIGH". |
| No Signal | Sensor not detecting a pulse. | Display shows "NO SIGNAL". All outputs OFF. |

*([Provide more detailed test results in your report or `data/test_results.csv`])*

## 🚧 Limitations & Future Improvements

### Current Limitations
- Hobby-grade sensor provides approximate readings.
- The BPM calculation is based on a simple threshold method, which is susceptible to motion artifacts.
- No data logging or remote connectivity.
- The threshold values (50 & 100 BPM) are educational and not clinically validated.

### Future Improvements
- **Advanced Sensor**: Use a MAX30102 for more accurate SpO2 and HR data.
- **Data Logging**: Add an SD card module to log BPM over time.
- **IoT Integration**: Add Wi-Fi (using ESP32) to send data to a cloud dashboard like ThingSpeak for remote monitoring.
- **Advanced Filtering**: Implement a moving average or median filter to improve signal stability.
- **User Interface**: Add a button to allow the user to adjust the threshold values.
- **Low-Power Design**: Implement deep sleep modes for battery-powered wearable applications.

## 📚 Learning Outcomes

Through this project, I have gained practical experience in:
- Interfacing a microcontroller with analog sensors and display modules.
- Implementing signal processing and peak detection algorithms.
- Writing non-blocking, `millis()` based code for real-time systems.
- Building complete hardware and software solutions.
- Understanding the core concepts of embedded systems used in the healthcare industry.

## 👤 Author

- **Your Name** - Shikha

## 📄 License

This project is open-source and available under the **MIT License**.
