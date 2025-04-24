#ifndef JVAV_REPL_H
#define JVAV_REPL_H

#include <string>
#include <vector>
#include <memory>
#include "JvavCompiler.h"

namespace jvav {

/**
 * Jvav交互式终端(REPL)类
 * 提供Read-Eval-Print-Loop功能
 */
class JvavREPL {
public:
    JvavREPL();
    ~JvavREPL();

    /**
     * 启动REPL循环
     */
    void run();
    
    /**
     * 设置REPL选项
     */
    void setOptions(const JvavCompilerOptions& options);
    
    /**
     * 加载并执行文件
     */
    bool loadFile(const std::string& filename);
    
    /**
     * 显示帮助信息
     */
    void showHelp();
    
    /**
     * 显示版本信息
     */
    void showVersion() const;

private:
    // 解析和执行单条语句
    bool evalLine(const std::string& line);
    
    // 处理REPL命令（以:开头的命令）
    bool handleCommand(const std::string& command);
    
    // 编译和执行代码
    bool compileAndRun(const std::string& code);
    
    // 用户输入处理函数
    std::string readUserInput(const std::string& prompt, const std::string& options);
    
    // 保存已输入的代码
    std::vector<std::string> history_;
    
    // 当前累积的代码
    std::string currentCode_;
    
    // 编译器实例
    JvavCompiler compiler_;
    
    // 编译选项
    JvavCompilerOptions options_;
    
    // 是否继续REPL循环
    bool shouldContinue_;
};

} // namespace jvav

#endif // JVAV_REPL_H 