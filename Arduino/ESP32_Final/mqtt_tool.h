#ifndef MQTT_TOOL_H
#define MQTT_TOOL_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// 定義全域物件
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// 定義訊息處理的回呼函式指標，讓主程式決定收到訊息後要做什麼
void (*onMqttMessageReceived)(String, String) = nullptr;

// 內部 Callback：處理來自 Broker 的原始訊息
void internalMqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  // 如果主程式有設定處理邏輯，則傳遞過去
  if (onMqttMessageReceived != nullptr) {
    onMqttMessageReceived(String(topic), message);
  }
}

// MQTT 連線與自動重連
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      // 連線成功後自動訂閱配置中的主題
      mqttClient.subscribe(TOPIC_SUBSCRIBE);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// 初始化 MQTT 設定
void initMQTT(void (*callback)(String, String)) {
  onMqttMessageReceived = callback;
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(internalMqttCallback);
}

// 發送訊息
bool mqttPublish(const char* topic, String payload) {
  if (mqttClient.connected()) {
    return mqttClient.publish(topic, payload.c_str());
  }
  return false;
}

// 處理 MQTT 核心任務
void handleMQTT() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
}

#endif