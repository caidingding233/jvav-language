#include "JvavREPL.h"
#include <iostream>
#include <string>

/**
 * Jvav终端程序入口点
 * 这是一个独立的可执行程序，可以作为Jvav语言的交互式终端
 */
int main(int argc, char* argv[]) {
    // 解析命令行参数
    JvavCompilerOptions options;
    std::string fileToLoad;
    
    // 设置默认值
    options.targetType = JvavTargetType::WASM;
    options.verbose = false;
    
    // 处理命令行参数
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            std::cout << "Jvav终端 - 交互式Jvav语言环境" << std::endl;
            std::cout << "用法: jvav_terminal [选项] [文件]" << std::endl;
            std::cout << "选项:" << std::endl;
            std::cout << "  -h, --help            显示此帮助信息" << std::endl;
            std::cout << "  -v, --version         显示版本信息" << std::endl;
            std::cout << "  -O<级别>              设置优化级别 (0-3)" << std::endl;
            std::cout << "  --target=<平台>       指定目标平台 (wasm, exe, windows, macos, linux, harmony)" << std::endl;
            std::cout << "  --verbose             显示详细编译信息" << std::endl;
            std::cout << "  --llvm                使用LLVM后端（如果可用）" << std::endl;
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "Jvav终端 v" 
                     << JVAV_COMPILER_VERSION_MAJOR << "." 
                     << JVAV_COMPILER_VERSION_MINOR << "." 
                     << JVAV_COMPILER_VERSION_PATCH << std::endl;
#ifdef JVAV_HAS_LLVM
            std::cout << "LLVM后端: 已启用 (支持原生可执行文件)" << std::endl;
#else
            std::cout << "LLVM后端: 未启用 (仅支持WebAssembly)" << std::endl;
#endif
            return 0;
        } else if (arg.find("-O") == 0) {
            options.optimize = true;
            if (arg.length() > 2) {
                options.optimizationLevel = std::stoi(arg.substr(2, 1));
            }
        } else if (arg.find("--target=") == 0) {
            std::string targetPlatform = arg.substr(9);
            
            if (targetPlatform == "wasm" || targetPlatform == "webassembly") {
                options.targetType = JvavTargetType::WASM;
            } else if (targetPlatform == "exe" || targetPlatform == "executable") {
                options.targetType = JvavTargetType::EXECUTABLE;
            } else if (targetPlatform == "windows" || targetPlatform == "win") {
                options.targetType = JvavTargetType::EXECUTABLE;
                options.targetPlatform = "windows";
            } else if (targetPlatform == "macos" || targetPlatform == "mac") {
                options.targetType = JvavTargetType::EXECUTABLE;
                options.targetPlatform = "macos";
            } else if (targetPlatform == "linux") {
                options.targetType = JvavTargetType::EXECUTABLE;
                options.targetPlatform = "linux";
            } else if (targetPlatform == "harmony") {
                options.targetType = JvavTargetType::EXECUTABLE;
                options.targetPlatform = "harmony";
            } else {
                std::cerr << "警告: 未知的目标平台: " << targetPlatform << std::endl;
            }
        } else if (arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "--llvm") {
#ifdef JVAV_HAS_LLVM
            // 已经启用LLVM，不需要做任何事情
#else
            std::cerr << "警告: LLVM后端未启用，此选项将被忽略" << std::endl;
#endif
        } else if (arg[0] != '-') {
            // 认为是要加载的文件
            fileToLoad = arg;
        } else {
            std::cerr << "警告: 未知选项: " << arg << std::endl;
        }
    }
    
    // 创建并配置REPL
    jvav::JvavREPL repl;
    repl.setOptions(options);
    
    // 如果有指定文件，先加载它
    if (!fileToLoad.empty()) {
        if (!repl.loadFile(fileToLoad)) {
            std::cerr << "警告: 加载文件失败，继续进入交互模式" << std::endl;
        }
    }
    
    // 运行REPL
    repl.run();
    
    return 0;
} 