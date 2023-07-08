/*
 * commands.h
 *
 *  Created on: 10 june 2023
 *      Author: Chokri
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

void wait(uint32_t time);


void executeCommand(char *cmd_string);
void showHelp(void);
void setEnable(char *arg);
void setSafestaten(char *arg);
void setChannelA(char *arg);
void setChannelB(char *arg);
void pulseChannelA(char *arg);
void burstChannelA(char *arg);
void toggleChannelA(void);
void pulseChannelB(char *arg);
void burstChannelB(char *arg);
void toggleChannelB(void);
void clearFailures(void);
void driverInit(void);
void printTemperature(void);
void printVbat(void);
void printVld(void);
void printVoltage(void);
void printIshunt(void);
void printIhall(void);
void printCurrent(void);
void precharge(void);
void setCSAG(char *arg);
void setOCTH(char *arg);
void setUVRT(char *arg);
void setOVRT(char *arg);
void setDSOV(char *arg1, char *arg2);
void setSST(char *arg1, char *arg2);
void setBLK(char *arg1, char *arg2);
void setFLT(char *arg1, char *arg2);
void setXCTR(char *arg);
void setCSHS(char *arg);
void setCSLD(char *arg);
void setConfig(char *arg, uint8_t reg_num, uint8_t bitmask, uint8_t bitpos);
void setConfigBit(char *arg, uint8_t reg_num, uint8_t bit);
void loadConfig(void);
void factoryConfig(void);
void saveConfigToFlash(void);
void saveInitConfig(void);
void printConfig(void);
void checkButtons(void);
void getAdcResults(void);
void setLEDs(void); 

void switchOnWithPrecharge(void);
void switchOff(void);
void setPrechargeTime(char *arg);
uint16_t getPrechargeTime(void);

void setHallSensitivity(char *arg);
uint16_t getHallSensitivity(void);

void ISR_timerChannelA(void);
void ISR_timerChannelB(void);
void ISR_timerPrecharge(void);



#endif /* COMMANDS_H_ */
