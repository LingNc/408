#ifndef PV_H
#define PV_H

// PV Library Master Header
// 按照依赖顺序包含所有模块（基础 -> 上层）

// 0. 表达式计算工具（无依赖，被 P/V 使用）
#include "expr.h"

// 1. 基础信号量（无依赖）
#include "Semaphore.h"

// 2. P/V 操作（依赖 Semaphore）
#include "P.h"
#include "V.h"

// 3. 日志系统（无依赖）
#include "log.h"

// 4. 线程管理器（依赖 P/V 和日志）
#include "thread.h"

// 5. 常用宏定义（依赖 thread 中的 get_current_tid）
#include "macros.h"

#endif // PV_H