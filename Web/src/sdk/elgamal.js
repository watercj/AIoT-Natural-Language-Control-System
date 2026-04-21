import bigInt from "big-integer";

class ElGamal {
  constructor() {
    // 初始化素數 p, 生成元 g
    // this.p = bigInt("45767"); // 定義了有限域的範圍
    // this.g = bigInt("5"); // 是用來生成密鑰的基礎數
    this.p = bigInt("45767");
    this.g = bigInt("5");
    this.y = bigInt("7796");
  }
  // (45767, 2646)
  
  generateKeys() {
    // 私鑰 x 隨機生成
    // this.x = bigInt.randBetween(1, this.p.minus(1));
    this.x = bigInt("2646");
    // 公鑰 y = g^x mod p
    this.y = this.g.modPow(this.x, this.p);
    return {
      publicKey: { p: this.p, g: this.g, y: this.y },
      privateKey: this.x,
    };
  }

  encrypt(publicKey, message) {
    const { p, g, y } = publicKey;
    const k = bigInt.randBetween(1, p.minus(1));
    const a = g.modPow(k, p);
    const b = bigInt(message).multiply(y.modPow(k, p)).mod(p);
    // return { a, b };
    return [a.toString(), b.toString()];
  }

  decrypt(privateKey, cipher) {
    const a = bigInt(cipher[0])
    const b = bigInt(cipher[1])
    const s = a.modPow(privateKey, this.p);
    const sInv = s.modInv(this.p);
    const message = b.multiply(sInv).mod(this.p);
    return message.toString();
  }
}

export default ElGamal;