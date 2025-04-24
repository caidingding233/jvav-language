#include "codegen/CodeGenerator.h"
#include "compiler/Functions.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>

namespace jvav {

// 前向声明私有实现类
class CodeGenerator::CodeGeneratorImpl {
public:
    void generateCode(const std::vector<std::unique_ptr<Stmt>>& ast, const std::string& outputFile) {
        // 重置状态
        resetState();
        
        // 生成模块头
        generateModuleHeader();
        
        // 生成导入函数
        generateImports();
        
        // 生成全局变量和主函数
        generateGlobals(ast);
        
        // 生成模块尾
        generateModuleFooter();
        
        // 写入输出文件
        writeToFile(outputFile);
        
        std::cout << "WebAssembly代码生成完成: " << outputFile << std::endl;
    }

private:
    // 代码缓冲区
    std::stringstream codeBuffer_;
    
    // 变量映射表
    std::unordered_map<std::string, int> variables_;
    
    // 函数映射表
    std::unordered_map<std::string, std::string> functions_;
    
    // 全局变量计数
    int globalVarCount_ = 0;
    
    // 局部变量计数
    int localVarCount_ = 0;
    
    // 当前函数名
    std::string currentFunction_;
    
    // 重置生成器状态
    void resetState();
    
    // 生成模块头
    void generateModuleHeader();
    
    // 生成模块尾
    void generateModuleFooter();
    
    // 生成导入函数
    void generateImports();
    
    // 生成全局变量
    void generateGlobals(const std::vector<std::unique_ptr<Stmt>>& ast);
    
    // 生成语句代码
    void generateStatement(const Stmt* stmt);
    
    // 生成打印语句
    void generatePrintStatement(const PrintStmt* stmt);
    
    // 生成设置变量语句
    void generateSetStatement(const SetStmt* stmt);
    
    // 生成IF语句
    void generateIfStatement(const IfStmt* stmt);
    
    // 生成循环语句
    void generateLoopStatement(const LoopStmt* stmt);
    
    // 生成函数定义
    void generateDefineStatement(const DefineStmt* stmt);
    
    // 生成表达式语句
    void generateExpressionStatement(const ExpressionStmt* stmt);
    
    // 生成语句块
    void generateBlockStatement(const BlockStmt* stmt);
    
    // 生成try-catch语句
    void generateTryCatchStatement(const TryCatchStmt* stmt);
    
    // 生成表达式代码
    void generateExpression(const Expr* expr);
    
    // 生成字面量表达式
    void generateLiteralExpression(const LiteralExpr* expr);
    
    // 生成变量引用表达式
    void generateVariableExpression(const VariableExpr* expr);
    
    // 生成二元表达式
    void generateBinaryExpression(const BinaryExpr* expr);
    
    // 生成一元表达式
    void generateUnaryExpression(const UnaryExpr* expr);
    
    // 生成函数调用表达式
    void generateCallExpression(const CallExpr* expr);
    
    // 生成赋值表达式
    void generateAssignmentExpression(const AssignmentExpr* expr);
    
