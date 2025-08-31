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
#include "S_modbus.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint32_t previousMillis, currentMillis;
uint8_t RxData[256], meow;
uint8_t TxData[256];
char key,show,t,last_key;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define a 	0x10		//PB4
#define b 	0x20		//PB5
#define c 	0x8000	//PB15
#define d 	0x800		//PA11
#define e 	0x1000	//PA12
#define f 	0x08		//PB3
#define g 	0x8000	//PA15
#define DP 	0x4000	//PB14

#define C1 0x02			//PB1
#define C2 0x04			//PB2
#define C3 0x400		//PB10
#define R1 0x20			//PA5
#define R2 0x40			//PA6
#define R3 0x80			//PA7
#define R4 0x01			//PB0

#define col_number  3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void SevenSegNumber(int num);
char keypad_getkey(void);
void beep(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t RxData[256];
uint8_t TxData[256];
int indx,flag,holding,shown,keyActive = 0;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	HAL_UARTEx_ReceiveToIdle_IT(huart, RxData, 256); 
	if (RxData[0] == SLAVE_ID)
	{
		switch (RxData[1]){
		case 0x03:
			readHoldingRegs();
			break;
		case 0x04:
			readInputRegs(show);
			break;
		case 0x01:
			readCoils();
			break;
		case 0x02:
			readInputs();
			break;
		case 0x10:
			writeHoldingRegs();
			break;
		case 0x06:
			writeSingleReg();
		break;
		case 0x05:
			writeSingleCoil();
		break;
		case 0x0f:
			writeMultiCoils();
		break;
		default:
			modbusException(ILLEGAL_FUNCTION);
			break;
		}
	}
	

	indx = Size;
	HAL_UARTEx_ReceiveToIdle_IT(&uart_ch, RxData, 256);
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

	
	HAL_UARTEx_ReceiveToIdle_IT(&uart_ch, RxData, 256);
		
	//GPIOA->ODR &= 0x00;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		key = keypad_getkey();
		if (show == TxData[4])HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_RESET);
		else if (show != RxData[5]) HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_RESET);
		
		if (key != 0 )  
			show=key; 
		else if (key ==0 && RxData[5] !=0  && RxData[1] == 6 && meow ==1)
		{
			show = RxData[5];
			beep();
			meow=0;
		}
	SevenSegNumber(show);


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

	if (GPIO_PIN == GPIO_PIN_13 && (currentMillis - previousMillis > 200))
	{
		show = 10;
		readInputRegs(show);
		
	}		
	previousMillis = currentMillis;
}

void SevenSegNumber(int num){
    GPIOA->ODR &= ~(d | e | g);
    GPIOB->ODR &= ~(a | b | c | f);

    switch (num) 
    {
        case 0:
            GPIOA->ODR |= (d | e);
            GPIOB->ODR |= (a | b | c | f);
            break;
        case 1:
            GPIOB->ODR |= (b | c);
            break;
        case 2:
            GPIOA->ODR |= (d | e | g);
            GPIOB->ODR |= (a | b);
            break;
        case 3:
            GPIOA->ODR |= (d | g);
            GPIOB->ODR |= (a | b | c);
            break;
        case 4:
            GPIOA->ODR |= (g);
            GPIOB->ODR |= (b | c | f);
            break;
        case 5:
            GPIOA->ODR |= (d | g);
            GPIOB->ODR |= (a | c | f);
            break;
        case 6:
            GPIOA->ODR |= (d | e | g);
            GPIOB->ODR |= (a | c | f);
            break;
        case 7:
            GPIOB->ODR |= (a | b | c);
            break;
        case 8:
            GPIOA->ODR |= (d | e | g);
            GPIOB->ODR |= (a | b | c | f);
            break;
        case 9:
            GPIOA->ODR |= (d | g);
            GPIOB->ODR |= (a | b | c | f);
            break;
				case 10:
            GPIOA->ODR |= (g);
            break;
    }
}		


char keypad_getkey(void) 
{
	int row, col;
	const int row_high[] =  {R1_Pin, R2_Pin, R3_Pin, R4_Pin}; /* one row is high/low */
	/* If a key is pressed, it gets here to find out which key.
	* It activates one row at a time and read the input to see which column is active. */
	for (row = 0; row < 4; row++) 
	{
		if (row<3)	HAL_GPIO_WritePin(GPIOA, row_high[row], GPIO_PIN_RESET); /* drive the active row low */	
		else 				HAL_GPIO_WritePin(GPIOB, row_high[row], GPIO_PIN_RESET); /* drive the active row low */
		HAL_Delay(5); /* wait for signal to settle */
		col = GPIOB->IDR & (C1 | C2 | C3); /* read all columns*/
		if (row<3)	HAL_GPIO_WritePin(GPIOA, row_high[row], GPIO_PIN_SET); /* drive the active row high */
		else 				HAL_GPIO_WritePin(GPIOB, row_high[row], GPIO_PIN_SET); /* drive the active row high */
		if (col != (C1 | C2 | C3)) break; /* if one of the input is low, some key is pressed. */
	}
	GPIOA->ODR |= (R1|R2|R3); /* drive all rows high before disable them*/
	GPIOB->ODR |= (R4); /* drive all rows high before disable them*/
	if (row == 4) return 0; /* if we get here, no key is pressed */
	/* gets here when one of the rows has key pressed, check which column it is */
	if (col == (C2|C3))		return row * col_number + 1; /* key in column 0 */
	if (col == (C1|C3)) 	return row * col_number + 2; /* key in column 1 */
	if (col == (C1|C2)) 	return row * col_number + 3; /* key in column 2 */
//	if (col == 0x0007) return row * col_number + 4; /* key in column 3 */
	return 0; /* just to be safe */
}

void beep(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
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
