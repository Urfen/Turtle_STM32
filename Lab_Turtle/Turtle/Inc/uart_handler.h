#include "stm32f3xx_hal.h"
#include "usart.h"
#include "string.h"
#include "stdbool.h"


/* Size of Transmission buffer */
#define BUFFERSIZE 1

//Typedef for function
typedef void (*FUNC_INTERPRETER)(UART_HandleTypeDef *);

static void Error_Handler(void);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
void sendBufferTx();
void activateRxIntr();
void uartSend(UART_HandleTypeDef *UartHandle, uint8_t *result, int length);
void initUart(FUNC_INTERPRETER inter);
void resetRxPointer(UART_HandleTypeDef *UartHandle);

