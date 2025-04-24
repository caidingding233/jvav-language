#include "parser/Parser.h"
#include <iostream>

namespace jvav {

// 构造函数
Parser::Parser(Lexer& lexer) : lexer_(lexer) {
    // 预先读取第一个token
    current_ = lexer_.getNextToken();
}

// 解析源代码
std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    try {
        // 解析直到文件结束
        while (!check(TokenType::END_OF_FILE)) {
            statements.push_back(declaration());
        }
    } catch (const ParseError& error) {
        // 恢复并继续解析
        synchronize();
    }
    
    return statements;
}

// 声明解析
std::unique_ptr<Stmt> Parser::declaration() {
    try {
        // 检查特定类型的声明
        if (match(TokenType::IMPORT) || match(TokenType::ZH_IMPORT)) {
            return importStatement();
        }
        if (match(TokenType::SET) || match(TokenType::ZH_SET)) {
            return setStatement();
        }
        if (match(TokenType::PRINT) || match(TokenType::ZH_PRINT)) {
            return printStatement();
        }
        if (match(TokenType::IF) || match(TokenType::ZH_IF)) {
            return ifStatement();
        }
        if (match(TokenType::LOOP) || match(TokenType::ZH_LOOP)) {
            return loopStatement();
        }
        if (match(TokenType::DEFINE) || match(TokenType::ZH_DEFINE)) {
            return defineStatement();
        }
        if (match(TokenType::RETURN) || match(TokenType::ZH_RETURN)) {
            return returnStatement();
        }
        if (match(TokenType::TRY) || match(TokenType::ZH_TRY)) {
            return tryStatement();
        }
        if (match(TokenType::ENUM) || match(TokenType::ZH_ENUM)) {
            return enumDefinition();
        }
        if (match(TokenType::JILU) || match(TokenType::ZH_JILU)) {
            return recordDefinition();
        }
        
        // 其他情况当作表达式语句处理
        return expressionStatement();
    } catch (const ParseError& error) {
        synchronize();
        return nullptr;
    }
}

// 导入语句
std::unique_ptr<Stmt> Parser::importStatement() {
    // 解析模块名
    Token module = consume(TokenType::IDENTIFIER, "期望是模块名称.");
    // 可选的别名
    Token alias;
    if (match(TokenType::IDENTIFIER) && previous().getValue() == "as") {
        alias = consume(TokenType::IDENTIFIER, "期望是模块别名.");
    }
    return std::make_unique<ImportStmt>(module, alias);
}

// 设置变量语句
std::unique_ptr<Stmt> Parser::setStatement() {
    Token name = consume(TokenType::IDENTIFIER, "期望是变量名.");
    
    // 可选的类型声明
    Token type;
    if (match(TokenType::COLON)) {
        type = consume(TokenType::IDENTIFIER, "期望是类型名.");
    }
    
    consume(TokenType::EQUAL, "期望是'=='.");
    if (check(TokenType::EQUAL)) {
        advance(); // 支持 "==" 作为赋值符号
    }
    
    auto value = expression();
    return std::make_unique<SetStmt>(name, std::move(value), type);
}

// 打印语句
std::unique_ptr<Stmt> Parser::printStatement() {
    consume(TokenType::LEFT_PAREN, "期望是'('.");
    auto value = expression();
    consume(TokenType::RIGHT_PAREN, "期望是')'.");
    return std::make_unique<PrintStmt>(std::move(value));
}

// 表达式语句
std::unique_ptr<Stmt> Parser::expressionStatement() {
    auto expr = expression();
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

// 表达式解析
std::unique_ptr<Expr> Parser::expression() {
    return assignment();
}

// 赋值表达式
std::unique_ptr<Expr> Parser::assignment() {
    auto expr = logicalOr();
    
    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        auto value = assignment();
        
        // 确保左边是一个可赋值的目标
        if (expr->getType() == ExprType::VARIABLE) {
            return std::make_unique<AssignmentExpr>(std::move(expr), equals, std::move(value));
        } else if (expr->getType() == ExprType::ARRAY_ACCESS || expr->getType() == ExprType::RECORD_ACCESS) {
            return std::make_unique<AssignmentExpr>(std::move(expr), equals, std::move(value));
        }
        
        error(equals, "无效的赋值目标.");
    }
    
    return expr;
}

