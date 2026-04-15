#include "V.h"
#include "log.h"

void V(semaphore &S){
    // 进入原语
    pthread_mutex_lock(&S.lock);

    // 释放资源
    S.value ++;
    if(S.value <= 0){
        // 阻塞队列中有东西
        pthread_cond_signal(&S.cond);
    }

    // 退出原语
    pthread_mutex_unlock(&S.lock);
}