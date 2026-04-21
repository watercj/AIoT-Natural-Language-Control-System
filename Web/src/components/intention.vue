<template>
  <div class="msg-box">
    <header>
      <span>Mode </span>
      <el-select v-model="pageActive" style="width:150px">
        <el-option value="Intention" label="Intention" />
        <el-option value="Elgamel" label="Elgamel 測試" />
        <el-option value="RSA" label="RSA 測試" />
      </el-select>
    </header>

    <main v-if="pageActive === 'Intention'" class="z-msg-body">
      <el-scrollbar ref="content" height="100%" style="width:100%">
        <section v-for="item in items" :key="item.id">
          <div class="right">
            <span class="q-name">Water</span>
            <div>{{ sentence_decrypt(decryptElgamalArrMsg(item.c_question), '') }}</div>
            <span class="time">{{ item.q_time }}</span>
          </div>
          <div class="left">
            <div class="img-bot">
              <el-icon :size="28"><Cpu /></el-icon>
            </div>

            <div class="answer" v-if="item.status === 0">Translating<loading-dot/></div>
            <template v-if="item.status === 1">
              <div class="answer">{{ sentence_decrypt(decryptElgamalArrMsg(item.c_answer), ' ') }}</div>
              <span class="time">{{ item.a_time }}</span>
            </template>
          </div>
        </section>
        <back-Bottom v-if="showBackBottom" @click="scrollToBottom" />
      </el-scrollbar>
    </main>
    <main v-if="pageActive === 'Elgamel'" style="color: white;align-items: flex-start">
      <p><el-input v-model="message" placeholder="輸入要加密的訊息" style="width: 150px" /></p>
      <el-button @click="decryptElgamal">加密訊息</el-button>
      <div v-if="elgamal_cipher">
        <p><strong>加密訊息:</strong> 
        a: <el-input v-model="elgamal_cipher[0]" style="width: 100px" />, 
        b: <el-input v-model="elgamal_cipher[1]" style="width: 100px" /></p>
        <el-button @click="elgamal_decryptMessage">解密訊息</el-button>
        <p v-if="elgamal_decryptedMsg"><strong>解密後的訊息:</strong> {{ elgamal_decryptedMsg }}</p>
      </div>
    </main>
    <main v-if="pageActive === 'RSA'" style="color: white;align-items: flex-start">
      <p><el-input v-model="RSE_message" placeholder="輸入要加密的訊息" /></p>
      <el-button @click="RSA_encryptMsg">加密訊息</el-button>
      <div v-if="RSA_encryptedMsg">
        <p><strong>加密訊息:</strong></p>
        <p><el-input v-model="RSA_encryptedMsg" placeholder="輸入加密訊息" /></p>
        <el-button @click="RSA_decryptMsg(RSA_encryptedMsg)">解密訊息</el-button>
        <p v-if="RSA_decryptedMsg"><strong>解密後的訊息:</strong> {{ RSA_decryptedMsg }}</p>
      </div>
    </main>

    <footer>
      <div v-if="pageActive === 'Intention'">
        <input v-model="newItem" placeholder="Key in your intention" @keyup.enter="addItem" />
        <el-button @click="addItem" type="info" icon="Top" circle :disabled="newItem.length === 0 || temp_id.length > 0" />
      </div>
    </footer>
  </div>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import { add, subscribe, read, unsubscribe }from '../sdk/firebase'
import ElGamal from '../sdk/elgamal';
import RSA from '../sdk/rsa';
import { check as sentence_check, encrpt as sentence_encrpt, decrypt as sentence_decrypt } from '../sdk/sentence';
import loadingDot from './tool/loading-dot.vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import BackBottom from './tool/backBottom.vue';

// 定義響應式數據
const items = ref([])
const newItem = ref('')
const temp_id = ref('')
const temp_question = ref()
const pageActive = ref('Intention') // Intention Elgamel RSA
const content = ref(null)

const elgamal = new ElGamal();
const elgamal_publicKey = ref();
const elgamal_privateKey = ref();
const elgamal_decryptedMsg = ref(null);
const message = ref('');
const elgamal_cipher = ref(null);

const RSE_message = ref('');
const RSA_encryptedMsg = ref('');
const RSA_decryptedMsg = ref('');

const showBackBottom = ref(false);

const rsa = new RSA();

// 組件掛載後讀取數據
onMounted(() => {
  genElgamalKeys()

  setTimeout(() => {
    content.value.wrapRef.addEventListener('scroll', handleScroll);
    handleScroll();
  }, 1000)
})

