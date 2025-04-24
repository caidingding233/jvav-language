#include "lexer/Token.h"

namespace jvav {

std::string Token::toString() const {
    std::string typeStr = tokenTypeToString(type);
    std::string result = typeStr;
    
    if (type == TokenType::IDENTIFIER || 
        type == TokenType::STRING_LITERAL || 
        type == TokenType::NUMBER_LITERAL) {
        result += " '" + value + "'";
    }
    
    result += " at " + location.toString();
    return result;
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::ERROR: return "ERROR";
        
        // 关键字
        case TokenType::IMPORT: return "IMPORT";
        case TokenType::SET: return "SET";
        case TokenType::PRINT: return "PRINT";
        case TokenType::IF: return "IF";
        case TokenType::ELIF: return "ELIF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::LOOP: return "LOOP";
        case TokenType::DEFINE: return "DEFINE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::TRY: return "TRY";
        case TokenType::CATCH: return "CATCH";
        case TokenType::ENUM: return "ENUM";
        case TokenType::THROW: return "THROW";
        case TokenType::JILU: return "JILU";
        
        // 中文关键字
        case TokenType::ZH_IMPORT: return "导入";
        case TokenType::ZH_SET: return "设置";
        case TokenType::ZH_PRINT: return "输出";
        case TokenType::ZH_IF: return "如果";
        case TokenType::ZH_ELIF: return "否则如果";
        case TokenType::ZH_ELSE: return "否则";
        case TokenType::ZH_LOOP: return "循环";
        case TokenType::ZH_DEFINE: return "定义";
        case TokenType::ZH_RETURN: return "返回";
        case TokenType::ZH_TRY: return "尝试";
        case TokenType::ZH_CATCH: return "捕获";
        case TokenType::ZH_ENUM: return "枚举";
        case TokenType::ZH_THROW: return "抛出";
        case TokenType::ZH_JILU: return "记录";
        
        // 标识符和字面量
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING_LITERAL: return "STRING";
        case TokenType::NUMBER_LITERAL: return "NUMBER";
        case TokenType::BOOL_LITERAL: return "BOOLEAN";
        
        // 操作符和分隔符
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::PERCENT: return "%";
        case TokenType::ASSIGN: return "=";
        case TokenType::EQUAL: return "==";
        case TokenType::NOT_EQUAL: return "!=";
        case TokenType::LESS: return "<";
        case TokenType::LESS_EQUAL: return "<=";
        case TokenType::GREATER: return ">";
        case TokenType::GREATER_EQUAL: return ">=";
        case TokenType::AND: return "&&";
        case TokenType::OR: return "||";
        case TokenType::NOT: return "!";
        case TokenType::DOT: return ".";
        case TokenType::COMMA: return ",";
        case TokenType::SEMICOLON: return ";";
        case TokenType::COLON: return ":";
        case TokenType::LEFT_PAREN: return "(";
        case TokenType::RIGHT_PAREN: return ")";
        case TokenType::LEFT_BRACE: return "{";
        case TokenType::RIGHT_BRACE: return "}";
        case TokenType::LEFT_BRACKET: return "[";
        case TokenType::RIGHT_BRACKET: return "]";
            
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.toString();
    return os;
}

} // namespace jvav 