    // 写入输出文件
    void writeToFile(const std::string& outputFile);
};

// 重置生成器状态
void CodeGenerator::CodeGeneratorImpl::resetState() {
    codeBuffer_.str("");
    variables_.clear();
    functions_.clear();
    globalVarCount_ = 0;
    localVarCount_ = 0;
    currentFunction_ = "";
}

// 生成模块头
void CodeGenerator::CodeGeneratorImpl::generateModuleHeader() {
    codeBuffer_ << "(module\n";
}

// 生成模块尾
void CodeGenerator::CodeGeneratorImpl::generateModuleFooter() {
    codeBuffer_ << ")\n";
}

// 生成导入函数
void CodeGenerator::CodeGeneratorImpl::generateImports() {
    // 导入控制台函数
    codeBuffer_ << "  ;; 导入控制台和运行时函数\n";
    codeBuffer_ << "  (import \"console\" \"log\" (func $console_log (param i32)))\n";
    codeBuffer_ << "  (import \"console\" \"log_str\" (func $console_log_str (param i32)))\n";
    // 添加ask函数导入
    codeBuffer_ << "  (import \"env\" \"ask\" (func $ask (param i32 i32) (result i32)))\n\n";

    // 导入内存
    codeBuffer_ << "  ;; 导入内存\n";
    codeBuffer_ << "  (import \"js\" \"mem\" (memory 1))\n\n";
    
    // 常用的工具函数
    codeBuffer_ << "  ;; 工具函数\n";
    codeBuffer_ << "  (func $print_number (param $num i32)\n";
    codeBuffer_ << "    local.get $num\n";
    codeBuffer_ << "    call $console_log\n";
    codeBuffer_ << "  )\n\n";
    
    // 添加全局临时变量
    codeBuffer_ << "  ;; 全局临时变量\n";
    codeBuffer_ << "  (global $temp (mut i32) (i32.const 0))\n\n";
}

// 生成全局变量
void CodeGenerator::CodeGeneratorImpl::generateGlobals(const std::vector<std::unique_ptr<Stmt>>& ast) {
    codeBuffer_ << "  ;; 全局变量定义\n";
    
    // 遍历AST寻找全局变量定义
    for (const auto& stmt : ast) {
        if (stmt->getType() == StmtType::SET) {
            auto setStmt = static_cast<const SetStmt*>(stmt.get());
            std::string varName = setStmt->name.getValue();
            
            // 添加到变量映射表
            variables_[varName] = globalVarCount_++;
            
            // 生成全局变量定义
            codeBuffer_ << "  (global $" << varName << " (mut i32) (i32.const 0))\n";
        }
    }
    
    // 添加main函数
    codeBuffer_ << "\n  ;; 主函数\n";
    codeBuffer_ << "  (func $main (result i32)\n";
    
    // 添加临时变量
    codeBuffer_ << "    ;; 局部临时变量\n";
    codeBuffer_ << "    (local $temp i32)\n\n";
    
    // 遍历AST生成执行代码
    for (const auto& stmt : ast) {
        // 跳过全局变量定义，它们已经被处理
        if (stmt->getType() != StmtType::SET) {
            generateStatement(stmt.get());
        }
    }
    
    // 返回0
    codeBuffer_ << "    i32.const 0 ;; 主函数返回值\n";
    codeBuffer_ << "  )\n\n";
    
    // 导出main函数
    codeBuffer_ << "  (export \"main\" (func $main))\n\n";
}

// 生成语句代码
void CodeGenerator::CodeGeneratorImpl::generateStatement(const Stmt* stmt) {
    switch (stmt->getType()) {
        case StmtType::PRINT:
            generatePrintStatement(static_cast<const PrintStmt*>(stmt));
            break;
        case StmtType::SET:
            generateSetStatement(static_cast<const SetStmt*>(stmt));
            break;
        case StmtType::IF:
            generateIfStatement(static_cast<const IfStmt*>(stmt));
            break;
        case StmtType::LOOP:
            generateLoopStatement(static_cast<const LoopStmt*>(stmt));
            break;
        case StmtType::DEFINE:
            generateDefineStatement(static_cast<const DefineStmt*>(stmt));
            break;
        case StmtType::EXPRESSION:
            generateExpressionStatement(static_cast<const ExpressionStmt*>(stmt));
            break;
        case StmtType::BLOCK:
            generateBlockStatement(static_cast<const BlockStmt*>(stmt));
            break;
        case StmtType::TRY_CATCH:
            generateTryCatchStatement(static_cast<const TryCatchStmt*>(stmt));
            break;
        default:
            std::cerr << "警告: 未支持的语句类型 " << (int)stmt->getType() << std::endl;
            break;
    }
}

// 生成打印语句
void CodeGenerator::CodeGeneratorImpl::generatePrintStatement(const PrintStmt* stmt) {
    codeBuffer_ << "  ;; 打印语句\n";
    generateExpression(stmt->value.get());
    codeBuffer_ << "  call $print_number\n\n";
}

// 生成设置变量语句
void CodeGenerator::CodeGeneratorImpl::generateSetStatement(const SetStmt* stmt) {
    codeBuffer_ << "  ;; 设置变量: " << stmt->name.getValue() << "\n";
    
    // 检查变量是否已经存在
    std::string varName = stmt->name.getValue();
    if (variables_.find(varName) == variables_.end()) {
        // 添加到变量映射表
        variables_[varName] = globalVarCount_++;
        
        // 生成全局变量定义
        codeBuffer_ << "  (global $" << varName << " (mut i32) (i32.const 0))\n";
    }
    
    // 生成表达式代码
    generateExpression(stmt->value.get());
    
    // 设置变量值
    codeBuffer_ << "  global.set $" << varName << "\n\n";
}

// 生成IF语句
void CodeGenerator::CodeGeneratorImpl::generateIfStatement(const IfStmt* stmt) {
    codeBuffer_ << "  ;; IF语句\n";
    
    // 生成条件代码
    generateExpression(stmt->branches[0].condition.get());
    
    // 生成IF结构
    codeBuffer_ << "  (if\n";
    codeBuffer_ << "    (then\n";
    
    // 生成IF块中的代码
    for (const auto& bodyStmt : stmt->branches[0].body) {
        generateStatement(bodyStmt.get());
    }
    
    codeBuffer_ << "    )\n";
    
    // 如果有ELSE块
    if (stmt->branches.size() > 1 && !stmt->branches.back().condition) {
        codeBuffer_ << "    (else\n";
        
        // 生成ELSE块中的代码
        for (const auto& bodyStmt : stmt->branches.back().body) {
            generateStatement(bodyStmt.get());
        }
        
        codeBuffer_ << "    )\n";
    }
    
    codeBuffer_ << "  )\n\n";
}

// 生成循环语句
void CodeGenerator::CodeGeneratorImpl::generateLoopStatement(const LoopStmt* stmt) {
    codeBuffer_ << "  ;; 循环语句\n";
    
    // 初始化循环计数器
    std::string iterName = stmt->variable.getValue();
    if (!iterName.empty()) {
        codeBuffer_ << "  (local $" << iterName << " i32)\n";
        codeBuffer_ << "  i32.const 0\n";
        codeBuffer_ << "  local.set $" << iterName << "\n";
    }
    
    // 生成循环次数表达式
    generateExpression(stmt->count.get());
    codeBuffer_ << "  (local $loop_count i32)\n";
    codeBuffer_ << "  local.set $loop_count\n";
    
    // 循环结构
    codeBuffer_ << "  (loop $loop\n";
    
    // 循环体
    for (const auto& bodyStmt : stmt->body) {
        generateStatement(bodyStmt.get());
    }
    
    // 增加迭代器
    if (!iterName.empty()) {
        codeBuffer_ << "    local.get $" << iterName << "\n";
        codeBuffer_ << "    i32.const 1\n";
        codeBuffer_ << "    i32.add\n";
        codeBuffer_ << "    local.set $" << iterName << "\n";
    }
    
    // 循环条件
    codeBuffer_ << "    local.get $" << iterName << "\n";
    codeBuffer_ << "    local.get $loop_count\n";
    codeBuffer_ << "    i32.lt_s\n";
    codeBuffer_ << "    br_if $loop\n";
    codeBuffer_ << "  )\n\n";
}

// 生成函数定义
void CodeGenerator::CodeGeneratorImpl::generateDefineStatement(const DefineStmt* stmt) {
    std::string funcName = stmt->name.getValue();
    currentFunction_ = funcName;
    
    codeBuffer_ << "  ;; 函数定义: " << funcName << "\n";
    codeBuffer_ << "  (func $" << funcName;
    
    // 参数列表
    for (const auto& param : stmt->parameters) {
        codeBuffer_ << " (param $" << param.getValue() << " i32)";
    }
    
    // 暂时所有函数都返回i32
    codeBuffer_ << " (result i32)\n";
    
    // 添加局部变量
    codeBuffer_ << "    (local $temp i32)\n";
    
    // 函数体
    for (const auto& bodyStmt : stmt->body) {
        // 检查是否是return语句
        if (bodyStmt->getType() == StmtType::RETURN) {
            auto* returnStmt = static_cast<const ReturnStmt*>(bodyStmt.get());
            if (returnStmt->value) {
                // 生成返回值表达式
                generateExpression(returnStmt->value.get());
                // 返回值已经在栈顶
                codeBuffer_ << "    return\n";
            } else {
                // 没有返回值，返回0
                codeBuffer_ << "    i32.const 0\n";
                codeBuffer_ << "    return\n";
            }
        } else {
            generateStatement(bodyStmt.get());
        }
    }
    
    // 默认返回0
    codeBuffer_ << "    i32.const 0 ;; 默认返回值\n";
    codeBuffer_ << "  )\n\n";
    
    // 导出函数
    codeBuffer_ << "  (export \"" << funcName << "\" (func $" << funcName << "))\n\n";
    
    currentFunction_ = "";
}

// 生成表达式语句
void CodeGenerator::CodeGeneratorImpl::generateExpressionStatement(const ExpressionStmt* stmt) {
    generateExpression(stmt->expression.get());
    // 丢弃表达式结果
    codeBuffer_ << "  drop\n";
}

// 生成语句块
void CodeGenerator::CodeGeneratorImpl::generateBlockStatement(const BlockStmt* stmt) {
    codeBuffer_ << "  ;; 语句块\n";
    codeBuffer_ << "  (block\n";
    
    for (const auto& bodyStmt : stmt->statements) {
        generateStatement(bodyStmt.get());
    }
    
    codeBuffer_ << "  )\n";
}

// 生成try-catch语句
void CodeGenerator::CodeGeneratorImpl::generateTryCatchStatement(const TryCatchStmt* stmt) {
    codeBuffer_ << "  ;; Try-Catch语句\n";
    codeBuffer_ << "  (block $try_block\n";
    
    // try块中的代码
    for (const auto& tryStmt : stmt->tryBlock) {
        generateStatement(tryStmt.get());
    }
    
    codeBuffer_ << "  )\n";
    
    // 在WebAssembly中暂时不支持异常处理，这里只是简单执行catch块
    codeBuffer_ << "  ;; Catch块 (简化实现，不支持真正的异常处理)\n";
    codeBuffer_ << "  (block $catch_block\n";
    
    for (const auto& catchStmt : stmt->catchBlock) {
        generateStatement(catchStmt.get());
    }
    
    codeBuffer_ << "  )\n\n";
}

// 生成表达式代码
void CodeGenerator::CodeGeneratorImpl::generateExpression(const Expr* expr) {
    switch (expr->getType()) {
        case ExprType::LITERAL:
            generateLiteralExpression(static_cast<const LiteralExpr*>(expr));
            break;
        case ExprType::VARIABLE:
            generateVariableExpression(static_cast<const VariableExpr*>(expr));
            break;
        case ExprType::BINARY:
            generateBinaryExpression(static_cast<const BinaryExpr*>(expr));
            break;
        case ExprType::UNARY:
            generateUnaryExpression(static_cast<const UnaryExpr*>(expr));
            break;
        case ExprType::CALL:
            generateCallExpression(static_cast<const CallExpr*>(expr));
            break;
        case ExprType::ASSIGNMENT:
            generateAssignmentExpression(static_cast<const AssignmentExpr*>(expr));
            break;
        default:
            std::cerr << "警告: 未支持的表达式类型 " << (int)expr->getType() << std::endl;
            // 默认值
            codeBuffer_ << "  i32.const 0 ;; 未支持的表达式\n";
            break;
    }
}

// 生成字面量表达式
void CodeGenerator::CodeGeneratorImpl::generateLiteralExpression(const LiteralExpr* expr) {
    const Token& token = expr->token;
    
    switch (token.getType()) {
        case TokenType::NUMBER_LITERAL: {
            // 数字字面量
            int value = std::stoi(token.getValue());
            codeBuffer_ << "  i32.const " << value << "\n";
            break;
        }
        case TokenType::BOOL_LITERAL: {
            // 布尔字面量
            bool value = (token.getValue() == "true" || token.getValue() == "真");
            codeBuffer_ << "  i32.const " << (value ? 1 : 0) << "\n";
            break;
        }
        case TokenType::STRING_LITERAL: {
            // 字符串字面量 - 这需要更复杂的内存管理，简化处理
            codeBuffer_ << "  i32.const 0 ;; 字符串暂不支持\n";
            break;
        }
        default:
            codeBuffer_ << "  i32.const 0 ;; 未知的字面量类型\n";
            break;
    }
}

// 生成变量引用表达式
void CodeGenerator::CodeGeneratorImpl::generateVariableExpression(const VariableExpr* expr) {
    std::string varName = expr->name.getValue();
    
    if (variables_.find(varName) != variables_.end()) {
        codeBuffer_ << "  global.get $" << varName << "\n";
    } else {
        std::cerr << "警告: 使用未定义的变量 " << varName << std::endl;
        codeBuffer_ << "  i32.const 0 ;; 未定义的变量\n";
    }
}

// 生成二元表达式
void CodeGenerator::CodeGeneratorImpl::generateBinaryExpression(const BinaryExpr* expr) {
    // 生成左右操作数
    generateExpression(expr->left.get());
    generateExpression(expr->right.get());
    
    // 生成操作符
    switch (expr->op.getType()) {
        case TokenType::PLUS:
            codeBuffer_ << "  i32.add\n";
            break;
        case TokenType::MINUS:
            codeBuffer_ << "  i32.sub\n";
            break;
        case TokenType::STAR:
            codeBuffer_ << "  i32.mul\n";
            break;
        case TokenType::SLASH:
            codeBuffer_ << "  i32.div_s\n";
            break;
        case TokenType::PERCENT:
            codeBuffer_ << "  i32.rem_s\n";
            break;
        case TokenType::EQUAL:
            codeBuffer_ << "  i32.eq\n";
            break;
        case TokenType::NOT_EQUAL:
            codeBuffer_ << "  i32.ne\n";
            break;
        case TokenType::LESS:
            codeBuffer_ << "  i32.lt_s\n";
            break;
        case TokenType::LESS_EQUAL:
            codeBuffer_ << "  i32.le_s\n";
            break;
        case TokenType::GREATER:
            codeBuffer_ << "  i32.gt_s\n";
            break;
        case TokenType::GREATER_EQUAL:
            codeBuffer_ << "  i32.ge_s\n";
            break;
        case TokenType::AND:
            codeBuffer_ << "  i32.and\n";
            break;
        case TokenType::OR:
            codeBuffer_ << "  i32.or\n";
            break;
        default:
            std::cerr << "警告: 未支持的二元操作符 " << (int)expr->op.getType() << std::endl;
            break;
    }
}

// 生成一元表达式
void CodeGenerator::CodeGeneratorImpl::generateUnaryExpression(const UnaryExpr* expr) {
    generateExpression(expr->right.get());
    
    switch (expr->op.getType()) {
        case TokenType::MINUS:
            codeBuffer_ << "  i32.const -1\n";
            codeBuffer_ << "  i32.mul\n";
            break;
        case TokenType::NOT:
            codeBuffer_ << "  i32.eqz\n";
            break;
        default:
            std::cerr << "警告: 未支持的一元操作符 " << (int)expr->op.getType() << std::endl;
            break;
    }
}

// 生成函数调用表达式
void CodeGenerator::CodeGeneratorImpl::generateCallExpression(const CallExpr* expr) {
    // 获取被调用的函数名
    if (expr->callee->getType() != ExprType::VARIABLE) {
        std::cerr << "警告: 只支持简单函数调用\n";
        codeBuffer_ << "  i32.const 0 ;; 不支持的函数调用\n";
        return;
    }
    
    auto* varExpr = static_cast<const VariableExpr*>(expr->callee.get());
    std::string funcName = varExpr->name.getValue();
    
    // 检查是否是内置函数
    bool isBuiltin = BuiltinFunctions::isBuiltin(funcName);
    BuiltinFunctionType builtinType = BuiltinFunctions::getType(funcName);
    
    // 生成参数
    for (const auto& arg : expr->arguments) {
        generateExpression(arg.get());
    }
    
    // 调用函数
    if (isBuiltin) {
        // 处理内置函数
        switch (builtinType) {
            case BuiltinFunctionType::PRINT:
                codeBuffer_ << "  call $console_log\n";
                codeBuffer_ << "  i32.const 0 ;; print函数返回0\n";
                break;
            case BuiltinFunctionType::PARSE_INT:
                // 假设参数已经是整数
                break;
            case BuiltinFunctionType::PARSE_FLOAT:
                // 假设参数已经是浮点数
                break;
            case BuiltinFunctionType::TO_STRING:
                // 简单实现，直接返回输入
                break;
            case BuiltinFunctionType::LENGTH:
                // 对于字符串和数组长度
                codeBuffer_ << "  i32.const 0 ;; length函数暂不支持\n";
                break;
            case BuiltinFunctionType::ASK:
                // 调用ask函数
                codeBuffer_ << "  call $ask\n";
                break;
            default:
                std::cerr << "警告: 未知的内置函数类型 " << (int)builtinType << std::endl;
                codeBuffer_ << "  i32.const 0 ;; 未支持的内置函数\n";
                break;
        }
    } else {
        // 调用自定义函数
        codeBuffer_ << "  call $" << funcName << "\n";
    }
}

// 生成赋值表达式
void CodeGenerator::CodeGeneratorImpl::generateAssignmentExpression(const AssignmentExpr* expr) {
    // 暂时只支持简单变量赋值
    if (expr->target->getType() != ExprType::VARIABLE) {
        std::cerr << "警告: 只支持简单变量赋值\n";
        codeBuffer_ << "  i32.const 0 ;; 不支持的赋值目标\n";
        return;
    }
    
    auto* varExpr = static_cast<const VariableExpr*>(expr->target.get());
    std::string varName = varExpr->name.getValue();
    
    // 生成值表达式
    generateExpression(expr->value.get());
    
    // 保存表达式结果的副本用于返回
    codeBuffer_ << "  local.set $temp\n";
    codeBuffer_ << "  local.get $temp\n";
    
    // 设置变量值
    codeBuffer_ << "  global.set $" << varName << "\n";
    
    // 返回赋值后的值
    codeBuffer_ << "  local.get $temp\n";
}

// 写入输出文件
void CodeGenerator::CodeGeneratorImpl::writeToFile(const std::string& outputFile) {
    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "错误: 无法创建输出文件: " << outputFile << std::endl;
        return;
    }
    
    outFile << codeBuffer_.str();
    outFile.close();
}

// 构造函数
CodeGenerator::CodeGenerator() : impl_(std::make_unique<CodeGeneratorImpl>()) {
}

// 析构函数
CodeGenerator::~CodeGenerator() = default;

// 生成代码
void CodeGenerator::generateCode(const std::vector<std::unique_ptr<Stmt>>& ast, const std::string& outputFile) {
    impl_->generateCode(ast, outputFile);
}

} // namespace jvav 