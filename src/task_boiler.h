#ifndef TASK_BOILER_H
#define TASK_BOILER_H

#include <time.h>

#include "boiler.h"
#include "task_common.h"

struct boiler_task_data {
  struct gaggit_state *state;
  struct timespec duration;
  boiler_t *boiler;
};

void *boiler_task(void *task_data);

#endif // TASK_BOILER_H
