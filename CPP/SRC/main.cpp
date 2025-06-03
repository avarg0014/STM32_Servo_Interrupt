#include "main.h"
extern "C" TIM_HandleTypeDef htim15;

volatile bool sweep_enabled = false;

volatile uint32_t button_pressed = 0;
volatile uint32_t button_releases = 0;

volatile uint32_t time_since_button_pressed = HAL_GetTick();

const uint32_t NORMAL_BUTTON_PRESS = 1000;
const uint32_t LONG_BUTTON_PRESS = 2000;

const uint32_t DEBOUNCE = 20;
volatile uint32_t lastRiseTick = 0;
volatile uint32_t lastFallTick = 0;

#define SERVO_TIMER htim15

void maincpp(void)
{
	HAL_TIM_PWM_Start(&SERVO_TIMER, TIM_CHANNEL_1);

	uint16_t pos = 500;
	  int8_t   dir = +1;

	while(1)
	{
		if(sweep_enabled)
		{
			SERVO_TIMER.Instance->CCR1 = pos;

			pos += dir;

			if(pos >= 2500)
			{
				pos = 2500;
				dir = -1;
			}
				else if ( pos <= 500)
				{
					pos = 500;
					dir = +1;

				}
			HAL_Delay(1);

				}
		else
		{
			HAL_Delay(10);

	}
}
}
	void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
	{
		if(GPIO_Pin ==BUTTON_INT13_Pin)
		{
			uint32_t now = HAL_GetTick();

			if( now - lastRiseTick < DEBOUNCE) return;

			uint32_t held = now - time_since_button_pressed;

			if(held < NORMAL_BUTTON_PRESS)
			{
				sweep_enabled = true;

				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			}
			else if (held< LONG_BUTTON_PRESS)
			{
				sweep_enabled = false;
				HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

			}
			time_since_button_pressed = now;
			button_releases++;
			lastRiseTick = now;

		}
	}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
	{
		if(GPIO_Pin == BUTTON_INT13_Pin)
		{
			uint32_t now = HAL_GetTick();

			if( now - lastFallTick < DEBOUNCE) return;

			time_since_button_pressed = HAL_GetTick();
			button_pressed++;
			lastFallTick = now;
		}
	}








