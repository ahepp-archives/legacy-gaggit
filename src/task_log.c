#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "task_common.h"
#include "task_log.h"

void *log_task(void *task_data) {
  struct log_task_data *data = (struct log_task_data *)task_data;

  printf("t,\tpv,\te,\tg\n");
  struct timespec deadline;
  int s0 = 0;

  clock_gettime(CLOCK_MONOTONIC, &deadline);
  s0 = deadline.tv_sec;
  while (1) {
    int s, ms, pv;
    int32_t e, g;
    struct timespec done, to_sleep;
    timespec_acc(&deadline, &data->duration);

    pthread_mutex_lock(&data->state->lock);
    pv = data->state->pv;
    e = data->state->e;
    g = data->state->g;
    pthread_mutex_unlock(&data->state->lock);

    s = deadline.tv_sec - s0;
    ms = deadline.tv_nsec / (NS_PER_S / 1000);
    printf("%d.%03d,\t%d,\t%d,\t%d\n", s, ms, pv, e, g);
    if (s % 5)
      fflush(stdout);

    clock_gettime(CLOCK_MONOTONIC, &done);
    timespec_sub(&to_sleep, &deadline, &done);
    nanosleep(&to_sleep, NULL);
  }
  return NULL;
}
