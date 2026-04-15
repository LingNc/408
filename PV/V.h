#ifndef V_H
#define V_H

#include <pthread.h>
#include <unistd.h>
#include "Semaphore.h"

// V操作内部实现（带信号量名称用于日志）
void V_internal(semaphore &S, const char* name);

// V操作宏（自动传递变量名）
#define V(sem) V_internal(sem, #sem)

#endif // V_H