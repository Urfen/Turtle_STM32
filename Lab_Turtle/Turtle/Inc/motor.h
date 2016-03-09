#include "stm32f3xx_hal.h"
#include "gpio.h"
#include "math.h"

//Define needed values for calculation of noumber of steps
#define PI 3.14159265358979323846
#define DEGREE 0.45
#define MM_DEGREE (DEGREE/360) //0.45 degrees is 1/8 step
#define DIAMETER1 47
#define DIAMETER2 120
#define FORWARD GPIO_PIN_SET
#define BACKWARD GPIO_PIN_RESET
#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

//Struct for A motor
typedef struct motor_struct{
  uint16_t              STEP_Pin;
  GPIO_TypeDef*         STEP_Port;
  uint16_t              DIR_Pin;
  GPIO_TypeDef*         DIR_Port;
} motor_struct;

//Struct for A motor-driverCard
typedef struct driverCard_struct{
  motor_struct          MOTOR_1;
  motor_struct          MOTOR_2;
  uint16_t              MS_1_Pin;
  GPIO_TypeDef*         MS_1_Port;
  uint16_t              MS_2_Pin;
  GPIO_TypeDef*         MS_2_Port;
  uint16_t              ENABLE_Pin;
  GPIO_TypeDef*         ENABLE_Port;
}driverCard_struct;

void motorInit(void);
void straight(int16_t distance);
void rotate(int16_t rotation);
void stepMotors(uint16_t steps);