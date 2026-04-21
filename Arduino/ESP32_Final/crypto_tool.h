#ifndef CRYPTO_TOOL_H
#define CRYPTO_TOOL_H

extern "C" {
  #include "ntlib.h"
}

struct RSA rsa;

// 初始化加密演算法
void initCrypto() {
  // 設定參數 p = 523, q = 541, e = 199
  rsa_init(&rsa, 523, 541, 199);
}

// 執行加密
uint64_t encryptData(int value) {
  return rsa.Encrypt(&rsa, value);
}

// 執行解密
int decryptData(uint64_t cipher) {
  return rsa.Decrypt(&rsa, cipher);
}

#endif