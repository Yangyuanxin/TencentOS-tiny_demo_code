/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "oled.h"
#include "multi_button.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/*创建一个按键事件*/
k_event_t key_event;
//创建几个按键的事件标志
const k_event_flag_t event_key1 = (k_event_flag_t)(1 << 0);
const k_event_flag_t event_key2 = (k_event_flag_t)(1 << 1);
const k_event_flag_t event_key3 = (k_event_flag_t)(1 << 2);
const k_event_flag_t event_key4 = (k_event_flag_t)(1 << 3);
const k_event_flag_t event_key5 = (k_event_flag_t)(1 << 4);
//定义描述按键的结构体变量
struct Button button1, button2, button3, button4;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t key1_read_pin(void);
uint8_t key2_read_pin(void);
uint8_t key3_read_pin(void);
uint8_t key4_read_pin(void);
void key_handler(void* btn);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//task1
#define TASK1_STK_SIZE		256
void task1(void *pdata);
osThreadDef(task1, osPriorityNormal, 1, TASK1_STK_SIZE);
void task1(void *pdata)
{
    button_init(&button1, key1_read_pin, 0);
    button_init(&button2, key2_read_pin, 0);
    button_init(&button3, key3_read_pin, 0);
    button_init(&button4, key4_read_pin, 0);

    button_attach(&button1, PRESS_DOWN,     	key_handler);
    button_attach(&button1, PRESS_UP,     		key_handler);

    button_attach(&button2, PRESS_DOWN,     	key_handler);
    button_attach(&button2, PRESS_UP,     		key_handler);

    button_attach(&button3, PRESS_DOWN,     	key_handler);
    button_attach(&button3, PRESS_UP,     		key_handler);

    button_attach(&button4, PRESS_DOWN,     	key_handler);
    button_attach(&button4, PRESS_UP,     		key_handler);

    button_start(&button1);
    button_start(&button2);
    button_start(&button3);
    button_start(&button4);
    while(1)
    {
        osDelay(100);
    }
}

//task2
#define TASK2_STK_SIZE		256
void task2(void *pdata);
osThreadDef(task2, osPriorityNormal, 1, TASK2_STK_SIZE);
void task2(void *pdata)
{
    k_event_flag_t flag_match;
    k_err_t err;

    while(1)
    {
        //同时监视4个事件
        err = tos_event_pend(&key_event, event_key5 | event_key1 | event_key2 | event_key3 | event_key4,
                             &flag_match, TOS_TIME_FOREVER, TOS_OPT_EVENT_PEND_ANY | TOS_OPT_EVENT_PEND_CLR); //

        if (err == K_ERR_NONE)
        {
            if (flag_match == event_key1)
            {
                printf("按键1\n");
                HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
            }

            if (flag_match == event_key2)
            {
                printf("按键2\n");
                HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
            }

            if (flag_match == event_key3)
            {
                printf("按键3\n");
                HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
            }

            if (flag_match == event_key4)
            {
                printf("按键4\n");
                HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
            }

            if (flag_match == event_key5)
            {
                printf("按键状态清除\n");
                HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
            }
        }
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C3_Init();
    /* USER CODE BEGIN 2 */
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"TencentOS tiny", 16);
    OLED_ShowString(0, 2, (uint8_t*)"Bruce.yang", 16);
    //初始化内核
    osKernelInitialize();
    //创建一个按键事件
    tos_event_create(&key_event, (k_event_flag_t)0u);
    //创建并启动一个任务用于通过按键发送事件
    osThreadCreate(osThread(task1), NULL);
    //创建并启动一个任务用于接收按键事件并执行相应的软件逻辑
    osThreadCreate(osThread(task2), NULL);
    //启动TencentOS tiny内核
    osKernelStart();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }

    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_I2C3;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
uint8_t key1_read_pin(void)
{
    return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
}
uint8_t key2_read_pin(void)
{
    return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
}
uint8_t key3_read_pin(void)
{
    return HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
}
uint8_t key4_read_pin(void)
{
    return HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
}

void key_handler(void* btn)
{
	struct Button *button = btn ;
	if(btn == &button1)
	{
		if(button->event == PRESS_DOWN)
			tos_event_post(&key_event, event_key1);
		else if(button->event == PRESS_UP)
			tos_event_post(&key_event, event_key5);
	}
	else if(btn == &button2)
	{
		if(button->event == PRESS_DOWN)
			tos_event_post(&key_event, event_key2);
		else if(button->event == PRESS_UP)
			tos_event_post(&key_event, event_key5);
	}
	else if(btn == &button3)
	{
		if(button->event == PRESS_DOWN)
			tos_event_post(&key_event, event_key3);
		else if(button->event == PRESS_UP)
			tos_event_post(&key_event, event_key5);
	}
	else if(btn == &button4)
	{
		if(button->event == PRESS_DOWN)
			tos_event_post(&key_event, event_key4);
		else if(button->event == PRESS_UP)
			tos_event_post(&key_event, event_key5);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
