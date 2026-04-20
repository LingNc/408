#include "PV.hpp"

int count=0;
semaphore mutex=1;
semaphore rw=1;
int w=0,r=0;


process writer(){
    P(rw);
    w++;
    work(1,100,"写者%d正在写文件", w);
    V(rw);
}

process reader(){
    P(mutex);
    r++;
    if(count==0){
        P(rw);LOG_DEBUG("%d: 第一个读者互斥写者",r);
    }
    count++;
    V(mutex);
    work(1,100,"%d: 读者正在读文件, count=%d", r, count);
    P(mutex);
    count--;
    if(count==0){
        V(rw);LOG_DEBUG("%d: 最后一个读者释放写锁", r);
    }
    V(mutex);
}

// 正常
void run_test_1(){
    ThreadManager tm;
    tm.addThread(reader, 1, "Reader", 5);
    tm.addThread(writer, 1, "Writer", 5);
    tm.runAll();
}

// 写者饥饿
void run_test_2(){
    ThreadManager tm;
    tm.addThread(reader, 1, "Reader", 20);
    tm.addThread(writer, 1, "Writer", 1);
    tm.runAll();
}

int main(){
    log_init(2);
    // LOG_INFO("运行测试1：正常读写");
    // run_test_1();
    // LOG_INFO("运行测试2：写者饥饿");
    // run_test_2();
    ThreadManager tm;
    tm.addThread(reader, 3, "Reader", 1);
    tm.addThread(writer, 1, "Writer", 1);
    tm.addThread(reader, 10, "Reader", 1);
    tm.runAll();

    return 0;
}