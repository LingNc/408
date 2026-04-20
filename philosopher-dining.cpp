#include "PV.hpp"
semaphore chopstick[5]={1,1,1,1,1};

void Pi(){
    work(1,2000,"哲学家%d思考中", tid);
    P(chopstick[i]);            LOG_DEBUG("哲学家%d拿起了左边的筷子%d", i+1, i);
    P(chopstick[(i+1)%5]);      LOG_DEBUG("哲学家%d拿起了右边的筷子%d", i+1, (i+1)%5);
    work(1,2000,"哲学家%d吃饭中", tid);
    V(chopstick[i]);            LOG_DEBUG("哲学家%d放下了左边的筷子%d", i+1, i);
    V(chopstick[(i+1)%5]);      LOG_DEBUG("哲学家%d放下了右边的筷子%d", i+1, (i+1)%5);
}

int main(){
    log_init(2);

    ThreadManager tm;
    tm.addThread(Pi, 5, "Philosopher", 10);
    tm.runAll();

    return 0;
}