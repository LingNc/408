#ifndef LOG_H
#define LOG_H

#include <cstdio>

// 日志级别
enum LogLevel {
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_DEBUG
};

// 当前日志级别配置（全局变量）
extern LogLevel g_log_level;

// 获取当前线程ID（从 thread.cpp 导入，C链接）
extern "C" int get_current_tid();

// 初始化日志系统，enable_debug 为 0 时关闭 debug 输出
void log_init(int enable_debug);

// 内部使用，不要直接调用
void log_print(const char *level, const char *file, int line, int tid, const char *fmt, ...);

// 日志宏（自动获取线程ID）
#define LOG_INFO(fmt, ...)  log_print("INFO", __FILE__, __LINE__, get_current_tid(), fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) \
    do { \
        if (g_log_level >= LOG_LEVEL_DEBUG) \
            log_print("DEBUG", __FILE__, __LINE__, get_current_tid(), fmt, ##__VA_ARGS__); \
    } while(0)

// 信号量操作日志宏（自动获取变量名）
// LOG_SEM_P(plate) -> P操作：plate减少到 0
// LOG_SEM_V(plate) -> V操作：plate增加到 1
#define LOG_SEM_P(sem) \
    LOG_DEBUG("P操作：%s减少到 %d", #sem, (sem).value)
#define LOG_SEM_V(sem) \
    LOG_DEBUG("V操作：%s增加到 %d", #sem, (sem).value)

#endif // LOG_H
