# LLVM 安装指南

要使Jvav编译器能够生成原生可执行文件，您需要安装LLVM工具链。本指南提供了各种平台上安装LLVM的详细步骤。

## macOS

最简单的方法是使用Homebrew：

```bash
brew install llvm
```

安装完成后，需要在构建Jvav编译器时指定LLVM的路径：

```bash
cd JvavCompiler
mkdir -p build && cd build
cmake -DJVAV_ENABLE_LLVM=ON -DLLVM_DIR=$(brew --prefix llvm)/lib/cmake/llvm ..
make
```

## Linux (Ubuntu/Debian)

使用apt安装LLVM：

```bash
sudo apt-get update
sudo apt-get install llvm-dev clang
```

然后构建Jvav编译器：

```bash
cd JvavCompiler
mkdir -p build && cd build
cmake -DJVAV_ENABLE_LLVM=ON ..
make
```

## Windows

1. 从[LLVM官方下载页面](https://releases.llvm.org/download.html)下载最新的LLVM安装包
2. 运行安装程序，确保选择"Add LLVM to the system PATH"选项
3. 使用Visual Studio或CMake构建Jvav编译器：

```bash
cd JvavCompiler
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -DJVAV_ENABLE_LLVM=ON ..
cmake --build . --config Release
```

如果LLVM安装在非标准位置，您可能需要指定LLVM_DIR：

```bash
cmake -G "Visual Studio 16 2019" -DJVAV_ENABLE_LLVM=ON -DLLVM_DIR="C:/Program Files/LLVM/lib/cmake/llvm" ..
```

## 鸿蒙系统

鸿蒙系统通常基于Linux，因此可以按照Linux的安装步骤进行操作：

```bash
sudo apt-get update
sudo apt-get install llvm-dev clang
```

然后构建Jvav编译器：

```bash
cd JvavCompiler
mkdir -p build && cd build
cmake -DJVAV_ENABLE_LLVM=ON ..
make
```

## 从源码构建LLVM (适用于所有平台)

如果您需要特定版本的LLVM或预编译包不可用，可以从源码构建：

```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build
cd build

# 配置构建
cmake -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../llvm

# 编译
make -j$(nproc)

# 安装
sudo make install
```

然后构建Jvav编译器：

```bash
cd JvavCompiler
mkdir -p build && cd build
cmake -DJVAV_ENABLE_LLVM=ON ..
make
```

## 验证LLVM集成

构建完成后，您可以使用以下命令验证Jvav编译器是否正确集成了LLVM：

```bash
./jvavc --version
```

输出应该显示"LLVM后端: 已启用"。

现在，您可以使用Jvav编译器生成原生可执行文件：

```bash
./jvavc -o hello hello.toilet
```

或生成特定平台的可执行文件：

```bash
./jvavc --target=windows -o hello.exe hello.toilet
``` 