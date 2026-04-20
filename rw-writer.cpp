#include "PV.hpp"
#include <time.h>

semaphore rw = 1, mutex = 1, w = 1;
int count=0;

process writer(){
    P(w);
    LOG_DEBUG("写者进入文件准备阶段");
    P(rw);
    work(1,1000,"写者正在写文件");
    V(rw);
    V(w);
}

process reader(){
    P(w);
    LOG_DEBUG("读者进入文件准备阶段");
    P(mutex);
    if(count==0){
        P(rw);LOG_DEBUG("第一个读者互斥写者");
    }
    count++;
    V(mutex);
    V(w);
    work(1,1000,"读者正在读文件, count=%d", count);
    P(mutex);
    count--;
    if(count==0){
        V(rw);LOG_DEBUG("最后一个读者释放写锁");
    }
    V(mutex);
}

int main(){
    log_init(2);

    ThreadManager tm;
    tm.addThread(reader, 1, "Reader", 10);
    tm.addThread(writer, 1, "Writer", 2);
    tm.runAll();

    return 0;
}