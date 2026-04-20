#include "PV.hpp"

semaphore chopstick[5]={1,1,1,1,1};
semaphore mutex=4;

process Pi(){
    work(1,1000,"哲学家%d思考中", tid);
                            LOG_DEBUG("哲学家%d准备进入餐桌，当前还剩%d个位置吃饭", i+1, mutex);
    P(mutex);               LOG_DEBUG("哲学家%d进入餐桌", i+1);
    P(chopstick[i]);        LOG_DEBUG("哲学家%d拿起了左边的筷子%d", i+1, i);
    P(chopstick[(i+1)%5]);  LOG_DEBUG("哲学家%d拿起了右边的筷子%d", i+1, (i+1)%5);
    work(1,1000,"哲学家%d吃饭中", tid);
    V(chopstick[i]);        LOG_DEBUG("哲学家%d放下了左边的筷子%d", i+1, i);
    V(chopstick[(i+1)%5]);  LOG_DEBUG("哲学家%d放下了右边的筷子%d", i+1, (i+1)%5);
    V(mutex);               LOG_DEBUG("哲学家%d离开了餐桌，当前还剩%d个位置吃饭", i+1, mutex);
}

int main(){
    log_init(2);
    work_init(123456);

    ThreadManager tm;
    tm.addThread(Pi, 5, "哲学家", 1);
    tm.runAll();

    return 0;
}