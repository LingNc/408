#ifndef P_H
#define P_H

#include <pthread.h>
#include <unistd.h>
#include "Semaphore.h"

// P操作内部实现（带信号量名称用于日志）
void P_internal(semaphore &S, const char* name);

// P操作宏（自动传递变量名）
#define P(sem) P_internal(sem, #sem)

#endif // P_H