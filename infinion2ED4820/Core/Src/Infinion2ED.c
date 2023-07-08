/*
 * infinion2ED4820.c
 *
 *  Created on: Jun 15, 2023
 *      Author: chokri

/*
 * SPI LOW-LEVEL FUNCTIONS
 */


#include "infinion2ED.h"


HAL_StatusTypeDef Fetcontroller_WriteRegister(Fetcontroller fet_spi, uint8_t regAddr, uint8_t data)
{

	/* Store number of transaction errors (to be returned at end of function) */
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;

	//HAL_GPIO_WritePin(fet_spi.CS_GPIO, fet_spi.CS_PIN, GPIO_PIN_RESET);

	uint8_t addr = (regAddr<<1) | 0x00;
	status = HAL_SPI_Transmit(fet_spi.spiHandle, &addr , 1 , 1);
	errNum += ( status != HAL_OK );
//	if ( regData != FAULT ) {
////		return 255;
//	}

	status =HAL_SPI_Transmit(fet_spi.spiHandle, &data, 1 , 1);
	errNum += ( status != HAL_OK );
//	if ( regData != FAULT ) {  critical err leave  and set chip to safe mode
//		return 255;
//	}

//	HAL_GPIO_WritePin(fet_spi.CS_GPIO, fet_spi.CS_PIN, GPIO_PIN_SET);

 /* Return number of errors (0 if successful ) */
	return status;  // or return errNum;
		}


HAL_StatusTypeDef Fetcontroller_ReadRegister(Fetcontroller fet_spi, uint8_t regAddr, uint8_t *data, uint8_t length)

{	/* Store number of transaction errors (to be returned at end of function) */
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;

//	HAL_GPIO_WritePin(adxl_spi.CS_GPIO, adxl_spi.CS_PIN, GPIO_PIN_RESET);


	uint8_t addr = (regAddr<<1) | 0x01;

	status = HAL_SPI_Transmit(fet_spi.spiHandle, &addr , 1 , 1);
	errNum += ( status != HAL_OK );
	//	if ( regData != FAULT ) {
	////		return 255;
	//	}

	status = HAL_SPI_Receive(fet_spi.spiHandle, data,length , 1);
	errNum += ( status != HAL_OK );
	//	if ( regData != FAULT ) {
	////		return 255;
	//	}


//	HAL_GPIO_WritePin(adxl_spi.CS_GPIO, adxl_spi.CS_PIN, GPIO_PIN_SET);

	 /* Return number of errors (0 if successful ) */
		return status;  // or return errNum;

}



/* Register STDIAG */
/* General Diagnostic functions
 *
 *
 *
 *
 * */

uint8_t checkVBATOV(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData =0;
    errNum = spiRead(STDIAG, &regData);
//    errNum += (status != HAL_OK);

    if ((regData & VBAT_OV) == VBAT_OV) {
        // VBAT_OV bit is equal to 1 (device is faulty)
        // Perform necessary actions
    }

    return errNum;
}




uint8_t checkVBATUV(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, STDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VBAT_UV) == VBAT_UV) {
        // VBAT undervoltage failure occurred
        // Perform necessary actions
    }

    return errNum;
}



uint8_t checkVDDUV(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, STDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VDD_UV) == VDD_UV) {
        // VDD undervoltage failure occurred
        // Perform necessary actions
    }

    return errNum;
}

uint8_t checkTSD(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, STDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & TSD) == TSD) {
        // Chip in Overtemperature state
        // Perform necessary actions
    }

    return errNum;
}

uint8_t checkOTWarning(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, STDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & OT_WARNING) == OT_WARNING) {
        // Temperature warning
        // Perform necessary actions
    }

    return errNum;
}

uint8_t checkMemFail(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, STDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & MEM_FAIL) == MEM_FAIL) {
        // Error in the memory, trimming not possible
        // Perform necessary actions
    }

    return errNum;
}

uint8_t checkCPReady(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, STDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VCP_READY) == VCP_READY) {
        // Charge pump is ready
        // Perform necessary actions
    }

    return errNum;
}


