#include "Semaphore.h"

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

// 类型转换运算符：支持 printf("%d", sem) 等用法
semaphore::operator int() const {
    return value;
}

// 析构函数
semaphore::~semaphore() {
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}

// 比较运算符实现（semaphore > int）
bool semaphore::operator>(int val) const { return value > val; }
bool semaphore::operator<(int val) const { return value < val; }
bool semaphore::operator>=(int val) const { return value >= val; }
bool semaphore::operator<=(int val) const { return value <= val; }
bool semaphore::operator==(int val) const { return value == val; }
bool semaphore::operator!=(int val) const { return value != val; }

// 友元函数实现（int > semaphore）
bool operator>(int val, const semaphore& sem) { return val > sem.value; }
bool operator<(int val, const semaphore& sem) { return val < sem.value; }
bool operator>=(int val, const semaphore& sem) { return val >= sem.value; }
bool operator<=(int val, const semaphore& sem) { return val <= sem.value; }
bool operator==(int val, const semaphore& sem) { return val == sem.value; }
bool operator!=(int val, const semaphore& sem) { return val != sem.value; }