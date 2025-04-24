#include "JvavREPL.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace jvav {

// 终端颜色定义
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    #define COLOR_RESET  SetConsoleTextAttribute(hConsole, 7)
    #define COLOR_GREEN  SetConsoleTextAttribute(hConsole, 10)
    #define COLOR_YELLOW SetConsoleTextAttribute(hConsole, 14)
    #define COLOR_BLUE   SetConsoleTextAttribute(hConsole, 9)
    #define COLOR_RED    SetConsoleTextAttribute(hConsole, 12)
#else
    #define COLOR_RESET  "\033[0m"
    #define COLOR_GREEN  "\033[32m"
    #define COLOR_YELLOW "\033[33m"
    #define COLOR_BLUE   "\033[34m"
    #define COLOR_RED    "\033[31m"
#endif

JvavREPL::JvavREPL() : shouldContinue_(true) {
    // 设置默认选项
    options_.targetType = JvavTargetType::WASM;  // 默认使用WebAssembly
    options_.optimize = true;
    options_.optimizationLevel = 1;
    options_.verbose = false;
    
    // 设置REPL标准输出文件
    options_.outputFile = "jvav_repl_output";
}

JvavREPL::~JvavREPL() {
    // 清理临时文件
    #ifdef _WIN32
    std::system("del /q jvav_repl_output*");
    #else
    std::system("rm -f jvav_repl_output*");
    #endif
}

void JvavREPL::setOptions(const JvavCompilerOptions& options) {
    options_ = options;
}

void JvavREPL::run() {
    // 显示欢迎信息
    showVersion();
    std::cout << "输入Jvav代码开始交互，输入 :help 查看帮助，输入 :quit 退出" << std::endl;
    
    std::string line;
    
    // REPL主循环
    while (shouldContinue_) {
        // 显示提示符
        if (currentCode_.empty()) {
#ifdef _WIN32
            COLOR_GREEN;
            std::cout << "jvav> ";
            COLOR_RESET;
#else
            std::cout << COLOR_GREEN << "jvav> " << COLOR_RESET;
#endif
        } else {
#ifdef _WIN32
            COLOR_YELLOW;
            std::cout << ".... > ";
            COLOR_RESET;
#else
            std::cout << COLOR_YELLOW << ".... > " << COLOR_RESET;
#endif
        }
        
        // 读取一行输入
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        // 添加到历史记录
        history_.push_back(line);
        
        // 处理输入
        if (line.empty()) {
            // 空行时，如果有累积的代码，则执行
            if (!currentCode_.empty()) {
                compileAndRun(currentCode_);
                currentCode_.clear();
            }
            continue;
        }
        
        // 检查是否是REPL命令（以:开头）
        if (line[0] == ':') {
            if (!handleCommand(line.substr(1))) {
                break;
            }
            continue;
        }
        
        // 累积代码
        if (!currentCode_.empty()) {
            currentCode_ += "\n";
        }
        currentCode_ += line;
        
        // 检查是否以;结尾，如果是则执行当前代码
        if (line.back() == ';') {
            compileAndRun(currentCode_);
            currentCode_.clear();
        }
    }
    
    std::cout << "再见!" << std::endl;
}

