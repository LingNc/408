#include "thread.h"
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