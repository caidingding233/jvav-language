#ifndef JVAV_PARSER_H
#define JVAV_PARSER_H

#include "lexer/Lexer.h"
#include "ast/AST.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

namespace jvav {

// 解析错误异常
class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

// 解析器类
class Parser {
public:
    // 构造函数需要一个词法分析器
    Parser(Lexer& lexer);
    
    // 解析源代码，生成语法树
    std::vector<std::unique_ptr<Stmt>> parse();
    
    // 获取解析错误
    const std::vector<std::string>& getErrors() const { return errors_; }
    
private:
    // 词法分析器
    Lexer& lexer_;
    
    // 当前处理的token
    Token current_;
    
    // 上一个处理的token
    Token previous_;
    
    // 解析错误列表
    std::vector<std::string> errors_;
    
    // 是否正在恢复错误
    bool panicMode_ = false;
    
    // 解析方法
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> importStatement();
    std::unique_ptr<Stmt> dakaiStatement();
    std::unique_ptr<Stmt> setStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> loopStatement();
    std::unique_ptr<Stmt> defineStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> arrayStatement();
    std::unique_ptr<Stmt> recordDefinition();
    std::unique_ptr<Stmt> enumDefinition();
    std::unique_ptr<Stmt> tryStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> block();
    
    // 表达式解析方法
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> logicalOr();
    std::unique_ptr<Expr> logicalAnd();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> primary();
    
    // 辅助方法
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    bool check(TokenType type) const;
    Token advance();
    Token peek() const;
    Token previous() const;
    Token consume(TokenType type, const std::string& message);
    Token consume(const std::vector<TokenType>& types, const std::string& message);
    
    ParseError error(const Token& token, const std::string& message);
    void synchronize();
    void addError(const std::string& message);
    void addError(const Token& token, const std::string& message);
    
    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);
};

} // namespace jvav

#endif // JVAV_PARSER_H 