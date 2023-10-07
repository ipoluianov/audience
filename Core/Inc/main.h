/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_pwr.h"

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
#define ETH_RST_Pin LL_GPIO_PIN_3
#define ETH_RST_GPIO_Port GPIOA
#define LED_RED_Pin LL_GPIO_PIN_13
#define LED_RED_GPIO_Port GPIOD
#define LED_GREEN_Pin LL_GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOG
#define LDAC_Pin LL_GPIO_PIN_6
#define LDAC_GPIO_Port GPIOD
#define CS1_Pin LL_GPIO_PIN_10
#define CS1_GPIO_Port GPIOG
#define CS2_Pin LL_GPIO_PIN_9
#define CS2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define LDAC_ON GPIOD->ODR |= 0x0040
#define LDAC_OFF GPIOD->ODR &= 0xFFBF

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
