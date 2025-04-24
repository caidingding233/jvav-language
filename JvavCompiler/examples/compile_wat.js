#!/usr/bin/env node
/**
 * 此脚本用于将WebAssembly文本格式(WAT)编译为二进制格式(WASM)
 * 使用方法: node compile_wat.js hello.wat
 */

const fs = require('fs');
const path = require('path');
const { execSync, exec } = require('child_process');

// 命令行参数处理
const args = process.argv.slice(2);
const helpText = `
使用方法: node compile_wat.js <wat文件> [选项]

选项:
  -o, --output <文件>   指定输出的WASM文件名
  -v, --verbose         显示详细输出
  -h, --help            显示此帮助信息

示例:
  node compile_wat.js hello.wat
  node compile_wat.js hello.wat -o custom_output.wasm
  node compile_wat.js hello.wat --verbose
`;

// 帮助选项
if (args.includes('-h') || args.includes('--help') || args.length === 0) {
  console.log(helpText);
  process.exit(0);
}

// 解析命令行参数
let inputFile = null;
let outputFile = null;
let verbose = false;

for (let i = 0; i < args.length; i++) {
  const arg = args[i];
  
  if (arg === '-o' || arg === '--output') {
    outputFile = args[i + 1];
    i++;
  } else if (arg === '-v' || arg === '--verbose') {
    verbose = true;
  } else if (!arg.startsWith('-') && !inputFile) {
    inputFile = arg;
  }
}

// 验证输入文件
if (!inputFile) {
  console.error('错误: 未指定输入文件');
  console.log(helpText);
  process.exit(1);
}

// 检查文件扩展名
if (!inputFile.toLowerCase().endsWith('.wat')) {
  console.warn('警告: 输入文件不是.wat扩展名。继续处理，但可能不是正确的WebAssembly文本格式文件。');
}

// 设置默认输出文件
if (!outputFile) {
  // 使用相同的文件名但扩展名为.wasm
  const inputBasename = path.basename(inputFile, path.extname(inputFile));
  outputFile = `${inputBasename}.wasm`;
}

// 验证输入文件是否存在
if (!fs.existsSync(inputFile)) {
  console.error(`错误: 无法找到输入文件 "${inputFile}"`);
  process.exit(1);
}

// 显示处理信息
if (verbose) {
  console.log(`输入文件: ${inputFile}`);
  console.log(`输出文件: ${outputFile}`);
  console.log('开始编译...');
}

/**
 * 检查是否安装了wat2wasm工具
 * @returns {boolean} 如果工具可用返回true
 */
function checkWat2Wasm() {
  try {
    // 尝试执行wat2wasm --version来检查是否存在
    execSync('wat2wasm --version', { stdio: 'ignore' });
    return true;
  } catch (error) {
    return false;
  }
}

/**
 * 编译WAT文件为WASM
 */
function compileWatToWasm() {
  // 检查工具是否可用
  if (!checkWat2Wasm()) {
    console.error('错误: 未找到wat2wasm工具');
    console.log('\n您需要安装WebAssembly二进制工具包(WABT)，请按照以下步骤安装:');
    
    // 根据系统提供安装指南
    if (process.platform === 'darwin') { // macOS
      console.log('对于macOS (使用Homebrew):');
      console.log('  brew install wabt');
    } else if (process.platform === 'linux') { // Linux
      console.log('对于Ubuntu/Debian:');
      console.log('  sudo apt-get install wabt');
      console.log('\n对于其他Linux发行版:');
      console.log('  请查看包管理器中的wabt软件包');
    } else if (process.platform === 'win32') { // Windows
      console.log('对于Windows:');
      console.log('  1. 访问 https://github.com/WebAssembly/wabt/releases');
      console.log('  2. 下载适用于Windows的最新版本');
      console.log('  3. 解压并将bin目录添加到PATH环境变量');
    }
    
    console.log('\n或者从源码构建:');
    console.log('  git clone --recursive https://github.com/WebAssembly/wabt');
    console.log('  cd wabt');
    console.log('  mkdir build');
    console.log('  cd build');
    console.log('  cmake ..');
    console.log('  cmake --build .');
    
    process.exit(1);
  }

  try {
    // 执行wat2wasm命令
    const command = `wat2wasm "${inputFile}" -o "${outputFile}"`;
    
    if (verbose) {
      console.log(`执行命令: ${command}`);
    }
    
    execSync(command, { stdio: verbose ? 'inherit' : 'ignore' });
    
    // 验证输出文件已创建
    if (fs.existsSync(outputFile)) {
      const sizeBytes = fs.statSync(outputFile).size;
      console.log(`✅ 编译成功: "${inputFile}" -> "${outputFile}" (${sizeBytes} 字节)`);
    } else {
      console.error('❌ 错误: 编译似乎成功但未生成输出文件');
      process.exit(1);
    }
  } catch (error) {
    console.error('❌ 编译失败:');
    console.error(error.message);
    
    // 尝试解析更有用的错误信息
    try {
      // 尝试运行命令并获取更详细的错误信息
      const result = execSync(`wat2wasm "${inputFile}" 2>&1`, { encoding: 'utf-8' });
      
      // 如果有错误信息，显示给用户
      if (result.includes('error:')) {
        console.error('\n详细错误信息:');
        const errorLines = result.split('\n').filter(line => line.includes('error:'));
        errorLines.forEach(line => console.error(line));
      }
    } catch (detailError) {
      // 如果二次尝试也失败，至少显示一些有用的错误信息
      if (detailError.stdout) {
        console.error('\n详细错误信息:');
        console.error(detailError.stdout);
      }
    }
    
    process.exit(1);
  }
}

// 执行编译
compileWatToWasm(); 