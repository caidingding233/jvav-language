(module
  ;; 导入JavaScript环境提供的函数和内存
  (import "env" "memory" (memory 1))
  (import "env" "createString" (func $createString (param i32 i32) (result i32)))
  (import "env" "readString" (func $readString (param i32) (result i32 i32)))

  ;; 数据段定义：预定义的字符串常量
  (data (i32.const 0) "Hello, WebAssembly!")           ;; 英文字符串
  (data (i32.const 32) "你好，WebAssembly！")          ;; 中文字符串（UTF-8编码）
  
  ;; 全局变量：定义常量字符串的位置和长度
  (global $hello_ptr i32 (i32.const 0))
  (global $hello_len i32 (i32.const 19))
  (global $chinese_ptr i32 (i32.const 32))
  (global $chinese_len i32 (i32.const 22))  ;; UTF-8编码的中文字符串长度
  
  ;; 内存分配指针，从预定义数据段之后开始
  (global $mem_ptr (mut i32) (i32.const 128))
  
  ;; 辅助函数：分配内存
  (func $alloc (param $size i32) (result i32)
    (local $ptr i32)
    
    ;; 获取当前内存位置
    global.get $mem_ptr
    local.set $ptr
    
    ;; 更新内存指针
    global.get $mem_ptr
    local.get $size
    i32.add
    global.set $mem_ptr
    
    ;; 返回分配的内存位置
    local.get $ptr
  )
  
  ;; 获取预定义的"Hello, WebAssembly!"字符串
  (func $getHelloString (export "getHelloString") (result i32)
    ;; 使用createString函数创建字符串对象，返回字符串ID
    global.get $hello_ptr
    global.get $hello_len
    call $createString
  )
  
  ;; 获取预定义的中文字符串
  (func $getChineseString (export "getChineseString") (result i32)
    global.get $chinese_ptr
    global.get $chinese_len
    call $createString
  )
  
  ;; 连接两个字符串
  (func $concatenateStrings (export "concatenateStrings") (param $str1_id i32) (param $str2_id i32) (result i32)
    (local $str1_ptr i32)
    (local $str1_len i32)
    (local $str2_ptr i32)
    (local $str2_len i32)
    (local $new_ptr i32)
    (local $total_len i32)
    (local $i i32)
    
    ;; 获取第一个字符串
    local.get $str1_id
    call $readString
    local.set $str1_len
    local.set $str1_ptr
    
    ;; 获取第二个字符串
    local.get $str2_id
    call $readString
    local.set $str2_len
    local.set $str2_ptr
    
    ;; 计算总长度
    local.get $str1_len
    local.get $str2_len
    i32.add
    local.set $total_len
    
    ;; 分配新字符串的内存
    local.get $total_len
    call $alloc
    local.set $new_ptr
    
    ;; 复制第一个字符串
    (block $copy_str1
      (loop $copy_loop1
        ;; 检查是否已复制完第一个字符串
        local.get $i
        local.get $str1_len
        i32.ge_u
        br_if $copy_str1
        
        ;; 复制一个字节
        local.get $new_ptr
        local.get $i
        i32.add
        
        local.get $str1_ptr
        local.get $i
        i32.add
        i32.load8_u
        
        i32.store8
        
        ;; 递增索引
        local.get $i
        i32.const 1
        i32.add
        local.set $i
        
        br $copy_loop1
      )
    )
    
    ;; 重置循环索引变量，用于复制第二个字符串
    i32.const 0
    local.set $i
    
    ;; 复制第二个字符串
    (block $copy_str2
      (loop $copy_loop2
        ;; 检查是否已复制完第二个字符串
        local.get $i
        local.get $str2_len
        i32.ge_u
        br_if $copy_str2
        
        ;; 复制一个字节，位置从第一个字符串末尾开始
        local.get $new_ptr
        local.get $str1_len
        i32.add
        local.get $i
        i32.add
        
        local.get $str2_ptr
        local.get $i
        i32.add
        i32.load8_u
        
        i32.store8
        
        ;; 递增索引
        local.get $i
        i32.const 1
        i32.add
        local.set $i
        
        br $copy_loop2
      )
    )
    
    ;; 创建并返回新字符串的ID
    local.get $new_ptr
    local.get $total_len
    call $createString
  )
  
  ;; 反转字符串（按照字节处理，不处理多字节字符）
  (func $reverseString (export "reverseString") (param $str_id i32) (result i32)
    (local $str_ptr i32)
    (local $str_len i32)
    (local $new_ptr i32)
    (local $i i32)
    
    ;; 获取原始字符串
    local.get $str_id
    call $readString
    local.set $str_len
    local.set $str_ptr
    
    ;; 分配新字符串的内存
    local.get $str_len
    call $alloc
    local.set $new_ptr
    
    ;; 复制字符串，但顺序相反
    (block $reverse
      (loop $reverse_loop
        ;; 检查是否已复制完
        local.get $i
        local.get $str_len
        i32.ge_u
        br_if $reverse
        
        ;; 复制一个字节，将源字符串的i位置复制到目标字符串的(len-i-1)位置
        local.get $new_ptr
        local.get $str_len
        i32.const 1
        i32.sub
        local.get $i
        i32.sub
        i32.add
        
        local.get $str_ptr
        local.get $i
        i32.add
        i32.load8_u
        
        i32.store8
        
        ;; 递增索引
        local.get $i
        i32.const 1
        i32.add
        local.set $i
        
        br $reverse_loop
      )
    )
    
    ;; 创建并返回新字符串的ID
    local.get $new_ptr
    local.get $str_len
    call $createString
  )
) 