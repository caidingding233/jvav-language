#ifndef JVAV_CODE_GENERATOR_H
#define JVAV_CODE_GENERATOR_H

#include "ast/AST.h"
#include <string>
#include <vector>
#include <memory>

namespace jvav {

// 代码生成器类
class CodeGenerator {
public:
    CodeGenerator();
    ~CodeGenerator();
    
    // 生成代码
    void generateCode(const std::vector<std::unique_ptr<Stmt>>& ast, const std::string& outputFile);
    
private:
    // 将在后续实现
    class CodeGeneratorImpl;
    std::unique_ptr<CodeGeneratorImpl> impl_;
};

} // namespace jvav

#endif // JVAV_CODE_GENERATOR_H 