#include "JvavCompiler.h"
#include "lexer/Lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

void printVersion() {
    std::cout << "Jvav编译器 v" 
              << JVAV_COMPILER_VERSION_MAJOR << "." 
              << JVAV_COMPILER_VERSION_MINOR << "." 
              << JVAV_COMPILER_VERSION_PATCH << std::endl;
    std::cout << "支持平台: ";
    
#if defined(JVAV_PLATFORM_WINDOWS)
    std::cout << "Windows ";
#endif
#if defined(JVAV_PLATFORM_MACOS)
    std::cout << "macOS ";
#endif
#if defined(JVAV_PLATFORM_LINUX)
    std::cout << "Linux ";
#endif

    std::cout << std::endl;
    
#ifdef JVAV_HAS_LLVM
    std::cout << "LLVM后端: 已启用 (支持原生可执行文件)" << std::endl;
#else
    std::cout << "LLVM后端: 未启用 (仅支持WebAssembly)" << std::endl;
#endif
}

void printHelp() {
    std::cout << "使用方法: jvavc [选项] <源文件>" << std::endl;
    std::cout << "选项:" << std::endl;
    std::cout << "  -h, --help            显示帮助信息" << std::endl;
    std::cout << "  -v, --version         显示版本信息" << std::endl;
    std::cout << "  -o <文件>             指定输出文件" << std::endl;
    std::cout << "  -c                    生成目标文件（不链接）" << std::endl;
    std::cout << "  -S                    生成汇编代码" << std::endl;
    std::cout << "  --emit-llvm           生成LLVM IR" << std::endl;
    std::cout << "  --target=<平台>       指定目标平台 (windows, macos, linux, harmony)" << std::endl;
    std::cout << "  --wasm                生成WebAssembly (默认)" << std::endl;
    std::cout << "  -O<级别>              设置优化级别 (0-3)" << std::endl;
    std::cout << "  -g                    生成调试信息" << std::endl;
    std::cout << "  --tokens              仅执行词法分析并输出tokens" << std::endl;
    std::cout << "  --parse               仅执行语法分析" << std::endl;
    std::cout << "  --verbose             显示详细编译信息" << std::endl;
}

// 解析命令行参数并填充编译器选项
bool parseArgs(int argc, char* argv[], JvavCompilerOptions& options, std::string& sourceFile) {
    if (argc < 2) {
        std::cerr << "错误: 未指定源文件" << std::endl;
        printHelp();
        return false;
    }
    
    bool onlyTokens = false;
    bool onlyParse = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printHelp();
            return false;
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            return false;
        } else if (arg == "-o" && i + 1 < argc) {
            options.outputFile = argv[++i];
        } else if (arg == "-c") {
            options.targetType = JvavTargetType::OBJECT_FILE;
        } else if (arg == "-S") {
            options.targetType = JvavTargetType::ASSEMBLY;
        } else if (arg == "--emit-llvm") {
            options.targetType = JvavTargetType::LLVM_IR;
        } else if (arg == "--wasm") {
            options.targetType = JvavTargetType::WASM;
        } else if (arg.find("--target=") == 0) {
            options.targetPlatform = arg.substr(9);
        } else if (arg.rfind("-O", 0) == 0) {
            options.optimize = true;
            if (arg.length() > 2) {
                options.optimizationLevel = std::stoi(arg.substr(2, 1));
            }
        } else if (arg == "-g") {
            options.emitDebugInfo = true;
        } else if (arg == "--tokens") {
            onlyTokens = true;
        } else if (arg == "--parse") {
            onlyParse = true;
        } else if (arg == "--verbose") {
            options.verbose = true;
        } else if (arg[0] != '-') {
            // 认为是源文件
            sourceFile = arg;
        } else {
            std::cerr << "警告: 未知选项: " << arg << std::endl;
        }
    }
    
    if (sourceFile.empty()) {
        std::cerr << "错误: 未指定源文件" << std::endl;
        return false;
    }
    
    // 如果未指定输出文件，创建默认名称
    if (options.outputFile.empty()) {
        size_t dotPos = sourceFile.find_last_of('.');
        if (dotPos != std::string::npos) {
            std::string baseName = sourceFile.substr(0, dotPos);
            
            if (options.targetType == JvavTargetType::OBJECT_FILE) {
                options.outputFile = baseName + ".o";
            } else if (options.targetType == JvavTargetType::ASSEMBLY) {
                options.outputFile = baseName + ".s";
            } else if (options.targetType == JvavTargetType::LLVM_IR) {
                options.outputFile = baseName + ".ll";
            } else if (options.targetType == JvavTargetType::WASM) {
                options.outputFile = baseName + ".wasm";
            } else {
                #if defined(JVAV_PLATFORM_WINDOWS)
                options.outputFile = baseName + ".exe";
                #else
                options.outputFile = baseName;
                #endif
            }
        } else {
            #if defined(JVAV_PLATFORM_WINDOWS)
            options.outputFile = sourceFile + ".exe";
            #else
            options.outputFile = sourceFile + ".out";
            #endif
        }
    }
    
    // 特殊模式处理
    if (onlyTokens) {
        // 仅执行词法分析
        std::ifstream file(sourceFile);
        if (!file) {
            std::cerr << "错误: 无法打开源文件: " << sourceFile << std::endl;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source = buffer.str();
        
        jvav::Lexer lexer(source, sourceFile);
        auto tokens = lexer.tokenize();
        
        for (const auto& token : tokens) {
            std::cout << token << std::endl;
        }
        
        if (!lexer.getErrors().empty()) {
            std::cerr << "\n词法错误:" << std::endl;
            for (const auto& error : lexer.getErrors()) {
                std::cerr << error << std::endl;
            }
        }
        
        return false;  // 停止继续编译
    }
    
    // TODO: 添加onlyParse的处理
    
    return true;
}

int main(int argc, char* argv[]) {
    JvavCompilerOptions options;
    std::string sourceFile;
    
    if (!parseArgs(argc, argv, options, sourceFile)) {
        return 1;
    }
    
    JvavCompiler compiler;
    compiler.setOptions(options);
    
    JvavErrorCode result = compiler.compileFile(sourceFile);
    
    if (result != JvavErrorCode::SUCCESS) {
        std::cerr << "编译失败: " << compiler.getLastError() << std::endl;
        return 1;
    }
    
    std::cout << "编译成功: " << options.outputFile << std::endl;
    return 0;
} 