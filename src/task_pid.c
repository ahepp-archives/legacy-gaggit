#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pidc.h"
#include "task_common.h"
#include "task_pid.h"

static int read_string_from_file(char *filepath, char *str) {
  FILE *fp = fopen(filepath, "r");
  if (fp == NULL) {
    printf("unable to open %s\n", filepath);
    return 1;
  }
  fscanf(fp, "%s", str);
  fclose(fp);
  return 0;
}

static int mcp9600_read() {
  char str[255];
  read_string_from_file("/sys/bus/iio/devices/iio:device0/in_temp_raw", str);
  return atoi(str);
}

void *pid_task(void *task_data) {
  struct pid_task_data *data = (struct pid_task_data *)task_data;

  struct timespec deadline;
  clock_gettime(CLOCK_MONOTONIC, &deadline);
  while (1) {
    struct timespec done, to_sleep;
    timespec_acc(&deadline, &data->duration);

    int pv = mcp9600_read();
    int32_t e = data->sp - pv;
    int32_t g = pidc_update(data->pidc, e) >> 4;

    pthread_mutex_lock(&data->state->lock);
    data->state->pv = pv;
    data->state->e = e;
    data->state->g = g;
    pthread_mutex_unlock(&data->state->lock);

    clock_gettime(CLOCK_MONOTONIC, &done);
    timespec_sub(&to_sleep, &deadline, &done);
    nanosleep(&to_sleep, NULL);
  }

  return NULL;
}
