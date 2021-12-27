/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

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
#define LED_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOA
#define ESP_SPI1_CS_Pin GPIO_PIN_4
#define ESP_SPI1_CS_GPIO_Port GPIOA
#define ESP_SPI1_SCK_Pin GPIO_PIN_5
#define ESP_SPI1_SCK_GPIO_Port GPIOA
#define ESP_SPI1_MISO_Pin GPIO_PIN_6
#define ESP_SPI1_MISO_GPIO_Port GPIOA
#define ESP_SPI1_MOSI_Pin GPIO_PIN_7
#define ESP_SPI1_MOSI_GPIO_Port GPIOA
#define ESP_SIG1_Pin GPIO_PIN_0
#define ESP_SIG1_GPIO_Port GPIOB
#define ESP_SIG2_Pin GPIO_PIN_1
#define ESP_SIG2_GPIO_Port GPIOB
#define ESP_EN_Pin GPIO_PIN_2
#define ESP_EN_GPIO_Port GPIOB
#define ESP_I2C2_SCL_Pin GPIO_PIN_10
#define ESP_I2C2_SCL_GPIO_Port GPIOB
#define ESP_I2C2_SDA_Pin GPIO_PIN_11
#define ESP_I2C2_SDA_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_11
#define BUTTON_GPIO_Port GPIOA
#define MLX_I2C1_SCL_Pin GPIO_PIN_6
#define MLX_I2C1_SCL_GPIO_Port GPIOB
#define MLX_I2C1_SDA_Pin GPIO_PIN_7
#define MLX_I2C1_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
