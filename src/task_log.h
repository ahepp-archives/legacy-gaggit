#ifndef TASK_LOG_H
#define TASK_LOG_H

#include <time.h>

#include "task_common.h"

struct log_task_data {
  struct gaggit_state *state;
  struct timespec duration;
};

void *log_task(void *task_data);

#endif // TASK_LOG_H
