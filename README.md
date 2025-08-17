# 🌦️ IoT Weather & Air Quality Monitor

An **IoT-based Weather & Air Quality Monitoring System** using an ESP32 microcontroller.
This project provides **real-time environmental data** including:

* 🌡️ **Temperature & Humidity** (via DHT11)
* 🏭 **Carbon Monoxide (CO) Levels** (via MQ-7 sensor)
* 🌫️ **PM2.5 Particulate Matter Concentration** (via Dust Sensor)

Data is displayed **locally** on a 16x2 LCD and streamed to the **Blynk IoT Cloud** for remote monitoring from a **web dashboard or smartphone app**.

---

## 🚀 Features

* Real-time temperature, humidity, CO, and PM2.5 monitoring
* Local **16x2 I2C LCD display** for immediate sensor readings
* Remote monitoring via **Blynk IoT Cloud**
* Simple **Wi-Fi-based connectivity** using ESP32
* Compact and beginner-friendly design
* Suitable for **home automation, student projects, and health monitoring**

---

## 🛠️ Hardware Required

* **ESP32 Development Board** (with Wi-Fi)
* **DHT11 Sensor** (Temperature & Humidity)
* **MQ-7 Sensor** (Carbon Monoxide detection)
* **PM2.5 Sensor** (e.g., GP2Y1010AU0F or similar)
* **16x2 I2C LCD**
* **Breadboard & Jumper Wires**

---

## 💻 Software & Libraries

* **Arduino IDE** (with ESP32 board support installed)
* **Blynk IoT Account** (for dashboard & authentication token)
* Required Libraries:

  * [Blynk](https://github.com/blynkkk/blynk-library)
  * [Adafruit DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
  * [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
  * [LiquidCrystal\_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

---

## 🔌 Circuit Connections

| Component      | ESP32 Pin | Description                    |
| -------------- | --------- | ------------------------------ |
| **DHT11**      | GPIO 4    | Data (Temp & Humidity)         |
| **MQ-7 AOUT**  | GPIO 34   | Analog output for CO level     |
| **PM2.5 Vout** | GPIO 35   | Analog output for dust density |
| **LCD SDA**    | GPIO 21   | I2C Data line                  |
| **LCD SCL**    | GPIO 22   | I2C Clock line                 |

⚡ Power sensors (5V) from **ESP32 VIN** when powered via USB.

---

## ☁️ Blynk Setup

1. Create a **new template** (Hardware: ESP32, Connection: WiFi).
2. Add **Datastreams**:

   * V0 → Temperature (°C)
   * V1 → Humidity (%)
   * V2 → CO Level (ppm)
   * V3 → PM2.5 (µg/m³)
3. Copy **Template ID, Device Name, and Auth Token**.
4. Use them in the Arduino sketch.

---

## 📜 Usage Instructions

1. Connect ESP32 to your computer and open Arduino IDE.
2. Paste your **Wi-Fi SSID, Password, and Blynk Credentials** in the sketch.
3. Select the correct **ESP32 board** and **COM port**.
4. Upload the code.
5. Open **Blynk app/web dashboard** → Monitor live data!
6. LCD will refresh values every 5 seconds.

---

## 🛠️ Troubleshooting

* **Blynk Credentials Error** → Re-check Template ID, Auth Token.
* **LCD Error (lcd.init / lcd.begin)** → Try changing function call.
* **No LCD Output / Garbled Text** → Verify I2C address (`0x27` or `0x3F`).
* **DHT Sensor Error** → Check wiring & ensure sensor isn’t faulty.
* **Wi-Fi Issues** → ESP32 supports only **2.4 GHz networks**.

---

## 📷 Demo & Screenshots

(Add images or GIFs of hardware setup, LCD output, and Blynk dashboard here.)

---

## 📌 Applications

* 🏠 Smart Home Automation
* 🎓 IoT Learning Projects
* 🌍 Environmental Monitoring
* 🏥 Health & Safety Systems

---

## 📄 License

This project is open-source under the **MIT License**. Feel free to use, modify, and share.
