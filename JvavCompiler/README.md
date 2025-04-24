# Jvav编译器

这是Jvav语言的官方编译器实现，支持多种平台包括Windows、macOS、Linux和鸿蒙系统。

## 特性

- 支持完整的Jvav语言特性
- 支持中英文混合编程
- 跨平台编译，可生成原生可执行文件
- 优化级别可调
- 支持枚举类型和异常处理
- 支持记录类型和模块系统
- **新功能**: 支持编译到各种目标平台的原生可执行文件

## 构建方法

### 前置依赖

- CMake 3.10+
- 现代C++编译器(支持C++17)
- LLVM工具链(用于生成原生代码，可选)

有关安装LLVM的详细指南，请参阅[LLVM安装指南](LLVM_INSTALL.md)。

### Windows构建

基本构建（仅支持WebAssembly）:
```bash
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

启用LLVM后端（支持原生可执行文件）:
```bash
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -DJVAV_ENABLE_LLVM=ON ..
cmake --build . --config Release
```

### macOS/Linux/鸿蒙构建

基本构建（仅支持WebAssembly）:
```bash
mkdir build
cd build
cmake ..
make
```

启用LLVM后端（支持原生可执行文件）:
```bash
mkdir build
cd build
cmake -DJVAV_ENABLE_LLVM=ON ..
make
```

## 使用方法

编译Jvav源文件(.toilet)为WebAssembly:

```bash
./jvavc example.toilet
```

生成原生可执行文件:

```bash
./jvavc -o myprogram example.toilet
```

生成特定平台的可执行文件:

```bash
./jvavc --target=windows -o myprogram.exe example.toilet
./jvavc --target=macos -o myprogram example.toilet
./jvavc --target=linux -o myprogram example.toilet
./jvavc --target=harmony -o myprogram example.toilet
```

生成目标文件:

```bash
./jvavc -c -o example.o example.toilet
```

生成汇编代码:

```bash
./jvavc -S -o example.s example.toilet
```

生成LLVM IR:

```bash
./jvavc --emit-llvm -o example.ll example.toilet
```

使用优化:

```bash
./jvavc -O2 example.toilet
```

查看帮助:

```bash
./jvavc --help
```

## 语言特性

Jvav是一种简洁但功能丰富的编程语言，主要特性如下:

### 基本类型
- 字符串(string)
- 数字(number)
- 布尔(boolean)
- 数组(array)
- 记录(record)
- 枚举(enum)

### 控制结构
- if/elif/else条件语句
- loop循环
- try/catch异常处理

### 功能
- 模块导入(import)
- 函数定义(define)
- 变量声明(set)
- 输出语句(print)

### 中文支持
Jvav支持使用中文关键字编程，例如:

```
设置 名字 == "张三"
输出("你好，" + 名字)

如果 年龄 > 18 {
    输出("成年人")
} 否则 {
    输出("未成年")
}
```

## 项目结构

```
JvavCompiler/
├── include/             # 头文件
│   ├── JvavCompiler.h   # 编译器主类头文件
│   ├── lexer/           # 词法分析相关头文件
│   ├── parser/          # 语法分析相关头文件
│   ├── ast/             # 抽象语法树相关头文件
│   ├── codegen/         # 代码生成相关头文件
│   └── optimizer/       # 优化器相关头文件
│
├── src/                 # 源文件
│   ├── main.cpp         # 主程序入口
│   ├── JvavCompiler.cpp # 编译器主类实现
│   ├── lexer/           # 词法分析器实现
│   ├── parser/          # 语法分析器实现
│   ├── ast/             # AST实现
│   ├── codegen/         # 代码生成器实现
│   └── optimizer/       # 优化器实现
│
└── CMakeLists.txt       # CMake构建配置
```

## 后续开发计划

- ✅ 实现代码生成器，支持编译到各种目标平台
- 添加更多优化选项
- 改进错误提示和调试信息
- 添加库函数支持
- 实现更完整的标准库
- 支持多文件工程和包管理

## 贡献指南

欢迎贡献代码! 请遵循以下步骤:

1. Fork本仓库
2. 创建功能分支
3. 提交修改
4. 推送到分支
5. 创建Pull Request

## 许可证

本项目不知道使用啥许可证