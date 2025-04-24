#ifndef JVAV_AST_H
#define JVAV_AST_H

#include "lexer/Token.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace jvav {

// 表达式节点类型
enum class ExprType {
    LITERAL,
    VARIABLE,
    UNARY,
    BINARY,
    CALL,
    ARRAY_ACCESS,
    RECORD_ACCESS,
    ASSIGNMENT
};

// 语句节点类型
enum class StmtType {
    EXPRESSION,
    IMPORT,
    DAKAI,
    SET,
    PRINT,
    IF,
    LOOP,
    DEFINE,
    RETURN,
    ARRAY,
    RECORD_DEF,
    RECORD_ACCESS,
    TRY_CATCH,
    ENUM_DEF,
    BLOCK
};

// 前置声明
class Expr;
class Stmt;

// 所有表达式的基类
class Expr {
public:
    virtual ~Expr() {}
    virtual ExprType getType() const = 0;
    
    // 节点位置信息
    SourceLocation location;
};

// 所有语句的基类
class Stmt {
public:
    virtual ~Stmt() {}
    virtual StmtType getType() const = 0;
    
    // 节点位置信息
    SourceLocation location;
};

// 字面量表达式
class LiteralExpr : public Expr {
public:
    LiteralExpr(const Token& token)
        : token(token), value(token.getValue()) {}
    
    ExprType getType() const override { return ExprType::LITERAL; }
    
    Token token;
    std::string value;
};

// 变量引用表达式
class VariableExpr : public Expr {
public:
    VariableExpr(const Token& name)
        : name(name) {}
    
    ExprType getType() const override { return ExprType::VARIABLE; }
    
    Token name;
};

// 一元操作表达式
class UnaryExpr : public Expr {
public:
    UnaryExpr(const Token& op, std::unique_ptr<Expr> right)
        : op(op), right(std::move(right)) {}
    
    ExprType getType() const override { return ExprType::UNARY; }
    
    Token op;
    std::unique_ptr<Expr> right;
};

