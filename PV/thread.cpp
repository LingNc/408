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
    ThreadInstance* inst = (ThreadInstance*)arg;
    current_tid = inst->tid;  // 设置线程局部ID（供日志使用）
    for (int i = 0; i < inst->repeat; i++) {
        inst->func();  // 不传递参数，线程ID从日志获取
    }
    return NULL;
}

ThreadManager::ThreadManager() : next_tid(1) {}

ThreadManager::~ThreadManager() {
    clear();
}

void ThreadManager::addThread(ThreadFunc func, int count, const char* name, int repeat) {
    // 创建 count 个线程实例
    for (int i = 0; i < count; i++) {
        ThreadInstance inst;
        inst.func = func;
        inst.repeat = repeat;  // 每个线程重复执行 repeat 次
        inst.name = name ? name : "";
        inst.tid = next_tid++;  // 全局线程ID从1开始递增
        instances.push_back(inst);
    }
}

void ThreadManager::runAll() {
    pthread_ids.clear();
    pthread_ids.resize(instances.size());

    // 创建所有线程
    for (size_t i = 0; i < instances.size(); i++) {
        ThreadInstance* inst = &instances[i];
        if (pthread_create(&pthread_ids[i], NULL, threadWrapper, inst) != 0) {
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
    instances.clear();
    pthread_ids.clear();
    next_tid = 1;
}