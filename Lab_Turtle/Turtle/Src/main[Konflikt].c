/**  
******************************************************************************
* File Name          : main.c
* Description        : Main program body
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  
   //Init the motors and corresponding ports.
  motorInit();
  
  
  /* Create the interpreter */
  FUNC_INTERPRETER intr;
  intr = interpret_command; 
  initUart(intr); //Activate the uart handler
  
  /* Init Uart and give it the interpreter */
  UART_TX uartTx;
  uartTx = uartSend;
  initInterp(uartTx);
  
  /* Send "Connected to TurtleTerminal \n\r> " */
  uint8_t startupmsg[] = "Connected to TurtleTerminal\n\r";
  uartSend(&huart3,startupmsg, strlen((const char*)startupmsg));
  
  
  while (1)
  {
    ;
  }
  
}
