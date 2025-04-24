#!/bin/bash

# Jvav编译器构建脚本
# 支持macOS, Linux和HarmonyOS

# 设置变量
BUILD_TYPE="Debug"
CLEAN_BUILD=0
LLVM_PATH=""
AUTO_DETECT_LLVM=1
ENABLE_LLVM=0  # 默认关闭LLVM支持

# 显示帮助信息
show_help() {
    echo "Jvav编译器构建脚本"
    echo "用法: $0 [选项]"
    echo "选项:"
    echo "  -h, --help         显示此帮助信息"
    echo "  -r, --release      构建发布版本（默认为调试版本）"
    echo "  -c, --clean        清理构建目录"
    echo "  --llvm=PATH        指定LLVM安装路径"
    echo "  --enable-llvm      启用LLVM支持（需要LLVM）"
    echo "  --disable-llvm     禁用LLVM支持"
    echo "  --no-auto-llvm     禁用自动检测LLVM"
    echo ""
    echo "示例:"
    echo "  $0 -r              构建发布版本"
    echo "  $0 -c              清理构建目录并重新构建"
    echo "  $0 --llvm=/usr/local/opt/llvm  使用指定的LLVM路径"
    exit 0
}

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=1
            shift
            ;;
        --llvm=PATH)
            if [[ -z "$2" || "$2" == -* ]]; then
                echo "错误: --llvm 选项需要一个参数"
                exit 1
            fi
            LLVM_PATH="$2"
            AUTO_DETECT_LLVM=0  # 如果手动指定了LLVM路径，则禁用自动检测
            ENABLE_LLVM=1  # 启用LLVM支持
            shift 2
            ;;
        --enable-llvm)
            ENABLE_LLVM=1  # 启用LLVM支持
            shift
            ;;
        --disable-llvm)
            ENABLE_LLVM=0  # 禁用LLVM支持
            LLVM_PATH=""  # 清空LLVM路径
            shift
            ;;
        --no-auto-llvm)
            AUTO_DETECT_LLVM=0  # 禁用自动检测LLVM
            shift
            ;;
        *)
            echo "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

# 检测操作系统
OS="unknown"
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
    # 检查是否为HarmonyOS
    if [[ -f /etc/harmony-version ]]; then
        OS="harmonyos"
    fi
else
    echo "不支持的操作系统: $OSTYPE"
    exit 1
fi

echo "检测到操作系统: $OS"

# 自动检测LLVM路径
if [[ "$AUTO_DETECT_LLVM" == 1 && -z "$LLVM_PATH" ]]; then
    if [[ "$OS" == "macos" ]]; then
        # 在macOS上查找Homebrew安装的LLVM
        if command -v brew &> /dev/null; then
            BREW_LLVM_PATH=$(brew --prefix llvm 2>/dev/null)
            if [[ -n "$BREW_LLVM_PATH" && -d "$BREW_LLVM_PATH" ]]; then
                LLVM_PATH="$BREW_LLVM_PATH"
                echo "检测到Homebrew安装的LLVM: $LLVM_PATH"
            fi
        fi
        
        # 如果找不到Homebrew安装的LLVM，检查XCode的LLVM
        if [[ -z "$LLVM_PATH" && -d "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang" ]]; then
            LLVM_PATH="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr"
            echo "使用XCode的LLVM: $LLVM_PATH"
        fi
    elif [[ "$OS" == "linux" || "$OS" == "harmonyos" ]]; then
        # 在Linux上查找LLVM安装
        if command -v llvm-config &> /dev/null; then
            LLVM_CONFIG_PATH=$(which llvm-config)
            LLVM_PATH=$(dirname $(dirname $LLVM_CONFIG_PATH))
            echo "检测到系统LLVM: $LLVM_PATH"
        fi
    fi
fi

# 检查构建所需的工具
if ! command -v cmake &> /dev/null; then
    echo "错误: cmake未安装，请先安装cmake"
    exit 1
fi

if ! command -v c++ &> /dev/null && ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "错误: 未找到C++编译器，请安装g++或clang++"
    exit 1
fi

# 创建并进入构建目录
BUILD_DIR="build"

if [[ "$CLEAN_BUILD" == 1 && -d "$BUILD_DIR" ]]; then
    echo "清理构建目录..."
    rm -rf "$BUILD_DIR"
fi

if [[ ! -d "$BUILD_DIR" ]]; then
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# 配置CMake
echo "配置CMake..."
# 构建CMake命令
CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"

# 如果启用LLVM支持且找到了LLVM路径
if [[ "$ENABLE_LLVM" == 1 && -n "$LLVM_PATH" ]]; then
    echo "启用LLVM支持，路径: $LLVM_PATH"
    CMAKE_FLAGS="$CMAKE_FLAGS -DJVAV_ENABLE_LLVM=ON -DLLVM_DIR=$LLVM_PATH/lib/cmake/llvm"
else
    echo "禁用LLVM支持"
    CMAKE_FLAGS="$CMAKE_FLAGS -DJVAV_ENABLE_LLVM=OFF"
fi

# 执行CMake配置
cmake .. $CMAKE_FLAGS

# 确定处理器核心数以加速构建
if [[ "$OS" == "macos" ]]; then
    CORES=$(sysctl -n hw.ncpu)
elif [[ "$OS" == "linux" || "$OS" == "harmonyos" ]]; then
    CORES=$(nproc)
else
    CORES=2
fi

# 开始构建
echo "开始构建，使用 $CORES 个核心..."
make -j $CORES

# 检查构建结果
if [ $? -eq 0 ]; then
    echo "构建成功，可执行文件位于:"
    echo "  $(pwd)/jvavc"
    echo "  $(pwd)/jvav_terminal"
else
    echo "构建失败"
    exit 1
fi

cd .. 