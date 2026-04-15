#include "P.h"
#include "log.h"

void P_internal(semaphore &S, const char* name){
    pthread_mutex_lock(&S.lock);

    S.value --;
    // 在锁内记录日志，保证原子性
    LOG_SEM("P操作：%s减少到 %d", name, S.value);

    if(S.value < 0){
        // 资源不足，阻塞
        pthread_cond_wait(&S.cond, &S.lock);
    }

    // 释放锁
    pthread_mutex_unlock(&S.lock);
}
