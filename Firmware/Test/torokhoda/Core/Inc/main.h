/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Transmitt_Pin GPIO_PIN_0
#define Transmitt_GPIO_Port GPIOA
#define Transmitt_EXTI_IRQn EXTI0_IRQn
#define Receive_Pin GPIO_PIN_1
#define Receive_GPIO_Port GPIOA
#define Receive_EXTI_IRQn EXTI1_IRQn
#define RE_Pin GPIO_PIN_4
#define RE_GPIO_Port GPIOA
#define c_Pin GPIO_PIN_13
#define c_GPIO_Port GPIOB
#define e_Pin GPIO_PIN_14
#define e_GPIO_Port GPIOB
#define com_Pin GPIO_PIN_15
#define com_GPIO_Port GPIOB
#define d_Pin GPIO_PIN_8
#define d_GPIO_Port GPIOA
#define g_Pin GPIO_PIN_5
#define g_GPIO_Port GPIOB
#define f_Pin GPIO_PIN_6
#define f_GPIO_Port GPIOB
#define a_Pin GPIO_PIN_7
#define a_GPIO_Port GPIOB
#define b_Pin GPIO_PIN_8
#define b_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
