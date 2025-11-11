# 🛰️ ESP32-CAM Autonomous Rover

An autonomous multi-terrain rover powered by **ESP32** and **ESP32-CAM**, integrating **ultrasonic obstacle avoidance**, **real-time video streaming**, and **PWM-based motor control** for exploration and monitoring in confined or hazardous environments.

---

## 🚀 Overview
This project combines embedded control, sensor fusion, and computer vision on a compact platform. The rover autonomously navigates using ultrasonic distance sensors and streams live camera footage via a built-in HTTP server hosted on the ESP32-CAM module.

---

## ⚙️ Features
- **Autonomous Navigation:** Dual ultrasonic-based obstacle avoidance
- **Vision System:** Real-time MJPEG streaming using ESP32-CAM
- **Algorithms:**
  - *Wall-Following with Directional Decision* – Scans left/right and chooses the safer path
  - *Simple Path Finder* – Rotates until clear distance (>30 cm) detected
- **Motor Control:** Dual DC motors with PWM-based H-bridge driver
- **Lighting:** Configurable LED illumination for low-light streaming
- **Web Interface:** Camera control GUI hosted directly from ESP32-CAM flash memory

---

## 🧠 Architecture
| Module | Description |
|--------|-------------|
| `wall_following_decision.cpp` | Implements wall-following algorithm with left/right scanning and decision-making. |
| `simple_path_finder.cpp` | Implements minimal obstacle-avoidance logic that turns until path is clear. |
| `LiveFeedCode.ino` | Initializes ESP32-CAM streaming service and synchronizes with motion control. |
| `app_httpd.cpp` | Implements HTTP handlers for `/stream`, `/capture`, and `/bmp`. |
| `camera_pins.h` / `camera_index.h` | Camera hardware mapping and embedded HTML interface. |
| `ci.json` | Build configuration ensuring cross-compatibility across ESP32, ESP32-S2/S3 variants. |

---

## 🔩 Hardware Setup
**Required Components:**
- ESP32-CAM (AI-Thinker model)
- ESP32 microcontroller (for motor and sensor control)
- HC-SR04 ultrasonic sensor
- Dual DC motors (12V)
- L298N motor driver module
- 3D-printed chassis and battery pack

**Wiring Diagram (Simplified):**
```
HC-SR04 → ESP32
  TRIG → GPIO 17
  ECHO → GPIO 18

Motor Driver (L298N)
  ENA → GPIO 16
  IN1 → GPIO 15
  IN2 → GPIO 7
  ENB → GPIO 4
  IN3 → GPIO 5
  IN4 → GPIO 6

ESP32-CAM
  LED GPIO → 4 (configurable)
```

---

## 🧰 Software Stack
- **Language:** C/C++ (Arduino Framework)
- **Platform:** ESP-IDF / Arduino Core for ESP32
- **Libraries:**
  - `esp_camera.h`
  - `esp_http_server.h`
  - `Arduino.h`
- **Development Tools:**
  - Arduino IDE / PlatformIO
  - Fusion 360 or SolidWorks for chassis design

---

## 📁 Repository Structure
```
esp32-autonomous-rover/
├── navigation/
│   ├── wall_following_decision.cpp   # Wall-following logic
│   ├── simple_path_finder.cpp        # Simple path-finder
├── vision/
│   ├── LiveFeedCode.ino              # Camera initialization
│   ├── app_httpd.cpp                 # HTTP streaming server
│   ├── camera_pins.h
│   ├── camera_index.h
│   ├── ci.json
├── docs/
│   ├── system_design.md
│   ├── wiring_diagram.png
│   └── usage_instructions.md
└── README.md
```

---

## 📸 Demo (Conceptual)
When powered on, the rover performs the following sequence:
1. Initializes ultrasonic and camera modules.
2. Begins live streaming accessible at `http://<ESP32_IP>:8080/stream`.
3. Continuously measures distance using ultrasonic sensors.
4. Adjusts motion using wall-following or path-finder algorithm.
5. Streams live video while navigating autonomously.

---

## 🧩 Applications
- Search & Rescue Operations
- Confined Space Exploration
- Remote Inspection Robotics
- Educational Robotics and Embedded Systems Projects

---

## 🧠 Future Work
- Add SLAM (Simultaneous Localization and Mapping)
- Integrate IMU-based motion estimation
- Implement path-memory and retrace functions
- Develop Android interface for control and streaming

---

## 👨‍💻 Author
**Samuel Huang**  
GitHub: [FlosMume](https://github.com/FlosMume)

---

## 🪪 License
This project is licensed under the [MIT License](LICENSE).

---

**Keywords:** ESP32, ESP32-CAM, Robotics, Obstacle Avoidance, Live Streaming, Embedded Systems, PWM Motor Control, Arduino.


**Revision Patch – Nov 2025**
 - ✅ Corrected **LED pin** to **GPIO22** (per `app_httpd.cpp`).
 - ✅ Updated stream URL to `http://<device_ip>/stream` (no fixed port).
 - ✅ Noted **single ultrasonic sensor** (TRIG = GPIO17, ECHO = GPIO18).
 - ✅ Confirmed motor driver wiring:
   - ENA = 16, IN1 = 15, IN2 = 7  
   - ENB = 4, IN3 = 5, IN4 = 6
 - ✅ Clarified build setting: *Partition Scheme = custom (per `ci.json`)*.
 - ✅ Removed stray `'/'` in `wall_following_decision.cpp` (compile fix).
