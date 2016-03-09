#include "tim.h"
#include "gpio.h"

//FRÅGA BEGNT

#define  PERIOD_VALUE       (uint32_t)(480 - 1)  /* Period Value  */


/* Pen down = 0,6ms => 0,6/20 = 0,03 => 3% */
/* Pen up = 1,0ms => 1/20 => 0,05 => 5% */
#define  PENUP_DUTY_CYCLE    5
#define  PENDOWN_DUTY_CYCLE  3
#define  PENUP_PULSE_VALUE       (uint32_t)(PERIOD_VALUE*(PENUP_DUTY_CYCLE)/(100 - 1)) /* Penup pulse value  */
#define  PENDOWN_PULSE_VALUE     (uint32_t)(PERIOD_VALUE*(PENDOWN_DUTY_CYCLE)/(100 - 1)) /* Pendown pulse value  */
#define  HALF_DUTY_CYCLE         (uint32_t)(PERIOD_VALUE*50/100)


TIM_HandleTypeDef htim4;
TIM_OC_InitTypeDef sConfigOC;

/*
Set the PWM pusle to eather 3 or 5% duty cycle
*/
void penPulse(uint8_t value){
  if(value == 0){
    sConfigOC.Pulse = PENUP_PULSE_VALUE;
  }else{
    sConfigOC.Pulse = PENDOWN_PULSE_VALUE;
  }
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_2);
  
}

/* TIM4 init function */
void MX_TIM4_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = (uint32_t)(SystemCoreClock / 24000) - 1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = PERIOD_VALUE;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim4);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = PENUP_PULSE_VALUE;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);
  
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_pwm->Instance==TIM4)
  {
    /* USER CODE BEGIN TIM4_MspInit 0 */
    
    /* USER CODE END TIM4_MspInit 0 */
    /* Peripheral clock enable */
    __TIM4_CLK_ENABLE();
    
    /**TIM4 GPIO Configuration    
    PD13     ------> TIM4_CH2 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM4_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
    /* USER CODE BEGIN TIM4_MspInit 1 */
    
    /* USER CODE END TIM4_MspInit 1 */
  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  
  if(htim_pwm->Instance==TIM4)
  {
    /* USER CODE BEGIN TIM4_MspDeInit 0 */
    
    /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM4_CLK_DISABLE();
    
    /**TIM4 GPIO Configuration    
    PD13     ------> TIM4_CH2 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13);
    
    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
    
  }
  /* USER CODE BEGIN TIM4_MspDeInit 1 */
  
  /* USER CODE END TIM4_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/