// --- Blynk Template Information ---
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

/*
 * IoT-based Weather Monitoring System
 * * This project uses an ESP32 to collect data from DHT11 (temperature & humidity),
 * MQ7 (Carbon Monoxide), and a generic PM2.5 (Particulate Matter) sensor.
 * Data is displayed on a 16x2 I2C LCD and sent to the Blynk Cloud for remote monitoring.
 *
 * Required Libraries:
 * 1. Blynk by Volodymyr Shymanskyy (Install from Arduino Library Manager)
 * 2. DHT sensor library by Adafruit (Install from Arduino Library Manager)
 * 3. Adafruit Unified Sensor by Adafruit (Dependency for DHT library)
 * 4. LiquidCrystal_I2C by Frank de Brabander (Install from Arduino Library Manager)
 *
 * Blynk App Setup:
 * 1. Create a new project in the Blynk app.
 * 2. Choose "ESP32 Dev Board" as the hardware.
 * 3. Add widgets to your dashboard and assign Virtual Pins:
 * - Gauge for Temperature (e.g., V0)
 * - Gauge for Humidity (e.g., V1)
 * - Gauge for CO Level (e.g., V2)
 * - Gauge for PM2.5 (e.g., V3)
 * 4. Your Auth Token will be sent to your email. Paste it into the `BLYNK_AUTH_TOKEN` variable below.
 *
 * Hardware Connections:
 * -----------------------------------------------------
 * Component      | ESP32 Pin
 * -----------------------------------------------------
 * DHT11          | GPIO 4  (Data Pin)
 * MQ7 (AOUT)     | GPIO 34 (Analog Input)
 * PM2.5 (Vout)   | GPIO 35 (Analog Input)
 * I2C LCD (SDA)  | GPIO 21
 * I2C LCD (SCL)  | GPIO 22
 * -----------------------------------------------------
 * Note: Connect VCC and GND of all sensors and the LCD 
 * to the appropriate 5V/3.3V and GND pins on the ESP32.
 * The MQ7 sensor requires a pre-heating period to give stable readings.
 */

// Define to print debugging information to the Serial Monitor
#define BLYNK_PRINT Serial

// Include necessary libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Your WiFi credentials
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// Define sensor pins
#define DHTPIN 4          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT 11
#define MQ7_PIN 34        // Analog pin for MQ7 sensor
#define PM25_PIN 35       // Analog pin for PM2.5 sensor

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize I2C LCD (address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize Blynk Timer
BlynkTimer timer;

// This function sends sensor data to Blynk and updates the LCD
void sendSensorData() {
    // Read Temperature and Humidity from DHT11
    float humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DHT Sensor Error");
        return;
    }

    // Read Analog value from MQ7 Carbon Monoxide sensor
    int co_value = analogRead(MQ7_PIN);
    // Simple mapping for demonstration. Calibration is needed for accurate PPM.
    float co_ppm = map(co_value, 0, 4095, 0, 1000); 

    // Read Analog value from PM2.5 sensor
    // Note: This is a simplified reading. Many PM2.5 sensors (like the GP2Y1010AU0F)
    // require a specific pulse to the LED before reading. This code assumes a direct
    // analog output proportional to dust density. Please consult your sensor's datasheet.
    int pm25_value = analogRead(PM25_PIN);
    // Simple mapping for demonstration. Calibration is crucial for accuracy.
    float pm25_density = map(pm25_value, 0, 4095, 0, 500); // ug/m3

    // Print readings to the Serial Monitor for debugging
    Serial.println("-----------------------------");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("CO Level: ");
    Serial.print(co_ppm);
    Serial.println(" ppm");
    Serial.print("PM2.5 Density: ");
    Serial.print(pm25_density);
    Serial.println(" ug/m3");

    // Send data to Blynk Virtual Pins
    Blynk.virtualWrite(V0, temperature); // Virtual Pin for Temperature
    Blynk.virtualWrite(V1, humidity);    // Virtual Pin for Humidity
    Blynk.virtualWrite(V2, co_ppm);      // Virtual Pin for CO Level
    Blynk.virtualWrite(V3, pm25_density);// Virtual Pin for PM2.5

    // Display data on the LCD
    lcd.clear();
    // First line: Temperature and Humidity
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature, 1); // 1 decimal place
    lcd.print("C ");
    lcd.print("H:");
    lcd.print(humidity, 1);
    lcd.print("%");

    // Second line: CO and PM2.5
    lcd.setCursor(0, 1);
    lcd.print("CO:");
    lcd.print(co_ppm, 0); // No decimal places
    lcd.print(" PM:");
    lcd.print(pm25_density, 0);
}

void setup() {
    // Start Serial communication
    Serial.begin(115200);
    Serial.println("Weather Monitoring System Starting...");

    // Initialize the LCD
    lcd.begin(); // <-- FIX: Changed from lcd.init() to lcd.begin()
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print("Blynk...");

    // Initialize the DHT sensor
    dht.begin();

    // Connect to WiFi and Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Check connection
    if (Blynk.connected()) {
        Serial.println("Connected to Blynk!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Connected!");
        delay(1000);
    } else {
        Serial.println("Failed to connect to Blynk.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Connection Fail");
    }

    // Setup a timer to call sendSensorData() every 5 seconds
    timer.setInterval(5000L, sendSensorData);
}

void loop() {
    // Run Blynk functions
    Blynk.run();
    // Run Timer
    timer.run();
}