bool JvavREPL::handleCommand(const std::string& command) {
    std::string cmd = command;
    std::string arg;
    
    // 提取命令和参数
    size_t spacePos = command.find(' ');
    if (spacePos != std::string::npos) {
        cmd = command.substr(0, spacePos);
        arg = command.substr(spacePos + 1);
    }
    
    // 处理各种命令
    if (cmd == "quit" || cmd == "exit" || cmd == "q") {
        shouldContinue_ = false;
        return false;
    } else if (cmd == "help" || cmd == "h") {
        showHelp();
    } else if (cmd == "version" || cmd == "v") {
        showVersion();
    } else if (cmd == "load" || cmd == "l") {
        if (arg.empty()) {
            std::cout << "错误: 未指定文件名" << std::endl;
        } else {
            loadFile(arg);
        }
    } else if (cmd == "history") {
        // 显示历史记录
        for (size_t i = 0; i < history_.size(); ++i) {
            std::cout << i << ": " << history_[i] << std::endl;
        }
    } else if (cmd == "clear") {
        // 清空当前代码
        currentCode_.clear();
        std::cout << "已清空当前输入" << std::endl;
    } else if (cmd == "optimize") {
        // 设置优化级别
        if (arg.empty() || arg == "off") {
            options_.optimize = false;
            std::cout << "优化已关闭" << std::endl;
        } else {
            options_.optimize = true;
            if (arg == "on") {
                options_.optimizationLevel = 1;
            } else {
                try {
                    int level = std::stoi(arg);
                    if (level >= 0 && level <= 3) {
                        options_.optimizationLevel = level;
                    } else {
                        std::cout << "错误: 优化级别必须在0-3之间" << std::endl;
                    }
                } catch (...) {
                    std::cout << "错误: 无效的优化级别" << std::endl;
                }
            }
            std::cout << "优化级别设置为: " << options_.optimizationLevel << std::endl;
        }
    } else if (cmd == "target") {
        // 设置目标类型
        if (arg == "wasm" || arg == "webassembly") {
            options_.targetType = JvavTargetType::WASM;
            std::cout << "目标平台设置为: WebAssembly" << std::endl;
        } else if (arg == "exe" || arg == "executable") {
            options_.targetType = JvavTargetType::EXECUTABLE;
            std::cout << "目标平台设置为: 可执行文件" << std::endl;
        } else if (arg == "windows" || arg == "win") {
            options_.targetType = JvavTargetType::EXECUTABLE;
            options_.targetPlatform = "windows";
            std::cout << "目标平台设置为: Windows 可执行文件" << std::endl;
        } else if (arg == "macos" || arg == "mac") {
            options_.targetType = JvavTargetType::EXECUTABLE;
            options_.targetPlatform = "macos";
            std::cout << "目标平台设置为: macOS 可执行文件" << std::endl;
        } else if (arg == "linux") {
            options_.targetType = JvavTargetType::EXECUTABLE;
            options_.targetPlatform = "linux";
            std::cout << "目标平台设置为: Linux 可执行文件" << std::endl;
        } else if (arg == "harmony") {
            options_.targetType = JvavTargetType::EXECUTABLE;
            options_.targetPlatform = "harmony";
            std::cout << "目标平台设置为: 鸿蒙 可执行文件" << std::endl;
        } else {
            std::cout << "错误: 未知的目标平台" << std::endl;
        }
    } else {
        std::cout << "未知命令: " << cmd << std::endl;
        std::cout << "输入 :help 查看可用命令" << std::endl;
    }
    
    return true;
}

bool JvavREPL::compileAndRun(const std::string& code) {
    // 编译代码
    compiler_.setOptions(options_);
    
    // 添加一个主要的print语句来确保代码有输出
    std::string wrappedCode = code;
    if (code.find("print") == std::string::npos && code.find("输出") == std::string::npos) {
        // 尝试提取赋值语句右侧的表达式作为输出
        if (code.find("==") != std::string::npos) {
            size_t eqPos = code.find("==");
            size_t semicolonPos = code.find(";", eqPos);
            if (semicolonPos != std::string::npos) {
                std::string expr = code.substr(eqPos + 2, semicolonPos - eqPos - 2);
                wrappedCode = code + "\nprint(" + expr + ");";
            }
        }
    }
    
    // 编译代码
    JvavErrorCode result = compiler_.compileString(wrappedCode, "<repl>");
    
    // 检查编译结果
    if (result != JvavErrorCode::SUCCESS) {
#ifdef _WIN32
        COLOR_RED;
        std::cout << "编译错误: " << compiler_.getLastError() << std::endl;
        COLOR_RESET;
#else
        std::cout << COLOR_RED << "编译错误: " << compiler_.getLastError() << COLOR_RESET << std::endl;
#endif
        return false;
    }
    
    // 如果目标是WASM或可执行文件，尝试执行它
    if (options_.targetType == JvavTargetType::WASM) {
        // 使用Node.js或浏览器运行WASM
#ifdef _WIN32
        std::string command = "node --experimental-wasm-modules -e \"require('./wasm_runner.js').run('" + options_.outputFile + "')\"";
#else
        std::string command = "node --experimental-wasm-modules -e \"require('./wasm_runner.js').run('" + options_.outputFile + "')\"";
#endif
        std::system(command.c_str());
    } else if (options_.targetType == JvavTargetType::EXECUTABLE) {
        // 直接运行可执行文件
#ifdef _WIN32
        std::string command = options_.outputFile + ".exe";
#else
        std::string command = "./" + options_.outputFile;
#endif
        std::system(command.c_str());
    }
    
    return true;
}

