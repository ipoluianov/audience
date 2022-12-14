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
#include "stm32h7xx_ll_dma.h"

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
#define LDAC2_Pin LL_GPIO_PIN_0
#define LDAC2_GPIO_Port GPIOF
#define LDAC1_Pin LL_GPIO_PIN_1
#define LDAC1_GPIO_Port GPIOF
#define SPI2_CS_Pin LL_GPIO_PIN_3
#define SPI2_CS_GPIO_Port GPIOF
#define SPI1_CS_Pin LL_GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOF
#define ETH_RST_Pin LL_GPIO_PIN_3
#define ETH_RST_GPIO_Port GPIOA
#define DISPLAY_RS_Pin LL_GPIO_PIN_0
#define DISPLAY_RS_GPIO_Port GPIOD
#define DISPLAY_E_Pin LL_GPIO_PIN_1
#define DISPLAY_E_GPIO_Port GPIOD
#define DISPLAY_D4_Pin LL_GPIO_PIN_2
#define DISPLAY_D4_GPIO_Port GPIOD
#define DISPLAY_D5_Pin LL_GPIO_PIN_3
#define DISPLAY_D5_GPIO_Port GPIOD
#define DISPLAY_D6_Pin LL_GPIO_PIN_4
#define DISPLAY_D6_GPIO_Port GPIOD
#define DISPLAY_D7_Pin LL_GPIO_PIN_5
#define DISPLAY_D7_GPIO_Port GPIOD
#define ETH_RX_ER_Pin LL_GPIO_PIN_9
#define ETH_RX_ER_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define LDAC0_ON GPIOF->ODR |= 0x0002
#define LDAC0_OFF GPIOF->ODR &= 0xFFFD
#define LDAC1_ON GPIOF->ODR |= 0x0001
#define LDAC1_OFF GPIOF->ODR &= 0xFFFE


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
