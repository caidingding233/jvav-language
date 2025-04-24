#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ast/AST.h"

namespace jvav {

// 定义目标代码类型枚举
enum class JvavTargetType {
    LLVM_IR,        // 输出LLVM IR文本格式
    OBJECT_FILE,    // 输出目标对象文件
    ASSEMBLY,       // 输出汇编代码
    EXECUTABLE      // 输出可执行文件
};

// LLVM代码生成器
class LLVMCodeGenerator {
public:
    LLVMCodeGenerator();
    ~LLVMCodeGenerator();

    // 生成代码
    // ast: 抽象语法树
    // outputFile: 输出文件路径
    // targetType: 目标代码类型
    // targetPlatform: 目标平台，可以是 "windows", "macos", "linux", "harmony" 或空字符串（默认为当前平台）
    bool generateCode(
        const std::vector<std::unique_ptr<Stmt>>& ast, 
        const std::string& outputFile,
        JvavTargetType targetType,
        const std::string& targetPlatform = ""
    );

private:
    // 使用PIMPL模式隐藏LLVM的具体实现
    class LLVMCodeGeneratorImpl;
    std::unique_ptr<LLVMCodeGeneratorImpl> impl_;
};

} // namespace jvav 