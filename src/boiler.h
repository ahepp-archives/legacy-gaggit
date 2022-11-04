#ifndef BOILER_H
#define BOILER_H

typedef struct boiler_t boiler_t;

void boiler_init(boiler_t **boiler_ptr, int gpio);
int boiler_enable(boiler_t *const boiler, int is_enabled);

#endif // BOILER_H
