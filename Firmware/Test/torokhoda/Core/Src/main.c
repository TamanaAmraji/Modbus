/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "M_modbus.h"
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
uint32_t previousMillis, currentMillis;
	int i, data=0;
void SevenSegNumber(int num){
	switch (num) 
	{
		case 0 :
		GPIOB->ODR = 0x9E3F; 
		GPIOA->ODR |= 0x00;
		break;
		case 1 :
		GPIOB->ODR |= 0x40E0; 
		GPIOA->ODR |= 0x100;
		break;
		case 2 :
		GPIOB->ODR = 0xBE5F;
		GPIOA->ODR &= 0x00;		
		break;
		case 3 :
		GPIOB->ODR = 0xDE5F;
		GPIOA->ODR &= 0x00;			
		break;
		case 4 :
		GPIOB->ODR = 0xDE9F; 
		GPIOA->ODR |= 0x100;
		break;
		case 5 :
		GPIOB->ODR = 0xC100; 
		GPIOA->ODR &= 0x00;	
		break;
		case 6 :
		GPIOB->ODR = 0x8100;
		GPIOA->ODR &= 0x00;			
		break;
		case 7 :
		GPIOB->ODR = 0xC030; 
		GPIOA->ODR |= 0x100;
		break;
		case 8 :
		GPIOB->ODR = 0x8000; 
		GPIOA->ODR &= 0x00;	
		break;
		case 9 :
		GPIOB->ODR |= 0xC000; 
		GPIOA->ODR &= 0x00;	
		break;
	}
		
}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//			  writeSingleHoldingRegister(9, 0, 0xFAFA); 
//				HAL_Delay(3000);
//		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
//			HAL_Delay(500);
			// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET);
//			for (int i=0; i<10 ; i++)
//		{
//			SevenSegNumber(i);
//			HAL_Delay(1000);
//		}
		
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
	currentMillis = HAL_GetTick();

	if (GPIO_PIN == GPIO_PIN_9 && (currentMillis - previousMillis > 200))
	{
		if ( i < 9)
			i = i+1;
		else 
			i = 0;
	}	
	else if (GPIO_PIN == GPIO_PIN_0 && (currentMillis - previousMillis > 200))
		writeSingleHoldingRegister(9, 0, 2); 
	else if (GPIO_PIN == GPIO_PIN_1 && (currentMillis - previousMillis > 200))
		readInputRegisters(4,0, 1);  
	SevenSegNumber(i);
	previousMillis = currentMillis;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
