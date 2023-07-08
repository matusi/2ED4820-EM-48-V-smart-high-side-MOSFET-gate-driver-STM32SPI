/*
 * helpercmds.h
 *
 *  Created on: Jun 19, 2023
 *      Author: Gaming
 */

#ifndef INC_HELPERCMDS_H_
#define INC_HELPERCMDS_H_




#include <stdio.h>
#include <stdint.h>

#include "spi.h"

#include "gpio.h"
#include "tim.h"

/* Private includes ----------------------------------------------------------*/
///* USER CODE BEGIN Includes */
//#include "registers.h"
#include "stm32g0xx_hal.h"


#include"helperRegisters.h"


#include "registers.h"


enum VBatOVRsttime {
    VBatOVRst_10us = 0x00,
    VBatOVRst_50us = 0x01,
    VBatOVRst_200us = 0x10,
    VBatOVRst_1ms = 0x11
};

enum VBatUVRsttime {
    VBatUVRst_1ms = 0x00 << 2,
    VBatUVRst_5ms = 0x01 << 2,
    VBatUVRst_20ms = 0x10 << 2,
    VBatUVRst_50ms = 0x11 << 2
};


enum MosFLT_A {
	MosFLT_A_05us = 0x00,
	MosFLT_A_1us = 0x01,
	MosFLT_A_2us = 0x02,
	MosFLT_A_5us = 0x03
};


enum MosBLK_A {
    MosBLK_A_10us = 0x00 << 2,
    MosBLK_A_20us = 0x01 << 2,
    MosBLK_A_50us = 0x02 << 2,
    MosBLK_A_100us = 0x03 << 2
};



enum MosFLT_B {
	MosFLT_B_05us = 0x00 << 4,
	MosFLT_B_1us = 0x01 << 4,
	MosFLT_B_2us = 0x02 << 4,
	MosFLT_B_5us = 0x03 << 4
};


enum MosBLK_B {
    MosBLK_B_10us = 0x00 <<6 ,
    MosBLK_B_20us = 0x01 << 6,
    MosBLK_B_50us = 0x02 << 6,
    MosBLK_B_100us = 0x03 << 6
};


enum VDS_TH_A {
	VDS_TH_A_100mv = 0x00 ,
	VDS_TH_A_150mv = 0x01 ,
	VDS_TH_A_200mv = 0x02 ,
	VDS_TH_A_250mv = 0x03 ,
	VDS_TH_A_300mv = 0x04,
	VDS_TH_A_400mv = 0x05 ,
	VDS_TH_A_500mv = 0x06 ,
	VDS_TH_A_600mv = 0x07
};



enum VDS_TH_B {
    VDS_TH_B_100mv = 0x00 << 4,
    VDS_TH_B_150mv = 0x01 << 4,
    VDS_TH_B_200mv = 0x02 << 4,
    VDS_TH_B_250mv = 0x03 << 4,
    VDS_TH_B_300mv = 0x04 << 4,
    VDS_TH_B_400mv = 0x05 << 4,
    VDS_TH_B_500mv = 0x06 << 4,
    VDS_TH_B_600mv = 0x07 << 4
};



enum CSA_GAIN {
	CSA_GAIN10VperV = 0x00 ,
	CSA_GAIN15VperV = 0x01 ,
	CSA_GAIN20VperV = 0x02 ,
	CSA_GAIN25VperV = 0x03 ,
	CSA_GAIN31VperV = 0x04,
	CSA_GAIN35VperV = 0x05 ,
	CSA_GAIN40VperV = 0x06 ,
	CSA_GAIN48VperV = 0x07
};


reg8bits regg  ={0} ;
STDIAG_reg_cmds stdiag ={0} ;
CHDIAG_reg_cmds chdiag  ={0} ;
DIAG_reg_cmds diag  ={0} ;


MOS_CHS_CTRL_reg_cmds mosctr  ={0} ;
FAILURE_CLEAN_reg_cmds fauls  ={0} ;
VDSTHA_B_reg_cmds vdsth ={0};

MOSFLTBLKA_B_reg_cmds  mosfltbl ={0} ;
CSAG_OCTH_reg_cmds   csag  ={0} ;
VBATOVUVRST_reg_cmds  vbatouv  ={0} ;
RESETS_reg_cmds 	resets ={0} ;
SPARE_reg_cmds  spars  ={0} ;


///****************************************************************************
 ////READ ALL
 ////****************************************************************************


void InfinionReadall (void);



///****************************************************************************
 ////SET CH A
 ////****************************************************************************
void InfinionSetChAon(void);
void InfinionSetChAoff(void);




///****************************************************************************
 ////SET CH B
 ////****************************************************************************
void InfinionSetChBon(void);
void InfinionSetChBoff(void);


///****************************************************************************
 ////CLEAR FAILURES
 ////****************************************************************************
void InfinionClearfailures(void);


///****************************************************************************
 ////Set Thresold CH A
 ////****************************************************************************
void InfinionSetThreshold_ChA(uint8_t thresconf);


///****************************************************************************
 ////Set Thresold CH B
 ////****************************************************************************
void InfinionSetThreshold_ChB(uint8_t thresconf);

///****************************************************************************
 ////Set Filter and blank time CHA
 ////****************************************************************************
void InfinionSetFltBlktime_CHA(uint8_t fltconf, uint8_t blktimeconf);


///****************************************************************************
 ////Set Filter and blank time CHB
 ////****************************************************************************
void InfinionSetFltBlktime_CHB(uint8_t fltconf, uint8_t blktimeconf);




///****************************************************************************
 ////Set Overvoltage reset time
 ////****************************************************************************
void InfinionSetOverVlt(uint8_t overvltconf);

///****************************************************************************
////Set Undervoltage reset time
 ////****************************************************************************
void InfinionSetUnderVlt(uint8_t undervltconf);



///****************************************************************************
 ////Configure Current sens gain
 ////****************************************************************************


void InfinionSetCurrsensGain(uint8_t Currgainconf);





#endif /* INC_HELPERCMDS_H_ */
