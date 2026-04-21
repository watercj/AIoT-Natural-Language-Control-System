#include "config.h"
#include "tool.h"
#include "crypto_tool.h"
#include "firebase_tool.h"
#include "syntax_parser.h"
#include <Wire.h>

// 自定義MQTT訊息處理邏輯
// void myMqttHandler(String topic, String message) {
//   Serial.printf("New Message - Topic: %s, Msg: %s\n", topic.c_str(), message.c_str());

//   // doSometing
// }

SyntaxEngine engine;

void setup() {
  Serial.begin(115200);

  // 初始化腳位
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_Y_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // 初始化溫溼度
  dht.begin();

  // 初始化溫OLED
  initDisplay();

  // 初始化加密
  initCrypto();

  // 初始化網路
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // 初始化Firebase資料庫連線
  initFirebase();
}

void loop() {
  if (Firebase.ready()) {
    // MQTT 連線與監聽
    // handleMQTT();

    // 取得感測器數據
    float temperature = getTemperature();
    float humidity = getHumidity();
    int light = getLightIntensity();
    float dist = getDistance();

    // 上傳溫溼度數據
    if (!isnan(humidity) && !isnan(temperature)) {
      // 加密數據
      uint64_t c_temp = encryptData(int(temperature));
      uint64_t c_humid = encryptData(int(humidity));

      // 上傳雲端
      updateRTDB(DATABASE_TEMP_PATH, String(c_temp));
      updateRTDB(DATABASE_HUMID_PATH, String(c_humid));
    }

    // 上傳光感數據
    if (!isnan(light)) {
      uint64_t c_light = encryptData(light);
      updateRTDB(DATABASE_LIGHT_PATH, String(c_light));
    }

    // 上傳距離數據
    if (!isnan(dist)) {
      uint64_t c_dist = encryptData(int(dist));
      updateRTDB(DATABASE_DIST_PATH, String(c_dist));
    } 

    // 從 Firebase 讀取
    String firebaseCommand = readCommandFromFirebase(); 
    if (firebaseCommand != "x") {
        engine.execute(firebaseCommand); // 一行解析所有指令
    }
  }

  // 週期 0.5 秒
  delay(500);
}