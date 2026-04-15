#ifndef P_H
#define P_H

#include <pthread.h>
#include <unistd.h>
#include "Semaphore.h"

void P(semaphore &S);

#endif // P_H