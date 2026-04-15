#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "Semaphore.h"
#include "P.h"
#include "V.h"
#include "log.h"
#include "thread.h"

semaphore mutex = 1;   // 互斥信号量
semaphore empty = 5;   // 空闲缓冲区数量
semaphore full = 0;    // 满缓冲区数量
int buffer = 0;        // 共享缓冲区

// 生产者线程函数
void producer() {
    P(empty);   // 等待空闲缓冲区
    P(mutex);   // 进入临界区

    buffer++;
    LOG_INFO("生产者生产了一个物品，当前缓冲区: %d", buffer);

    V(mutex);   // 离开临界区
    V(full);    // 增加满缓冲区数量
}

// 消费者线程函数
void consumer() {
    P(full);    // 等待有物品
    P(mutex);   // 进入临界区

    buffer--;
    LOG_INFO("消费者消费了一个物品，当前缓冲区: %d", buffer);

    V(mutex);   // 离开临界区
    V(empty);   // 增加空闲缓冲区数量
}

int main() {
    // 配置是否开启 debug 日志：1 开启，0 关闭
    log_init(1);

    LOG_INFO("====== 生产者-消费者问题演示 ======");
    LOG_INFO("缓冲区大小: 5，初始为空");

    ThreadManager tm;

    // 添加线程：函数、重复次数、名称
    tm.addThread(producer, 3, "生产者");  // 生产者执行 3 次
    tm.addThread(consumer, 3, "消费者");  // 消费者执行 3 次

    LOG_INFO("启动所有线程...");
    tm.runAll();

    LOG_INFO("====== 所有线程执行完毕 ======");
    LOG_INFO("最终缓冲区物品数: %d", buffer);

    return 0;
}
