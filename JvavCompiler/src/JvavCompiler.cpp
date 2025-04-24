#include "JvavCompiler.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "codegen/CodeGenerator.h"
#include "optimizer/Optimizer.h"
#include "codegen/LLVMCodeGenerator.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <vector>

// 实现JvavCompiler的私有实现类
class JvavCompiler::JvavCompilerImpl {
public:
    JvavCompilerImpl() {}
    ~JvavCompilerImpl() {}
    
    // 编译源代码
    JvavErrorCode compile(const std::string& source, const std::string& sourceName, const JvavCompilerOptions& options, std::string& lastError) {
        try {
            // 创建词法分析器
            jvav::Lexer lexer(source, sourceName);
            
            // 执行词法分析
            if (options.verbose) {
                std::cout << "执行词法分析..." << std::endl;
            }
            
            // 创建语法分析器
            jvav::Parser parser(lexer);
            
            // 执行语法分析
            if (options.verbose) {
                std::cout << "执行语法分析..." << std::endl;
            }
            
            auto ast = parser.parse();
            
            // 检查语法错误
            const auto& errors = parser.getErrors();
            if (!errors.empty()) {
                // 合并所有错误信息
                lastError = "语法分析出错:\n";
                for (const auto& error : errors) {
                    lastError += error + "\n";
                }
                return JvavErrorCode::SYNTAX_ERROR;
            }
            
            // 优化
            if (options.optimize) {
                if (options.verbose) {
                    std::cout << "执行优化，级别: " << options.optimizationLevel << std::endl;
                }
                jvav::Optimizer optimizer;
                optimizer.optimize(ast, options.optimizationLevel);
            }
            
            // 代码生成
            if (options.verbose) {
                std::cout << "生成代码..." << std::endl;
            }
            
            // 确定目标平台
            std::string targetPlatform = options.targetPlatform;
            if (targetPlatform.empty()) {
                // 自动检测平台
#ifdef _WIN32
                targetPlatform = "windows";
#elif defined(__APPLE__)
                targetPlatform = "macos";
#elif defined(__linux__)
                targetPlatform = "linux";
#endif
            }
            
            // 根据目标类型和是否支持LLVM选择代码生成器
            if (options.targetType == JvavTargetType::EXECUTABLE || 
                options.targetType == JvavTargetType::LIBRARY || 
                options.targetType == JvavTargetType::OBJECT_FILE || 
                options.targetType == JvavTargetType::ASSEMBLY || 
                options.targetType == JvavTargetType::LLVM_IR) {
#ifdef JVAV_HAS_LLVM
                // 使用LLVM代码生成器生成原生代码
                if (options.verbose) {
                    std::cout << "使用LLVM代码生成器生成目标代码..." << std::endl;
                }
                
                jvav::LLVMCodeGenerator codeGenerator;
                // 转换目标类型
                jvav::JvavTargetType targetType;
                switch (options.targetType) {
                    case JvavTargetType::EXECUTABLE:
                        targetType = jvav::JvavTargetType::EXECUTABLE;
                        break;
                    case JvavTargetType::LIBRARY:
                        targetType = jvav::JvavTargetType::EXECUTABLE; // LIBRARY暂未实现，使用EXECUTABLE代替
                        break;
                    case JvavTargetType::OBJECT_FILE:
                        targetType = jvav::JvavTargetType::OBJECT_FILE;
                        break;
                    case JvavTargetType::ASSEMBLY:
                        targetType = jvav::JvavTargetType::ASSEMBLY;
                        break;
                    case JvavTargetType::LLVM_IR:
                        targetType = jvav::JvavTargetType::LLVM_IR;
                        break;
                    default:
                        targetType = jvav::JvavTargetType::EXECUTABLE;
                        break;
                }
                
                if (!codeGenerator.generateCode(ast, options.outputFile, targetType, targetPlatform)) {
                    lastError = "LLVM代码生成失败";
                    return JvavErrorCode::CODEGEN_ERROR;
                }
#else
                // LLVM未启用，但需要生成原生代码
                if (options.targetType != JvavTargetType::EXECUTABLE || options.verbose) {
                    std::cout << "警告: LLVM后端未启用，将生成WebAssembly代码并转换..." << std::endl;
                }
                
                // 使用WebAssembly代码生成器，然后通过外部工具转换（不完美的替代方案）
                jvav::CodeGenerator codeGenerator;
                codeGenerator.generateCode(ast, options.outputFile);
                
                // 提醒用户需要启用LLVM支持
                std::cout << "注意: 要生成完整的原生可执行文件，请使用CMake选项 -DJVAV_ENABLE_LLVM=ON 重新构建编译器。" << std::endl;
#endif
            } else {
                // 默认使用WebAssembly代码生成器
                if (options.verbose) {
                    std::cout << "使用WebAssembly代码生成器..." << std::endl;
                }
                
                jvav::CodeGenerator codeGenerator;
                codeGenerator.generateCode(ast, options.outputFile);
            }
            
            // 写入输出文件
            if (options.verbose) {
                std::cout << "写入输出文件: " << options.outputFile << std::endl;
            }
            
            return JvavErrorCode::SUCCESS;
            
        } catch (const std::exception& e) {
            lastError = "编译过程中发生错误: " + std::string(e.what());
            return JvavErrorCode::INTERNAL_ERROR;
        }
    }
};

JvavCompiler::JvavCompiler() : impl_(std::make_unique<JvavCompilerImpl>()) {
    // 默认构造函数
}

JvavCompiler::~JvavCompiler() {
    // 析构函数(impl_会自动被销毁)
}

void JvavCompiler::setOptions(const JvavCompilerOptions& options) {
    options_ = options;
}

JvavErrorCode JvavCompiler::compileFile(const std::string& filePath) {
    // 读取源文件
    std::ifstream file(filePath);
    if (!file) {
        lastError_ = "无法打开源文件: " + filePath;
        return JvavErrorCode::FILE_NOT_FOUND;
    }
    
    // 读取文件内容
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    return compileString(source, filePath);
}

JvavErrorCode JvavCompiler::compileString(const std::string& source, const std::string& sourceName) {
    return impl_->compile(source, sourceName, options_, lastError_);
}

const std::string& JvavCompiler::getLastError() const {
    return lastError_;
}

std::string JvavCompiler::getVersionString() {
    return std::to_string(JVAV_COMPILER_VERSION_MAJOR) + "." +
           std::to_string(JVAV_COMPILER_VERSION_MINOR) + "." +
           std::to_string(JVAV_COMPILER_VERSION_PATCH);
} 