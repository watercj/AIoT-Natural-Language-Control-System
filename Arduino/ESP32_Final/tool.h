#ifndef TOOL_H
#define TOOL_H


#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "config.h"

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Allocation Failed");
  }
}

// --- 顯示工具 (LCD/OLED) ---
void displayValue(String label, float value) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print(label + ": ");
  display.setTextSize(2);
  display.println(value);
  display.display();
}

void displayText(String text) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println(text);
  display.display();
}

// 蜂鳴器獨立控制
void beep(int duration_ms) {
  tone(BUZZER_PIN, 1000);
  delay(duration_ms);
  noTone(BUZZER_PIN);
}



// RGB LED 顏色控制
void setLEDColor(const char* color) {
  if (strcmp(color, "RED") == 0) {
    digitalWrite(LED_R_PIN, HIGH); digitalWrite(LED_G_PIN, LOW);  digitalWrite(LED_Y_PIN, LOW);
  } else if (strcmp(color, "GREEN") == 0) {
    digitalWrite(LED_R_PIN, LOW);  digitalWrite(LED_G_PIN, HIGH); digitalWrite(LED_Y_PIN, LOW);
  } else if (strcmp(color, "YELLOW") == 0) {
    digitalWrite(LED_R_PIN, HIGH); digitalWrite(LED_G_PIN, HIGH); digitalWrite(LED_Y_PIN, LOW);
  } else {
    digitalWrite(LED_R_PIN, LOW);  digitalWrite(LED_G_PIN, LOW);  digitalWrite(LED_Y_PIN, LOW);
  }
}

// LED 邏輯 (OER=紅, OEG=綠, OEY=黃)
void handleLED(String type, String action, int val) {
  String color = "OFF";
  if (type == "OER") color = "RED";
  else if (type == "OEG") color = "GREEN";
  else if (type == "OEY") color = "YELLOW";

  if (action == "C") { // BLINK (Count)
    for(int i=0; i<val; i++) {
      setLEDColor(color.c_str()); 
      delay(300);
      setLEDColor("OFF"); 
      delay(300);
    }
  } else if (action == "T") { // LIGHT (Time/Seconds)
    setLEDColor(color.c_str());
    delay(val * 1000);
    setLEDColor("OFF");
  }
}

// LED 閃爍模式
void blinkLED(const char* color, int times, int interval_ms) {
  for (int i = 0; i < times; i++) {
    setLEDColor(color);
    delay(interval_ms);
    setLEDColor("OFF");
    delay(interval_ms);
  }
}

// LED 定時長亮
void lightLEDForDuration(const char* color, int duration_ms) {
  setLEDColor(color);
  delay(duration_ms);
  setLEDColor("OFF");
}

// 超音波讀取
float getDistance() {
  digitalWrite(TRIG_PIN, LOW); 
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
}


// 溫度讀取
float getTemperature() {
  return dht.readTemperature();
}

// 濕度讀取
float getHumidity() {
  return dht.readHumidity();
}

// 光感讀取
int getLightIntensity() {
  return analogRead(LDR_PIN);
}

// 蜂鳴器
void runBuzzer() {
  tone(BUZZER_PIN, 1000, 200);
}

// 感測器數值獲取 (用於條件判斷)
float getCurrentSensorValue(String type) {
  if (type == "TMP") return getTemperature();
  if (type == "HUM") return getHumidity();
  if (type == "SSC") return getDistance();
  if (type == "SLT") return analogRead(LDR_PIN);
  return 0;
}

#endif