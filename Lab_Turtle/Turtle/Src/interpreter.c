#include "interpreter.h"

UART_TX send; //Uart send function
int commandLength = 0;
uint8_t command[COMMANDLENGTH] = {0};

//Array with all the availibale command.
char const *commands[] = {"straight", "rotate", "penup", "pendown"};

/*
Initiate the interpreter by givig it the uart send funktion.
*/
void initInterp(UART_TX uartSend){
  send = uartSend;
}

/*
Process the text in the commands[] array and start the correpsonding 
function for eather the motors or the pwm
*/
char *processCommand(){
  
  //Delay so respons is not sent to fast for the wireles uart.
  HAL_Delay(100);
  
  //Split the string into more strings evry space:
  char **splitCommand = NULL;
  int numberOfArrays =0;
  
  //Split on " "
  split((char*)command, " ", &splitCommand, &numberOfArrays);
 
  //Start with a non exixting command number.
  int8_t commandNr = -1; 
 
  //Compare input to all availibale commands
  if(strlen((char*)splitCommand[0]) > 0){
    for(int i = 0 ; i < 4;i++){
      if(strcmp(((const char*)splitCommand[0]), commands[i]) == 0){
        commandNr = i;
        break;
      }
    }
  }
  
  //Find what command was given buy the result of find command.
  switch(commandNr){
    
  case STRAIGHT:
    //Check that a value was given.
    if(numberOfArrays == 2){
      straight(atoi(splitCommand[1]));
      return "straight executed";
    }
    return "straight not executed, need value";
    break;
    
  case ROTATE:
    //Check that a value was given.
    if(numberOfArrays == 2){
      rotate(atoi(splitCommand[1]));
      return "rotation executed";
    }
    return "rotation not executed, need value";
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
  
  
  //The inup was incorrect.
  return "not a command";
  
}

void interpret_command(UART_HandleTypeDef *UartHandle){
  //Create buffer for the char.
  uint8_t buffer[BUFFERLENGTH] = {0};
  
  //If new line
  if((*UartHandle->pRxBuffPtr) == 13 || (*UartHandle->pRxBuffPtr) == 0x0A){
    
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


/*
Reset the command array and length of the command.
*/
void resetCommand(){
  for(int i = 0; i < commandLength; i++){
    command[i] = 0;
  }
  commandLength = 0;
}

