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

    // 比较运算符（比较 value）- 声明
    bool operator>(int val) const;
    bool operator<(int val) const;
    bool operator>=(int val) const;
    bool operator<=(int val) const;
    bool operator==(int val) const;
    bool operator!=(int val) const;

    // 友元函数，支持 int > semaphore 语法 - 声明
    friend bool operator>(int val, const semaphore& sem);
    friend bool operator<(int val, const semaphore& sem);
    friend bool operator>=(int val, const semaphore& sem);
    friend bool operator<=(int val, const semaphore& sem);
    friend bool operator==(int val, const semaphore& sem);
    friend bool operator!=(int val, const semaphore& sem);

    // 析构函数
    ~semaphore();
};

#endif // SEMAPHORE_H