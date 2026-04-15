#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>
#include <unistd.h>

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