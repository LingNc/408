#ifndef MACROS_H
#define MACROS_H

// 线程相关宏，用于简化哲学家问题等场景
// 依赖于 thread.cpp 中定义的 get_current_tid()

// 获取当前线程ID (从1开始，对应第几个创建的线程)
#define _tid (get_current_tid())

// 获取0-based索引 (tid-1)，用于数组下标如 chopstick[_i]
#define _i (get_current_tid() - 1)

// 获取线程名称字符串
#define _name (get_current_thread_name())

#endif // MACROS_H
