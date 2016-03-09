#include "stm32f3xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int split(char *str, const char *delim, char ***array, int *length );