// 逻辑或
std::unique_ptr<Expr> Parser::logicalOr() {
    auto expr = logicalAnd();
    
    while (match(TokenType::OR)) {
        Token op = previous();
        auto right = logicalAnd();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

// 逻辑与
std::unique_ptr<Expr> Parser::logicalAnd() {
    auto expr = equality();
    
    while (match(TokenType::AND)) {
        Token op = previous();
        auto right = equality();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

// 相等性比较
std::unique_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    
    while (match({TokenType::EQUAL, TokenType::NOT_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

// 比较
std::unique_ptr<Expr> Parser::comparison() {
    auto expr = term();
    
    while (match({TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

// 加减法
std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();
    
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

// 乘除法和取模
std::unique_ptr<Expr> Parser::factor() {
    auto expr = unary();
    
    while (match({TokenType::STAR, TokenType::SLASH, TokenType::PERCENT})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

// 一元操作
std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::MINUS, TokenType::NOT})) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }
    
    return call();
}

// 函数调用
std::unique_ptr<Expr> Parser::call() {
    auto expr = primary();
    
    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(std::move(expr));
        } else if (match(TokenType::DOT)) {
            Token name = consume(TokenType::IDENTIFIER, "期望是属性名.");
            expr = std::make_unique<RecordAccessExpr>(std::move(expr), name);
        } else if (match(TokenType::LEFT_BRACKET)) {
            auto index = expression();
            Token bracket = consume(TokenType::RIGHT_BRACKET, "期望是']'.");
            expr = std::make_unique<ArrayAccessExpr>(std::move(expr), std::move(index), bracket);
        } else {
            break;
        }
    }
    
    return expr;
}

// 完成函数调用解析
std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee) {
    std::vector<std::unique_ptr<Expr>> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }
    
    Token paren = consume(TokenType::RIGHT_PAREN, "期望是')'.");
    
    return std::make_unique<CallExpr>(std::move(callee), paren, std::move(arguments));
}

// 基本表达式
std::unique_ptr<Expr> Parser::primary() {
    if (match(TokenType::BOOL_LITERAL)) {
        return std::make_unique<LiteralExpr>(previous());
    }
    
    if (match(TokenType::NUMBER_LITERAL)) {
        return std::make_unique<LiteralExpr>(previous());
    }
    
    if (match(TokenType::STRING_LITERAL)) {
        return std::make_unique<LiteralExpr>(previous());
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<VariableExpr>(previous());
    }
    
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "期望是')'.");
        return expr;
    }
    
    throw error(peek(), "期望是表达式.");
}

// 辅助方法
bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) const {
    return current_.getType() == type;
}

Token Parser::advance() {
    Token previous = current_;
    previous_ = current_;  // 保存当前token为上一个token
    current_ = lexer_.getNextToken();
    return previous;
}

Token Parser::peek() const {
    return current_;
}

Token Parser::previous() const {
    return previous_;  // 返回存储的上一个token
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        return advance();
    }
    
    addError(current_, message);
    throw ParseError(message);
}

// 新增支持多种类型的consume方法
Token Parser::consume(const std::vector<TokenType>& types, const std::string& message) {
    for (TokenType type : types) {
        if (check(type)) {
            return advance();
        }
    }
    
    addError(current_, message);
    throw ParseError(message);
}

ParseError Parser::error(const Token& token, const std::string& message) {
    addError(token, message);
    return ParseError(message);
}

void Parser::synchronize() {
    panicMode_ = false;
    
    while (!check(TokenType::END_OF_FILE)) {
        // 同步到语句的开始
        if (check(TokenType::IMPORT) || check(TokenType::ZH_IMPORT) ||
            check(TokenType::SET) || check(TokenType::ZH_SET) ||
            check(TokenType::PRINT) || check(TokenType::ZH_PRINT) ||
            check(TokenType::IF) || check(TokenType::ZH_IF) ||
            check(TokenType::LOOP) || check(TokenType::ZH_LOOP) ||
            check(TokenType::DEFINE) || check(TokenType::ZH_DEFINE) ||
            check(TokenType::RETURN) || check(TokenType::ZH_RETURN) ||
            check(TokenType::TRY) || check(TokenType::ZH_TRY) ||
            check(TokenType::ENUM) || check(TokenType::ZH_ENUM) ||
            check(TokenType::JILU) || check(TokenType::ZH_JILU)) {
            return;
        }
        
        advance();
    }
}

