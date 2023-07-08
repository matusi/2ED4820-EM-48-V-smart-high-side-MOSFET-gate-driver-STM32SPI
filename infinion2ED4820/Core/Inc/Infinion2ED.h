/*
 * infinion2ED4820.h
 *
 *  Created on: Jun 15, 2023
 *      Author: jovani
 */

#ifndef INC_INFINION2ED_H_
#define INC_INFINION2ED_H_

//
//#include "spi.h"
//
//#include "gpio.h"
//#include "tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "registers.h"
#include "stm32l4xx_hal.h"

#include "main.h"

typedef struct Fetcontroller{

	/* spi handle */
	SPI_HandleTypeDef *spiHandle;

	GPIO_TypeDef ENAB_GPIO;
	uint16_t ENAB_PIN;

	/* sturctr parms 1*/
	uint8_t  data;

	/* sturct failure / success flag *///



}Fetcontroller;






/* General Diagnos functions
 *
 * Register STDIAG
 *
 *
 * VBAT overvoltage failure (not latched)
 *
 * VBAT undervoltage failure (not latched)
 *
 * VDD undervoltage propagated in the first command and indicates an under voltage event
 *
 * Chip in Overtemperature (latched)
 *
 * Error in the memory, trimming not possible (flag cannot be cleaned)
 *
 * Charge pump is ready
 *
 * MSB : Main Failure indication, 1 when there is a failureMem Failure
 *
 */
uint8_t checkVBATOV(Fetcontroller fet_spi);
uint8_t checkVBATUV(Fetcontroller fet_spi);
uint8_t checkVDDUV(Fetcontroller fet_spi);
uint8_t checkTSD(Fetcontroller fet_spi);
uint8_t checkOTWarning(Fetcontroller fet_spi);
uint8_t checkMemFail(Fetcontroller fet_spi);
uint8_t checkCPReady(Fetcontroller fet_spi);
uint8_t checkMainFailure(Fetcontroller fet_spi);



/* CHANEL Diagnos functions
 *
 * Register CHDIAG
 *
 *
 * // LSB : Source overvoltage - Channel_A
 *
 * // Drain to Source overvoltage - Channel_A (latched)
 *
 * // Gate-Source undervoltage - Channel_A (latched)
 * / Source overvoltage - Channel_B
 *
 * Chip in Overtemperature (latched)
 *
 * Drain to Source overvoltage - Channel_B (latched)
 *
 * // Gate-Source undervoltage - Channel_B (latched)
 *
 * // Overcurrent failure (latched)
 *
// MSB : Charge pump undervoltage
 *
 */

uint8_t checkSourceOV_A(Fetcontroller fet_spi);
uint8_t checkDSTripOV_A(Fetcontroller fet_spi);
uint8_t checkGSUV_A(Fetcontroller fet_spi);
uint8_t checkSourceOV_B(Fetcontroller fet_spi);
uint8_t checkDSTripOV_B(Fetcontroller fet_spi);
uint8_t checkGSUV_B(Fetcontroller fet_spi);
uint8_t checkOvercurrent(Fetcontroller fet_spi);
uint8_t checkChargePumpUV(Fetcontroller fet_spi);



/*  Diagnos functions
 *
´*LSB : SAFESTATEN

Loss of ground connection on  CPGND
Loss of ground  connection on  GND
Loss of ground connection on GNDA
 */


uint8_t checkSafeStateEN(Fetcontroller fet_spi);
uint8_t checkLossOfCPGND(Fetcontroller fet_spi);
uint8_t checkLossOfGND(Fetcontroller fet_spi);
uint8_t checkLossOfGNDA(Fetcontroller fet_spi);








/*
 * SPI LOW-LEVEL FUNCTIONS
 */

HAL_StatusTypeDef Fetcontroller_ReadRegister(Fetcontroller fet_spi, uint8_t regAddr, uint8_t *data, uint8_t length);
HAL_StatusTypeDef Fetcontroller_WriteRegister(Fetcontroller fet_spi, uint8_t regAddr, uint8_t data);









#endif /* INC_INFINION2ED4820_H_ */
