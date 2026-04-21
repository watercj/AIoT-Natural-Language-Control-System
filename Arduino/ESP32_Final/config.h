#ifndef CONFIG_H
#define CONFIG_H

// WiFi 連線資訊
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

// --- MQTT 配置 ---
#define MQTT_SERVER      "broker.emqx.io"
#define MQTT_PORT        1883
#define MQTT_CLIENT_ID   "ESP32_Client_01"

// --- MQTT 主題 (Topic) ---
#define TOPIC_TEMP       "esp32/sensors/temp"
#define TOPIC_DIST       "esp32/sensors/dist"
#define TOPIC_CMD        "esp32/commands"

// Firebase 專案配置
#define API_KEY "API_KEY"
#define DATABASE_URL "https://intention-eb98e-default-rtdb.firebaseio.com/"
#define DATABASE_ROOT "/chat"
#define DATABASE_SESSION "/chat/session"
#define DATABASE_TEMP_PATH "dht11/temperature"
#define DATABASE_HUMID_PATH "dht11/humidity"
#define DATABASE_LIGHT_PATH "sensors/light"
#define DATABASE_DIST_PATH "sensors/distance"

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// 硬體腳位定義
#define DHT_PIN 32
#define DHT_TYPE DHT11
#define LDR_PIN 34
#define TRIG_PIN 25
#define ECHO_PIN 26

#define LED_R_PIN 14
#define LED_G_PIN 12
#define LED_Y_PIN 13

#define BUZZER_PIN 27

#endif