#include "uart_handler.h"
#include "string.h"
#include "stddef.h"
#include "stdbool.h"
#include "tim.h"
#include "motor.h"
#include "split.h"

#define COMMANDLENGTH 200
#define BUFFERLENGTH 200

#define STRAIGHT        0
#define ROTATE          1
#define PENUP           2
#define PENDOWN         3

//typedef for function
typedef void (*UART_TX)(UART_HandleTypeDef *UartHandle, uint8_t *message, int length);

void HAL_Delay(__IO uint32_t Delay);
char *processCommand();
void interpret_command(UART_HandleTypeDef *UartHandle);
void initInterp(UART_TX uartSend);
void resetCommand();