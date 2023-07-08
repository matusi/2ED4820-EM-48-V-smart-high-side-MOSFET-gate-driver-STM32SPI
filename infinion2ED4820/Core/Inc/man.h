/*
 * man.h
 *
 *  Created on: Jun 15, 2023
 *      Author: Gaming
 */

#ifndef INC_MAN_H_
#define INC_MAN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

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
#define VerBit_A_Pin GPIO_PIN_9
#define VerBit_A_GPIO_Port GPIOB
#define VBatt_Pin GPIO_PIN_2
#define VBatt_GPIO_Port GPIOA
#define VBus_Pin GPIO_PIN_3
#define VBus_GPIO_Port GPIOA
#define IBatt_Pin GPIO_PIN_4
#define IBatt_GPIO_Port GPIOA
#define IBus_Pin GPIO_PIN_5
#define IBus_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_2
#define LED_R_GPIO_Port GPIOB
#define LED_Y_Pin GPIO_PIN_8
#define LED_Y_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_9
#define LED_G_GPIO_Port GPIOA
#define INTE_Pin GPIO_PIN_10
#define INTE_GPIO_Port GPIOA
#define ENAB_Pin GPIO_PIN_11
#define ENAB_GPIO_Port GPIOA
#define SAFES_Pin GPIO_PIN_12
#define SAFES_GPIO_Port GPIOA
#define USART1_RE_Pin GPIO_PIN_4
#define USART1_RE_GPIO_Port GPIOB
#define VerBit_C_Pin GPIO_PIN_5
#define VerBit_C_GPIO_Port GPIOB
#define VerBit_B_Pin GPIO_PIN_8
#define VerBit_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */



#endif /* INC_MAN_H_ */
