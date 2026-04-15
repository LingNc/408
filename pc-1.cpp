#include "PV.hpp"

semaphore plate = 1,apple =0, orange=0;

int o = 0, a = 0;

void fa(){
    P(plate);LOG_INFO("[父亲] 等待盘子...");
    V(apple);LOG_INFO("[父亲] 生产了一个苹果");
    a++;
}

void ma(){
    P(plate);LOG_INFO("[母亲] 等待盘子...");
    V(orange);LOG_INFO("[母亲] 生产了一个橘子");
    LOG_INFO("[母亲] 放入橘子 %d", o);
    o++;
}

void son(){
    LOG_INFO("[儿子] 等待橘子...");
    P(orange);
    V(plate);LOG_INFO("[儿子] 取出橘子 %d", o);
}

void daughter(){
    LOG_INFO("[女儿] 等待苹果...");
    P(apple);
    V(plate);LOG_INFO("[女儿] 取出苹果 %d", a);
}

int main(){
    log_init(1);

    ThreadManager tm;
    tm.addThread(fa, 3, "父亲", 10);
    tm.addThread(ma, 5, "母亲", 3);
    tm.addThread(son, 5, "儿子", 3);
    tm.addThread(daughter, 3, "女儿", 10);

    tm.runAll();
    return 0;
}