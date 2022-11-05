#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "boiler.h"
#include "pidc.h"
#include "task_boiler.h"
#include "task_common.h"
#include "task_log.h"
#include "task_pid.h"

#define ESTOP_TEMPERATURE 2400

int main(int argc, char **argv) {
  int32_t sp, kp, ki, kd;

  if (argc != 5) {
    printf("usage: %s sp kp ki kd\n", argv[0]);
    return 1;
  }
  sp = atoi(argv[1]);
  kp = atoi(argv[2]);
  ki = atoi(argv[3]);
  kd = atoi(argv[4]);

  struct gaggit_state shared_state = {0};
  pthread_mutex_init(&shared_state.lock, NULL);

  struct boiler_task_data boiler_data = {
      .state = &shared_state, .duration = {.tv_sec = 1, .tv_nsec = 0}};
  boiler_init(&boiler_data.boiler, 14);

  struct pid_task_data pid_data = {.state = &shared_state,
                                   .sp = sp,
                                   .duration = {.tv_sec = 1, .tv_nsec = 0}};
  pidc_init(&pid_data.pidc, kp, ki, kd);

  struct log_task_data log_data = {.state = &shared_state,
                                   .duration = {.tv_sec = 1, .tv_nsec = 0}};

  pthread_t log_tid, pid_tid, boiler_tid;
  pthread_create(&boiler_tid, NULL, &boiler_task, (void *)&boiler_data);
  pthread_create(&log_tid, NULL, &log_task, (void *)&log_data);
  pthread_create(&pid_tid, NULL, &pid_task, (void *)&pid_data);

  pthread_join(boiler_tid, NULL);
  pthread_join(log_tid, NULL);
  pthread_join(pid_tid, NULL);
  return 0;
}