bool JvavREPL::loadFile(const std::string& filename) {
    // 打开文件
    std::ifstream file(filename);
    if (!file) {
#ifdef _WIN32
        COLOR_RED;
        std::cout << "无法打开文件: " << filename << std::endl;
        COLOR_RESET;
#else
        std::cout << COLOR_RED << "无法打开文件: " << filename << COLOR_RESET << std::endl;
#endif
        return false;
    }
    
    // 读取文件内容
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    
    // 编译和执行文件
    std::cout << "加载文件: " << filename << std::endl;
    return compileAndRun(code);
}

void JvavREPL::showHelp() {
    std::cout << "Jvav交互式终端帮助：" << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << ":help, :h           - 显示帮助信息" << std::endl;
    std::cout << ":quit, :exit, :q    - 退出终端" << std::endl;
    std::cout << ":version, :v        - 显示版本信息" << std::endl;
    std::cout << ":load, :l [文件名]   - 加载并执行文件" << std::endl;
    std::cout << ":history            - 显示命令历史" << std::endl;
    std::cout << ":clear              - 清除当前输入" << std::endl;
    std::cout << ":optimize [级别]    - 设置优化级别(0-3)" << std::endl;
    std::cout << ":target [目标]      - 设置目标平台" << std::endl;
    std::cout << std::endl;
    std::cout << "示例：" << std::endl;
    std::cout << "print(\"Hello, Jvav!\");" << std::endl;
    std::cout << "var x = 10;" << std::endl;
    std::cout << "if (x > 5) { print(\"x大于5\"); }" << std::endl;
    std::cout << "ask(\"您喜欢Jvav吗?\", \"y/n\"); // 输入y/n" << std::endl;
    std::cout << "ask(\"您的名字是?\", \"free\"); // 自由输入" << std::endl;
}

void JvavREPL::showVersion() const {
    std::cout << "Jvav交互式终端 v" 
              << JVAV_COMPILER_VERSION_MAJOR << "." 
              << JVAV_COMPILER_VERSION_MINOR << "." 
              << JVAV_COMPILER_VERSION_PATCH << std::endl;
    
#ifdef JVAV_HAS_LLVM
    std::cout << "LLVM后端: 已启用 (支持原生可执行文件)" << std::endl;
#else
    std::cout << "LLVM后端: 未启用 (仅支持WebAssembly)" << std::endl;
#endif
}

// 添加输入处理函数
std::string JvavREPL::readUserInput(const std::string& prompt, const std::string& options) {
    std::string input;
    
    std::cout << prompt;
    if (options == "y/n") {
        std::cout << " (y/n): ";
        
        // 读取并验证y/n输入
        while (true) {
            std::getline(std::cin, input);
            if (input == "y" || input == "n") {
                break;
            }
            std::cout << "请只输入y或n: ";
        }
    } else {
        std::cout << ": ";
        std::getline(std::cin, input);
    }
    
    return input;
}

} // namespace jvav 