/**
 * Jvav WASM运行器
 * 用于在REPL中执行编译后的WebAssembly模块
 */

const fs = require('fs');
const path = require('path');

// WebAssembly内存管理
const memory = new WebAssembly.Memory({ initial: 1 });
const memoryBuffer = new Uint8Array(memory.buffer);

// 字符串引用计数器
let nextStringId = 1;
const stringRegistry = new Map();

// 控制台输出函数
const jvavConsole = {
    log: function(value) {
        console.log(value);
    },
    
    log_str: function(id) {
        if (stringRegistry.has(id)) {
            const { ptr, len } = stringRegistry.get(id);
            const bytes = memoryBuffer.slice(ptr, ptr + len);
            const text = new TextDecoder('utf-8').decode(bytes);
            console.log(text);
        } else {
            console.log(`[无效的字符串ID: ${id}]`);
        }
    }
};

// 创建字符串并返回其引用ID
function createString(ptr, len) {
    const bytes = memoryBuffer.slice(ptr, ptr + len);
    const stringData = { ptr, len };
    
    const id = nextStringId++;
    stringRegistry.set(id, stringData);
    
    return id;
}

// 读取字符串，返回指针和长度
function readString(id) {
    if (!stringRegistry.has(id)) {
        console.error(`错误：尝试读取不存在的字符串ID ${id}`);
        return [0, 0];
    }
    
    const { ptr, len } = stringRegistry.get(id);
    return [ptr, len];
}

// 导入对象
const importObject = {
    env: {
        memory,
        createString,
        readString,
        print: jvavConsole.log,
        print_str: jvavConsole.log_str
    },
    console: {
        log: jvavConsole.log
    }
};

// 主要函数：运行WASM文件
async function run(wasmFile) {
    try {
        // 检查文件是否存在
        if (!fs.existsSync(wasmFile)) {
            console.error(`错误: 无法找到WASM文件: ${wasmFile}`);
            return;
        }
        
        // 添加.wasm扩展名（如果需要）
        if (!wasmFile.endsWith('.wasm')) {
            wasmFile += '.wasm';
        }
        
        // 读取WASM文件
        const wasmBuffer = fs.readFileSync(wasmFile);
        
        // 编译WASM模块
        const wasmModule = await WebAssembly.compile(wasmBuffer);
        
        // 实例化WASM模块
        const instance = await WebAssembly.instantiate(wasmModule, importObject);
        
        // 执行main函数（如果存在）
        if (typeof instance.exports.main === 'function') {
            const result = instance.exports.main();
            if (result !== undefined && result !== 0) {
                console.log(`> 返回值: ${result}`);
            }
        }
        
        // 打印导出函数列表（除了main）
        const exports = Object.keys(instance.exports)
            .filter(name => typeof instance.exports[name] === 'function' && name !== 'main');
        
        if (exports.length > 0) {
            console.log('\n可用的导出函数:');
            exports.forEach(name => {
                console.log(`- ${name}()`);
            });
        }
        
        return instance;
    } catch (err) {
        console.error('执行WASM时发生错误:', err.message);
    }
}

// 导出函数
module.exports = {
    run
}; 