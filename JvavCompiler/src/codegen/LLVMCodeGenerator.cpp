#include "codegen/LLVMCodeGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <system_error>
#include <optional>

// 引入LLVM库的头文件（如果LLVM依赖已安装）
#ifdef JVAV_HAS_LLVM
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/CodeGen/CodeGenCommonISel.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>
#endif

namespace jvav {

// LLVM代码生成器的私有实现
class LLVMCodeGenerator::LLVMCodeGeneratorImpl {
public:
    LLVMCodeGeneratorImpl() {}
    ~LLVMCodeGeneratorImpl() {}

    bool generateCode(
        const std::vector<std::unique_ptr<Stmt>>& ast, 
        const std::string& outputFile,
        JvavTargetType targetType,
        const std::string& targetPlatform
    ) {
#ifdef JVAV_HAS_LLVM
        // 初始化LLVM
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();

        // 创建LLVM上下文和模块
        llvm::LLVMContext context;
        auto module = std::make_unique<llvm::Module>("jvav_module", context);
        llvm::IRBuilder<> builder(context);

        // 为目标平台设置triple
        std::string targetTriple;
        if (targetPlatform.empty()) {
            targetTriple = llvm::sys::getDefaultTargetTriple();
        } else if (targetPlatform == "windows") {
            targetTriple = "x86_64-pc-windows-msvc";
        } else if (targetPlatform == "macos") {
            targetTriple = "x86_64-apple-darwin";
        } else if (targetPlatform == "linux") {
            targetTriple = "x86_64-unknown-linux-gnu";
        } else if (targetPlatform == "harmony") {
            targetTriple = "aarch64-unknown-linux-gnu"; // 假设鸿蒙系统使用ARM架构
        } else {
            std::cerr << "不支持的目标平台: " << targetPlatform << std::endl;
            return false;
        }
        
        module->setTargetTriple(targetTriple);

        // 设置数据布局
        std::string error;
        const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
        if (!target) {
            std::cerr << "目标平台查找失败: " << error << std::endl;
            return false;
        }

        llvm::TargetOptions opt;
        auto RM = std::optional<llvm::Reloc::Model>(llvm::Reloc::PIC_);
        auto targetMachine = target->createTargetMachine(
            targetTriple, "generic", "", opt, RM);

        module->setDataLayout(targetMachine->createDataLayout());

        // 将AST转换为LLVM IR
        if (!generateLLVMIR(ast, module.get(), builder)) {
            std::cerr << "LLVM IR生成失败" << std::endl;
            return false;
        }

        // 验证模块
        std::string verifyError;
        llvm::raw_string_ostream verifyErrorStream(verifyError);
        if (llvm::verifyModule(*module, &verifyErrorStream)) {
            std::cerr << "模块验证失败: " << verifyError << std::endl;
            return false;
        }

        // 根据目标类型生成不同格式的输出
        if (targetType == JvavTargetType::LLVM_IR) {
            // 输出LLVM IR到文件
            std::error_code EC;
            llvm::raw_fd_ostream dest(outputFile, EC, llvm::sys::fs::OF_None);
            if (EC) {
                std::cerr << "无法打开输出文件: " << EC.message() << std::endl;
                return false;
            }
            module->print(dest, nullptr);
            return true;
        } else {
            // 创建一个PassManager进行优化
            llvm::legacy::PassManager passManager;
            
            // 添加一些基本优化
            passManager.add(llvm::createPromoteMemoryToRegisterPass());
            passManager.add(llvm::createInstructionCombiningPass());
            passManager.add(llvm::createReassociatePass());
            passManager.add(llvm::createGVNPass());
            passManager.add(llvm::createCFGSimplificationPass());

            // 根据目标类型设置输出格式
            llvm::CodeGenFileType fileType;
            if (targetType == JvavTargetType::OBJECT_FILE) {
                fileType = llvm::CodeGenFileType::ObjectFile;
            } else if (targetType == JvavTargetType::ASSEMBLY) {
                fileType = llvm::CodeGenFileType::AssemblyFile;
            } else {
                // 默认输出对象文件，然后由外部链接器链接
                fileType = llvm::CodeGenFileType::ObjectFile;
            }

            // 输出到文件
            std::error_code EC;
            llvm::raw_fd_ostream dest(outputFile, EC, llvm::sys::fs::OF_None);
            if (EC) {
                std::cerr << "无法打开输出文件: " << EC.message() << std::endl;
                return false;
            }

            // 添加目标特定的passes
            if (targetMachine->addPassesToEmitFile(passManager, dest, nullptr, fileType)) {
                std::cerr << "目标机器无法生成文件" << std::endl;
                return false;
            }

            // 运行pass manager
            passManager.run(*module);
            dest.flush();

            // 如果是可执行文件，还需要链接
            if (targetType == JvavTargetType::EXECUTABLE) {
                return linkExecutable(outputFile, targetTriple);
            }

            return true;
        }
#else
        std::cerr << "错误: LLVM支持未启用，无法生成LLVM IR或原生代码。" << std::endl;
        std::cerr << "请使用CMake -DJVAV_ENABLE_LLVM=ON选项重新构建编译器。" << std::endl;
        return false;
#endif
    }

private:
#ifdef JVAV_HAS_LLVM
    // 将AST转换为LLVM IR
    bool generateLLVMIR(
        const std::vector<std::unique_ptr<Stmt>>& ast,
        llvm::Module* module,
        llvm::IRBuilder<>& builder
    ) {
        // 创建一个全局上下文
        llvm::LLVMContext& context = module->getContext();
        
        // 声明printf函数
        std::vector<llvm::Type*> printfArgs;
        printfArgs.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0));
        llvm::FunctionType* printfType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context), printfArgs, true);
        llvm::Function* printfFunc = llvm::Function::Create(
            printfType, llvm::Function::ExternalLinkage, "printf", module);

        // 创建main函数
        llvm::FunctionType* mainType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context), false);
        llvm::Function* mainFunc = llvm::Function::Create(
            mainType, llvm::Function::ExternalLinkage, "main", module);
        
        // 创建main函数的基本块
        llvm::BasicBlock* mainBlock = llvm::BasicBlock::Create(context, "entry", mainFunc);
        builder.SetInsertPoint(mainBlock);

        // 用于存储变量的映射表
        std::unordered_map<std::string, llvm::Value*> variables;
        
        // 遍历AST并生成LLVM IR
        for (const auto& stmt : ast) {
            if (!generateStmt(stmt.get(), module, builder, variables, printfFunc)) {
                return false;
            }
        }
        
        // 返回0表示程序成功执行
        builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));
        
        return true;
    }
    
    // 生成语句的LLVM IR
    bool generateStmt(
        const Stmt* stmt,
        llvm::Module* module,
        llvm::IRBuilder<>& builder,
        std::unordered_map<std::string, llvm::Value*>& variables,
        llvm::Function* printfFunc
    ) {
        // 根据语句类型生成不同的IR
        switch (stmt->getType()) {
            case StmtType::PRINT:
                return generatePrintStmt(static_cast<const PrintStmt*>(stmt), 
                                        module, builder, variables, printfFunc);
            case StmtType::SET:
                return generateSetStmt(static_cast<const SetStmt*>(stmt), 
                                      module, builder, variables);
            case StmtType::IF:
                return generateIfStmt(static_cast<const IfStmt*>(stmt), 
                                     module, builder, variables, printfFunc);
            // 其他语句类型...
            default:
                std::cerr << "未实现的语句类型: " << static_cast<int>(stmt->getType()) << std::endl;
                return false;
        }
    }
    
    // 生成打印语句的LLVM IR
    bool generatePrintStmt(
        const PrintStmt* stmt,
        llvm::Module* module,
        llvm::IRBuilder<>& builder,
        std::unordered_map<std::string, llvm::Value*>& variables,
        llvm::Function* printfFunc
    ) {
        // 简单实现：目前只支持直接的字符串字面量
        if (stmt->value->getType() == ExprType::LITERAL) {
            auto* literalExpr = static_cast<const LiteralExpr*>(stmt->value.get());
            if (literalExpr->token.getType() == TokenType::STRING_LITERAL) {
                // 获取字符串值
                std::string strValue = literalExpr->token.getValue();
                // 删除引号
                strValue = strValue.substr(1, strValue.length() - 2);
                // 添加换行符
                strValue += "\n";
                
                // 创建全局字符串常量
                llvm::Constant* strConstant = llvm::ConstantDataArray::getString(
                    module->getContext(), strValue, true);
                llvm::GlobalVariable* strGlobal = new llvm::GlobalVariable(
                    *module, strConstant->getType(), true,
                    llvm::GlobalValue::PrivateLinkage, strConstant, ".str");
                
                // 创建对全局字符串的引用
                llvm::Constant* zero = llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module->getContext()), 0);
                llvm::Constant* indices[] = {zero, zero};
                llvm::Constant* strPtr = llvm::ConstantExpr::getGetElementPtr(
                    strGlobal->getValueType(), strGlobal, indices, true);
                
                // 调用printf函数
                std::vector<llvm::Value*> args;
                args.push_back(strPtr);
                builder.CreateCall(printfFunc, args);
                
                return true;
            }
        }
        
        std::cerr << "暂时只支持直接的字符串字面量打印" << std::endl;
        return false;
    }
    
    // 生成设置变量语句的LLVM IR
    bool generateSetStmt(
        const SetStmt* stmt,
        llvm::Module* module,
        llvm::IRBuilder<>& builder,
        std::unordered_map<std::string, llvm::Value*>& variables
    ) {
        // 暂时只支持整数字面量赋值
        if (stmt->value->getType() == ExprType::LITERAL) {
            auto* literalExpr = static_cast<const LiteralExpr*>(stmt->value.get());
            if (literalExpr->token.getType() == TokenType::NUMBER_LITERAL) {
                // 解析数字
                int numValue = std::stoi(literalExpr->token.getValue());
                
                // 创建整数常量
                llvm::Value* val = llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module->getContext()), numValue);
                
                // 为变量分配内存
                std::string varName = stmt->name.getValue();
                llvm::AllocaInst* alloca = builder.CreateAlloca(
                    llvm::Type::getInt32Ty(module->getContext()), nullptr, varName);
                
                // 存储值
                builder.CreateStore(val, alloca);
                
                // 将变量添加到映射表
                variables[varName] = alloca;
                
                return true;
            }
        }
        
        std::cerr << "暂时只支持整数字面量赋值" << std::endl;
        return false;
    }
    
    // 生成if语句的LLVM IR
    bool generateIfStmt(
        const IfStmt* stmt,
        llvm::Module* module,
        llvm::IRBuilder<>& builder,
        std::unordered_map<std::string, llvm::Value*>& variables,
        llvm::Function* printfFunc
    ) {
        // 暂时只实现简单的条件
        llvm::Value* condValue = nullptr;
        
        // 创建基本块
        llvm::Function* function = builder.GetInsertBlock()->getParent();
        llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(
            module->getContext(), "then", function);
        llvm::BasicBlock* elseBlock = nullptr;
        
        // 检查是否有else分支（第二个分支）
        bool hasElse = stmt->branches.size() > 1;
        
        if (hasElse) {
            elseBlock = llvm::BasicBlock::Create(module->getContext(), "else", function);
        }
        llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(
            module->getContext(), "ifcont", function);
        
        // 简单条件实现 - 暂时只支持布尔字面量
        if (stmt->branches[0].condition->getType() == ExprType::LITERAL) {
            auto* literalExpr = static_cast<const LiteralExpr*>(stmt->branches[0].condition.get());
            if (literalExpr->token.getType() == TokenType::BOOL_LITERAL && 
                literalExpr->token.getValue() == "true") {
                condValue = llvm::ConstantInt::getTrue(module->getContext());
            } else if (literalExpr->token.getType() == TokenType::BOOL_LITERAL && 
                       literalExpr->token.getValue() == "false") {
                condValue = llvm::ConstantInt::getFalse(module->getContext());
            }
        }
        
        if (!condValue) {
            std::cerr << "暂时只支持布尔字面量条件" << std::endl;
            return false;
        }
        
        // 创建条件分支
        if (hasElse) {
            builder.CreateCondBr(condValue, thenBlock, elseBlock);
        } else {
            builder.CreateCondBr(condValue, thenBlock, mergeBlock);
        }
        
        // 生成then分支代码
        builder.SetInsertPoint(thenBlock);
        for (const auto& thenStmt : stmt->branches[0].body) {
            if (!generateStmt(thenStmt.get(), module, builder, variables, printfFunc)) {
                return false;
            }
        }
        builder.CreateBr(mergeBlock);
        
        // 生成else分支代码
        if (hasElse) {
            builder.SetInsertPoint(elseBlock);
            for (const auto& elseStmt : stmt->branches[1].body) {
                if (!generateStmt(elseStmt.get(), module, builder, variables, printfFunc)) {
                    return false;
                }
            }
            builder.CreateBr(mergeBlock);
        }
        
        // 设置合并块为当前插入点
        builder.SetInsertPoint(mergeBlock);
        
        return true;
    }
    
    // 链接生成可执行文件
    bool linkExecutable(const std::string& objFile, const std::string& targetTriple) {
        // 使用系统链接器链接目标文件
        std::string cmd;
        std::string outFile = objFile;
        size_t dot = outFile.find_last_of('.');
        if (dot != std::string::npos) {
            outFile = outFile.substr(0, dot);
        }
        
        if (targetTriple.find("windows") != std::string::npos) {
            outFile += ".exe";
            cmd = "link " + objFile + " /OUT:" + outFile;
        } else {
            cmd = "clang " + objFile + " -o " + outFile;
        }
        
        std::cout << "链接命令: " << cmd << std::endl;
        int result = std::system(cmd.c_str());
        
        return result == 0;
    }
#endif
};

// 构造函数
LLVMCodeGenerator::LLVMCodeGenerator() : impl_(std::make_unique<LLVMCodeGeneratorImpl>()) {}

// 析构函数
LLVMCodeGenerator::~LLVMCodeGenerator() = default;

// 生成代码
bool LLVMCodeGenerator::generateCode(
    const std::vector<std::unique_ptr<Stmt>>& ast, 
    const std::string& outputFile,
    JvavTargetType targetType,
    const std::string& targetPlatform
) {
    return impl_->generateCode(ast, outputFile, targetType, targetPlatform);
}

} // namespace jvav 