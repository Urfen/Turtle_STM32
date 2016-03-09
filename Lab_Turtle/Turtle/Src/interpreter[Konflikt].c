 #include "interpreter.h"

UART_TX send;
int commandLength = 0;
uint8_t command[COMMANDLENGTH] = {0};

char const *commands[] = {"straight", "rotate", "penup", "pendown"};

void initInterp(UART_TX uartSend){
  send = uartSend;
}

char *processCommand(){
  
  HAL_Delay(100);
  
  //split the string into more strings:
  char **arr = NULL;
  int count =0;
  
  split((char*)command, " ", &arr, &count);
  
  
  //ITERATE THROUGH AVAILABLE COMMANDS:
  int8_t commandNr = -1;
  
  if(strlen((char*)arr[0]) > 0){
    for(int i = 0 ; i < 4;i++){
      if(strcmp(((const char*)arr[0]), commands[i]) == 0){
        commandNr = i;
        break;
      }
    }
  }

  
  //Execute the function for the  commandNr
  if(commandNr >= 0){
    switch(commandNr){
    case STRAIGHT:
      if(count == 2){
        straight(atoi(arr[1]));
      }
      return "straight executed";
      break;
    case ROTATE:
      if(count == 2){
        rotate(atoi(arr[1]));
      }
      return "rotation executed";
      break;
    case PENUP:
      penPulse(0);
      return "penup executed";
      break;
    case PENDOWN:
      penPulse(1);
      return "pendown executed";
      break;
    
    }
  }
  
  //Else no a command.
  return "not a command";
  
}

void interpret_command(UART_HandleTypeDef *UartHandle){
  uint8_t buffer[BUFFERLENGTH] = {0};
  
  //If new line
  if((*UartHandle->pRxBuffPtr) == 13 || (*UartHandle->pRxBuffPtr) == 0x0A){
    
    //Add a new line to the returning message.
    //strcat((char*)buffer,"\r\n  ");
    
    //Process command and add the result to the returning message.
    strcat((char*)buffer,processCommand());
    
    //Add ">" to the returning message.
    strcat((char*)buffer,"\n\r");
    
    //Reset the command.
    resetCommand();
    
    //Send char back if something was sent
    send(UartHandle,buffer, strlen((const char*)buffer));
  }
  
  else{
    //Fill the buffer with the legal char.
    buffer[0] = (*UartHandle->pRxBuffPtr);
    buffer[1] = '\0';
    
    //Add the char to the command for process.
    command[commandLength++] = buffer[0];
  }
  
}

void resetCommand(){
  for(int i = 0; i < commandLength; i++){
    command[i] = 0;
  }
  commandLength = 0;
}

