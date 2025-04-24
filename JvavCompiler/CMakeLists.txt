cmake_minimum_required(VERSION 3.10)
project(JvavCompiler VERSION 1.0 LANGUAGES C CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加编译选项
option(JVAV_ENABLE_LLVM "启用LLVM后端支持" OFF)
option(JVAV_BUILD_TERMINAL "构建Jvav交互式终端" ON)

# 指定头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 编译器源文件
file(GLOB_RECURSE COMPILER_SOURCES 
    "src/*.cpp"
    "src/lexer/*.cpp"
    "src/parser/*.cpp"
    "src/ast/*.cpp"
    "src/codegen/*.cpp"
    "src/optimizer/*.cpp"
)

# 排除REPL相关文件，这些将单独编译
list(FILTER COMPILER_SOURCES EXCLUDE REGEX "src/repl/.*\\.cpp$")

# 创建Jvav编译器
add_executable(jvavc ${COMPILER_SOURCES})

# 设置编译选项
target_compile_options(jvavc PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# 处理LLVM依赖
if(JVAV_ENABLE_LLVM)
    # 查找LLVM包
    find_package(LLVM REQUIRED CONFIG)
    
    # 获取LLVM版本信息
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
    
    # 设置LLVM编译选项
    target_compile_definitions(jvavc PRIVATE JVAV_HAS_LLVM)
    
    # 包含LLVM头文件
    include_directories(${LLVM_INCLUDE_DIRS})
    add_definitions(${LLVM_DEFINITIONS})
    
    # 添加LLVM库
    llvm_map_components_to_libnames(llvm_libs support core irreader passes native MCParser TargetParser)
    target_link_libraries(jvavc ${llvm_libs})
    
    message(STATUS "LLVM Libraries: ${llvm_libs}")
endif()

# 根据不同平台进行配置
if(WIN32)
    # Windows特定配置
    target_compile_definitions(jvavc PRIVATE JVAV_PLATFORM_WINDOWS)
elseif(APPLE)
    # macOS特定配置
    target_compile_definitions(jvavc PRIVATE JVAV_PLATFORM_MACOS)
elseif(UNIX)
    # Linux特定配置
    target_compile_definitions(jvavc PRIVATE JVAV_PLATFORM_LINUX)
endif()

# 构建Jvav终端
if(JVAV_BUILD_TERMINAL)
    # REPL源文件
    file(GLOB REPL_SOURCES
        "src/repl/*.cpp"
    )
    
    # 复制编译器源文件，但排除main.cpp
    set(TERMINAL_SOURCES ${COMPILER_SOURCES})
    list(FILTER TERMINAL_SOURCES EXCLUDE REGEX "src/main\\.cpp$")
    
    # 添加终端可执行文件
    add_executable(jvav_terminal ${REPL_SOURCES} ${TERMINAL_SOURCES})
    
    # 设置编译选项
    target_compile_options(jvav_terminal PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )
    
    # 应用相同的平台定义
    if(WIN32)
        target_compile_definitions(jvav_terminal PRIVATE JVAV_PLATFORM_WINDOWS)
    elseif(APPLE)
        target_compile_definitions(jvav_terminal PRIVATE JVAV_PLATFORM_MACOS)
    elseif(UNIX)
        target_compile_definitions(jvav_terminal PRIVATE JVAV_PLATFORM_LINUX)
    endif()
    
    # 如果启用了LLVM，也为终端添加LLVM支持
    if(JVAV_ENABLE_LLVM)
        target_compile_definitions(jvav_terminal PRIVATE JVAV_HAS_LLVM)
        target_link_libraries(jvav_terminal ${llvm_libs})
    endif()
    
    # 安装规则
    install(TARGETS jvav_terminal DESTINATION bin)
    
    # 复制wasm_runner.js
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/repl/wasm_runner.js DESTINATION bin)
    message(STATUS "Jvav terminal will be built")
endif()

# 安装规则
install(TARGETS jvavc DESTINATION bin) 