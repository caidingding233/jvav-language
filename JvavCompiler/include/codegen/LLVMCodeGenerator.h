#ifndef JVAV_LLVM_CODE_GENERATOR_H
#define JVAV_LLVM_CODE_GENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ast/AST.h"
#include "JvavCompiler.h"

namespace jvav {

/**
 * LLVM代码生成器类
 * 用于将AST转换为LLVM IR并生成目标平台的代码
 */
class LLVMCodeGenerator {
public:
    LLVMCodeGenerator();
    ~LLVMCodeGenerator();

    /**
     * 生成代码
     * @param ast 抽象语法树
     * @param outputFile 输出文件路径
     * @param targetType 目标类型（可执行文件、库等）
     * @param targetPlatform 目标平台（windows, macos, linux, harmony等）
     * @return 是否成功
     */
    bool generateCode(
        const std::vector<std::unique_ptr<Stmt>>& ast, 
        const std::string& outputFile,
        JvavTargetType targetType,
        const std::string& targetPlatform = ""
    );

private:
    class LLVMCodeGeneratorImpl;
    std::unique_ptr<LLVMCodeGeneratorImpl> impl_;
};

} // namespace jvav

#endif // JVAV_LLVM_CODE_GENERATOR_H 