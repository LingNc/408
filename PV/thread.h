#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>

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