#ifndef TASK_COMMON_H
#define TASK_COMMON_H

#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define NS_PER_S 1000000000

struct gaggit_state {
  pthread_mutex_t lock;
  int pv;
  int cv;
  int32_t e;
  int32_t g;
};

void timespec_acc(struct timespec *sum, struct timespec *to_add);
void timespec_sub(struct timespec *result, struct timespec *minuend,
                  struct timespec *subtrahend);

#endif // TASK_COMMON_H
