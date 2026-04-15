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

// 初始化日志系统，enable_debug 为 0 时关闭 debug 输出
void log_init(int enable_debug);

// 内部使用，不要直接调用
void log_print(const char *level, const char *file, int line, const char *fmt, ...);

// 日志宏
#define LOG_INFO(fmt, ...)  log_print("INFO", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) \
    do { \
        if (g_log_level >= LOG_LEVEL_DEBUG) \
            log_print("DEBUG", __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#endif // LOG_H
