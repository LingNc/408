/* ============================================
 * PV.hpp - Combined Single-File Library
 * Generated automatically from source files
 * Dependency order from PV.h
 * ============================================ */

#ifndef PV_HPP
#define PV_HPP

/* ============================================
 * SYSTEM HEADERS
 * ============================================ */
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <vector>
#include <string>

/* ============================================
 * DECLARATIONS (Headers in dependency order)
 * ============================================ */

/* ---------- Semaphore.h ---------- */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H


class semaphore {
public:
    int value;
    pthread_mutex_t lock;
    pthread_cond_t cond;

    // 默认构造函数
    semaphore();

    // 带初始值的构造函数
    semaphore(int val);

    // 拷贝构造函数（禁用，避免复制）
    semaphore(const semaphore& other) = delete;

    // 赋值运算符（支持 sem = 2 语法）
    semaphore& operator=(int val);

    // 析构函数
    ~semaphore();
};

#endif // SEMAPHORE_H
/* ---------- P.h ---------- */
#ifndef P_H
#define P_H


void P(semaphore &S);

#endif // P_H
/* ---------- V.h ---------- */
#ifndef V_H
#define V_H


void V(semaphore &S);

#endif // V_H
/* ---------- log.h ---------- */
#ifndef LOG_H
#define LOG_H


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

/* ---------- thread.h ---------- */
#ifndef THREAD_H
#define THREAD_H


// 线程函数类型：无参数，无返回值
typedef void (*ThreadFunc)();

// 线程信息结构体
struct ThreadInfo {
    ThreadFunc func;
    int repeat;
    std::string name;
};

// 线程管理器类
class ThreadManager {
private:
    std::vector<ThreadInfo> threads;
    std::vector<pthread_t> pthread_ids;

    // 包装函数，用于适配 pthread
    static void* threadWrapper(void* arg);

public:
    ThreadManager();
    ~ThreadManager();

    // 添加线程：函数指针、重复次数、线程名称
    void addThread(ThreadFunc func, int repeat = 1, const char* name = "");

    // 运行所有线程并等待完成
    void runAll();

    // 清空所有线程
    void clear();
};

#endif // THREAD_H
/* ============================================
 * IMPLEMENTATIONS (CPP files in dependency order)
 * ============================================ */

/* ---------- Semaphore.cpp ---------- */
// 默认构造函数
semaphore::semaphore() : value(0) {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
}
// 带初始值的构造函数
semaphore::semaphore(int val) : value(val) {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
}
// 赋值运算符：支持 sem = 2
semaphore& semaphore::operator=(int val) {
    value = val;
    // 注意：不重新初始化锁和条件变量，假设它们已初始化
    return *this;
}
// 析构函数
semaphore::~semaphore() {
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}
/* ---------- P.cpp ---------- */
void P(semaphore &S){
    pthread_mutex_lock(&S.lock);
    S.value --;
    if(S.value < 0){
        // 资源不足，阻塞
        pthread_cond_wait(&S.cond, &S.lock);
    }
    // 释放锁
    pthread_mutex_unlock(&S.lock);
}

/* ---------- V.cpp ---------- */
void V(semaphore &S){
    // 进入原语
    pthread_mutex_lock(&S.lock);
    // 释放资源
    S.value ++;
    if(S.value <= 0){
        // 阻塞队列中有东西
        pthread_cond_signal(&S.cond);
    }
    // 退出原语
    pthread_mutex_unlock(&S.lock);
}
/* ---------- log.cpp ---------- */
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

/* ---------- thread.cpp ---------- */
#include <cstdio>
// 线程包装函数
void* ThreadManager::threadWrapper(void* arg) {
    ThreadInfo* info = (ThreadInfo*)arg;
    for (int i = 0; i < info->repeat; i++) {
        info->func();
    }
    return NULL;
}
ThreadManager::ThreadManager() {}
ThreadManager::~ThreadManager() {
    clear();
}
void ThreadManager::addThread(ThreadFunc func, int repeat, const char* name) {
    ThreadInfo info;
    info.func = func;
    info.repeat = repeat;
    info.name = name ? name : "";
    threads.push_back(info);
}
void ThreadManager::runAll() {
    pthread_ids.clear();
    pthread_ids.resize(threads.size());
    // 创建所有线程
    for (size_t i = 0; i < threads.size(); i++) {
        ThreadInfo* info = &threads[i];
        if (pthread_create(&pthread_ids[i], NULL, threadWrapper, info) != 0) {
            perror("pthread_create failed");
            return;
        }
    }
    // 等待所有线程完成
    for (size_t i = 0; i < pthread_ids.size(); i++) {
        pthread_join(pthread_ids[i], NULL);
    }
}
void ThreadManager::clear() {
    threads.clear();
    pthread_ids.clear();
}
#endif // PV_HPP

/* ============================================
 * END OF PV.hpp
 * ============================================ */
