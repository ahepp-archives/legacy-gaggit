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
    int cv;
    struct timespec done, to_sleep;
    timespec_acc(&deadline, &data->duration);

    pthread_mutex_lock(&data->state->lock);
    cv = data->state->cv;
    pthread_mutex_unlock(&data->state->lock);

    if (cv == 0) {
      boiler_enable(data->boiler, 0);
    } else {
      boiler_enable(data->boiler, 1);

      /*
       * If over 800/1000, clamp to 1000
       */
      if (cv <= 800) {
        int ns = cv * (NS_PER_S / 1000);
        struct timespec duty_sleep = {.tv_sec = 0, .tv_nsec = ns};
        nanosleep(&duty_sleep, NULL);
        boiler_enable(data->boiler, 0);
      }
    }

    clock_gettime(CLOCK_MONOTONIC, &done);
    timespec_sub(&to_sleep, &deadline, &done);
    nanosleep(&to_sleep, NULL);
  }
  return NULL;
}
