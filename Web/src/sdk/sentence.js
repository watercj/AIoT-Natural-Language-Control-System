import mappingTable from '../assets/file/token2idx_ast.json';

console.log(mappingTable)

const check = (word) => {
    let newWord = word.split('').filter(item => mappingTable.hasOwnProperty(item)).join('')
    return word.length == newWord.length

};

const encrpt = (sentence) => {
    let arr = []
    for(let i = 0; i < sentence.length; i++) {
        // 預設中文
        arr.push(mappingTable[sentence[i]].toString())
    }
    console.log('sentence_encrpt', arr)
    return arr
};
const decrypt = (arr, splitChar) => {
    let newArr = arr.map((item) => {
        let k = getKeyByValue(Number(item))
        if (k === 'lp') k = '{'
        if (k === 'rp') k = '}'
        return k
    })
    return newArr.join(splitChar)
};
const getKeyByValue = (value) => {
    return Object.keys(mappingTable).find(key => mappingTable[key] === value);
}
export { check, encrpt, decrypt }