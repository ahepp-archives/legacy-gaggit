#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pidc.h"
#include "boiler.h"

#define ESTOP_TEMPERATURE 150

int mcp9600_read();

int main(int argc, char **argv) {
  int32_t sp, kp, ki, kd;
  pidc_t *pidc;
  boiler_t *boiler;
  int t;

  if (argc != 5) {
    printf("usage: %s sp kp ki kd\n", argv[0]);
    return 1;
  }
  sp = atoi(argv[1]);
  kp = atoi(argv[2]);
  ki = atoi(argv[3]);
  kd = atoi(argv[4]);

  t = 0;
  pidc_init(&pidc, kp, ki, kd);
  boiler_init(&boiler, 14);
  printf("t,\tpv,\te,\tg\n");
  while (1) {
    int pv = mcp9600_read();
    if (pv > ESTOP_TEMPERATURE) {
      printf("boiler overheating...\n");
      boiler_enable(boiler, 0);
      exit(1);
    }

    int32_t e = sp - pv;
    int32_t gain = pidc_update(pidc, e);

    if (gain > 0)
      boiler_enable(boiler, 1);
    sleep(1);
    boiler_enable(boiler, 0);

    printf("%d,\t%d,\t%d,\t%d,\n", t, pv, e, gain);
    fflush(stdout);
    sleep(4);
    t += 5;
  }

  return 0;
}

int read_string_from_file(char *filepath, char *str) {
  FILE *fp = fopen(filepath, "r");
  if (fp == NULL) {
    printf("unable to open %s\n", filepath);
    return 1;
  }
  fscanf(fp, "%s", str);
  fclose(fp);
  return 0;
}

int mcp9600_read() {
  char str[255];
  read_string_from_file("/sys/bus/iio/devices/iio:device0/in_temp_raw", str);
  return atoi(str) / 16;
}
