#include "log.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <pthread.h>

// 全局日志级别变量
LogLevel g_log_level = LOG_LEVEL_INFO;

// 递归互斥锁，允许同一线程重复加锁（防止 LOG_BEGIN/END 嵌套死锁）
static pthread_mutexattr_t log_mutex_attr;
static pthread_once_t log_mutex_once = PTHREAD_ONCE_INIT;
pthread_mutex_t log_mutex;

// 初始化递归锁（只执行一次）
static void init_log_mutex_once() {
    pthread_mutexattr_init(&log_mutex_attr);
    pthread_mutexattr_settype(&log_mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&log_mutex, &log_mutex_attr);
}

void log_init(int level) {
    // 首先初始化锁（线程安全，只执行一次）
    pthread_once(&log_mutex_once, init_log_mutex_once);

    // level: 0=INFO, 1=DEBUG, 2=SEM
    if (level >= 2) {
        g_log_level = LOG_LEVEL_SEM;
        printf("日志系统初始化完成，级别: SEM (信号量操作)\n");
    } else if (level >= 1) {
        g_log_level = LOG_LEVEL_DEBUG;
        printf("日志系统初始化完成，级别: DEBUG\n");
    } else {
        g_log_level = LOG_LEVEL_INFO;
        printf("日志系统初始化完成，级别: INFO\n");
    }
}

void log_print(const char *level, const char *file, int line, int tid, const char *fmt, ...) {
    // 确保锁已初始化（线程安全，只执行一次）
    pthread_once(&log_mutex_once, init_log_mutex_once);

    // 加锁，保证日志输出原子性
    pthread_mutex_lock(&log_mutex);

    // 获取当前时间
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    // 提取文件名（去掉路径）
    const char *filename = file;
    const char *p = file;
    while (*p) {
        if (*p == '/' || *p == '\\') {
            filename = p + 1;
        }
        p++;
    }

    // 打印时间、线程ID、级别、文件和行号
    printf("[%02d:%02d:%02d] [T%02d] [%-5s] %-7s:%-4d ",
           local->tm_hour, local->tm_min, local->tm_sec,
           tid, level, filename, line);

    // 打印用户消息
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
    fflush(stdout);  // 立即刷新缓冲区

    // 解锁
    pthread_mutex_unlock(&log_mutex);
}