// #region Firebase
const addItem = () => {
  if (newItem.value.length === 0 || temp_id.value.length > 0) {
    return
  }

  let cq = encryptElgamalArrMsg(sentence_encrpt(newItem.value))
  let param = {
    c_answer: '',
    c_question: cq,
    a_time: '',
    q_time: getTime (new Date()),
    status: 0
  }
  console.log('param', param)
  temp_id.value = add('chat', param)

  temp_question.value = newItem.value
  newItem.value = ''
  subscribeData()

  setTimeout(() => {
    scrollToBottom()
  }, 200);
}
const subscribeData = () => {
  const path = `chat/${temp_id.value}`
  subscribe(path, temp_id.value, (data) => {
    console.log('get data', data)
    switch(data.status) {
      case 0:
        if ((items.value.length > 0 && items.value[items.value.length - 1].id !== data.id) || items.value.length === 0)
          items.value.push(data)
        break
      case 1:
        if(data.c_answer[0] === '$') 
          data.c_answer.substring(1)
        console.log('c_answer', data.c_answer)
        if (['1', '2', '3'].includes(data.c_answer)) {
          // 溫溼度
          read('dht11', (val) => {
            console.log('dht11', val)
            const h = `濕度為: ${rsa.rsaDecrypt(val.humidity)}`
            const t = `溫度為: ${rsa.rsaDecrypt(val.temperature)}`
            let ans = ''
            if (data.c_answer === '1') ans = h
            if (data.c_answer === '2') ans = t
            if (data.c_answer === '3') ans = `${h}, ${t}`
            items.value[items.value.length - 1] = {
              ...data,
              c_answer: ans
            }
          })
          temp_id.value = ""
        } else {
          unsubscribe(path, () => {
            temp_id.value = ""
            items.value[items.value.length - 1] = {
              ...data, 
              c_answer: data.c_answer[0] === '$' ? data.c_answer.substring(1) : data.c_answer 
            }
          })
        }
        break
    }
  })
}
// #endregion Firebase

// #region Elgamal
const genElgamalKeys = () => {
  const keys = elgamal.generateKeys();
  elgamal_publicKey.value = keys.publicKey;
  elgamal_privateKey.value = keys.privateKey;
};
const decryptElgamal = () => {
  elgamal_cipher.value = elgamal.encrypt(elgamal_publicKey.value, message.value);
};
const elgamal_decryptMessage = () => {
  elgamal_decryptedMsg.value = elgamal.decrypt(elgamal_privateKey.value, elgamal_cipher.value);
};
const encryptElgamalArrMsg = (arr) => {
  let res = []
  for(let i = 0; i < arr.length; i++) {
    res.push(elgamal.encrypt(elgamal_publicKey.value, arr[i]))
  }
  console.log('encryptElgamalArrMsg', res)
  return res
};
const decryptElgamalArrMsg = (arr) => {
  let res = []
  for(let i = 0; i < arr.length; i++) {
    res.push(elgamal.decrypt(elgamal_privateKey.value, arr[i]))
  }
  console.log('decryptElgamalArrMsg', res)
  return res
};
// #endregion Elgamal

// #region RSA
const RSA_encryptMsg = () => {
  if (!RSE_message.value) {
    alert('請輸入要加密的訊息');
    return;
  }
  RSA_encryptedMsg.value = rsa.rsaEncrypt(RSE_message.value);
};

const RSA_decryptMsg = (val) => {
  if (!val) {
    alert('請先加密訊息');
    return;
  }
  RSA_decryptedMsg.value = rsa.rsaDecrypt(val);
};
// #endregion RSA

const handleScroll = () => {
  const scrollbar = content.value.wrapRef;
  showBackBottom.value = scrollbar.scrollTop + scrollbar.clientHeight < scrollbar.scrollHeight;
};
const scrollToBottom = () => {
  if (content.value) {
    const sectionHeight = content.value.wrapRef.scrollHeight;
    content.value.scrollTo({
      top: sectionHeight,
      behavior: 'smooth'
    });
  }
};
    
const getTime = (date) => {
  // 獲取日期和時間的各個部分
  const year = date.getFullYear();
  const month = String(date.getMonth() + 1).padStart(2, '0'); // 補零
  const day = String(date.getDate()).padStart(2, '0'); // 補零
  const hour = String(date.getHours()).padStart(2, '0'); // 補零
  const minute = String(date.getMinutes()).padStart(2, '0'); // 補零
  const second = String(date.getSeconds()).padStart(2, '0'); // 補零
  // const millisecond = String(date.getMilliseconds()).padStart(3, '0'); // 補零到三位數

  // 格式化日期時間字串
  const dateString = `${year}-${month}-${day} ${hour}:${minute}:${second}`;
  return dateString; // 例如 "2024-06-01 14:45:36.123"
}
</script>