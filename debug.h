#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdint.h>

char *reg_name(int);
int debug(const char *fmt, ...);

#endif
