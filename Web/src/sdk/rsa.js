class RSA {
  constructor() {
      this.p = BigInt(523);
      this.q = BigInt(541);
      this.e = BigInt(199);
      [this.n, this.d] = this.rsaKeygen(this.p, this.q, this.e);
  }

  modInverse(e, phi) {
      function egcd(a, b) {
          if (a === 0n) {
              return [b, 0n, 1n];
          } else {
              const [g, x, y] = egcd(b % a, a);
              return [g, y - (b / a) * x, x];
          }
      }

      const [g, x] = egcd(e, phi);
      if (g !== 1n) {
          throw new Error('Modular inverse does not exist');
      } else {
          return (x % phi + phi) % phi;
      }
  }

  rsaKeygen(p, q, e) {
      const n = p * q;
      const phi = (p - 1n) * (q - 1n);
      const d = this.modInverse(e, phi);
      return [n, d];
  }

  rsaEncrypt(number) {
      return modPow(number, this.e, this.n);
  }

  rsaDecrypt(cipher) {
      return modPow(cipher, this.d, this.n);
  }

  getPublicKey() {
      return { e: this.e, n: this.n };
  }

  getPrivateKey() {
      return { d: this.d, n: this.n };
  }
}

function modPow(base, exp, mod) {
  let result = 1n;
  base = BigInt(base);
  exp = BigInt(exp);
  mod = BigInt(mod);
  while (exp > 0n) {
      if (exp % 2n === 1n) {
          result = (result * base) % mod;
      }
      base = (base * base) % mod;
      exp = exp / 2n;
  }
  return result;
}

export default RSA;