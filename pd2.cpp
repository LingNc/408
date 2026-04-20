#include "PV.hpp"

semaphore chopstick[5]={1,1,1,1,1};

process Pi(){
    work(1,1000,"哲学家%d思考中", tid);
    if(i%2){
        P(chopstick[i]);
        P(chopstick[(i+1)%5]);
    }
    else{
        P(chopstick[(i+1)%5]);
        P(chopstick[i]);
    }
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