void Parser::addError(const std::string& message) {
    errors_.push_back(message);
}

void Parser::addError(const Token& token, const std::string& message) {
    std::string errorMsg;
    if (token.getType() == TokenType::END_OF_FILE) {
        errorMsg = token.getLocation().toString() + " 文件结束: " + message;
    } else {
        errorMsg = token.getLocation().toString() + " 在 '" + token.getValue() + "': " + message;
    }
    errors_.push_back(errorMsg);
}

// 还需要实现其他语句解析方法，如ifStatement、loopStatement等
std::unique_ptr<Stmt> Parser::ifStatement() {
    // 解析条件
    consume(TokenType::LEFT_PAREN, "期望是'('.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "期望是')'.");
    
    // 解析if块
    consume(TokenType::LEFT_BRACE, "期望是'{'.");
    auto ifBody = block();
    auto* blockStmt = static_cast<BlockStmt*>(ifBody.get());
    
    // 创建分支数组
    std::vector<Branch> branches;
    branches.push_back(Branch(std::move(condition), std::move(blockStmt->statements)));
    
    // 解析elif块
    while (match({TokenType::ELIF, TokenType::ZH_ELIF})) {
        consume(TokenType::LEFT_PAREN, "期望是'('.");
        auto elifCondition = expression();
        consume(TokenType::RIGHT_PAREN, "期望是')'.");
        
        consume(TokenType::LEFT_BRACE, "期望是'{'.");
        auto elifBody = block();
        auto* elifBlockStmt = static_cast<BlockStmt*>(elifBody.get());
        
        branches.push_back(Branch(std::move(elifCondition), std::move(elifBlockStmt->statements)));
    }
    
    // 解析else块
    if (match({TokenType::ELSE, TokenType::ZH_ELSE})) {
        consume(TokenType::LEFT_BRACE, "期望是'{'.");
        auto elseBody = block();
        auto* elseBlockStmt = static_cast<BlockStmt*>(elseBody.get());
        
        // else块的条件为null
        branches.push_back(Branch(nullptr, std::move(elseBlockStmt->statements)));
    }
    
    return std::make_unique<IfStmt>(std::move(branches));
}

std::unique_ptr<Stmt> Parser::loopStatement() {
    // 检查是否有as关键字
    bool hasIterator = false;
    Token iterVar;
    
    if (match(TokenType::IDENTIFIER) && previous().getValue() == "as") {
        hasIterator = true;
        iterVar = consume(TokenType::IDENTIFIER, "期望是迭代变量名.");
    }
    
    // 获取循环计数表达式
    auto countExpr = expression();
    
    // 解析循环体
    consume(TokenType::LEFT_BRACE, "期望是'{'.");
    auto loopBody = block();
    auto* blockStmt = static_cast<BlockStmt*>(loopBody.get());
    
    if (hasIterator) {
        return std::make_unique<LoopStmt>(iterVar, std::move(countExpr), std::move(blockStmt->statements));
    } else {
        // 使用空的iterator
        return std::make_unique<LoopStmt>(Token(), std::move(countExpr), std::move(blockStmt->statements));
    }
}

std::unique_ptr<Stmt> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        statements.push_back(declaration());
    }
    
    consume(TokenType::RIGHT_BRACE, "期望是'}'.");
    
    return std::make_unique<BlockStmt>(std::move(statements));
}

// 函数定义
std::unique_ptr<Stmt> Parser::defineStatement() {
    // 解析函数名
    Token name = consume(TokenType::IDENTIFIER, "期望是函数名.");
    
    // 解析参数列表
    consume(TokenType::LEFT_PAREN, "期望是'('.");
    std::vector<Token> parameters;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER, "期望是参数名."));
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN, "期望是')'.");
    
    // 解析函数体
    consume(TokenType::LEFT_BRACE, "期望是'{'.");
    auto functionBody = block();
    auto* blockStmt = static_cast<BlockStmt*>(functionBody.get());
    
    return std::make_unique<DefineStmt>(name, std::move(parameters), std::move(blockStmt->statements));
}

