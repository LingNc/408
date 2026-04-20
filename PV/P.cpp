#include "P.h"
#include "log.h"
#include "expr.h"

void P_internal(semaphore &S, const char* name){
    pthread_mutex_lock(&S.lock);

    S.value --;
    // 在锁内记录日志，保证原子性
    // 使用格式化后的名称，将 "chopstick[i]" 转换为 "chopstick[tid-1]"（实际数值）
    const char* formatted_name = format_sem_name(name);
    LOG_SEM("P操作：%s减少到 %d", formatted_name, S.value);

    if(S.value < 0){
        // 资源不足，阻塞
        pthread_cond_wait(&S.cond, &S.lock);
    }

    // 释放锁
    pthread_mutex_unlock(&S.lock);
}
