#include "lexer/Lexer.h"
#include <cctype>
#include <iostream>

namespace jvav {

// 构造函数
Lexer::Lexer(const std::string& source, const std::string& filename)
    : source_(source), filename_(filename) {
    initKeywords();
}

// 析构函数
Lexer::~Lexer() {}

// 获取下一个token
Token Lexer::getNextToken() {
    if (hasNextToken_) {
        hasNextToken_ = false;
        return nextToken_;
    }
    
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", getCurrentLocation());
    }
    
    return scanToken();
}

// 预览下一个token（不消耗）
Token Lexer::peekNextToken() {
    if (!hasNextToken_) {
        nextToken_ = getNextToken();
        hasNextToken_ = true;
    }
    return nextToken_;
}

// 检查是否到达文件末尾
bool Lexer::isAtEnd(size_t offset) const {
    return position_ + offset >= source_.length();
}

// 获取当前位置信息
SourceLocation Lexer::getCurrentLocation() const {
    return SourceLocation(filename_, line_, column_);
}

// 获取所有tokens（调试用）
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (true) {
        Token token = getNextToken();
        tokens.push_back(token);
        
        if (token.isEOF()) {
            break;
        }
    }
    
    return tokens;
}

// 初始化关键字表
void Lexer::initKeywords() {
    // 英文关键字
    keywords_["import"] = TokenType::IMPORT;
    keywords_["set"] = TokenType::SET;
    keywords_["print"] = TokenType::PRINT;
    keywords_["if"] = TokenType::IF;
    keywords_["elif"] = TokenType::ELIF;
    keywords_["else"] = TokenType::ELSE;
    keywords_["loop"] = TokenType::LOOP;
    keywords_["define"] = TokenType::DEFINE;
    keywords_["return"] = TokenType::RETURN;
    keywords_["try"] = TokenType::TRY;
    keywords_["catch"] = TokenType::CATCH;
    keywords_["enum"] = TokenType::ENUM;
    keywords_["throw"] = TokenType::THROW;
    keywords_["jilu"] = TokenType::JILU;
    
    // 布尔值
    keywords_["true"] = TokenType::BOOL_LITERAL;
    keywords_["false"] = TokenType::BOOL_LITERAL;
    
    // 中文关键字
    keywords_["导入"] = TokenType::ZH_IMPORT;
    keywords_["设置"] = TokenType::ZH_SET;
    keywords_["输出"] = TokenType::ZH_PRINT;
    keywords_["如果"] = TokenType::ZH_IF;
    keywords_["否则如果"] = TokenType::ZH_ELIF;
    keywords_["否则"] = TokenType::ZH_ELSE;
    keywords_["循环"] = TokenType::ZH_LOOP;
    keywords_["定义"] = TokenType::ZH_DEFINE;
    keywords_["返回"] = TokenType::ZH_RETURN;
    keywords_["尝试"] = TokenType::ZH_TRY;
    keywords_["捕获"] = TokenType::ZH_CATCH;
    keywords_["枚举"] = TokenType::ZH_ENUM;
    keywords_["抛出"] = TokenType::ZH_THROW;
    keywords_["记录"] = TokenType::ZH_JILU;
    
    // 中文布尔值
    keywords_["真"] = TokenType::BOOL_LITERAL;
    keywords_["假"] = TokenType::BOOL_LITERAL;
}

// 读取下一个字符并前进
char Lexer::advance() {
    char c = source_[position_++];
    
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    
    return c;
}

// 查看当前字符
char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[position_];
}

// 查看下一个字符
char Lexer::peekNext() const {
    if (isAtEnd(1)) return '\0';
    return source_[position_ + 1];
}

// 如果当前字符匹配，则消耗并返回true
bool Lexer::match(char expected) {
    if (isAtEnd() || source_[position_] != expected) return false;
    
    position_++;
    column_++;
    return true;
}

// 跳过空白字符
void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                advance();
                break;
            case '#':
                skipComment();
                break;
            default:
                return;
        }
    }
}

// 跳过注释
void Lexer::skipComment() {
    // 跳过直到行尾
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
}

// 检查是否是数字
bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

// 检查是否是字母
bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
           c == '_' || 
           isChineseChar(c);
}

// 检查是否是字母或数字
bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

// 检查是否是中文字符（简单实现，实际中需要更复杂的Unicode处理）
bool Lexer::isChineseChar(char c) const {
    // UTF-8中文字符的首字节范围通常是0xE4-0xE9
    // 这是一个简化实现，完整实现需要处理多字节UTF-8字符
    unsigned char uc = static_cast<unsigned char>(c);
    return (uc >= 0xE4 && uc <= 0xE9) || uc >= 0x80;
}

// 添加词法错误
void Lexer::addError(const std::string& message) {
    std::string error = getCurrentLocation().toString() + ": " + message;
    errors_.push_back(error);
}