// 返回语句
std::unique_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::unique_ptr<Expr> value = nullptr;
    
    // 解析返回值(如果有)
    if (!check(TokenType::RIGHT_BRACE)) {
        value = expression();
    }
    
    return std::make_unique<ReturnStmt>(keyword, std::move(value));
}

// 数组语句
std::unique_ptr<Stmt> Parser::arrayStatement() {
    Token name = consume(TokenType::IDENTIFIER, "期望是数组名.");
    
    // 可选的元素类型
    Token elementType;
    if (match(TokenType::COLON)) {
        elementType = consume(TokenType::IDENTIFIER, "期望是类型名.");
    }
    
    // 解析数组内容
    std::vector<std::unique_ptr<Expr>> elements;
    
    consume(TokenType::LEFT_BRACKET, "期望是'['.");
    
    if (!check(TokenType::RIGHT_BRACKET)) {
        do {
            elements.push_back(expression());
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_BRACKET, "期望是']'.");
    
    return std::make_unique<ArrayStmt>(name, elementType, std::move(elements));
}

// 记录定义
std::unique_ptr<Stmt> Parser::recordDefinition() {
    Token name = consume(TokenType::IDENTIFIER, "期望是记录类型名.");
    
    // 解析字段列表
    consume(TokenType::LEFT_BRACE, "期望是'{'.");
    
    std::vector<FieldDefinition> fields;
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        Token fieldName = consume(TokenType::IDENTIFIER, "期望是字段名.");
        
        // 字段类型(可选)
        Token fieldType;
        if (match(TokenType::COLON)) {
            fieldType = consume(TokenType::IDENTIFIER, "期望是类型名.");
        }
        
        fields.push_back(FieldDefinition(fieldName, fieldType));
    }
    
    consume(TokenType::RIGHT_BRACE, "期望是'}'.");
    
    return std::make_unique<RecordDefStmt>(name, std::move(fields));
}

// 枚举定义
std::unique_ptr<Stmt> Parser::enumDefinition() {
    Token name = consume(TokenType::IDENTIFIER, "期望是枚举类型名.");
    
    // 解析枚举值列表
    consume(TokenType::LEFT_BRACE, "期望是'{'.");
    
    std::vector<Token> values;
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        values.push_back(consume(TokenType::IDENTIFIER, "期望是枚举值名."));
    }
    
    consume(TokenType::RIGHT_BRACE, "期望是'}'.");
    
    return std::make_unique<EnumDefStmt>(name, std::move(values));
}

// try-catch语句
std::unique_ptr<Stmt> Parser::tryStatement() {
    // 解析try块
    consume(TokenType::LEFT_BRACE, "期望是'{'.");
    auto tryBlock = block();
    auto* tryBlockStmt = static_cast<BlockStmt*>(tryBlock.get());
    
    // 解析catch块
    consume({TokenType::CATCH, TokenType::ZH_CATCH}, "期望是'catch'或'捕获'.");
    
    // 捕获的异常类型(可选)
    // 注意：当前TryCatchStmt不支持存储异常类型信息，我们只解析但不使用它
    if (match(TokenType::LEFT_PAREN)) {
        Token exceptionType = consume(TokenType::IDENTIFIER, "期望是异常类型.");
        consume(TokenType::RIGHT_PAREN, "期望是')'.");
        // 这里不保存exceptionType，因为TryCatchStmt没有相应的字段
    }
    
    consume(TokenType::LEFT_BRACE, "期望是'{'.");
    auto catchBlock = block();
    auto* catchBlockStmt = static_cast<BlockStmt*>(catchBlock.get());
    
    return std::make_unique<TryCatchStmt>(
        std::move(tryBlockStmt->statements), 
        std::move(catchBlockStmt->statements)
    );
}

// dakai语句
std::unique_ptr<Stmt> Parser::dakaiStatement() {
    // 解析路径表达式
    consume(TokenType::LEFT_PAREN, "期望是'('.");
    auto path = expression();
    consume(TokenType::RIGHT_PAREN, "期望是')'.");
    
    return std::make_unique<DakaiStmt>(std::move(path));
}

} // namespace jvav 