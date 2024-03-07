/*
 * registers.h
 *
 *  Created on: 21 Feb 2020

 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

/* Register Numbers */
#define STDIAG          0x00
#define CHDIAG          0x01
#define DIAG            0x02
#define MOS_CHS_CTRL    0x03
#define FAILURECLEAN    0x04
#define VDSTHA_B        0x05
#define MOSFLTBLKA_B    0x06
#define CSAG_OCTH       0x07
#define VBATOVUVRST     0x08
#define RESETS          0x09
#define SPARE_REG       0x0A

/* Bit Numbers */

/* Register STDIAG */
#define VBAT_OV        0x01  // LSB : VBAT overvoltage failure (not latched)
#define VBAT_UV        0x02  // VBAT undervoltage failure (not latched)
#define VDD_UV         0x04  // VDD undervoltage propagated in the first command and indicates an under voltage event
#define TSD            0x08  // Chip in Overtemperature (latched)
#define OT_WARNING     0x10  // Temperature warning
#define MEM_FAIL       0x20  // Error in the memory, trimming not possible (flag cannot be cleaned)
#define VCP_READY      0x40  // Charge pump is ready
#define FAILURE        0x80  // MSB : Main Failure indication, 1 when there is a failure

/* Register CHDIAG */
#define VSOURCE_A      0x01 // LSB : Source overvoltage - Channel_A
#define VDSTRIP_A      0x02 // Drain to Source overvoltage - Channel_A (latched)
#define VGSTH_A        0x04 // Gate-Source undervoltage - Channel_A (latched)
#define VSOURCE_B      0x08 // Source overvoltage - Channel_B
#define VDSTRIP_B      0x10 // Drain to Source overvoltage - Channel_B (latched)
#define VGSTH_B        0x20 // Gate-Source undervoltage - Channel_B (latched)
#define ITRIP          0x40 // Overcurrent failure (latched)
#define VCP_UV         0x80 // MSB : Charge pump undervoltage

/* Register DIAG */
#define SAFESTATEN     0x01 // LSB : SAFESTATEN
#define ADD_NOT_AVAIL  0x02 // Address not available
#define LOG_CP         0x04 // Loss of ground
#define LOG_D          0x08 // Loss of ground
#define LOG_A          0x10 // Loss of ground
#define D5             0x20 // Not assigned
#define D6             0x40 // Not assigned
#define D7             0x80 // Not assigned

/* Register MOS_CHS_CTRL */
#define MOSONCH_A      0x01 // LSB : Switch on channel A
#define VDSA_CL        0x02 // Clear VDS flag Channel A
#define VGSTH_A_CL     0x04 // Clear VGS flag Channel A
#define MOSONCH_B      0x08 // Switch on channel B
#define VDSB_CL        0x10 // Clear VDS flag Channel B
#define VGSTH_B_CL     0x20 // Clear VGS flag Channel B
#define ITRIP_CL       0x40 // Clear ITRIP flag
#define CHCRCTRL       0x80 // MSB : Channel cross control activation
#define MOS_CTRL_FAILURE_CLEAR  0x76 // all failure bits

/* Register FAILURE_CLEAN */
#define VBATUV_CL      0x01 // LSB : Clear VBAT undervoltage failure
#define VBATOV_CL      0x02 // Clear VBAT overvoltage failure
#define VDD_UV_CL      0x04 // Clear Vdd undervoltage failure
#define TSD_CL         0x08 // Clear overtemperature failure
#define CLEAR_INT      0x10 // Clear interrupt
#define SAFESTATE_CL   0x20 // Safe Satate clear
#define D6             0x40 // Not assigned
#define VCPUV_CL       0x80 // MSB : VCP under voltage clear

/* Register VDSTHA_B */
#define VDSTH_A_0      0x01  // LSB : Drain-Source overvoltage threshold Channel_A
#define VDSTH_A_1      0x02  // Drain-Source overvoltage threshold Channel_A
#define VDSTH_A_2      0x04  // Drain-Source overvoltage threshold Channel_A
#define VDSTH_A_MASK   0x07  // Bitmask
#define VDSTH_A_POS    0     // Position of bitmask lsb
#define VDSA_SS        0x08  // VDS Channel A safe state: ON when 0, OFF when 1 (default)
#define VDSTH_B_0      0x10  // Drain-Source overvoltage threshold Channel_B
#define VDSTH_B_1      0x20  // Drain-Source overvoltage threshold Channel_B
#define VDSTH_B_2      0x40  // Drain-Source overvoltage threshold Channel_B
#define VDSTH_B_MASK   0x70  
#define VDSTH_B_POS    4     
#define VDSB_SS        0x80  // MSB : VDS Channel B safe state: ON when 0, OFF when 1 (default)

