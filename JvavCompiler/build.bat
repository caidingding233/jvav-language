@echo off
REM Jvav编译器Windows构建脚本

setlocal enabledelayedexpansion

REM 获取脚本的绝对路径
set "SCRIPT_DIR=%~dp0"
REM 去除末尾的反斜杠
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

REM 颜色定义
set "RED=[91m"
set "GREEN=[92m"
set "YELLOW=[93m"
set "BLUE=[94m"
set "NC=[0m"

REM 默认参数
set BUILD_TYPE=Release
set CLEAN=0
set VERBOSE=0
set LLVM_PATH=
set BUILD_GENERATOR="Visual Studio 17 2022"

REM 解析命令行参数
:parse_args
if "%~1"=="" goto :end_parse_args
if /i "%~1"=="-h" (
    call :show_help
    exit /b 0
) else if /i "%~1"=="--help" (
    call :show_help
    exit /b 0
) else if /i "%~1"=="-c" (
    set CLEAN=1
    shift
    goto :parse_args
) else if /i "%~1"=="--clean" (
    set CLEAN=1
    shift
    goto :parse_args
) else if /i "%~1"=="-d" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
) else if /i "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
) else if /i "%~1"=="-r" (
    set BUILD_TYPE=Release
    shift
    goto :parse_args
) else if /i "%~1"=="--release" (
    set BUILD_TYPE=Release
    shift
    goto :parse_args
) else if /i "%~1"=="-v" (
    set VERBOSE=1
    shift
    goto :parse_args
) else if /i "%~1"=="--verbose" (
    set VERBOSE=1
    shift
    goto :parse_args
) else if /i "%~1"=="--generator" (
    set BUILD_GENERATOR=%~2
    shift
    shift
    goto :parse_args
) else if /i "%~1"=="--llvm-path" (
    set LLVM_PATH=%~2
    shift
    shift
    goto :parse_args
) else (
    echo %RED%错误: 未知选项 %~1%NC%
    call :show_help
    exit /b 1
)

:end_parse_args

REM 显示构建信息
echo %GREEN%在 Windows 上构建 Jvav编译器 (%BUILD_TYPE%)...%NC%

REM 检查必要的工具
where cmake >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo %RED%错误: 找不到cmake，请先安装%NC%
    exit /b 1
)

REM 进入项目根目录
cd /d "%SCRIPT_DIR%"

REM 创建构建目录
if not exist build (
    mkdir build
) else if %CLEAN% equ 1 (
    echo %YELLOW%清理构建目录...%NC%
    rmdir /s /q build
    mkdir build
)

REM 进入构建目录
cd build

REM 配置CMake构建
set CMAKE_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_TYPE%

REM 添加LLVM路径（如果指定）
if not "%LLVM_PATH%"=="" (
    set CMAKE_ARGS=%CMAKE_ARGS% -DLLVM_DIR=%LLVM_PATH%
)

echo %YELLOW%配置 CMake...%NC%
cmake -G %BUILD_GENERATOR% %CMAKE_ARGS% "%SCRIPT_DIR%"

if %ERRORLEVEL% neq 0 (
    echo %RED%CMake配置失败!%NC%
    cd /d "%SCRIPT_DIR%"
    exit /b 1
)

REM 执行构建
echo %YELLOW%开始构建...%NC%

if %VERBOSE% equ 1 (
    cmake --build . --config %BUILD_TYPE% --verbose
) else (
    cmake --build . --config %BUILD_TYPE%
)

if %ERRORLEVEL% neq 0 (
    echo %RED%构建失败!%NC%
    cd /d "%SCRIPT_DIR%"
    exit /b 1
)

echo %GREEN%构建成功!%NC%
echo 可执行文件位于: %cd%\%BUILD_TYPE%\jvavc.exe

REM 返回到原始目录
cd /d "%SCRIPT_DIR%"
exit /b 0

:show_help
echo %BLUE%Jvav编译器Windows构建脚本%NC%
echo 用法: build.bat [选项]
echo 选项:
echo   -h, --help      显示此帮助信息
echo   -c, --clean     清除构建文件后重新构建
echo   -d, --debug     构建调试版本
echo   -r, --release   构建发布版本（默认）
echo   -v, --verbose   显示详细构建信息
echo   --generator     指定CMake生成器 (默认: "Visual Studio 17 2022")
echo   --llvm-path     指定LLVM安装路径
exit /b 0 