#include "PV.hpp"

int n=5;

semaphore mutex = 1;
semaphore empty = n;
semaphore full = 0;

int a=0,b=0,c=0;

process producer(){
    a++;
    work(1,1,"[生产者] 生产 %d", a);
    P(empty);
    P(mutex);
    // a--;
    b++;
    work(1,1,"[生产者] 放入缓冲区，当前缓冲区=%d", b);
    V(mutex);
    V(full);
}

process consumer(){
    LOG_DEBUG("[消费者] 获取资源");
    P(full);
    P(mutex);
    b--;
    work(1,1,"[消费者] 从缓冲区取出，当前缓冲区=%d", b);
    V(mutex);
    V(empty);
    c++;
    LOG_INFO("[消费者] 消费 %d", c);
}

int main(){
    log_init(1);

    ThreadManager tm;
    // addThread(函数, 线程数量, 名称, 每个线程重复次数)
    tm.addThread(producer, 20, "生产者",5);  // 5个生产者线程，每个执行2次
    tm.addThread(consumer, 1, "消费者",100);  // 5个消费者线程，每个执行2次

    tm.runAll();
    return 0;
}