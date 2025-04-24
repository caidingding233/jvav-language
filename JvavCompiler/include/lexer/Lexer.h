#ifndef JVAV_LEXER_H
#define JVAV_LEXER_H

#include "lexer/Token.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace jvav {

// 词法分析器类
class Lexer {
public:
    // 构造函数接受源代码和文件名
    Lexer(const std::string& source, const std::string& filename = "<source>");
    
    // 析构函数
    ~Lexer();
    
    // 获取下一个token
    Token getNextToken();
    
    // 预览下一个token（不消耗）
    Token peekNextToken();
    
    // 检查是否到达文件末尾
    bool isAtEnd(size_t offset = 0) const;
    
    // 获取当前位置信息
    SourceLocation getCurrentLocation() const;
    
    // 获取所有tokens（调试用）
    std::vector<Token> tokenize();
    
    // 获取词法错误
    const std::vector<std::string>& getErrors() const { return errors_; }

private:
    // 源代码
    std::string source_;
    
    // 文件名
    std::string filename_;
    
    // 当前位置
    size_t position_ = 0;
    
    // 当前行和列
    int line_ = 1;
    int column_ = 1;
    
    // 存储已扫描但未消耗的token
    Token nextToken_;
    bool hasNextToken_ = false;
    
    // 关键字映射表
    std::unordered_map<std::string, TokenType> keywords_;
    
    // 错误消息列表
    std::vector<std::string> errors_;
    
    // 辅助方法
    char advance();             // 读取下一个字符并前进
    char peek() const;          // 查看当前字符
    char peekNext() const;      // 查看下一个字符
    bool match(char expected);  // 如果当前字符匹配，则消耗并返回true
    
    void skipWhitespace();      // 跳过空白字符
    void skipComment();         // 跳过注释
    
    Token scanToken();          // 扫描单个token
    Token identifier();         // 处理标识符和关键字
    Token number();             // 处理数字字面量
    Token string();             // 处理字符串字面量
    
    // 辅助函数
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isChineseChar(char c) const;
    
    // 添加词法错误
    void addError(const std::string& message);
    
    // 初始化关键字表
    void initKeywords();
};

} // namespace jvav

#endif // JVAV_LEXER_H 