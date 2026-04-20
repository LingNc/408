#include "PV.hpp"

semaphore a12 = 0, a13 = 0, a24 = 0, a25 = 0, a36 = 0, a46 = 0, a56 = 0;

process S1(){
    work(100, 500, "S1执行中");
    LOG_INFO("S1完成");
    V(a12); V(a13);
}

process S2(){
    P(a12);
    LOG_DEBUG("S2: S1已完成");
    work(100, 500, "S2执行中");
    LOG_INFO("S2完成");
    V(a24); V(a25);
}

process S3(){
    P(a13);
    LOG_DEBUG("S3: S1已完成");
    work(100, 500, "S3执行中");
    LOG_INFO("S3完成");
    V(a36);
}

process S4(){
    P(a24);
    LOG_DEBUG("S4: S2已完成");
    work(100, 500, "S4执行中");
    LOG_INFO("S4完成");
    V(a46);
}

process S5(){
    P(a25);
    LOG_DEBUG("S5: S2已完成");
    work(100, 500, "S5执行中");
    LOG_INFO("S5完成");
    V(a56);
}

process S6(){
    P(a36);
    P(a46);
    P(a56);
    LOG_DEBUG("S6: S3、S4、S5已完成");
    work(100, 500, "S6执行中");
    LOG_INFO("S6完成");
}

int main(){
    log_init(2);
    work_init(123456);

    ThreadManager tm;
    tm.addThread(S1, 1, "S1", 1);
    tm.addThread(S2, 1, "S2", 1);
    tm.addThread(S3, 1, "S3", 1);
    tm.addThread(S4, 1, "S4", 1);
    tm.addThread(S5, 1, "S5", 1);
    tm.addThread(S6, 1, "S6", 1);
    tm.runAll();

    return 0;
}