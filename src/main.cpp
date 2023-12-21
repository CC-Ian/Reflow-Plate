#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
// #include <EEPROM.h>

#include "graphics.h"

// Device Information
#define HARDWARE_VERSION "1.0"
#define SOFTWARE_VERSION "0.0"

// Pins
#define MOSFET_GATE_PIN 3
#define INCREMENT_SWITCH_PIN 6
#define DECREMENT_SWITCH_PIN 5
#define TEMPERATURE_SENSOR_PIN 4    // A2
#define VOLTAGE_MEASURE_PIN 14      // A0

// Screen config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0X3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void ISR_incrementSwitch() {
  Serial.println("Increment Detected");
}

void ISR_decremetSwitch() {
  Serial.println("decrement Detected");
}

float getTemperature() {
  float t = 0;
  for (byte i = 0; i < 100; i++){ //Poll temp reading 100 times
    t = t + analogRead(TEMPERATURE_SENSOR_PIN);
  }
  return ((t / 100) * -1.46) + 434; //Average, convert to C, and return
}

float getVolts() {
  float v = 0;
  for (byte i = 0; i < 20; i++){ //Poll Voltage reading 20 times
    v = v + analogRead(VOLTAGE_MEASURE_PIN);
  }
  return ((v / 20.0) / 52.0); //Average, convert to V, and return
}


void setup() {
  // Serial interface for debug
  Serial.begin(9600);

  // Configure Pins
  pinMode(MOSFET_GATE_PIN, OUTPUT);
  pinMode(TEMPERATURE_SENSOR_PIN, INPUT);
  pinMode(VOLTAGE_MEASURE_PIN, INPUT);
  Serial.println("IO Configured");

  // Display Configuration
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.drawBitmap(0, 0, logo, logo_width, logo_height, SSD1306_WHITE);
  display.setCursor(80,16);
  display.print("S/W V" + String(SOFTWARE_VERSION));
  display.setCursor(80,24);
  display.print("H/W V" + String(HARDWARE_VERSION));
  display.display();
  delay(1000);

  // Attach interrupts for buttons
  attachInterrupt(INCREMENT_SWITCH_PIN, ISR_incrementSwitch, FALLING);
  attachInterrupt(DECREMENT_SWITCH_PIN, ISR_decremetSwitch, FALLING);
  // 
}

void loop() {

}
