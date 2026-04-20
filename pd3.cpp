#include "PV.hpp"

semaphore chopstick[5]={1,1,1,1,1};
semaphore mutex=1;

process Pi(){
    work(1,1000,"哲学家%d思考中", tid);
    P(mutex);               LOG_DEBUG("哲学家%d准备进入餐桌", i+1);
    P(chopstick[i]);        LOG_INFO("哲学家%d拿起了左边的筷子%d", i+1, i);
    P(chopstick[(i+1)%5]);  LOG_INFO("哲学家%d拿起了右边的筷子%d", i+1, (i+1)%5);
    V(mutex);               LOG_DEBUG("哲学家%d进入餐桌", i+1);
    work(1,1000,"哲学家%d吃饭中", tid);
    V(chopstick[i]);
    V(chopstick[(i+1)%5]);
}

int main(){
    log_init(2);
    work_init(123456);

    ThreadManager tm;
    tm.addThread(Pi, 5, "哲学家", 1);
    tm.runAll();

    return 0;
}