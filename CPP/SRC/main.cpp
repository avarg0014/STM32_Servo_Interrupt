// THIS PROJECT AUTOMATICALLY WITH NO BUTTON PRESS ALLOWS THE SERVO TO GO FROM ONE END TO THE OTHER NONSTOP
#include "main.h"
extern "C" TIM_HandleTypeDef htim15;

volatile uint32_t button_pressed = 0;
volatile uint32_t button_releases = 0;




#define SERVO_TIMER htim15

void maincpp(void)

{
	HAL_TIM_PWM_Start(&SERVO_TIMER, TIM_CHANNEL_1);

	uint16_t pos = 500;
	int8_t dir = +1;

	while(1)
	{
		SERVO_TIMER.Instance->CCR1 = pos;
		pos += dir;

		if(pos >= 2500)
		{
			pos = 2500;
			dir = -1;
		}
		else if( pos <= 500)
		{
			pos = 500;
			dir = +1;
		}
		HAL_Delay(1);
	}
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)

{
	if(GPIO_Pin == BUTTON_INT13_Pin)
	{
		button_releases++;
		}
	}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)

{
	if(GPIO_Pin == BUTTON_INT13_Pin)
	{

		button_pressed++;
	}
}


