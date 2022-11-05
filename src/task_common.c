#include <time.h>

#include "task_common.h"

void timespec_acc(struct timespec *sum, struct timespec *to_add) {
  sum->tv_nsec += to_add->tv_nsec;
  sum->tv_sec += to_add->tv_sec;
  if (sum->tv_nsec >= NS_PER_S) {
    sum->tv_nsec -= NS_PER_S;
    sum->tv_sec += 1;
  }
}
void timespec_sub(struct timespec *result, struct timespec *minuend,
                  struct timespec *subtrahend) {
  result->tv_sec = minuend->tv_sec - subtrahend->tv_sec;
  result->tv_nsec = minuend->tv_nsec - subtrahend->tv_nsec;
  if (result->tv_nsec < 0) {
    result->tv_nsec += NS_PER_S;
    result->tv_sec -= 1;
  }
}
