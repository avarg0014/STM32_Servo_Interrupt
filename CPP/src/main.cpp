#include "main.h"

extern "C" TIM_HandleTypeDef htim15;

enum PRESS_TYPE{
	UNDEFINED = 0,
	NORMAL,
	LONG_PRESS,
	SUPER_LONG_PRESS,

};

PRESS_TYPE buttonAction;

volatile uint32_t button_pressed = 0;
volatile uint32_t button_releases = 0;

volatile uint32_t time_since_button_pressed = HAL_GetTick();

const uint32_t NORMAL_BUTTON_PRESS_MS = 1000;
const uint32_t LONG_BUTTONL_PRESS_MS = 2000;


#define SERVO_TIMER htim15

void maincpp(void)
{


	SERVO_TIMER.Instance->CCR1 = 1500;

	HAL_TIM_PWM_Start(&SERVO_TIMER, TIM_CHANNEL_1);

	buttonAction = PRESS_TYPE::UNDEFINED;
	while(1)
	{
//		HAL_Delay(1);

		switch(buttonAction)
		{

		case PRESS_TYPE::NORMAL:
			if( SERVO_TIMER.Instance->CCR1 > 500 )
			{
				SERVO_TIMER.Instance->CCR1--;
			}
			else if( SERVO_TIMER.Instance->CCR1 < 500)
			{
					SERVO_TIMER.Instance->CCR1++;
			}

		break;
		case PRESS_TYPE::LONG_PRESS:
			if( SERVO_TIMER.Instance->CCR1 > 1500 )
			{
				SERVO_TIMER.Instance->CCR1--;
			}
			else if( SERVO_TIMER.Instance->CCR1 < 1500)
			{
				SERVO_TIMER.Instance->CCR1++;
			}
		break;
		case PRESS_TYPE::SUPER_LONG_PRESS:
			if( SERVO_TIMER.Instance->CCR1 > 2500 )
			{
				SERVO_TIMER.Instance->CCR1--;
			}
			else if( SERVO_TIMER.Instance->CCR1 < 2500)
			{
				SERVO_TIMER.Instance->CCR1++;
			}
		break;

		case PRESS_TYPE::UNDEFINED:
			break;
		}

	}
}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{

	if( GPIO_Pin == BUTTON_INT13_Pin)
	{
		uint32_t now = HAL_GetTick();

		if( (now - time_since_button_pressed)  < NORMAL_BUTTON_PRESS_MS)
		{
			buttonAction = PRESS_TYPE::NORMAL;
		}
		else if(  (now - time_since_button_pressed) < LONG_BUTTONL_PRESS_MS)
		{
			buttonAction = PRESS_TYPE::LONG_PRESS;
		}
		else{
			buttonAction = PRESS_TYPE::SUPER_LONG_PRESS;
		}
		button_releases++;
	}
}


void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	if( GPIO_Pin == BUTTON_INT13_Pin)
	{
		time_since_button_pressed = HAL_GetTick();
		button_pressed++;
	}
}
