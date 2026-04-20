#include "V.h"
#include "log.h"
#include "expr.h"

void V_internal(semaphore &S, const char* name){
    // 进入原语
    pthread_mutex_lock(&S.lock);

    // 释放资源
    S.value ++;
    // 在锁内记录日志，保证原子性
    // 使用格式化后的名称，将 "chopstick[i]" 转换为 "chopstick[tid-1]"（实际数值）
    const char* formatted_name = format_sem_name(name);
    LOG_SEM("V操作：%s增加到 %d", formatted_name, S.value);

    if(S.value <= 0){
        // 阻塞队列中有东西
        pthread_cond_signal(&S.cond);
    }

    // 退出原语
    pthread_mutex_unlock(&S.lock);
}