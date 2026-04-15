#include "log.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>

// 全局日志级别变量
LogLevel g_log_level = LOG_LEVEL_INFO;

void log_init(int enable_debug) {
    g_log_level = enable_debug ? LOG_LEVEL_DEBUG : LOG_LEVEL_INFO;
    LOG_INFO("日志系统初始化完成，Debug模式: %s", enable_debug ? "开启" : "关闭");
}

void log_print(const char *level, const char *file, int line, const char *fmt, ...) {
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

    // 打印时间、级别、文件和行号
    printf("[%02d:%02d:%02d] [%-5s] %-7s:%-4d ",
           local->tm_hour, local->tm_min, local->tm_sec,
           level, filename, line);

    // 打印用户消息
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}
