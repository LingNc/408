#include "log.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <pthread.h>

// 全局日志级别变量
LogLevel g_log_level = LOG_LEVEL_INFO;
bool g_log_show_file = false;  // 默认不显示文件名

// 线程名字对齐相关
static int g_max_thread_name_len = 0;  // 0表示自动检测
static int g_current_max_name_width = 0;  // 当前遇到的最大名字宽度

// 文件名和行号对齐相关
static int g_current_max_filename_width = 0;  // 当前遇到的最大文件名宽度
static int g_current_max_linenum_width = 0;   // 当前遇到的最大行号位数

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

// 计算字符串显示宽度（中文字符算2个宽度）
int str_width(const char* str) {
    int width = 0;
    const unsigned char* p = (const unsigned char*)str;
    while (*p) {
        if (*p < 0x80) {
            width++;  // ASCII字符
            p++;
        } else {
            width += 2;  // 中文字符（UTF-8多字节）
            // UTF-8: 0xC0-0xDF开头是2字节，0xE0-0xEF是3字节，0xF0-0xF7是4字节
            if (*p >= 0xC0 && *p <= 0xDF) p += 2;
            else if (*p >= 0xE0 && *p <= 0xEF) p += 3;
            else if (*p >= 0xF0 && *p <= 0xF7) p += 4;
            else p++;  // 非法UTF-8，跳过
        }
    }
    return width;
}

// 设置最大线程名字长度
void log_set_max_name_len(int len) {
    pthread_mutex_lock(&log_mutex);
    g_max_thread_name_len = len;
    pthread_mutex_unlock(&log_mutex);
}

void log_init(int level, bool show_file) {
    // 首先初始化锁（线程安全，只执行一次）
    pthread_once(&log_mutex_once, init_log_mutex_once);

    g_log_show_file = show_file;

    // level: 0=INFO, 1=DEBUG, 2=SEM
    if (level >= 2) {
        g_log_level = LOG_LEVEL_SEM;
        printf("日志系统初始化完成，级别: SEM (信号量操作)%s\n", show_file ? ", 显示文件名" : "");
    } else if (level >= 1) {
        g_log_level = LOG_LEVEL_DEBUG;
        printf("日志系统初始化完成，级别: DEBUG%s\n", show_file ? ", 显示文件名" : "");
    } else {
        g_log_level = LOG_LEVEL_INFO;
        printf("日志系统初始化完成，级别: INFO%s\n", show_file ? ", 显示文件名" : "");
    }
}

void log_print(const char *level, const char *file, int line, int tid, const char *name, const char *fmt, ...) {
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

    // 动态检测并更新最大名字宽度
    int name_width = str_width(name && name[0] ? name : "");
    if (name_width > g_current_max_name_width) {
        g_current_max_name_width = name_width;
    }

    // 动态检测并更新最大文件名宽度和行号宽度
    int filename_width = str_width(filename);
    if (filename_width > g_current_max_filename_width) {
        g_current_max_filename_width = filename_width;
    }
    // 计算行号的位数
    int line_num = line;
    int linenum_width = 0;
    do {
        linenum_width++;
        line_num /= 10;
    } while (line_num > 0);
    if (linenum_width > g_current_max_linenum_width) {
        g_current_max_linenum_width = linenum_width;
    }

    // 确定用于对齐的最大宽度（用户设置或自动检测）
    int max_width = (g_max_thread_name_len > 0) ? g_max_thread_name_len : g_current_max_name_width;

    // 计算需要填充的空格数
    int padding = max_width - name_width;
    if (padding < 0) padding = 0;

    // 打印时间
    printf("[%02d:%02d:%02d] [%-5s] ", local->tm_hour, local->tm_min, local->tm_sec, level);

    // 根据开关决定是否显示文件名（动态对齐：文件名左对齐，行号右对齐）
    if (g_log_show_file) {
        printf("%-*s:%*d ", g_current_max_filename_width, filename, g_current_max_linenum_width, line);
    }

    // 打印线程ID和名字（对齐）
    if (name && name[0]) {
        printf("[T%02d:%s]%*s ", tid, name, padding, "");
    } else {
        printf("[T%02d]%*s ", tid, padding + 1, "");  // +1 是因为没有冒号和名字
    }

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
