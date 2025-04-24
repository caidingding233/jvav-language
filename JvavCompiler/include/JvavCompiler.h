#ifndef JVAV_COMPILER_H
#define JVAV_COMPILER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <fstream>

// 平台定义
#if defined(JVAV_PLATFORM_WINDOWS)
    #define JVAV_EXPORT __declspec(dllexport)
    #define JVAV_IMPORT __declspec(dllimport)
#else
    #define JVAV_EXPORT __attribute__((visibility("default")))
    #define JVAV_IMPORT
#endif

#if defined(JVAV_BUILD_SHARED)
    #define JVAV_API JVAV_EXPORT
#else
    #define JVAV_API
#endif

// 版本信息
#define JVAV_COMPILER_VERSION_MAJOR 1
#define JVAV_COMPILER_VERSION_MINOR 0
#define JVAV_COMPILER_VERSION_PATCH 0

// 错误码
enum class JvavErrorCode {
    SUCCESS = 0,
    FILE_NOT_FOUND,
    SYNTAX_ERROR,
    TYPE_ERROR,
    NAME_ERROR,
    VALUE_ERROR,
    CODEGEN_ERROR,
    PLATFORM_ERROR,
    INTERNAL_ERROR
};

// 编译目标
enum class JvavTargetType {
    EXECUTABLE,    // 可执行文件
    LIBRARY,       // 动态库
    OBJECT_FILE,   // 目标文件
    ASSEMBLY,      // 汇编代码
    LLVM_IR,       // LLVM中间表示
    WASM           // WebAssembly (默认)
};

// 编译器配置
struct JvavCompilerOptions {
    bool optimize = false;              // 是否优化
    int optimizationLevel = 0;          // 优化级别 (0-3)
    bool emitDebugInfo = false;         // 是否生成调试信息
    bool verbose = false;               // 是否输出详细信息
    JvavTargetType targetType = JvavTargetType::WASM;  // 编译目标类型
    std::string outputFile;             // 输出文件路径
    std::string targetPlatform;         // 目标平台 (windows, macos, linux, harmony)
};

// 编译器主类
class JVAV_API JvavCompiler {
public:
    // 构造函数
    JvavCompiler();
    
    // 析构函数
    ~JvavCompiler();
    
    // 设置编译选项
    void setOptions(const JvavCompilerOptions& options);
    
    // 编译文件
    JvavErrorCode compileFile(const std::string& filePath);
    
    // 编译字符串
    JvavErrorCode compileString(const std::string& source, const std::string& sourceName = "");
    
    // 获取最后一次错误
    const std::string& getLastError() const;
    
    // 获取版本信息
    static std::string getVersionString();

private:
    // PIMPL模式
    class JvavCompilerImpl;
    std::unique_ptr<JvavCompilerImpl> impl_;
    
    // 编译选项
    JvavCompilerOptions options_;
    
    // 最后一次错误
    std::string lastError_;
};

#endif // JVAV_COMPILER_H 