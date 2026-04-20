#include "PV.hpp"

semaphore plate = 1, apple = 0, orange = 0;
int apple_count = 0, orange_count = 0;

process father(){
    work(100, 500, "父亲生产苹果");
    P(plate);
    LOG_INFO("父亲放入苹果 %d", ++apple_count);
    V(apple);
}

process mother(){
    work(100, 500, "母亲生产橘子");
    P(plate);
    LOG_INFO("母亲放入橘子 %d", ++orange_count);
    V(orange);
}

process son(){
    P(orange);
    LOG_INFO("儿子取出橘子 %d", orange_count--);
    V(plate);
    work(100, 500, "儿子吃橘子");
}

process daughter(){
    P(apple);
    LOG_INFO("女儿取出苹果 %d", apple_count--);
    V(plate);
    work(100, 500, "女儿吃苹果");
}

int main(){
    log_init(1);
    work_init(123456);

    ThreadManager tm;
    tm.addThread(father, 1, "父亲", 10);
    tm.addThread(mother, 1, "母亲", 10);
    tm.addThread(son, 1, "儿子", 10);
    tm.addThread(daughter, 1, "女儿", 10);

    tm.runAll();
    return 0;
}