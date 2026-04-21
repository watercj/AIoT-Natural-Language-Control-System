#ifndef FIREBASE_TOOL_H
#define FIREBASE_TOOL_H

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "config.h"
#include "crypto_tool.h"

FirebaseData fbdo;
FirebaseData fbdo2;
FirebaseAuth auth;
FirebaseConfig fb_config;

void initFirebase() {
  fb_config.api_key = API_KEY;
  fb_config.database_url = DATABASE_URL;
  fb_config.token_status_callback = tokenStatusCallback;

  if (Firebase.signUp(&fb_config, &auth, "", "")) {
    Serial.println("Firebase SignUp OK!");
  }
  
  Firebase.begin(&fb_config, &auth);
  Firebase.reconnectWiFi(true);
}

// 上傳加密數據到 RTDB
void updateRTDB(const char* path, String message) {
  if (Firebase.RTDB.setString(&fbdo, path, message)) {
    Serial.println("Saved: " + String(path));
  } else {
    Serial.println("Write Failed: " + fbdo.errorReason());
  }
}

// 讀取並顯示雲端訊息
String readCommandFromFirebase() {
  if (Firebase.RTDB.getString(&fbdo, DATABASE_SESSION)) {
    String sessionId = fbdo.stringData();
    
    if (sessionId.isEmpty() || sessionId == "x") return "";

    String fullPath = "/chat/" + sessionId + "/m_to_edge";
    updateRTDB(DATABASE_SESSION, "x"); // 讀取後重置

    if (Firebase.RTDB.getArray(&fbdo2, fullPath)) {
      String decryptedText = "";
      FirebaseJsonArray &jsonArray = fbdo2.jsonArray();
      
      for (size_t i = 0; i < jsonArray.size(); i++) {
        FirebaseJsonData jsonData;
        jsonArray.get(jsonData, i);
        uint64_t cipherChar = jsonData.stringValue.toInt();
        decryptedText += char(decryptData(cipherChar));
      }
      
      return decryptedText.c_str();
    }
  }

  return "";
}

#endif