/*
 * deriver.c
 *
 *  Created on: Jun 15, 2023
 *      Author: Gaming
 */




#include "driver.h"

#include "man.h"


// binairy com
void Infineon2ED4820_ReadRegister(SMART_FET_SPI fet_spi, uint8_t regAddr, uint8_t *data, uint8_t length){

	HAL_GPIO_WritePin(fet_spi.CS_GPIO, fet_spi.CS_PIN, GPIO_PIN_RESET);
	uint8_t addr = (regAddr<<1) | 0x01;
	HAL_SPI_Transmit(fet_spi.fet_spi, &addr , 1 , 1);
	HAL_SPI_Receive(fet_spi. fet_spi, data,length , 1);
	HAL_GPIO_WritePin(fet_spi.CS_GPIO, fet_spi.CS_PIN, GPIO_PIN_SET);

}


void Infineon2ED4820_WriteRegister(SMART_FET_SPI fet_spi, uint8_t regAddr, uint8_t data){
	HAL_GPIO_WritePin(fet_spi.CS_GPIO, fet_spi.CS_PIN, GPIO_PIN_RESET);
	uint8_t addr = (regAddr<<1) | 0x00;
	HAL_SPI_Transmit(fet_spi.fet_spi, &addr , 1 , 1);
	HAL_SPI_Transmit(fet_spi.fet_spi, &data, 1 , 1);
	HAL_GPIO_WritePin(fet_spi.CS_GPIO, fet_spi.CS_PIN, GPIO_PIN_SET);


	//str com


	void SPI_WriteRegister(uint8_t reg, uint8_t data)
	{
	  uint8_t txData[2] = {reg, data};  // Combine the register address and data into a single array
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // Activate the chip select (CS) pin

	  HAL_SPI_Transmit(&hspi1, txData, 2, HAL_MAX_DELAY);  // Send the register address and data

	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // Deactivate the chip select (CS) pin
	}

//int switchONchannelA();


//{
//	/*** *//
//
//	// ...
//
//
//
//
//
//}


void turnOnMOSFETs(void)
{
  // Turn on MOSFET A
  SPI_WriteRegister(MOSFET_A_CTRL_REG, 0x01);

  // Turn on MOSFET B
  SPI_WriteRegister(MOSFET_B_CTRL_REG, 0x01);
}






void turnOnMOSFETs(void)
{
  // Turn on MOSFET A
  SPI_WriteRegister(MOSFET_A_CTRL_REG, 0x01);

  // Turn on MOSFET B
  SPI_WriteRegister(MOSFET_B_CTRL_REG, 0x01);
}

void SPI_WriteRegister(uint8_t reg, uint8_t data)
{
  uint8_t txData[2] = {reg, data};  // Combine the register address and data into a single array
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // Activate the chip select (CS) pin

  HAL_SPI_Transmit(&hspi1, txData, 2, HAL_MAX_DELAY);  // Send the register address and data

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // Deactivate the chip select (CS) pin
}






