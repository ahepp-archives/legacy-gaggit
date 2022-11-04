#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "boiler.h"
#include "boiler_internal.h"

static int write_string_to_file(char *filepath, char *str) {
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

void boiler_init(boiler_t **boiler_ptr, int gpio) {
  char buf[255];
  boiler_t *boiler = (boiler_t *)malloc(sizeof(boiler_t));
  boiler->gpio = gpio;

  sprintf(buf, "%d", gpio);
  write_string_to_file("/sys/class/gpio/export", buf);
  sprintf(buf, "%s%d%s", "/sys/class/gpio/gpio", gpio, "/direction");
  write_string_to_file(buf, "out");
  boiler_enable(boiler, 0);

  *boiler_ptr = boiler;
}

int boiler_enable(boiler_t *const boiler, int is_enabled) {
  char buf[255];
  char *str = is_enabled ? "1" : "0";
  sprintf(buf, "%s%d%s", "/sys/class/gpio/gpio", boiler->gpio, "/value");
  if (write_string_to_file(buf, str))
    return 1;
  return 0;
}