uint8_t checkMainFailure(Fetcontroller fet_spi) {
    /* Store number of transaction errors (to be returned at end of function) */
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    /* Check device */
    uint8_t regData =0;
//    status = Fetcontroller_ReadRegister(fet_spi, STDIAG, &regData, 1);

    errNum = spiRead(STDIAG, &regData);
//    errNum += (status != HAL_OK);

    if ((regData & FAILURE) == FAILURE) {
        // Main Failure indication occurred
        // Perform necessary actions
    }

    return errNum;
}

/*Channel and current Sens diagnose functions
 *
 *
 *
 *
 *
 *
 *
 */


/* Channel and current sensor diagnostic functions */

/**
 * Check Source Overvoltage in Channel_A.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkSourceOV_A(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VSOURCE_A) == VSOURCE_A) {
        // Source overvoltage failure occurred in Channel_A
        // Perform necessary actions
    }

    return errNum;
}

/**
 * Check Drain to Source Overvoltage in Channel_A.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkDSTripOV_A(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VDSTRIP_A) == VDSTRIP_A) {
        // Drain to Source overvoltage failure occurred in Channel_A (latched)
        // Perform necessary actions
    }

    return errNum;
}

/**
 * Check Gate-Source Undervoltage in Channel_A.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkGSUV_A(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VGSTH_A) == VGSTH_A) {
        // Gate-Source undervoltage failure occurred in Channel_A (latched)
        // Perform necessary actions
    }

    return errNum;
}

/**
 * Check Source Overvoltage in Channel_B.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkSourceOV_B(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VSOURCE_B) == VSOURCE_B) {
        // Source overvoltage failure occurred in Channel_B
        // Perform necessary actions
    }

    return errNum;
}

/**
 * Check Drain to Source Overvoltage in Channel_B.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkDSTripOV_B(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VDSTRIP_B) == VDSTRIP_B) {
        // Drain to Source overvoltage failure occurred in Channel_B (latched)
        // Perform necessary actions
    }

    return errNum;
}

/**
 * Check Gate-Source Undervoltage in Channel_B.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkGSUV_B(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VGSTH_B) == VGSTH_B) {
        // Gate-Source undervoltage failure occurred in Channel_B (latched)
        // Perform necessary actions
    }

    return errNum;
}

/**
 * Check Overcurrent Failure.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkOvercurrent(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & ITRIP) == ITRIP) {
        // Overcurrent failure occurred (latched)
        // Perform necessary actions
    }

    return errNum;
}

/**
 * Check Charge Pump Undervoltage.
 *
 * @param fet_spi The FET controller SPI instance.
 * @return Error number indicating the success or failure of the operation.
 */
uint8_t checkChargePumpUV(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, CHDIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & VCP_UV) == VCP_UV) {
        // Charge pump undervoltage failure occurred
        // Perform necessary actions
    }

    return errNum;
}


/*  Diagnos functions
 *
´*LSB : SAFESTATEN

Loss of ground connection on  CPGND
Loss of ground  connection on  GND
Loss of ground connection on GNDA
 */


uint8_t checkSafeStateEN(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, DIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & SAFESTATEN) == SAFESTATEN) {
        // Reflection on SAFESTATEN safe state enable pin detected
        // Perform necessary actions
    }

    return errNum;
}


uint8_t checkLossOfCPGND(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, DIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & LOG_CP) == LOG_CP) {
        // Loss of ground connection on CPGND detected
        // Perform necessary actions
    }

    return errNum;
}

uint8_t checkLossOfGND(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, DIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & LOG_D) == LOG_D) {
        // Loss of ground connection on GND detected
        // Perform necessary actions
    }

    return errNum;
}

uint8_t checkLossOfGNDA(Fetcontroller fet_spi) {
    uint8_t errNum = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    status = Fetcontroller_ReadRegister(fet_spi, DIAG, &regData, 1);
    errNum += (status != HAL_OK);

    if ((regData & LOG_A) == LOG_A) {
        // Loss of ground connection on GNDA detected
        // Perform necessary actions
    }

    return errNum;
}






