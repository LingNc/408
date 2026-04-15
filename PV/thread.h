#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>

// 线程函数类型：无参数，无返回值
typedef void (*ThreadFunc)();

// 线程信息结构体（单个线程实例）
struct ThreadInstance {
    ThreadFunc func;
    int repeat;
    std::string name;
    int tid;  // 全局线程ID (从1开始，供日志使用)
};

// 线程管理器类
class ThreadManager {
private:
    std::vector<ThreadInstance> instances;  // 所有线程实例
    std::vector<pthread_t> pthread_ids;
    int next_tid;  // 下一个线程ID

    // 包装函数，用于适配 pthread
    static void* threadWrapper(void* arg);

public:
    ThreadManager();
    ~ThreadManager();

    // 添加多个线程：函数指针、线程数量、线程名称、每个线程重复次数
    void addThread(ThreadFunc func, int count = 1, const char* name = "", int repeat = 1);

    // 运行所有线程并等待完成
    void runAll();

    // 清空所有线程
    void clear();
};

#endif // THREAD_H