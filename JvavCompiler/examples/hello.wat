(module
  ;; 导入JavaScript函数
  (import "console" "log" (func $console_log (param i32)))
  (import "console" "log_str" (func $console_log_str (param i32)))

  ;; 内存定义
  (import "js" "mem" (memory 1))

  ;; 工具函数
  (func $print_number (param $num i32)
    local.get $num
    call $console_log
  )

  ;; 全局临时变量
  (global $temp (mut i32) (i32.const 0))

  ;; 全局变量定义
  (global $name (mut i32) (i32.const 0))
  (global $count (mut i32) (i32.const 0))
  (global $result (mut i32) (i32.const 0))

  ;; 主函数
  (func $main (result i32)
    ;; 局部临时变量
    (local $temp i32)

  ;; 打印语句
  i32.const 0 ;; 字符串暂不支持
  global.get $name
  i32.add
  i32.const 0 ;; 字符串暂不支持
  i32.add
  call $print_number

  ;; IF语句
  global.get $count
  i32.const 0
  i32.gt_s
  (if
    (then
  ;; 打印语句
  i32.const 0 ;; 字符串暂不支持
  call $print_number

  ;; 循环语句
  (local $i i32)
  i32.const 0
  local.set $i
  global.get $count
  (local $loop_count i32)
  local.set $loop_count
  (loop $loop
  ;; 打印语句
  global.get $count
  i32.const 0 ;; 未定义的变量
  i32.sub
  call $print_number

    local.get $i
    i32.const 1
    i32.add
    local.set $i
    local.get $i
    local.get $loop_count
    i32.lt_s
    br_if $loop
  )

  ;; 打印语句
  i32.const 0 ;; 字符串暂不支持
  call $print_number

    )
    (else
  ;; 打印语句
  i32.const 0 ;; 字符串暂不支持
  call $print_number

    )
  )

  ;; 函数定义: 计算平方
  (func $计算平方 (param $x i32) (result i32)
    (local $temp i32)
  i32.const 0 ;; 未定义的变量
  i32.const 0 ;; 未定义的变量
  i32.mul
    return
    i32.const 0 ;; 默认返回值
  )

  (export "计算平方" (func $计算平方))

  ;; 打印语句
  i32.const 0 ;; 字符串暂不支持
  global.get $result
  i32.add
  call $print_number

    i32.const 0 ;; 主函数返回值
  )

  (export "main" (func $main))

)
