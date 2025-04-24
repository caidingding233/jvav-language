// WebAssembly字符串处理运行时
// 提供字符串操作的辅助函数和内存管理

// 创建一个新的内存实例（1页 = 64KB）
const memory = new WebAssembly.Memory({ initial: 1 });
const memoryBuffer = new Uint8Array(memory.buffer);

// 字符串引用计数器，用于分配字符串ID
let nextStringId = 1;
// 字符串注册表，存储字符串的位置和长度
const stringRegistry = new Map();

// 用户输入函数，返回字符串ID
function ask(questionId, optionsId) {
  // 读取问题
  const [qPtr, qLen] = readString(questionId);
  const questionBytes = memoryBuffer.slice(qPtr, qPtr + qLen);
  const question = bytesToString(questionBytes);
  
  // 读取选项
  const [oPtr, oLen] = readString(optionsId);
  const optionsBytes = memoryBuffer.slice(oPtr, oPtr + oLen);
  const options = bytesToString(optionsBytes);
  
  // 根据选项类型处理
  let answer;
  if (options === "y/n") {
    answer = prompt(question + " (y/n)");
    // 验证输入格式
    while (answer !== null && answer !== "y" && answer !== "n") {
      answer = prompt(question + " (请只输入 y 或 n)");
    }
  } else {
    // 自由输入模式
    answer = prompt(question);
  }
  
  // 如果用户取消，返回空字符串
  if (answer === null) {
    answer = "";
  }
  
  // 将答案转换为字节并保存到内存
  const answerBytes = new TextEncoder().encode(answer);
  const answerLen = answerBytes.length;
  
  // 分配内存
  const ptr = allocateMemory(answerLen);
  // 复制到内存
  memoryBuffer.set(answerBytes, ptr);
  
  // 创建并返回字符串ID
  return createString(ptr, answerLen);
}

// 分配内存函数
function allocateMemory(size) {
  // 简单的内存分配，实际中可能需要更复杂的内存管理
  const ptr = 1024; // 从1KB开始分配
  return ptr;
}

// 创建字符串并返回其引用ID
function createString(ptr, len) {
  // 从内存中读取字符串数据
  const bytes = memoryBuffer.slice(ptr, ptr + len);
  const stringData = { ptr, len };
  
  // 分配新的字符串ID并注册
  const id = nextStringId++;
  stringRegistry.set(id, stringData);
  
  console.log(`创建字符串 ID=${id}, 内容="${bytesToString(bytes)}"`);
  
  return id;
}

// 读取字符串，返回指针和长度（多返回值）
function readString(id) {
  if (!stringRegistry.has(id)) {
    console.error(`错误：尝试读取不存在的字符串ID ${id}`);
    return [0, 0]; // 返回空字符串
  }
  
  const { ptr, len } = stringRegistry.get(id);
  console.log(`读取字符串 ID=${id}, 指针=${ptr}, 长度=${len}`);
  
  // 在实际使用中，这里可能会对字符串进行更复杂的处理
  
  // 在WASM中返回多个值：这里返回[ptr, len]
  // 在调用函数中，第一个值会被存储到栈上，第二个值会被保存到结果中
  return [ptr, len];
}

// 将字节数组转换为JavaScript字符串（支持UTF-8）
function bytesToString(bytes) {
  return new TextDecoder('utf-8').decode(bytes);
}

// WASM实例的导入对象
const imports = {
  env: {
    memory,
    createString,
    readString,
    ask
  }
};

// 加载并初始化WASM模块
async function loadWasmModule(wasmPath) {
  try {
    const response = await fetch(wasmPath);
    const wasmBuffer = await response.arrayBuffer();
    const wasmModule = await WebAssembly.instantiate(wasmBuffer, imports);
    
    // 获取WASM导出的函数
    const instance = wasmModule.instance;
    const { 
      getHelloString, 
      getChineseString, 
      concatenateStrings,
      reverseString
    } = instance.exports;

    // 测试字符串操作
    console.log("==== 字符串测试 ====");
    
    // 获取示例字符串并显示
    const str1Id = getHelloString();
    console.log(`字符串1 ID: ${str1Id}`);
    displayString(str1Id);
    
    const str2Id = getChineseString();
    console.log(`字符串2 ID: ${str2Id}`);
    displayString(str2Id);
    
    // 测试字符串连接
    const concatId = concatenateStrings(str1Id, str2Id);
    console.log("连接后的字符串:");
    displayString(concatId);
    
    // 测试字符串反转
    const reversedId = reverseString(str1Id);
    console.log("反转后的字符串:");
    displayString(reversedId);
    
    return instance;
  } catch (err) {
    console.error("加载WASM模块时出错:", err);
    throw err;
  }
}

// 辅助函数：显示字符串内容
function displayString(id) {
  if (!stringRegistry.has(id)) {
    console.error(`错误：尝试显示不存在的字符串ID ${id}`);
    return;
  }
  
  const { ptr, len } = stringRegistry.get(id);
  const bytes = memoryBuffer.slice(ptr, ptr + len);
  const text = bytesToString(bytes);
  
  console.log(`字符串内容 (ID=${id}): "${text}"`);
}

// 导出主要函数
module.exports = {
  loadWasmModule,
  createString,
  readString,
  bytesToString
};

// 如果直接运行此脚本，加载并运行示例WASM模块
if (require.main === module) {
  const wasmPath = process.argv[2] || './string_example.wasm';
  console.log(`加载WASM模块: ${wasmPath}`);
  
  loadWasmModule(wasmPath)
    .then(() => console.log("WASM模块测试完成"))
    .catch(err => console.error("测试过程中出错:", err));
} 