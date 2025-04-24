#ifndef JVAV_TOKEN_H
#define JVAV_TOKEN_H

#include <string>
#include <iostream>

namespace jvav {

// 词法单元类型
enum class TokenType {
    // 特殊标记
    END_OF_FILE,
    ERROR,
    
    // 关键字
    IMPORT,
    SET,
    PRINT,
    IF,
    ELIF,
    ELSE,
    LOOP,
    DEFINE,
    RETURN,
    TRY,
    CATCH,
    ENUM,
    THROW,
    JILU,  // 记录类型定义

    // 中文关键字
    ZH_IMPORT,     // 导入
    ZH_SET,        // 设置
    ZH_PRINT,      // 输出
    ZH_IF,         // 如果
    ZH_ELIF,       // 否则如果
    ZH_ELSE,       // 否则
    ZH_LOOP,       // 循环
    ZH_DEFINE,     // 定义
    ZH_RETURN,     // 返回
    ZH_TRY,        // 尝试
    ZH_CATCH,      // 捕获
    ZH_ENUM,       // 枚举
    ZH_THROW,      // 抛出
    ZH_JILU,       // 记录
    
    // 标识符和字面量
    IDENTIFIER,
    STRING_LITERAL,
    NUMBER_LITERAL,
    BOOL_LITERAL,

    // 操作符和分隔符
    PLUS,          // +
    MINUS,         // -
    STAR,          // *
    SLASH,         // /
    PERCENT,       // %
    ASSIGN,        // =
    EQUAL,         // ==
    NOT_EQUAL,     // !=
    LESS,          // <
    LESS_EQUAL,    // <=
    GREATER,       // >
    GREATER_EQUAL, // >=
    AND,           // &&
    OR,            // ||
    NOT,           // !
    DOT,           // .
    COMMA,         // ,
    SEMICOLON,     // ;
    COLON,         // :
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET  // ]
};

// 词法单元位置信息
struct SourceLocation {
    std::string filename;
    int line;
    int column;
    
    SourceLocation() : line(1), column(1) {}
    SourceLocation(const std::string& file, int l, int c) 
        : filename(file), line(l), column(c) {}
        
    std::string toString() const {
        return filename + ":" + std::to_string(line) + ":" + std::to_string(column);
    }
};

// 词法单元类
class Token {
public:
    Token() : type(TokenType::ERROR) {}
    
    Token(TokenType t, const std::string& v, const SourceLocation& loc)
        : type(t), value(v), location(loc) {}
    
    // 获取token类型
    TokenType getType() const { return type; }
    
    // 获取token值
    const std::string& getValue() const { return value; }
    
    // 获取token位置
    const SourceLocation& getLocation() const { return location; }
    
    // 转为字符串
    std::string toString() const;
    
    // 检查是否为指定类型
    bool is(TokenType t) const { return type == t; }
    
    // 检查是否为结束标记
    bool isEOF() const { return type == TokenType::END_OF_FILE; }

private:
    TokenType type;
    std::string value;
    SourceLocation location;
};

// 将TokenType转换为字符串
std::string tokenTypeToString(TokenType type);

// 输出运算符重载
std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace jvav

#endif // JVAV_TOKEN_H 