#include "P.h"

void P(semaphore &S){
    pthread_mutex_lock(&S.lock);

    S.value --;
    if(S.value < 0){
        // 资源不足，阻塞
        pthread_cond_wait(&S.cond, &S.lock);
    }

    // 释放锁
    pthread_mutex_unlock(&S.lock);
}
