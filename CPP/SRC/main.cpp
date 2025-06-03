#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>

extern "C" TIM_HandleTypeDef htim15;
extern "C" ADC_HandleTypeDef hadc1;
extern "C" UART_HandleTypeDef huart2;

static uint8_t rx_byte;
volatile uint32_t pot_value = 0;

enum Mode {
	MODE_FREEZE,
	MODE_SWEEP,
	MODE_MANUAL
};

volatile Mode currentMode = MODE_FREEZE;


void debugPrint(const char *fmt, ...)
{
    char buf[64];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);
}


static const uint32_t SWEEP_INTERVAL_MS  = 1;
static const uint32_t FREEZE_INTERVAL_MS = 500;
static const uint32_t MANUAL_INTERVAL_MS = 200;

volatile uint32_t button_pressed  = 0;
volatile uint32_t button_releases = 0;
volatile uint32_t time_since_button_pressed = HAL_GetTick();
volatile uint32_t lastRiseTick = 0;
volatile uint32_t lastFallTick = 0;

const uint32_t NORMAL_BUTTON_PRESS = 1000;
const uint32_t LONG_BUTTON_PRESS = 2000;
const uint32_t DEBOUNCE = 20;

#define SERVO_TIMER htim15
#define SERVO_MIN 500
#define SERVO_MAX 2500
#define ADC_MAX 4095
#define SERVO_RANGE (SERVO_MAX - SERVO_MIN)

uint16_t pos = SERVO_MIN;
	int8_t dir = +1;

void maincpp(void)
{
	HAL_TIM_PWM_Start(&SERVO_TIMER, TIM_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&pot_value, 1);
	MX_USART2_UART_Init();
	HAL_UART_Receive_IT(&huart2, &rx_byte, 1);

	time_since_button_pressed = HAL_GetTick();

	 static uint32_t lastSweepTick  = 0;
	    static uint32_t lastFreezeTick = 0;
	    static uint32_t lastManualTick = 0;

	while(1)
	{
		uint32_t now = HAL_GetTick();

    switch (currentMode)
    {
        case MODE_SWEEP:
        {
            if (now - lastSweepTick >= SWEEP_INTERVAL_MS)
            {
                lastSweepTick = now;
                SERVO_TIMER.Instance->CCR1 = pos;
                pos += dir;
                if (pos >= SERVO_MAX)

                { pos = SERVO_MAX; dir = -1; } // I moved these set of braces together in a sentence because it was confusing keeping track of the open and closed braces
                else if (pos <= SERVO_MIN)
                { pos = SERVO_MIN; dir = +1; }
            }
            break;
        }  // closes MODE_SWEEP

        case MODE_FREEZE:
        {
            if (now - lastFreezeTick >= FREEZE_INTERVAL_MS)
            {
                lastFreezeTick = now;
                HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            }
            break;
        }  // closes MODE_FREEZE

        case MODE_MANUAL:
        {
            if (now - lastManualTick >= MANUAL_INTERVAL_MS)
            {
                lastManualTick = now;
                pos = (pot_value * SERVO_RANGE) / ADC_MAX + SERVO_MIN;
                SERVO_TIMER.Instance->CCR1 = pos;
                debugPrint("MANUAL POS=%u, ADC=%lu\r\n", pos, pot_value);
            }
            break;
        }  // closes MODE_MANUAL
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
			currentMode = MODE_SWEEP;

			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		}
		else if (held< LONG_BUTTON_PRESS)
		{
			currentMode = MODE_FREEZE;
			HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			  debugPrint("FROZEN at POS=%u\r\n", pos);
		}
		else
		{
			currentMode = MODE_MANUAL;
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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {

        HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
    }
}

