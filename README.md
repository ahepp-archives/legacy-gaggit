# Gaggit

Gaggit is a replacement temperature controller for the Gaggia Classic Pro espresso machine.

The project is currently targeted at a Raspberry Pi Zero W with [my mcp9600 IIO kernel module](https://github.com/ahepp/iio-mcp9600) installed.
Portability is expected to improve in the future, but if you are interested in trying the program out you may need to adjust the boiler GPIO and sysfs path for the mcp9600.

## Quickstart

All parameters are currently 16 bit fixed points, with 4 fractional bits.
In simple words, this means all values are 16ths.

The program takes a setpoint, and three gains for the internal [PID controller](https://github.com/ahepp/pidc).

The update rates of the boiler, controller, and logger are decoupled. Diagnostic information includes the time, the setpoint, error, and gain.

```
$ mkdir gaggit/build && cd gaggit/build
$ cmake .. -DGAGGIT_WITH_SUBMODULES=1
$ make
$ ./src/gaggit 
usage: ./src/gaggit sp kp ki kd
$ sudo ./src/gaggit 1632 16 0 0 | tee log.csv
t,      pv,     e,      g
1.595,  0,      0,      0
2.595,  373,    1259,   1259
3.595,  373,    1259,   1259
4.595,  373,    1259,   1259
5.595,  373,    1259,   1259
```
