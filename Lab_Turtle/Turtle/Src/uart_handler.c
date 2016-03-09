#include "uart_handler.h"

bool sending = false; //Trure is tx in progress
uint8_t Buffer[BUFFERSIZE]; //Buffer for the tx/rx 
FUNC_INTERPRETER interpreter; //Interpreter function for command process


/*
Initiate the uart handler by giving it the needed interpreter that is 
called when someting is recived (RxCallback)
*/
void initUart(FUNC_INTERPRETER inter){
  //Set the interpreter to the given one.
  interpreter = inter;
  
  //Not needed due to welcome message activating RxIRQ
  //Activate intr on Rx
  //activateRxIntr();
}


/*
Activats the interrupt on the Rx buffer.
*/
void activateRxIntr(UART_HandleTypeDef *UartHandle){
  //Set pointer to beggining of buffer
  resetRxPointer(UartHandle);
  
  //Activate RxIRQ if not active
  if(UartHandle->State != HAL_UART_STATE_BUSY_RX){
    if(HAL_UART_Receive_IT(&huart3, (uint8_t *)Buffer, BUFFERSIZE) != HAL_OK)
    {
      Error_Handler();
    }
  }
  // Turn led on to to indicate intr is set.
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET); 
}


/*
Sends a given string over uart.
*/
void uartSend(UART_HandleTypeDef *UartHandle, uint8_t *messageToSend, int length){
  
  /* Reset transmission flag */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
  
  /* Start the transmission process */
  if(HAL_UART_Transmit_IT(UartHandle,messageToSend, length)!= HAL_OK)
  {
    Error_Handler();
  }
}


/*
Called when Rx is done.
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  // Turn led off to to indicate intr is reset.
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET); 
  
  //Set pointer to beggining of buffer
  resetRxPointer(UartHandle);
  
  //Interpret the char in the buffer
  interpreter(UartHandle);
  
  //Active intr again.
  activateRxIntr(UartHandle);
  
}


/*
Set the pointer to the beginning of the buffer and the size of the buffer.
*/
void resetRxPointer(UART_HandleTypeDef *UartHandle){
  UartHandle->pRxBuffPtr = Buffer;
  UartHandle->RxXferSize = BUFFERSIZE;
}


/*
When an erro occured during rx/tx.
*/
static void Error_Handler(void)
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); // Blinka PE9
  printf("ERRORO\n\r");
  while(1)
  {
    ;
  } 
}




/*
When Tx is done sending
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
  /* Set transmission flag: transfer complete*/
  sending = false;
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET); // Turn of PE15
  
  //Reactivte and reset the adress.
  activateRxIntr(UartHandle);
}


/*
If an error occurred during tx/rx
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle){ 
  //Stop Rx/Tx and reset Rx intr
  sending = false;
}