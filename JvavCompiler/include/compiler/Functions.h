#ifndef JVAV_FUNCTIONS_H
#define JVAV_FUNCTIONS_H

#include <string>
#include <unordered_map>
#include <vector>

namespace jvav {

// 内置函数类型
enum class BuiltinFunctionType {
    PRINT,
    PARSE_INT,
    PARSE_FLOAT,
    TO_STRING,
    LENGTH,
    ASK,  // 添加ask函数
    UNKNOWN
};

// 内置函数信息
struct BuiltinFunction {
    std::string name;
    BuiltinFunctionType type;
    int minArgs;
    int maxArgs;
    
    BuiltinFunction(const std::string& name, BuiltinFunctionType type, int minArgs, int maxArgs = -1)
        : name(name), type(type), minArgs(minArgs), maxArgs(maxArgs == -1 ? minArgs : maxArgs) {}
};

// 内置函数注册表
class BuiltinFunctions {
public:
    static const std::unordered_map<std::string, BuiltinFunction>& getAll() {
        static std::unordered_map<std::string, BuiltinFunction> functions = {
            {"print", {"print", BuiltinFunctionType::PRINT, 1, 1}},
            {"输出", {"输出", BuiltinFunctionType::PRINT, 1, 1}},
            {"parseInt", {"parseInt", BuiltinFunctionType::PARSE_INT, 1, 1}},
            {"转整数", {"转整数", BuiltinFunctionType::PARSE_INT, 1, 1}},
            {"parseFloat", {"parseFloat", BuiltinFunctionType::PARSE_FLOAT, 1, 1}},
            {"转浮点", {"转浮点", BuiltinFunctionType::PARSE_FLOAT, 1, 1}},
            {"toString", {"toString", BuiltinFunctionType::TO_STRING, 1, 1}},
            {"转字符串", {"转字符串", BuiltinFunctionType::TO_STRING, 1, 1}},
            {"length", {"length", BuiltinFunctionType::LENGTH, 1, 1}},
            {"长度", {"长度", BuiltinFunctionType::LENGTH, 1, 1}},
            {"ask", {"ask", BuiltinFunctionType::ASK, 2, 2}},
            {"询问", {"询问", BuiltinFunctionType::ASK, 2, 2}}
        };
        return functions;
    }
    
    static bool isBuiltin(const std::string& name) {
        return getAll().find(name) != getAll().end();
    }
    
    static BuiltinFunctionType getType(const std::string& name) {
        auto it = getAll().find(name);
        return it != getAll().end() ? it->second.type : BuiltinFunctionType::UNKNOWN;
    }
};

} // namespace jvav

#endif // JVAV_FUNCTIONS_H 