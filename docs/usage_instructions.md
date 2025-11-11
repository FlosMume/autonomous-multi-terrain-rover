# 📘 Usage Instructions – ESP32-CAM Autonomous Rover

This guide explains how to **assemble, configure, and operate** the ESP32-CAM Autonomous Rover. It covers setup for both the **navigation control system** and the **live streaming vision module**.

---

## 🧰 Prerequisites
### Hardware Checklist
- ESP32-CAM (AI Thinker or compatible)
- ESP32 microcontroller board
- HC-SR04 ultrasonic distance sensor
- L298N motor driver module
- 2 × 12V DC motors with wheels
- 3D-printed chassis and Li-ion battery pack
- Jumper wires and USB cables

### Software & Tools
- **Arduino IDE** or **PlatformIO**
- **ESP32 board support package** (install via Arduino Board Manager)
- **CH340 / CP2102 driver** (for USB connection, if needed)
- Libraries:
  - `esp_camera.h`
  - `esp_http_server.h`
  - `WiFi.h`

---

## ⚙️ Step 1. Assemble the Rover
1. **Mount** the DC motors on the 3D-printed chassis.
2. **Connect** both motors to the L298N driver output pins.
3. **Wire** the motor driver inputs and enable pins to the ESP32 GPIOs as follows:
   ```
   ENA → GPIO 16
   IN1 → GPIO 15
   IN2 → GPIO 7
   ENB → GPIO 4
   IN3 → GPIO 5
   IN4 → GPIO 6
   ```
4. **Attach** the HC-SR04 sensor to the front:
   ```
   TRIG → GPIO 17
   ECHO → GPIO 18
   ```
5. **Install** the ESP32-CAM module (separate power if needed) and connect its LED pin (GPIO 4).
6. Power the entire system via a **12V battery pack** or **regulated adapter**.

---

## 💻 Step 2. Flash the Firmware
### Option A: Using Arduino IDE
1. Open `wall_following_decision.cpp`, `simple_path_finder.cpp`, or `LiveFeedCode.ino` depending on desired mode.
2. Go to **Tools → Board → ESP32 Dev Module**.
3. Select the correct **COM port**.
4. Set the following parameters:
   - Flash Mode: DIO
   - Partition Scheme: Minimal SPIFFS
   - Upload Speed: 115200 baud
5. Upload the sketch to the ESP32 or ESP32-CAM.

### Option B: Using PlatformIO
- Update `platformio.ini` to include:
  ```ini
  [env:esp32cam]
  platform = espressif32
  board = esp32cam
  framework = arduino
  monitor_speed = 115200
  ```
- Run:
  ```bash
  pio run --target upload
  ```

---

## 📡 Step 3. Connect to Wi-Fi
Edit `LiveFeedCode.ino` and configure your network:
```cpp
const char* ssid = "YourNetworkName";
const char* password = "YourPassword";
```
After flashing, open the **Serial Monitor** (115200 baud). The ESP32-CAM will print its **assigned IP address**.

---

## 🎥 Step 4. Access the Live Stream
Once powered on and connected to Wi-Fi:
1. Open a browser on your PC or phone.
2. Navigate to:
   ```
   http://<ESP32_IP>:8080/stream
   ```
3. You should see a **live video feed** from the ESP32-CAM.
4. Optionally, adjust LED intensity in `app_httpd.cpp` via `enable_led()`.

---

## 🚗 Step 5. Run Autonomous Navigation
Choose between the two navigation modes:

### Mode 1 – Wall Following with Directional Decision
```bash
Upload: navigation/wall_following_decision.cpp
```
**Behavior:**
- Moves forward until an obstacle is detected (<30 cm)
- Reverses slightly, scans left/right, and turns toward the side with more space
- Continues forward autonomously

### Mode 2 – Simple Path Finder
```bash
Upload: navigation/simple_path_finder.cpp
```
**Behavior:**
- Moves forward until obstacle detection
- Rotates step-by-step until an open path (>30 cm) is found
- Resumes forward motion

---

## 🧩 Step 6. Monitor & Debug
Use **Serial Monitor** to view live sensor readings:
```
Distance: 25 cm
Left Distance: 40 cm
Right Distance: 32 cm
```
You can adjust sensor thresholds or delay values to fine-tune behavior.

---

## ⚠️ Safety Notes
- Do not operate on wet or metallic surfaces.
- Ensure sufficient ventilation for the ESP32-CAM.
- Avoid prolonged LED illumination — use PWM to limit brightness.
- Disconnect power before rewiring.

---

## 🔄 Reset & Recovery
If the ESP32-CAM becomes unresponsive:
1. Press the **RST** button.
2. If still unresponsive, reflash firmware with minimal sketch.
3. Check serial logs for `Brownout detector was triggered` (use stable 5V power).

---

## 🧠 Troubleshooting
| Problem | Possible Cause | Solution |
|----------|----------------|-----------|
| No video stream | Wrong IP / Wi-Fi config | Recheck SSID and password |
| Camera init failed | Incorrect pin map | Ensure `camera_pins.h` matches board |
| Motors not moving | Wrong wiring or PWM config | Verify L298N inputs and enable pins |
| Sensor always reads 0 | Wiring reversed | Swap TRIG/ECHO connections |

---

## ✅ Successful Operation
When configured correctly, you should observe:
- Smooth forward movement with reactive obstacle avoidance.
- Live MJPEG feed accessible over Wi-Fi.
- Stable operation for up to 1 hour on a 3-cell battery pack.

---

## 📦 Maintenance
- Recharge or replace Li-ion batteries regularly.
- Inspect motor gears for dust buildup.
- Clean camera lens for optimal visibility.

---

**Author:** Samuel Huang  
**License:** MIT