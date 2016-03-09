/**  
******************************************************************************
* File Name          : main.c
* Description        : Main program body
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "string.h"
#include "usart.h"
#include "gpio.h"
#include "init.h"
#include "uart_handler.h"
#include "interpreter.h"


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


int main(void)
{
  
  
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  
  //Create the uart hadler and give it the interpreter
  FUNC_INTERPRETER intr;
  intr = interpret_command; 
  initUart(intr); //Activate the uart handler
  
  //Create the interpreter ardnd give it the uart handler
  UART_TX uartTx;
  uartTx = uartSend;
  initInterp(uartTx);
  
  //Send "Connected to TurtleTerminal \n\r> " to show that tha connection is up
  uint8_t startupmsg[] = "Connected to TurtleTerminal\n\r";
  uartSend(&huart3,startupmsg, strlen((const char*)startupmsg));
  
  /* Set up tx/rx */
  //activateRxIntr();
  
  while (1)
  {
    ;
  }
  
  
}


