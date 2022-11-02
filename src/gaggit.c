#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pidc.h"

#define ESTOP_TEMPERATURE 150
#define BOILER_GPIO "14"

int boiler_init();
int boiler_enable(int is_enabled);
int boiler_guard(int tempc) {
  if (tempc > ESTOP_TEMPERATURE) {
    printf("boiler overheating\n");
    boiler_enable(0);
    exit(1);
  }
  return tempc;
}
int mcp9600_read();

int main(int argc, char **argv) {
  int32_t sp, kp, ki, kd;
  pidc_t *pidc;
  int t;

  if (argc != 5) {
    printf("usage: %s sp kp ki kd\n", argv[0]);
    return 1;
  }
  sp = atoi(argv[1]);
  kp = atoi(argv[2]);
  ki = atoi(argv[3]);
  kd = atoi(argv[4]);

  if (boiler_init()) {
    printf("failed to initialize boiler\n");
  }

  t = 0;
  pidc_create(&pidc, kp, ki, kd);
  printf("t,\tpv,\te,\tg\n");
  while (1) {
    int pv = boiler_guard(mcp9600_read());
    int32_t e = sp - pv;
    int32_t gain = pidc_update(pidc, e);
    if (gain > 0)
      boiler_enable(1);
    else
      boiler_enable(0);

    printf("%d,\t%d,\t%d,\t%d,\n", t, pv, e, gain);
    sleep(1);
    ++t;
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

int write_string_to_file(char *filepath, char *str) {
  int fd;
  int ret = 0;
  fd = open(filepath, O_WRONLY);
  if (fd == -1) {
    printf("unable to open %s\n", filepath);
    return 1;
  }
  if (write(fd, str, strlen(str)) != strlen(str)) {
    printf("error writing to %s\n", filepath);
    ret = 1;
  }
  close(fd);
  return ret;
}

int boiler_enable(int is_enabled) {
  char *str = is_enabled ? "1" : "0";
  // printf("setting boiler to %s\n", str);
  if (write_string_to_file("/sys/class/gpio/gpio" BOILER_GPIO "/value", str))
    return 1;
  return 0;
}

int boiler_init() {
  printf("initializing boiler\n");
  write_string_to_file("/sys/class/gpio/export", BOILER_GPIO);
  write_string_to_file("/sys/class/gpio/gpio" BOILER_GPIO "/direction", "out");
  return boiler_enable(0);
}

int mcp9600_read() {
  char str[255];
  read_string_from_file("/sys/bus/iio/devices/iio:device0/in_temp_raw", str);
  return atoi(str) / 16;
}
