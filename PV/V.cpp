#include "V.h"
#include "log.h"

void V_internal(semaphore &S, const char* name){
    // 进入原语
    pthread_mutex_lock(&S.lock);

    // 释放资源
    S.value ++;
    // 在锁内记录日志，保证原子性
    LOG_SEM("V操作：%s增加到 %d", name, S.value);

    if(S.value <= 0){
        // 阻塞队列中有东西
        pthread_cond_signal(&S.cond);
    }

    // 退出原语
    pthread_mutex_unlock(&S.lock);
}