// 扫描单个token
Token Lexer::scanToken() {
    char c = advance();
    
    // 标识符
    if (isAlpha(c)) {
        return identifier();
    }
    
    // 数字
    if (isDigit(c)) {
        return number();
    }
    
    switch (c) {
        // 字符串字面量
        case '"': return string();
        
        // 单字符tokens
        case '(': return Token(TokenType::LEFT_PAREN, "(", getCurrentLocation());
        case ')': return Token(TokenType::RIGHT_PAREN, ")", getCurrentLocation());
        case '{': return Token(TokenType::LEFT_BRACE, "{", getCurrentLocation());
        case '}': return Token(TokenType::RIGHT_BRACE, "}", getCurrentLocation());
        case '[': return Token(TokenType::LEFT_BRACKET, "[", getCurrentLocation());
        case ']': return Token(TokenType::RIGHT_BRACKET, "]", getCurrentLocation());
        case ',': return Token(TokenType::COMMA, ",", getCurrentLocation());
        case '.': return Token(TokenType::DOT, ".", getCurrentLocation());
        case ';': return Token(TokenType::SEMICOLON, ";", getCurrentLocation());
        case ':': return Token(TokenType::COLON, ":", getCurrentLocation());
        case '+': return Token(TokenType::PLUS, "+", getCurrentLocation());
        case '-': return Token(TokenType::MINUS, "-", getCurrentLocation());
        case '*': return Token(TokenType::STAR, "*", getCurrentLocation());
        case '/': return Token(TokenType::SLASH, "/", getCurrentLocation());
        case '%': return Token(TokenType::PERCENT, "%", getCurrentLocation());
        
        // 可能是双字符tokens
        case '=':
            if (match('=')) {
                return Token(TokenType::EQUAL, "==", getCurrentLocation());
            } else {
                return Token(TokenType::ASSIGN, "=", getCurrentLocation());
            }
            
        case '!':
            if (match('=')) {
                return Token(TokenType::NOT_EQUAL, "!=", getCurrentLocation());
            } else {
                return Token(TokenType::NOT, "!", getCurrentLocation());
            }
            
        case '<':
            if (match('=')) {
                return Token(TokenType::LESS_EQUAL, "<=", getCurrentLocation());
            } else {
                return Token(TokenType::LESS, "<", getCurrentLocation());
            }
            
        case '>':
            if (match('=')) {
                return Token(TokenType::GREATER_EQUAL, ">=", getCurrentLocation());
            } else {
                return Token(TokenType::GREATER, ">", getCurrentLocation());
            }
            
        case '&':
            if (match('&')) {
                return Token(TokenType::AND, "&&", getCurrentLocation());
            } else {
                addError("Unexpected character '&', did you mean '&&'?");
                return Token(TokenType::ERROR, "&", getCurrentLocation());
            }
            
        case '|':
            if (match('|')) {
                return Token(TokenType::OR, "||", getCurrentLocation());
            } else {
                addError("Unexpected character '|', did you mean '||'?");
                return Token(TokenType::ERROR, "|", getCurrentLocation());
            }
            
        default:
            // 未识别的字符
            addError("Unexpected character: " + std::string(1, c));
            return Token(TokenType::ERROR, std::string(1, c), getCurrentLocation());
    }
}

// 处理标识符和关键字
Token Lexer::identifier() {
    size_t start = position_ - 1;
    
    while (isAlphaNumeric(peek())) {
        advance();
    }
    
    std::string text = source_.substr(start, position_ - start);
    
    // 检查是否是关键字
    auto it = keywords_.find(text);
    if (it != keywords_.end()) {
        return Token(it->second, text, getCurrentLocation());
    }
    
    return Token(TokenType::IDENTIFIER, text, getCurrentLocation());
}

// 处理数字字面量
Token Lexer::number() {
    size_t start = position_ - 1;
    
    while (isDigit(peek())) {
        advance();
    }
    
    // 小数部分
    if (peek() == '.' && isDigit(peekNext())) {
        // 消耗点号
        advance();
        
        while (isDigit(peek())) {
            advance();
        }
    }
    
    std::string text = source_.substr(start, position_ - start);
    return Token(TokenType::NUMBER_LITERAL, text, getCurrentLocation());
}

// 处理字符串字面量
Token Lexer::string() {
    size_t start = position_;
    int startLine = line_;
    int startColumn = column_ - 1;  // 已经消耗了开引号
    
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            addError("Unterminated string");
            return Token(TokenType::ERROR, "Unterminated string", 
                        SourceLocation(filename_, startLine, startColumn));
        }
        
        if (peek() == '\\' && !isAtEnd(1)) {
            // 处理转义字符
            advance();  // 消耗反斜杠
        }
        
        advance();
    }
    
    if (isAtEnd()) {
        addError("Unterminated string");
        return Token(TokenType::ERROR, "Unterminated string", 
                    SourceLocation(filename_, startLine, startColumn));
    }
    
    // 消耗结束引号
    advance();
    
    // 获取字符串内容（不包括引号）
    std::string value = source_.substr(start, position_ - start - 1);
    return Token(TokenType::STRING_LITERAL, value, 
                SourceLocation(filename_, startLine, startColumn));
}

} // namespace jvav 