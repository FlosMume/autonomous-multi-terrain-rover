# 🧩 System Design – ESP32-CAM Autonomous Rover

## 1. System Overview
The **ESP32-CAM Autonomous Rover** integrates embedded sensing, motion control, and live video streaming into a modular robotics platform. The rover operates autonomously using real-time distance feedback from ultrasonic sensors and transmits live video via Wi-Fi for remote monitoring.

The system demonstrates principles of **mechatronics integration** — combining hardware, firmware, and networking components to achieve autonomous mobility.

---

## 2. System Architecture

**Subsystems:**
1. **Sensing and Environment Perception** – Uses ultrasonic sensors for obstacle detection.
2. **Actuation and Motion Control** – Dual DC motors driven through an L298N H-bridge module.
3. **Vision and Telemetry** – ESP32-CAM module for live video streaming and LED illumination.
4. **Computation and Control** – ESP32 microcontroller managing sensor data, decision-making, and motor control.

```
                ┌───────────────────────────────────────┐
                │           ESP32 Microcontroller       │
                │───────────────────────────────────────│
                │ - Motion Control (PWM)               │
                │ - Obstacle Detection Logic            │
                │ - Decision-Making (Path Planner)      │
                └──────────────┬────────────────────────┘
                               │ UART / GPIO
                               ▼
                 ┌────────────────────────────────────┐
                 │            ESP32-CAM Module        │
                 │ - Live MJPEG HTTP Stream           │
                 │ - LED Flash Controller             │
                 └────────────────────────────────────┘
                               ▲
                               │ Wi-Fi
                               ▼
                    ┌──────────────────────────┐
                    │  Web Interface / Client  │
                    │  - Stream Viewer         │
                    │  - Camera Control GUI    │
                    └──────────────────────────┘

         ┌───────────────┐      ┌───────────────┐
         │ Ultrasonic    │      │ Ultrasonic    │
         │  Sensor (L)   │      │  Sensor (R)   │
         └───────────────┘      └───────────────┘
                   │                     │
                   ▼                     ▼
             ┌─────────────────────────────────┐
             │    L298N Motor Driver Module    │
             │ - Dual DC Motor Control (PWM)   │
             └─────────────────────────────────┘
                   │                     │
                   ▼                     ▼
                Left Motor          Right Motor
```

---

## 3. Control Logic
### A. Wall-Following with Directional Decision
- The rover moves forward until an obstacle is detected within **30 cm**.
- It reverses, scans **left** and **right**, and compares distance values.
- It turns toward the direction with greater open space.
- Motion resumes with updated direction.

### B. Simple Path Finder
- On detecting an obstacle (<30 cm), the rover stops and continuously rotates in small steps.
- It measures distance after each step until a safe forward path is found.
- Simpler but effective in structured environments.

---

## 4. Communication and Streaming
The **ESP32-CAM** acts as a web server hosting endpoints:
- `/stream` → Live MJPEG feed (multi-part HTTP response)
- `/capture` → Snapshot image (JPEG)
- `/bmp` → Bitmap frame for analysis

The `app_httpd.cpp` module manages:
- Frame capture
- LED flash intensity control
- Real-time timestamping of frames

Users can access the feed through any browser at `http://<ESP32_IP>:8080/stream`.

---

## 5. Electrical Design
| Component | Voltage | Current | Notes |
|------------|----------|----------|-------|
| ESP32-CAM | 5V | ~160 mA | Powered via USB or external 5V regulator |
| ESP32 | 5V | ~120 mA | Motor control logic |
| L298N Motor Driver | 12V | 0.6–1A | Drives both DC motors |
| Ultrasonic Sensor | 5V | <20 mA | TRIG/ECHO lines use GPIO 17/18 |
| Motors | 12V | ~0.3A each | Controlled via PWM from ESP32 |

**Power Source:** 3-cell Li-ion battery pack or regulated DC adapter (12V 2A).

---

## 6. Software Workflow
```
[System Boot]
     ↓
[Initialize GPIO, PWM, and Camera]
     ↓
[Start Live Stream Server]
     ↓
[Loop]
     → Measure Distance
     → Evaluate Threshold
     → Execute Motion (Forward/Turn)
     → Update Video Feed
```

---

## 7. Safety and Reliability
- **Timeouts:** PulseIn() limited to 30 ms to avoid blocking.
- **Failsafe:** Motors stop if no distance reading available.
- **Camera LED Limit:** Intensity capped to prevent overheating.

---

## 8. Future Expansion
- Integrate IMU (accelerometer + gyroscope) for stability feedback.
- Implement SLAM or A* path planning.
- Add Bluetooth/Android control panel.
- Deploy TensorFlow Lite for edge object detection.

---

## 9. Conclusion
The system successfully demonstrates a low-cost, embedded robotics solution combining **real-time sensing**, **motion control**, and **computer vision**. It serves as a scalable foundation for advanced autonomous systems such as inspection bots, educational robotics, and prototype search-and-rescue units.

---

**Author:** Samuel Huang   
**License:** MIT

**Revision Patch – Nov 2025**
 - ✅ Corrected **LED pin** to **GPIO22** (per `app_httpd.cpp`).
 - ✅ Updated stream URL to `http://<device_ip>/stream` (no fixed port).
 - ✅ Noted **single ultrasonic sensor** (TRIG = GPIO17, ECHO = GPIO18).
 - ✅ Confirmed motor driver wiring:
   - ENA = 16, IN1 = 15, IN2 = 7  
   - ENB = 4, IN3 = 5, IN4 = 6
 - ✅ Clarified build setting: *Partition Scheme = custom (per `ci.json`)*.
 - ✅ Removed stray `'/'` in `wall_following_decision.cpp` (compile fix).