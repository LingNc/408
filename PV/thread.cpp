#include "thread.h"
#include <cstdio>

// 线程局部存储：当前线程ID
static __thread int current_tid = 0;

// 获取当前线程ID（供日志系统使用）
extern "C" int get_current_tid() {
    return current_tid;
}

// 线程包装函数
void* ThreadManager::threadWrapper(void* arg) {
    ThreadInfo* info = (ThreadInfo*)arg;
    current_tid = info->tid;  // 设置线程局部ID（供日志使用）
    for (int i = 0; i < info->repeat; i++) {
        info->func();  // 不传递参数，线程ID从日志获取
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
    info.tid = threads.size() + 1;  // 线程ID从1开始
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