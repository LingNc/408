#include "PV.hpp"

int n=5;

semaphore mutex = 1;
semaphore empty = n;
semaphore full = 0;

int a=0,b=0,c=0;

void producer(){
    a++;
    LOG_INFO("生产 %d",a);
    P(empty);
    P(mutex);
    a--;
    b++;
    LOG_INFO("放入缓冲区 %d",b);
    V(mutex);
    V(full);
}

void consumer(){
    P(full);
    P(mutex);
    b--;
    LOG_INFO("取出 %d",b);
    V(mutex);
    V(empty);
    c++;
    LOG_INFO("消费 %d",c);
}

int main(){
    log_init(1);

    ThreadManager tm;
    tm.addThread(producer,10,"生产者");
    tm.addThread(consumer,10,"消费者");

    tm.runAll();
    return 0;
}