// 二元操作表达式
class BinaryExpr : public Expr {
public:
    BinaryExpr(std::unique_ptr<Expr> left, const Token& op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
    
    ExprType getType() const override { return ExprType::BINARY; }
    
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

// 函数调用表达式
class CallExpr : public Expr {
public:
    CallExpr(std::unique_ptr<Expr> callee, const Token& paren, 
            std::vector<std::unique_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}
    
    ExprType getType() const override { return ExprType::CALL; }
    
    std::unique_ptr<Expr> callee;
    Token paren;  // 右括号位置，用于错误报告
    std::vector<std::unique_ptr<Expr>> arguments;
};

// 数组访问表达式
class ArrayAccessExpr : public Expr {
public:
    ArrayAccessExpr(std::unique_ptr<Expr> array, std::unique_ptr<Expr> index, 
                   const Token& bracket)
        : array(std::move(array)), index(std::move(index)), bracket(bracket) {}
    
    ExprType getType() const override { return ExprType::ARRAY_ACCESS; }
    
    std::unique_ptr<Expr> array;
    std::unique_ptr<Expr> index;
    Token bracket;  // 右括号位置，用于错误报告
};

// 记录字段访问表达式
class RecordAccessExpr : public Expr {
public:
    RecordAccessExpr(std::unique_ptr<Expr> record, const Token& field)
        : record(std::move(record)), field(field) {}
    
    ExprType getType() const override { return ExprType::RECORD_ACCESS; }
    
    std::unique_ptr<Expr> record;
    Token field;
};

// 赋值表达式
class AssignmentExpr : public Expr {
public:
    AssignmentExpr(std::unique_ptr<Expr> target, const Token& op, std::unique_ptr<Expr> value)
        : target(std::move(target)), op(op), value(std::move(value)) {}
    
    ExprType getType() const override { return ExprType::ASSIGNMENT; }
    
    std::unique_ptr<Expr> target;
    Token op;
    std::unique_ptr<Expr> value;
};

// 表达式语句
class ExpressionStmt : public Stmt {
public:
    ExpressionStmt(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
    
    StmtType getType() const override { return StmtType::EXPRESSION; }
    
    std::unique_ptr<Expr> expression;
};

// 导入语句
class ImportStmt : public Stmt {
public:
    ImportStmt(const Token& module, const Token& alias = Token())
        : module(module), alias(alias) {}
    
    StmtType getType() const override { return StmtType::IMPORT; }
    
    Token module;
    Token alias;
};

// 打开语句
class DakaiStmt : public Stmt {
public:
    DakaiStmt(std::unique_ptr<Expr> path)
        : path(std::move(path)) {}
    
    StmtType getType() const override { return StmtType::DAKAI; }
    
    std::unique_ptr<Expr> path;
};

// 设置变量语句
class SetStmt : public Stmt {
public:
    SetStmt(const Token& name, std::unique_ptr<Expr> value, const Token& type = Token())
        : name(name), value(std::move(value)), type(type) {}
    
    StmtType getType() const override { return StmtType::SET; }
    
    Token name;
    std::unique_ptr<Expr> value;
    Token type;  // 可选的类型
};

// 打印语句
class PrintStmt : public Stmt {
public:
    PrintStmt(std::unique_ptr<Expr> value)
        : value(std::move(value)) {}
    
    StmtType getType() const override { return StmtType::PRINT; }
    
    std::unique_ptr<Expr> value;
};

// 条件分支
struct Branch {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> body;
    
    Branch(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Stmt>> b)
        : condition(std::move(cond)), body(std::move(b)) {}
};

// If语句
class IfStmt : public Stmt {
public:
    IfStmt(std::vector<Branch> branches)
        : branches(std::move(branches)) {}
    
    StmtType getType() const override { return StmtType::IF; }
    
    std::vector<Branch> branches;
};

// 循环语句
class LoopStmt : public Stmt {
public:
    LoopStmt(const Token& variable, std::unique_ptr<Expr> count, 
            std::vector<std::unique_ptr<Stmt>> body)
        : variable(variable), count(std::move(count)), body(std::move(body)) {}
    
    StmtType getType() const override { return StmtType::LOOP; }
    
    Token variable;  // 循环变量，可为空
    std::unique_ptr<Expr> count;
    std::vector<std::unique_ptr<Stmt>> body;
};

// 函数定义语句
class DefineStmt : public Stmt {
public:
    DefineStmt(const Token& name, std::vector<Token> parameters, 
               std::vector<std::unique_ptr<Stmt>> body)
        : name(name), parameters(std::move(parameters)), body(std::move(body)) {}
    
    StmtType getType() const override { return StmtType::DEFINE; }
    
    Token name;
    std::vector<Token> parameters;
    std::vector<std::unique_ptr<Stmt>> body;
};

// 返回语句
class ReturnStmt : public Stmt {
public:
    ReturnStmt(const Token& keyword, std::unique_ptr<Expr> value = nullptr)
        : keyword(keyword), value(std::move(value)) {}
    
    StmtType getType() const override { return StmtType::RETURN; }
    
    Token keyword;
    std::unique_ptr<Expr> value;
};

// 数组定义语句
class ArrayStmt : public Stmt {
public:
    ArrayStmt(const Token& name, const Token& elementType, 
             std::vector<std::unique_ptr<Expr>> elements)
        : name(name), elementType(elementType), elements(std::move(elements)) {}
    
    StmtType getType() const override { return StmtType::ARRAY; }
    
    Token name;
    Token elementType;
    std::vector<std::unique_ptr<Expr>> elements;
};

// 记录字段定义
struct FieldDefinition {
    Token name;
    Token type;
    
    FieldDefinition(const Token& n, const Token& t)
        : name(n), type(t) {}
};

// 记录类型定义语句
class RecordDefStmt : public Stmt {
public:
    RecordDefStmt(const Token& name, std::vector<FieldDefinition> fields)
        : name(name), fields(std::move(fields)) {}
    
    StmtType getType() const override { return StmtType::RECORD_DEF; }
    
    Token name;
    std::vector<FieldDefinition> fields;
};

// 记录访问语句
class RecordAccessStmt : public Stmt {
public:
    RecordAccessStmt(const Token& record, const Token& field, std::unique_ptr<Expr> value)
        : record(record), field(field), value(std::move(value)) {}
    
    StmtType getType() const override { return StmtType::RECORD_ACCESS; }
    
    Token record;
    Token field;
    std::unique_ptr<Expr> value;
};

// Try-Catch语句
class TryCatchStmt : public Stmt {
public:
    TryCatchStmt(std::vector<std::unique_ptr<Stmt>> tryBlock,
                std::vector<std::unique_ptr<Stmt>> catchBlock)
        : tryBlock(std::move(tryBlock)), catchBlock(std::move(catchBlock)) {}
    
    StmtType getType() const override { return StmtType::TRY_CATCH; }
    
    std::vector<std::unique_ptr<Stmt>> tryBlock;
    std::vector<std::unique_ptr<Stmt>> catchBlock;
};

// 枚举定义语句
class EnumDefStmt : public Stmt {
public:
    EnumDefStmt(const Token& name, std::vector<Token> values)
        : name(name), values(std::move(values)) {}
    
    StmtType getType() const override { return StmtType::ENUM_DEF; }
    
    Token name;
    std::vector<Token> values;
};

// 语句块
class BlockStmt : public Stmt {
public:
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
    
    StmtType getType() const override { return StmtType::BLOCK; }
    
    std::vector<std::unique_ptr<Stmt>> statements;
};

} // namespace jvav

#endif // JVAV_AST_H 