/* Register MOSFLTBLKA_B */
#define MOSFLT_A_0     0x01 // LSB : MOS voltage filter Channel_A
#define MOSFLT_A_1     0x02 // MOS voltage filter Channel_A
#define MOSFLT_A_MASK  0x03 
#define MOSFLT_A_POS   0    
#define MOSBLK_A_0     0x04 // MOS voltage blank time Channel_A
#define MOSBLK_A_1     0x08 // MOS voltage blank time Channel_A
#define MOSBLK_A_MASK  0x0C 
#define MOSBLK_A_POS   2 
#define MOSFLT_B_0     0x10 // MOS voltage filter Channel_B
#define MOSFLT_B_1     0x20 // MOS voltage filter Channel_B
#define MOSFLT_B_MASK  0x30 
#define MOSFLT_B_POS   4 
#define MOSBLK_B_0     0x40 // MOS voltage blank time Channel_B
#define MOSBLK_B_1     0x80 // MSB : MOS voltage blank time Channel_B
#define MOSBLK_B_MASK  0xC0 
#define MOSBLK_B_POS   6

/* Register CSAG_OCTH */
#define CSAG_0         0x01 // LSB : Current sense amplifier gain GDIFF
#define CSAG_1         0x02 // LSB : Current sense amplifier gain GDIFF
#define CSAG_2         0x04 // LSB : Current sense amplifier gain GDIFF
#define CSAG_MASK      0x07 
#define CSAG_POS       0 
#define OCTH_0         0x08 // Over current detection thresholds
#define OCTH_1         0x10 // Over current detection thresholds
#define OCTH_MASK      0x18 
#define OCTH_POS       3 
#define CSA_HSS        0x20 // Control signal to select the CSA configuration LSS or HSS(or bidirectional)
#define CSA_COUTSEL    0x40 // Configures opamp output stage vs load cap
#define D7             0x80 // MSB : Not assigned

/* Register VBATOVUVRST */
#define VBATOVARST_0    0x01 // LSB : VBAT overvoltage auto-restart time
#define VBATOVARST_1    0x02 // VBAT overvoltage auto-restart time
#define VBATOVARST_MASK 0x03 
#define VBATOVARST_POS  0 
#define VBATUVARST_0    0x04 // Over VBAT undervoltage auto-restart time
#define VBATUVARST_1    0x08 // Over VBAT undervoltage auto-restart time
#define VBATUVARST_MASK 0x0C 
#define VBATUVARST_POS  2 
#define D4              0x10 // Not assigned
#define D5              0x20 // Not assigned
#define D6              0x40 // Not assigned
#define D7              0x80 // Not assigned


/* configuration registers in EEPROM emulation */
#define EE_ARRAY_OFFSET         0
#define EE_ARRAY_SIZE           7

#define EE_ADDR_VDSTHA_B        0
#define EE_ADDR_MOSFLTBLKA_B    1
#define EE_ADDR_CSAG_OCTH       2
#define EE_ADDR_VBATOVUVRST     3
#define EE_ADDR_MOS_CHS_CTRL    4

#define EE_PRECH_TIME_UPPER     5 // upper 8 bits of 16 bit precharge time
#define EE_PRECH_TIME_LOWER     6 // lower 8 bits of 16 bit precharge time

#define EE_HALL_SENSITIVITY_UPPER     7 // upper 8 bits of 16 bit hall sensitivity
#define EE_HALL_SENSITIVITY_LOWER     8 // lower 8 bits of 16 bit hall sensitivity

/* factory settings */
#define STDIAG_FACTORY_SETTING          0x00
#define CHDIAG_FACTORY_SETTING          0x00
#define DIAG_FACTORY_SETTING            0x00

#define MOS_CHS_CTRL_FACTORY_SETTING    0x80
#define FAILURECLEAN_FACTORY_SETTING    0x00
#define VDSTHA_B_FACTORY_SETTING        0x33
#define MOSFLTBLKA_B_FACTORY_SETTING    0xAA
#define CSAG_OCTH_FACTORY_SETTING       0x56
#define VBATOVUVRST_FACTORY_SETTING     0x00
#define RESETS_FACTORY_SETTING          0x00
#define SPARE_REG_FACTORY_SETTING       0xF0

#define PRECHARGE_TIME_FACTORY_SETTING  "500"   // ms
#define HALL_SENSITIVITY_FACTORY_SETTING "4800" // µV/A



/* global flags */
uint8_t driverInterruptOccurred;
uint8_t failureBitWasSet;

/* function prototypes */
const char* getRegisterName(uint8_t reg_num);
uint8_t setRegisterData(uint8_t reg_num, uint8_t reg_data);
uint8_t getRegisterData(uint8_t reg_num);
uint8_t setInitRegisterData(uint8_t reg_num, uint8_t reg_data);
uint8_t getInitRegisterData(uint8_t reg_num);
const char* getBitName(uint8_t reg_num, uint8_t bit_num);
void setBits(uint8_t reg_num, uint8_t bitmask);
void clearBits(uint8_t reg_num, uint8_t bitmask);
void readRegister(char *arg);
void readAllRegisters(void);
void writeRegister(char *adr, char *dat);
void printRegister(uint8_t reg_num);
void reportChanges();
void reportFailures();
void setRegisterDefaultValues(void);
int xtoi(char *hexstring);
int spiRead(uint8_t adr, uint8_t *data);
int spiWrite(uint8_t adr, uint8_t data);

#endif /* REGISTERS_H_ */

