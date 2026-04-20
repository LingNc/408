#ifndef LOG_H
#define LOG_H

#include <cstdio>

// 日志级别（值越大越详细）
enum LogLevel {
    LOG_LEVEL_INFO = 0,   // INFO 级别
    LOG_LEVEL_DEBUG,      // DEBUG 级别
    LOG_LEVEL_SEM         // SEM 级别（最详细，包括信号量操作）
};

// 当前日志级别配置（全局变量）
extern LogLevel g_log_level;
extern pthread_mutex_t log_mutex;  // 日志互斥锁（外部可访问）

// 获取当前线程ID和名字（从 thread.cpp 导入，C链接）
extern "C" int get_current_tid();
extern "C" const char* get_current_thread_name();

// 初始化日志系统
// level: 0=INFO, 1=DEBUG, 2=SEM
void log_init(int level);

// 内部使用，不要直接调用
void log_print(const char *level, const char *file, int line, int tid, const char *name, const char *fmt, ...);

// 日志宏（自动获取线程ID和名字）
#define LOG_INFO(fmt, ...)  log_print("INFO", __FILE__, __LINE__, get_current_tid(), get_current_thread_name(), fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) \
    do { \
        if (g_log_level >= LOG_LEVEL_DEBUG) \
            log_print("DEBUG", __FILE__, __LINE__, get_current_tid(), get_current_thread_name(), fmt, ##__VA_ARGS__); \
    } while(0)
#define LOG_SEM(fmt, ...) \
    do { \
        if (g_log_level >= LOG_LEVEL_SEM) \
            log_print("SEM  ", __FILE__, __LINE__, get_current_tid(), get_current_thread_name(), fmt, ##__VA_ARGS__); \
    } while(0)

// 信号量操作日志宏（使用 SEM 级别）
#define LOG_SEM_P(sem) \
    LOG_SEM("P操作：%s减少到 %d", #sem, (sem).value)
#define LOG_SEM_V(sem) \
    LOG_SEM("V操作：%s增加到 %d", #sem, (sem).value)

// 日志区域加锁宏（用于包裹一段日志，保证原子输出）
#define LOG_BEGIN()   pthread_mutex_lock(&log_mutex)
#define LOG_END()     pthread_mutex_unlock(&log_mutex)

#endif // LOG_H
