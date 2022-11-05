#ifndef TASK_PID_H
#define TASK_PID_H

#include <time.h>

#include "boiler.h"
#include "pidc.h"
#include "task_common.h"

struct pid_task_data {
  struct gaggit_state *state;
  struct timespec duration;
  pidc_t *pidc;
  boiler_t *boiler;
  int sp;
};

void *pid_task(void *task_data);

#endif // TASK_PID_H
