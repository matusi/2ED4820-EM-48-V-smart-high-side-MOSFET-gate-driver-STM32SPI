/*
 * driver.h
 *
 *  Created on: Jun 15, 2023
 *      Author: chokri
 */

#ifndef INC_DRIVER_H_
#define INC_DRIVER_H_

//#include "spi.h"
//
//#include "gpio.h"
//#include "tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "registers.h"

#include "main.h"


typedef struct {
	SPI_HandleTypeDef* fet_spi;
	GPIO_TypeDef* CS_GPIO;
	uint16_t CS_PIN;
}SMART_FET_SPI;




void Infineon2ED4820_ReadRegister(SMART_FET_SPI fet_spi, uint8_t regAddr, uint8_t *data, uint8_t length);
void Infineon2ED4820_WriteRegister(SMART_FET_SPI fet_spi, uint8_t regAddr, uint8_t data);

/* initialisation funtions
 *
 *look for init regs
 *
 *- normal mode
 *-
 *- make modes list :
 *-
 *-
 *
 *
 *  */



/*
 *
 *
 * Sense current funtions
 *
 * init current
 *
 * look for current sensor related regs
 *
 * prlly
 * gain
 * calibrate
 *
 * setoffset
 *
 * sens current periodically
 *
 *
 */

int switchONchannelA();



/* functions to be periodically executed with a timer

/* //Switch on channel A
 *
 *
 * set reg MOSONCH_A      0x01 // LSB : Switch on channel A
 */


int switchONchannelA();







/*
// Switch off channel A
 *
 *
 *
 * set reg MOSONCH_A      0x01 // LSB : Switch on channel A
 */
int switchOFFchannelA();




/*
// //Switch on channel B
 *
 *
 *
 * set MOSONCH_B      0x08 // Switch on channel B
 */
int switchONchannelB();



/*
// //Switch off channel B
 *
 *
 *
 * set reg MOSONCH_A      0x01 // LSB : Switch on channel A
 */
int switchOFFchannelB();





/*
 * Drainovervoltage functions see datasheet page  23-24
 *
 *
 *
 *
 */

// Activate overvoltage  lock  Reg VSD_X_SS[0]  activated in default













#endif /* INC_DRIVER_H_ */
