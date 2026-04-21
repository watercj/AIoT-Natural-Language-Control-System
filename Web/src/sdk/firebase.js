import { initializeApp } from 'firebase/app'
import { getDatabase, ref, set, get, onValue, remove, push, update, off } from "firebase/database";

// 你的 Firebase 配置對象
const firebaseConfig = {
    apiKey: "apiKey",
    authDomain: "ncku-20250105.firebaseapp.com",
    databaseURL: "https://ncku-20250105-default-rtdb.firebaseio.com",
    projectId: "ncku-20250105",
    storageBucket: "ncku-20250105.firebasestorage.app",
    messagingSenderId: "552149688402",
    appId: "1:552149688402:web:2b43bcc90934570fb6bf8b",
    measurementId: "G-18MGTRWMYW"
};

// 初始化 Firebase 應用
const app = initializeApp(firebaseConfig)

// 初始化 Firestore
const db = getDatabase(app)

const fatherPath = 'chat'

// read
const read = async (path, callback) => {
    let p = `${fatherPath}/${path}`
    console.log(p)
    try {
        const snapshot = await get(ref(db, p));
        if (snapshot.exists()) {
            const data = {...snapshot.val()}
        callback(data)
        } else {
            console.log('No data available');
        }
    } catch (error) {
        console.error('Error reading data:', error);
    }
};
// subscribe
const subscribe = (path, id, callback) => {
    const dbRef = ref(db, path);
    onValue(dbRef, (snapshot) => {
        const data = {...snapshot.val(), id: id}
        callback(data)
    });
};
// 銷訂閱數據變更
const unsubscribe = (path, callback) => {
    const dbRef = ref(db, path);
    off(dbRef, 'value', callback());
  };

// write
const add = (path, data) => {
    const newRef = push(ref(db, path));
    set(newRef, data)
        .catch((error) => {
            console.error('Error adding new item:', error);
        });
    return newRef.key
};

const resetSession = () => {
    const newRef = ref(db, fatherPath);
    update(newRef, {session: ""})
        .catch((error) => {
            console.error('Error updating data:', error);
        });
};

export { add, read, subscribe, unsubscribe, resetSession }
