#include "motor.h"


driverCard_struct dCard;

//Test variables for liveView
uint16_t forward_temp, rotate_temp;

/*
Creates the three structs for the card and the two motors.
And filles them with the correct ports and pins from the info from 
CubeMX
*/
void motorInit(){

  //Motor 1 Left
  dCard.MOTOR_1.DIR_Pin = DIR1_Pin;
  dCard.MOTOR_1.DIR_Port = DIR1_GPIO_Port; 
  dCard.MOTOR_1.STEP_Pin = STEP1_Pin;
  dCard.MOTOR_1.STEP_Port = STEP1_GPIO_Port;
  
  //Motor 1 Right
  dCard.MOTOR_2.DIR_Pin = DIR2_Pin;
  dCard.MOTOR_2.DIR_Port = DIR2_GPIO_Port;
  dCard.MOTOR_2.STEP_Pin = STEP2_Pin;
  dCard.MOTOR_2.STEP_Port = STEP2_GPIO_Port;
  
  //DriverCard settings
  dCard.ENABLE_Pin = Enable_Pin;
  dCard.ENABLE_Port = Enable_GPIO_Port;
  dCard.MS_1_Pin = MS1_Pin;
  dCard.MS_1_Port = MS1_GPIO_Port;
  dCard.MS_2_Pin = MS2_Pin;
  dCard.MS_2_Port = MS2_GPIO_Port;
  
  //Set step size to smallest possible (1/8).
  HAL_GPIO_WritePin(dCard.MS_1_Port, dCard.MS_1_Pin, HIGH);
  HAL_GPIO_WritePin(dCard.MS_2_Port, dCard.MS_2_Pin, HIGH);
  
  //Disable power to the DriverCard.
  HAL_GPIO_WritePin(dCard.ENABLE_Port, dCard.ENABLE_Pin, HIGH);
}

/*
Calculate the number and dirction of steps for the given amount of degrees 
to rotate.
*/
void rotate(int16_t rotation){
  //Set the correct direction on the motors
  if(rotation > 0){ //Right
    HAL_GPIO_WritePin(dCard.MOTOR_1.DIR_Port, dCard.MOTOR_1.DIR_Pin, FORWARD);
    HAL_GPIO_WritePin(dCard.MOTOR_2.DIR_Port, dCard.MOTOR_2.DIR_Pin, BACKWARD);
  }

  if(rotation < 0){  //Left
    HAL_GPIO_WritePin(dCard.MOTOR_1.DIR_Port, dCard.MOTOR_1.DIR_Pin, BACKWARD);
    HAL_GPIO_WritePin(dCard.MOTOR_2.DIR_Port, dCard.MOTOR_2.DIR_Pin, FORWARD);
    rotation = rotation * (-1); //Make rotation positive.
  }

  //Calculate how many small steps to take.
  double numberOfSteps = (DIAMETER2/(DIAMETER1*DEGREE));
  numberOfSteps *= rotation;
  //Round of to nearest amont of steps.
  uint16_t wholeNumberOfSteps = (int) round(numberOfSteps);
  rotate_temp = wholeNumberOfSteps;
  //Step the resulting amount of steps.
  stepMotors(wholeNumberOfSteps);
}

/*
Calculate the number and dirction of steps for the given amount of millimteters 
to move in a direction.
*/
void straight(int16_t distance){

  //Set the correct direction on the motors
  if(distance > 0){  //Forward
    HAL_GPIO_WritePin(dCard.MOTOR_1.DIR_Port, dCard.MOTOR_1.DIR_Pin, FORWARD);
    HAL_GPIO_WritePin(dCard.MOTOR_2.DIR_Port, dCard.MOTOR_2.DIR_Pin, FORWARD);
  }

  if(distance < 0){ //Backward
    HAL_GPIO_WritePin(dCard.MOTOR_1.DIR_Port, dCard.MOTOR_1.DIR_Pin, BACKWARD);
    HAL_GPIO_WritePin(dCard.MOTOR_2.DIR_Port, dCard.MOTOR_2.DIR_Pin, BACKWARD);
    distance = distance * (-1); //Make the distance positive.
  }

  //Calculate how many small steps to take.
  double numberOfSteps = distance/(PI*DIAMETER1*MM_DEGREE);
  //Round of to nearest amont of steps.
  uint16_t wholeNumberOfSteps = (int) round(numberOfSteps);
  forward_temp = wholeNumberOfSteps;
  //Step the resulting amount of steps.

  stepMotors(wholeNumberOfSteps);
}


/*
Step the given amount of steps.
*/
void stepMotors(uint16_t steps){
    //Disable power to the DriverCard.
  HAL_GPIO_WritePin(dCard.ENABLE_Port, dCard.ENABLE_Pin, LOW);
  
  //Wait 0,1s so not to damage the motors.
  HAL_Delay(100);  
  
  for(int i = 0; i < steps; i++){
    //Set high (take step)
    HAL_GPIO_WritePin(dCard.MOTOR_1.STEP_Port, dCard.MOTOR_1.STEP_Pin, HIGH);
    HAL_GPIO_WritePin(dCard.MOTOR_2.STEP_Port, dCard.MOTOR_2.STEP_Pin, HIGH);
    
    //Make pulse 1ms
    HAL_Delay(1);
    
    //Set low (prepare for next "flank")
    HAL_GPIO_WritePin(dCard.MOTOR_1.STEP_Port, dCard.MOTOR_1.STEP_Pin, LOW);
    HAL_GPIO_WritePin(dCard.MOTOR_2.STEP_Port, dCard.MOTOR_2.STEP_Pin, LOW);
    
    //Wait 1ms untill next step.
    HAL_Delay(1);

  }
   //Wait 0,1s so not to damage the motors.
  HAL_Delay(100); 
  
    //Disable power to the DriverCard.
  HAL_GPIO_WritePin(dCard.ENABLE_Port, dCard.ENABLE_Pin, HIGH);
  
}
