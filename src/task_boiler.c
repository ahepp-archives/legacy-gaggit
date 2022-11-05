#include <pthread.h>
#include <time.h>

#include "boiler.h"
#include "task_boiler.h"
#include "task_common.h"

void *boiler_task(void *task_data) {
  struct boiler_task_data *data = (struct boiler_task_data *)task_data;
  struct timespec deadline;

  clock_gettime(CLOCK_MONOTONIC, &deadline);
  while (1) {
    int g;
    struct timespec done, to_sleep;
    timespec_acc(&deadline, &data->duration);

    pthread_mutex_lock(&data->state->lock);
    g = data->state->g;
    pthread_mutex_unlock(&data->state->lock);

    if (g > 0)
      boiler_enable(data->boiler, 1);
    else
      boiler_enable(data->boiler, 0);

    clock_gettime(CLOCK_MONOTONIC, &done);
    timespec_sub(&to_sleep, &deadline, &done);
    nanosleep(&to_sleep, NULL);
  }
  return NULL;
}
