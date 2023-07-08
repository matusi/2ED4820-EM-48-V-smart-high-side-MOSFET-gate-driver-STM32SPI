/*
 * commands.c
 *
 *  Created on: 4 Mar 2020
 *      Author: schwarzg
 */
#include <DAVE.h> //Declarations from DAVE Code Generation (includes SFR declaration)
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h> // for va list in uartPrintff
#include <stdlib.h> // includes strtol
#include "uart.h"
#include "registers.h"
#include "commands.h"
#include "analog.h"
#include "shell.h"

void wait(uint32_t time)
{
	while (time > 0)
	{
		__NOP();
		time--;
	}
}

//****************************************************************************
// executeCommand
//
// analyzes the command string and calls command functions
//****************************************************************************
void executeCommand(char *cmd_string)
{
	static char cmd[CMD_BUF_LEN];
	static char arg1[CMD_BUF_LEN];
	static char arg2[CMD_BUF_LEN];

	cmd[0] = '\0';
	arg1[0] = '\0';
	arg2[0] = '\0';

	// extract command and arguments from command string
	sscanf(cmd_string, "%s %s %s", cmd, arg1, arg2);

	// convert strings to lowercase
	strlwr(cmd);
	strlwr(arg1);
	strlwr(arg2);

	// choose function to be executed
	if (!strcmp(cmd, "?") || !strcmp(cmd, "help"))
		showHelp();
	else if (!strcmp(cmd, "e")) // enable 0/1
		setEnable(arg1);
	else if (!strcmp(cmd, "s")) // safe state enable 0/1
		setSafestaten(arg1);
	else if (!strcmp(cmd, "r")) // read register; arg1 = address
		readRegister(arg1);
	else if (!strcmp(cmd, "w")) // write register; arg1 = address, arg2 = data
		writeRegister(arg1, arg2);
	else if (!strcmp(cmd, "cf")) // clear failures
		clearFailures();
	else if (!strcmp(cmd, "a")) // channel A 0/1
		setChannelA(arg1);
	else if (!strcmp(cmd, "b")) // channel B 0/1
		setChannelB(arg1);
	else if (!strcmp(cmd, "pa")) // pulse on channel A
		pulseChannelA(arg1);
	else if (!strcmp(cmd, "ba")) // burst on channel A
		burstChannelA(arg1);
	else if (!strcmp(cmd, "pb")) // pulse on channel B
		pulseChannelB(arg1);
	else if (!strcmp(cmd, "bb")) // burst on channel B
		burstChannelB(arg1);
	else if (!strcmp(cmd, "on")) // precharge and then switch on
		switchOnWithPrecharge();
	else if (!strcmp(cmd, "off")) // switch off both channels and disable
		switchOff();
	else if (!strcmp(cmd, "t")) // print NTC temperature
		printTemperature();
	else if (!strcmp(cmd, "v")) // print voltage
		printVoltage();
	else if (!strcmp(cmd, "i")) // print current
		printCurrent();
	else if (!strcmp(cmd, "csag")) // current sense gain; arg1 = gain setting (0..7)
		setCSAG(arg1);
	else if (!strcmp(cmd, "octh")) // overcurrent threshold; arg1 = setting (0..3)
		setOCTH(arg1);
	else if (!strcmp(cmd, "uvrt")) // undervoltage reset threshold; arg1 = setting (0..3)
		setUVRT(arg1);
	else if (!strcmp(cmd, "ovrt")) // overvoltage reset threshold; arg1 = setting (0..3)
		setOVRT(arg1);
	else if (!strcmp(cmd, "dsov")) // VDS overvoltage threshold; arg1 = a/b,  arg2 = 0..7
		setDSOV(arg1, arg2);
	else if (!strcmp(cmd, "sst")) // Safe state; arg1 = a/b,  arg2 = 0/1
		setSST(arg1, arg2);
	else if (!strcmp(cmd, "blk")) // blanking time; arg1 = a/b,  arg2 = 0..3
		setBLK(arg1, arg2);
	else if (!strcmp(cmd, "flt")) // blanking time; arg1 = a/b,  arg2 = 0..3
		setFLT(arg1, arg2);
	else if (!strcmp(cmd, "xctr")) // channel cross control; arg1 = 0/1
		setXCTR(arg1);
	else if (!strcmp(cmd, "cshs")) // current sense on high side; arg1 = 0/1
		setCSHS(arg1);
	else if (!strcmp(cmd, "csld")) // current sense output load >100pF; arg1 = 0/1
		setCSLD(arg1);
	else if (!strcmp(cmd, "pt")) // set the precharge time
		setPrechargeTime(arg1);
	else if (!strcmp(cmd, "hss")) // set hall sensor sensitivity (µV/A)
		setHallSensitivity(arg1);
	else if (!strcmp(cmd, "c"))   // print configuration
		printConfig();
	else if (!strcmp(cmd, "fc")) // apply factory configuration
		factoryConfig();
	else if (!strcmp(cmd, "sc")) // save configuration to flash memory
		saveConfigToFlash();
	else if (!strcmp(cmd, "lc")) // load configuration from RAM buffer
		loadConfig();
	else if (!strcmp(cmd, "init")) // initialise driver
		driverInit();
	else if (!strcmp(cmd, ""))
		;
	// do nothing -  just a new line;
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Unknown command \"%s\"", cmd_string);
	}

	/*
    else if (!strcmp(cmd,"vb")) // print battery voltage
        printVbat();
    else if (!strcmp(cmd,"vl")) // print load voltage
        printVld();
    else if (!strcmp(cmd,"is")) // print load current (shunt)
        printIshunt();
    else if (!strcmp(cmd,"ih")) // print load current (hall sensor)
        printIhall();
	 */
}

//****************************************************************************
// showHelp
//
// shows a list of available commands
//****************************************************************************
void showHelp(void)
{
	uartPrintf("\r\n");
	uartPrintf("48V Battery Switch command line interface, Rev. 1.0, 2022-02-02\r\n");
	uartPrintf("--------------------------------------------------------------------------------------\r\n");
	uartPrintf("Command Argument1  Argument2  Example          Description\r\n");
	uartPrintf("--------------------------------------------------------------------------------------\r\n");
	uartPrintf("?       -          -          '?'              show available commands\r\n");
	uartPrintf("on      -          -          'on'             switch on (incl. init & precharge)\r\n");
	uartPrintf("off     -          -          'off'            switch off & disable\r\n");
	uartPrintf("e       -/0/1      -          'e'/'e 0'/'e 1'  enable pin toggle/low/high\r\n");
	uartPrintf("s       -/0/1      -          's'/'s 0'/'s 1'  safestaten pin toggle/low/high\r\n");
	uartPrintf("r       adr/all    -          'r 5' 'r all'    read register / read all registers\r\n");
	uartPrintf("w       adr        data (hex) 'w 4 ff'         write register\r\n");
	uartPrintf("cf      -          -          'cf'             clear failures\r\n");
	uartPrintf("a       -/0/1      -          'a'/'a 0'/'a 1'  channel A toggle/low/high\r\n");
	uartPrintf("b       -/0/1      -          'b'/'b 0'/'b 1'  channel B toggle/low/high\r\n");
	uartPrintf("pa      time[ms]   -          'pa 10'          generate a pulse on channel A\r\n");
	uartPrintf("pb      time[ms]   -          'pb 15'          generate a pulse on channel B\r\n");
	uartPrintf("v       -          -          'v'              show Vbat and Vld\r\n");
	uartPrintf("i       -          -          'i'              show Ishunt and Ihall\r\n");
	uartPrintf("t       -          -          't'              show NTC temperature\r\n");
	uartPrintf("--------------------- configuration settings -----------------------------------------\r\n");
	uartPrintf("uvrt    0..3       -          'uvrt 2'         vbat undervoltage restart time\r\n");
	uartPrintf("ovrt    0..3       -          'ovrt 2'         vbat overvoltage restart time\r\n");
	uartPrintf("dsov    a/b        0..7       'dsov a 2'       vds overvoltage threshold ch. A/B\r\n");
	uartPrintf("sst     a/b        0/1        'sst a 1'        safe state ch. A/B\r\n");
	uartPrintf("blk     a/b        0..3       'blk b 2'        blanking time ch. A/B\r\n");
	uartPrintf("flt     a/b        0..3       'flt a 2'        filter time ch. A/B\r\n");
	uartPrintf("pt      time[ms]   -          'pt 500'         precharge time in ms for 'on' command\r\n");
	uartPrintf("xctr    0/1        -          'xctr 0'         channel cross control\r\n");
	uartPrintf("cshs    0/1        -          'cshs 0'         current sense on high side\r\n");
	uartPrintf("csld    0/1        -          'csld 1'         current sense output load (1: >100pF)\r\n");
	uartPrintf("csag    0..7       -          'csag 3'         current sense amplifier gain\r\n");
	uartPrintf("octh    0..3       -          'octh 2'         overcurrent threshold\r\n");
	uartPrintf("hss     time[ms]   -          'hss 4800'       hall sensor sensitivity in µV/A\r\n");
	uartPrintf("c       -          -          'c'              show configuration\r\n");
	uartPrintf("fc      -          -          'fc'             factory configuration\r\n");
	uartPrintf("sc      -          -          'sc'             save configuration to uC flash memory\r\n");
	uartPrintf("lc      -          -          'lc'             load configuration from uC flash memory\r\n");
	uartPrintf("init    -          -          'init'           initialise driver with last config\r\n");
	uartPrintf("--------------------------------------------------------------------------------------");

	/* commented out for clarity
	 * stored here for later use
    uartPrintf("48V Battery Switch command line interface, Rev. 1.0, 2020-08-27\r\n");
    uartPrintf("help    -          -          'help'           show available commands\r\n");
    uartPrintf("ba      n          -          'ba 10'          burst with n pulses on channel a \r\n");
    uartPrintf("bb      n          -          'bb 10'          burst with n pulses on channel b \r\n");
    uartPrintf("vb      -          -          'vb'             show battery voltage VBAT\r\n");
    uartPrintf("vl      -          -          'vl'             show load voltage VLD\r\n");
    uartPrintf("is      -          -          'is'             show load current (shunt)\r\n");
    uartPrintf("ih      -          -          'ih'             show load current (hall sensor)\r\n");
	 */
}

//****************************************************************************
// setEnable
//
// sets the ENABLE pin high or low
// LED2 displays the status of ENABLE
//****************************************************************************
void setEnable(char *arg)
{
	if (!strcmp(arg, "0"))
	{
		DIGITAL_IO_SetOutputLow(&ENABLE);
	}
	else if (!strcmp(arg, "1"))
	{
		// save configuration for next driver initialisation
		saveInitConfig();
		// enable driver
		DIGITAL_IO_SetOutputHigh(&ENABLE);
		// driver is in reset state after enabling
		// set register default values to avoid changed bit messages
		setRegisterDefaultValues();
	}
	else if (!strcmp(arg, ""))
	{
		if (DIGITAL_IO_GetInput(&ENABLE)) // driver is enabled
		{
			// disable driver
			DIGITAL_IO_SetOutputLow(&ENABLE);
			// append enable pin setting to command
			uartPrintf(" 0");
		}
		else // driver is disabled
		{
			// save configuration for next driver initialisation
			saveInitConfig();
			// enable driver
			DIGITAL_IO_SetOutputHigh(&ENABLE);
			// driver is in reset state after enabling
			// set register default values to avoid changed bit messages

			// wait for the driver to power up
			wait(5000);

			// driver is enabled and switches are still off (load current = 0)
			// this is a good time to measure the actual cso output reference voltage (typ. vdd/2)
			// to avoid offset errors
			setCsoReferenceVoltage();

			setRegisterDefaultValues();
			// append enable pin setting to command
			uartPrintf(" 1");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Invalid argument");
	}
}

//****************************************************************************
// setSafestaten
//
// sets the SAF pin high or low
//****************************************************************************
void setSafestaten(char *arg)
{
	if (!strcmp(arg, "0"))
	{
		DIGITAL_IO_SetOutputLow(&SAF);
	}
	else if (!strcmp(arg, "1"))
	{
		DIGITAL_IO_SetOutputHigh(&SAF);
	}
	else if (!strcmp(arg, ""))
	{
		DIGITAL_IO_ToggleOutput(&SAF);

		// report status of SAF pin by appending it to the command
		if (DIGITAL_IO_GetInput(&SAF))
		{
			uartPrintf(" 1");
		}
		else
		{
			uartPrintf(" 0");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Invalid argument");
	}
}

//****************************************************************************
// setChannelA
//
// set channel A on or off
//****************************************************************************
void setChannelA(char *arg)
{
	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		if (!strcmp(arg, "0")) // OFF
		{
			clearBits(MOS_CHS_CTRL, MOSONCH_A); // switch off chan A
		}
		else if (!strcmp(arg, "1")) // ON
		{
			setBits(MOS_CHS_CTRL, MOSONCH_A); // switch on chan A
		}
		else if (!strcmp(arg, "")) // toggle
		{
			if (getRegisterData(MOS_CHS_CTRL) & MOSONCH_A) // channel A is ON
			{
				uartPrintf(" 0");
				clearBits(MOS_CHS_CTRL, MOSONCH_A); // switch off chan A
			}
			else // channel A is OFF
			{
				uartPrintf(" 1");

				setBits(MOS_CHS_CTRL, MOSONCH_A); // switch on chan A

				// check if channel really was switched on
				readAllRegisters();

				if ((getRegisterData(MOS_CHS_CTRL) & MOSONCH_A) == 0) // channel A is off
				{
					uartPrintf("\r\n"); // new line
					uartPrintf("Couldn't switch on channel A");
					failureBitWasSet = 1; // will trigger reporting of failures
				}
			}
		}
		else
		{
			uartPrintf("\r\n"); // new line
			uartPrintf("[ERROR]: Invalid argument");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("Couldn't switch on channel A - Driver is disabled");
	}
}

//****************************************************************************
// setChannelB
//
// set channel B on or off
//****************************************************************************
void setChannelB(char *arg)
{
	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		if (!strcmp(arg, "0")) // OFF
		{
			clearBits(MOS_CHS_CTRL, MOSONCH_B); // switch off chan B
		}
		else if (!strcmp(arg, "1")) // ON
		{
			setBits(MOS_CHS_CTRL, MOSONCH_B); // switch on chan B
		}
		else if (!strcmp(arg, "")) // toggle
		{
			if (getRegisterData(MOS_CHS_CTRL) & MOSONCH_B) // channel B is ON
			{
				uartPrintf(" 0");
				clearBits(MOS_CHS_CTRL, MOSONCH_B); // switch off chan B
			}
			else // channel B is OFF
			{
				uartPrintf(" 1");
				setBits(MOS_CHS_CTRL, MOSONCH_B); // switch on chan B

				// check if channel really was switched on
				readAllRegisters();

				if ((getRegisterData(MOS_CHS_CTRL) & MOSONCH_B) == 0) // channel B is off
				{
					uartPrintf("\r\n"); // new line
					uartPrintf("Couldn't switch on channel B");
					failureBitWasSet = 1; // will trigger reporting of failures
				}
			}
		}
		else
		{
			uartPrintf("\r\n"); // new line
			uartPrintf("[ERROR]: Invalid argument");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("Couldn't switch on channel B - Driver is disabled");
	}
}

//****************************************************************************
// pulseChannelA
//
// generate a pulse on channel A
//****************************************************************************
void pulseChannelA(char *arg)
{
	static uint32_t pulse_time = 0;

	pulse_time = atoi(arg); // convert string to integer

	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		if (pulse_time > 0 && pulse_time <= 10000)
		{
			// pulse time is in ms, timer expects time in 0.01 µs
			// (there is a scaling factor of 100
			//  e.g. 10.56 us --> value = 1056)
			pulse_time *= 100000;

			TIMER_Clear(&TIMER_A);
			TIMER_SetTimeInterval(&TIMER_A, pulse_time);
			setBits(MOS_CHS_CTRL, MOSONCH_A); // switch on chan A
			TIMER_Start(&TIMER_A);			  // start timer

			reportChanges(); // make sure the setting gets reported

			// when the timer is expired it will trigger an interrupt
			// the interrupt will call ISR_timerChannelA()
			// channel A is switched of in this function
		}
		else
		{
			uartPrintf("\r\n"); // new line
			uartPrintf("[ERROR]: Pulse time out of range");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Driver is disabled");
	}
}

//****************************************************************************
// burstChannelA
//
// generate number of pulses as fast as possible on channel A
//****************************************************************************
void burstChannelA(char *arg)
{
	static uint32_t pulse_count = 0;
	static uint32_t i = 0;

	pulse_count = atoi(arg); // convert string to integer

	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		if (pulse_count > 0 && pulse_count <= 10000)
		{
			for (i = 0; i < pulse_count; i++)
			{
				setBits(MOS_CHS_CTRL, MOSONCH_A);	// switch on chan A
				clearBits(MOS_CHS_CTRL, MOSONCH_A); //switch off chan A
			}
		}
		else
		{
			uartPrintf("\r\n"); // new line
			uartPrintf("[ERROR]: Pulse count out of range");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Driver is disabled");
	}
}

//****************************************************************************
// burstChannelB
//
// generate number of pulses as fast as possible on channel B
//****************************************************************************
void burstChannelB(char *arg)
{
	static uint32_t pulse_count = 0;
	static uint32_t i = 0;

	pulse_count = atoi(arg); // convert string to integer

	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		// max 100 pulses to protect precharge resistor
		if (pulse_count > 0 && pulse_count <= 100)
		{
			for (i = 0; i < pulse_count; i++)
			{
				setBits(MOS_CHS_CTRL, MOSONCH_B);	// switch on chan B
				clearBits(MOS_CHS_CTRL, MOSONCH_B); //switch off chan B
			}
		}
		else
		{
			uartPrintf("\r\n"); // new line
			uartPrintf("[ERROR]: Pulse count out of range");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Driver is disabled");
	}
}

//****************************************************************************
// pulseChannelB
//
// generate a pulse on channel B
//****************************************************************************
void pulseChannelB(char *arg)
{
	static uint32_t pulse_time = 0;

	pulse_time = atoi(arg); // convert string to integer

	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		if (pulse_time > 0 && pulse_time <= 10000)
		{
			// pulse time is in ms, timer expects time in 0.01 µs
			// (there is a scaling factor of 100
			//  e.g. 10.56 us --> value = 1056)
			pulse_time *= 100000;

			TIMER_Clear(&TIMER_B);
			TIMER_SetTimeInterval(&TIMER_B, pulse_time);
			setBits(MOS_CHS_CTRL, MOSONCH_B); // switch on chan B
			TIMER_Start(&TIMER_B);			  // start timer

			reportChanges(); // make sure the setting gets reported

			// when the timer is expired it will trigger an interrupt
			// the interrupt will call ISR_timerChannelB()
			// channel B is switched of in this function
		}
		else
		{
			uartPrintf("\r\n"); // new line
			uartPrintf("[ERROR]: Pulse time out of range");
		}
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Driver is disabled");
	}
}

//****************************************************************************
// switchOnWithPrecharge
//
// enable, initialise, precharge and switch on main channel
//****************************************************************************
void switchOnWithPrecharge(void)
{
	static uint32_t pulse_time; // precharge pulse time in ms

	if (DIGITAL_IO_GetInput(&ENABLE) == 0) // driver not enabled yet
	{
		driverInit();	 // initialise driver with last configuration
		reportChanges(); // make sure the setting gets reported
	}

	pulse_time = (uint32_t)getPrechargeTime();

	if (pulse_time > 0 && pulse_time <= 3000)
	{
		// pulse time is in ms, timer expects time in 0.01 µs
		// (there is a scaling factor of 100
		//  e.g. 10.56 us --> value = 1056)
		pulse_time *= 100000;

		TIMER_Clear(&TIMER_PRECHARGE);
		TIMER_SetTimeInterval(&TIMER_PRECHARGE, pulse_time);
		setBits(MOS_CHS_CTRL, MOSONCH_B); // switch on chan B (precharge)
		TIMER_Start(&TIMER_PRECHARGE);	  // start timer

		reportChanges(); // make sure the setting gets reported

		// when the timer is expired it will trigger an interrupt
		// the interrupt will call ISR_timerChannelA()
		// channel A is switched of in this function
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Precharge time out of range");
	}
}

//****************************************************************************
// switchOff
//
// switch off both channels and disable driver
//****************************************************************************
void switchOff(void)
{
	clearBits(MOS_CHS_CTRL, MOSONCH_B); // switch off chan B
	clearBits(MOS_CHS_CTRL, MOSONCH_A); // switch off chan A
	DIGITAL_IO_SetOutputLow(&ENABLE);	// disable driver
}

//****************************************************************************
// clearFailures
//****************************************************************************
void clearFailures(void)
{
	static uint8_t reg_data;

	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		// brute force approach:
		// set all bits that have to do with failure clearing
		spiWrite(FAILURECLEAN, 0xFF);
		spiWrite(MOS_CHS_CTRL, (getRegisterData(MOS_CHS_CTRL) | MOS_CTRL_FAILURE_CLEAR));

		// check if there are still failures present
		readAllRegisters();
		reg_data = getRegisterData(STDIAG);

		if (reg_data & FAILURE) // still failures present
		{
			uartPrintf("\r\n"); // new line
			uartPrintf("Couldn't clear all failures");
			failureBitWasSet = 1; // trigger reporting of remaining failures
		}
	}
}

//****************************************************************************
// printTemperature
//****************************************************************************
void printTemperature(void)
{
	static double ntc_temp;

	ntc_temp = getNtcTemperature();

	uartPrintf("\r\n"); // new line
	uartPrintf("%3.1f °C", ntc_temp);
}

//****************************************************************************
// printVbat
//****************************************************************************
void printVbat(void)
{
	static double vbat;

	vbat = getVbat();

	uartPrintf("\r\n"); // new line
	uartPrintf("%3.1f V", vbat);
}

//****************************************************************************
// printVld
//****************************************************************************
void printVld(void)
{
	static double vld;

	vld = getVld();

	uartPrintf("\r\n"); // new line
	uartPrintf("%3.1f V", vld);
}

//****************************************************************************
// printVoltage
//****************************************************************************
void printVoltage(void)
{
	uartPrintf("\r\n"); // new line
	uartPrintf("Vbat = %3.1f V\r\n", getVbat());
	uartPrintf("Vld = %3.1f V", getVld());
}

//****************************************************************************
// printIshunt
//****************************************************************************
void printIshunt(void)
{
	static double current;

	current = getIshunt();

	uartPrintf("\r\n"); // new line
	uartPrintf("%3.1f A", current);
}

//****************************************************************************
// printIhall
//****************************************************************************
void printIhall(void)
{
	static double current;

	current = getIhall();

	uartPrintf("\r\n"); // new line
	uartPrintf("%3.1f A", current);
}

//****************************************************************************
// printCurrent
//****************************************************************************
void printCurrent(void)
{
	uartPrintf("\r\n"); // new line
	uartPrintf("Ishunt = %3.1f A\r\n", getIshunt());
	uartPrintf("Ihall = %3.1f A", getIhall());
}

//****************************************************************************
// setPrechargeTime
//
// set precharge time in ms for 'on' command
// the value is stored in the EEPROM Ram buffer
// therefore the 16 bit value is split into two bytes
//****************************************************************************
void setPrechargeTime(char *arg)
{
	static uint16_t precharge_time = 0;
	static uint8_t upper; // upper 8 bit
	static uint8_t lower; // lower 8 bit

	precharge_time = atoi(arg); // convert string to integer

	if (precharge_time < 0 || precharge_time >= 3000)
	{
		uartPrintf("\r\n");
		uartPrintf("[ERROR]: Precharge time out of range");
	}
	else
	{
		upper = (precharge_time & 0xFF00) >> 8;
		lower = precharge_time & 0xFF;

		E_EEPROM_XMC4_WriteByte(EE_PRECH_TIME_UPPER, upper);
		E_EEPROM_XMC4_WriteByte(EE_PRECH_TIME_LOWER, lower);
	}
}

//****************************************************************************
// getPrechargeTime
//
// returns precharge time (stored in EEPROM Ram buffer)
//****************************************************************************
uint16_t getPrechargeTime()
{
	static uint16_t precharge_time;
	static uint8_t upper; // upper 8 bit
	static uint8_t lower; // lower 8 bit

	E_EEPROM_XMC4_ReadByte(EE_PRECH_TIME_UPPER, &upper);
	E_EEPROM_XMC4_ReadByte(EE_PRECH_TIME_LOWER, &lower);

	precharge_time = 0;
	precharge_time |= upper << 8; // set upper 8 bits
	precharge_time |= lower;	  // set lower 8 bits

	return precharge_time;
}



//****************************************************************************
// setHallSensitivity
//
// set hall sensor sensitivity in µV/A
// the value is stored in the EEPROM Ram buffer
// therefore the 16 bit value is split into two bytes
//****************************************************************************
void setHallSensitivity(char *arg)
{
	static uint16_t hall_sensitivity = 0;
	static uint8_t upper; // upper 8 bit
	static uint8_t lower; // lower 8 bit

	hall_sensitivity = atoi(arg); // convert string to integer

	if (hall_sensitivity > 0 && hall_sensitivity < 10000)
	{
		upper = (hall_sensitivity & 0xFF00) >> 8;
		lower = hall_sensitivity & 0xFF;

		E_EEPROM_XMC4_WriteByte(EE_HALL_SENSITIVITY_UPPER, upper);
		E_EEPROM_XMC4_WriteByte(EE_HALL_SENSITIVITY_LOWER, lower);
	}
	else
	{
		uartPrintf("\r\n");
		uartPrintf("[ERROR]: Hall sensitivity out of range");
	}
}

//****************************************************************************
// getHallSensitivity
//
// returns hall sensor sensitivity in µV/A (stored in EEPROM Ram buffer)
//****************************************************************************
uint16_t getHallSensitivity()
{
	static uint16_t hall_sensitivity;
	static uint8_t upper; // upper 8 bit
	static uint8_t lower; // lower 8 bit

	E_EEPROM_XMC4_ReadByte(EE_HALL_SENSITIVITY_UPPER, &upper);
	E_EEPROM_XMC4_ReadByte(EE_HALL_SENSITIVITY_LOWER, &lower);

	hall_sensitivity = 0;
	hall_sensitivity |= upper << 8; // set upper 8 bits
	hall_sensitivity |= lower;	  // set lower 8 bits

	return hall_sensitivity;
}




//****************************************************************************
// setConfig
//****************************************************************************
void setConfig(char *arg, uint8_t reg_num, uint8_t bitmask, uint8_t bitpos)
{
	static uint8_t reg_data;
	static uint8_t setting;
	static uint8_t max_setting;

	setting = atoi(arg);
	max_setting = (bitmask >> bitpos);

	if (setting >= 0 && setting <= max_setting)
	{

		reg_data = getRegisterData(reg_num); // get current register data
		reg_data &= ~bitmask;				 // clear configuration bits
		reg_data |= (setting << bitpos);	 // set selected configuration

		if (DIGITAL_IO_GetInput(&ENABLE))
			spiWrite(reg_num, reg_data); // write new register data
		else
			setRegisterData(reg_num, reg_data); // update shadow register
	}
	else
	{
		uartPrintf("\r\n"); // new line
		uartPrintf("[ERROR]: Setting out of range");
	}
}

//****************************************************************************
// setConfigBit
//****************************************************************************
void setConfigBit(char *arg, uint8_t reg_num, uint8_t bit)
{
	static uint8_t setting;

	setting = atoi(arg);

	if (setting == 0)
	{
		clearBits(reg_num, bit);
	}
	else if (setting == 1)
	{
		setBits(reg_num, bit);
	}
	else
	{
		uartPrintf("\r\n");
		uartPrintf("[ERROR]: Invalid setting");
	}
}

//****************************************************************************
// setCSAG
//****************************************************************************

void setCSAG(char *arg)
{
	setConfig(arg, CSAG_OCTH, CSAG_MASK, CSAG_POS);
}

//****************************************************************************
// setOCTH
//****************************************************************************
void setOCTH(char *arg)
{
	setConfig(arg, CSAG_OCTH, OCTH_MASK, OCTH_POS);
}

//****************************************************************************
// setUVRT
//****************************************************************************
void setUVRT(char *arg)
{
	setConfig(arg, VBATOVUVRST, VBATUVARST_MASK, VBATUVARST_POS);
}

//****************************************************************************
// setOVRT
//****************************************************************************
void setOVRT(char *arg)
{
	setConfig(arg, VBATOVUVRST, VBATOVARST_MASK, VBATOVARST_POS);
}

//****************************************************************************
// setDSOV
//****************************************************************************
void setDSOV(char *arg1, char *arg2)
{
	if (!strcmp(arg1, "a")) // Channel A
	{
		setConfig(arg2, VDSTHA_B, VDSTH_A_MASK, VDSTH_A_POS);
	}
	else if (!strcmp(arg1, "b")) // Channel B
	{
		setConfig(arg2, VDSTHA_B, VDSTH_B_MASK, VDSTH_B_POS);
	}
	else
	{
		uartPrintf("\r\n");
		uartPrintf("[ERROR]: Invalid channel");
	}
}

//****************************************************************************
// setSST
//****************************************************************************
void setSST(char *arg1, char *arg2)
{
	if (!strcmp(arg1, "a")) // Channel A
	{
		// VDS Channel A safe state: OFF when 0, ON when 1 (default)
		setConfigBit(arg2, VDSTHA_B, VDSA_SS);
	}
	else if (!strcmp(arg1, "b")) // Channel B
	{
		// VDS Channel B safe state: OFF when 0, ON when 1 (default)
		setConfigBit(arg2, VDSTHA_B, VDSB_SS);
	}
	else
	{
		uartPrintf("\r\n");
		uartPrintf("[ERROR]: Invalid channel");
	}
}

//****************************************************************************
// setBLK
//****************************************************************************
void setBLK(char *arg1, char *arg2)
{
	if (!strcmp(arg1, "a")) // Channel A
	{
		setConfig(arg2, MOSFLTBLKA_B, MOSBLK_A_MASK, MOSBLK_A_POS);
	}
	else if (!strcmp(arg1, "b")) // Channel B
	{
		setConfig(arg2, MOSFLTBLKA_B, MOSBLK_B_MASK, MOSBLK_B_POS);
	}
	else
	{
		uartPrintf("\r\n");
		uartPrintf("[ERROR]: Invalid channel");
	}
}

//****************************************************************************
// setFLT
//****************************************************************************
void setFLT(char *arg1, char *arg2)
{
	if (!strcmp(arg1, "a")) // Channel A
	{
		setConfig(arg2, MOSFLTBLKA_B, MOSFLT_A_MASK, MOSFLT_A_POS);
	}

	if (!strcmp(arg1, "b")) // Channel B
	{
		setConfig(arg2, MOSFLTBLKA_B, MOSFLT_B_MASK, MOSFLT_B_POS);
	}
}

//****************************************************************************
// setXCTR
//****************************************************************************
void setXCTR(char *arg)
{
	setConfigBit(arg, MOS_CHS_CTRL, CHCRCTRL);
}

//****************************************************************************
// setCSHS
//****************************************************************************
void setCSHS(char *arg)
{
	setConfigBit(arg, CSAG_OCTH, CSA_HSS);
}

//****************************************************************************
// setCSLD
//****************************************************************************
void setCSLD(char *arg)
{
	setConfigBit(arg, CSAG_OCTH, CSA_COUTSEL);
}

//****************************************************************************
// saveConfigToFlash
//
// save configuration to flash memory
//****************************************************************************
void saveConfigToFlash(void)
{
	// store current configuration in EEPROM RAM buffer
	E_EEPROM_XMC4_WriteByte(EE_ADDR_VDSTHA_B, getRegisterData(VDSTHA_B));
	E_EEPROM_XMC4_WriteByte(EE_ADDR_MOSFLTBLKA_B, getRegisterData(MOSFLTBLKA_B));
	E_EEPROM_XMC4_WriteByte(EE_ADDR_CSAG_OCTH, getRegisterData(CSAG_OCTH));
	E_EEPROM_XMC4_WriteByte(EE_ADDR_VBATOVUVRST, getRegisterData(VBATOVUVRST));
	// channel cross control configuration is just one bit in register MOS_CHS_CTRL
	E_EEPROM_XMC4_WriteByte(EE_ADDR_MOS_CHS_CTRL, (getRegisterData(MOS_CHS_CTRL) & CHCRCTRL));

	// write data to flash
	E_EEPROM_XMC4_UpdateFlashContents();

	// Note: since the precharge time is stored in the EEPROM RAM buffer as well
	// it will also be saved to flash by this function
}

//****************************************************************************
// factoryConfig
//
// apply factory configuration
//****************************************************************************
void factoryConfig(void)
{
	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		// if driver is enabled
		// apply configuration

		spiWrite(MOS_CHS_CTRL, MOS_CHS_CTRL_FACTORY_SETTING);
		spiWrite(VDSTHA_B, VDSTHA_B_FACTORY_SETTING);
		spiWrite(MOSFLTBLKA_B, MOSFLTBLKA_B_FACTORY_SETTING);
		spiWrite(CSAG_OCTH, CSAG_OCTH_FACTORY_SETTING);
		spiWrite(VBATOVUVRST, VBATOVUVRST_FACTORY_SETTING);
	}
	else
	{
		// if driver is disabled
		// just update the register data
		// for viewing configuration with command "c"

		setRegisterData(MOS_CHS_CTRL, MOS_CHS_CTRL_FACTORY_SETTING);
		setRegisterData(VDSTHA_B, VDSTHA_B_FACTORY_SETTING);
		setRegisterData(MOSFLTBLKA_B, MOSFLTBLKA_B_FACTORY_SETTING);
		setRegisterData(CSAG_OCTH, CSAG_OCTH_FACTORY_SETTING);
		setRegisterData(VBATOVUVRST, VBATOVUVRST_FACTORY_SETTING);

		// save configuration for initialisation
		saveInitConfig();
	}

	setPrechargeTime(PRECHARGE_TIME_FACTORY_SETTING);
	setHallSensitivity(HALL_SENSITIVITY_FACTORY_SETTING);
}

//****************************************************************************
// saveInitConfig
//
// save current configuration for next initialisation
//****************************************************************************
void saveInitConfig(void)
{
	setInitRegisterData(VDSTHA_B, getRegisterData(VDSTHA_B));
	setInitRegisterData(MOSFLTBLKA_B, getRegisterData(MOSFLTBLKA_B));
	setInitRegisterData(CSAG_OCTH, getRegisterData(CSAG_OCTH));
	setInitRegisterData(VBATOVUVRST, getRegisterData(VBATOVUVRST));
	// channel cross control configuration is just one bit in register MOS_CHS_CTRL
	setInitRegisterData(MOS_CHS_CTRL, (getRegisterData(MOS_CHS_CTRL) & CHCRCTRL));
}

//****************************************************************************
// loadConfig
//
// read configuration from flash memory
// apply configuration
//****************************************************************************
void loadConfig(void)
{
	static uint8_t config[EE_ARRAY_SIZE]; // buffer for configuration register data

	// read configuration data from eeprom RAM buffer
	// configuration registers are
	// VDSTHA_B, MOSFLTBLKA_B, CSAG_OCTH and VBATOVUVRST
	E_EEPROM_XMC4_ReadArray(EE_ARRAY_OFFSET, config, EE_ARRAY_SIZE); // (offset, buffer, size)

	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		// if driver is enabled
		// apply configuration

		spiWrite(VDSTHA_B, config[EE_ADDR_VDSTHA_B]);
		spiWrite(MOSFLTBLKA_B, config[EE_ADDR_MOSFLTBLKA_B]);
		spiWrite(CSAG_OCTH, config[EE_ADDR_CSAG_OCTH]);
		spiWrite(VBATOVUVRST, config[EE_ADDR_VBATOVUVRST]);
		// XCTRL setting is just one bit in register MOS_CHS_CTRL
		spiWrite(MOS_CHS_CTRL, ((getRegisterData(MOS_CHS_CTRL) & ~CHCRCTRL) | config[EE_ADDR_MOS_CHS_CTRL]));
	}
	else
	{
		// if driver is disabled
		// just update the register data
		// for viewing configuration with command "c"

		setRegisterData(VDSTHA_B, config[EE_ADDR_VDSTHA_B]);
		setRegisterData(MOSFLTBLKA_B, config[EE_ADDR_MOSFLTBLKA_B]);
		setRegisterData(CSAG_OCTH, config[EE_ADDR_CSAG_OCTH]);
		setRegisterData(VBATOVUVRST, config[EE_ADDR_VBATOVUVRST]);
		// XCTRL setting is just one bit in register MOS_CHS_CTRL
		setRegisterData(MOS_CHS_CTRL, ((getRegisterData(MOS_CHS_CTRL) & ~CHCRCTRL) | config[EE_ADDR_MOS_CHS_CTRL]));

		// save configuration for initialisation
		saveInitConfig();
	}
}

//****************************************************************************
// driverInit
//
// initialise the driver with the configuration stored in flash memory
//****************************************************************************
void driverInit(void)
{
	// save current configuration for initialisation
	if (!DIGITAL_IO_GetInput(&ENABLE))
		saveInitConfig();

	// disable interrupt to suppress vdd undervoltage message
	PIN_INTERRUPT_SetEdgeSensitivity(&PIN_INT, PIN_INTERRUPT_EDGE_NONE);
	// PIN_INTERRUPT_Disable(&PIN_INT);

	// set enable pin high
	DIGITAL_IO_SetOutputHigh(&ENABLE);

	setRegisterDefaultValues();

	// wait for the driver to power up
	wait(5000);

	// driver is enabled and switches are still off (load current = 0)
	// this is a good time to measure the actual cso output reference voltage (typ. vdd/2)
	// to avoid offset errors
	setCsoReferenceVoltage();

	// clear VDD_UV failure after reset
	clearFailures();

	// apply init configuration
	spiWrite(VDSTHA_B, getInitRegisterData(VDSTHA_B));
	spiWrite(MOSFLTBLKA_B, getInitRegisterData(MOSFLTBLKA_B));
	spiWrite(CSAG_OCTH, getInitRegisterData(CSAG_OCTH));
	spiWrite(VBATOVUVRST, getInitRegisterData(VBATOVUVRST));
	// XCTRL setting is just one bit in register MOS_CHS_CTRL
	spiWrite(MOS_CHS_CTRL, ((getRegisterData(MOS_CHS_CTRL) & ~CHCRCTRL) | getInitRegisterData(MOS_CHS_CTRL)));

	// enable interrupt again
	PIN_INTERRUPT_SetEdgeSensitivity(&PIN_INT, PIN_INTERRUPT_EDGE_RISING);
	// PIN_INTERRUPT_Enable(&PIN_INT);
}

//****************************************************************************
// printConfig
//
// print configuration
//****************************************************************************
void printConfig(void)
{
	static uint8_t data;
	static uint8_t csag_setting;
	static uint8_t octh_setting;

	// VBAT overvoltage restart time
	const char OVRT[4][12] = {
			"0 (10 µs)",
			"1 (50 µs)",
			"2 (200 µs)",
			"3 (1 ms)"};

	// VBAT undervoltage restart time
	const char UVRT[4][12] = {
			"0 (1 ms)",
			"1 (5 ms)",
			"2 (20 ms)",
			"3 (50 ms)"};

	// Mosfet voltage blanking time
	const char BLK[4][12] = {
			"0 (10 µs)",
			"1 (20 µs)",
			"2 (50 µs)",
			"3 (100 µs)"};

	// Mosfet voltage filter time
	const char FLT[4][12] = {
			"0 (0.5 µs)",
			"1 (1 µs)",
			"2 (2 µs)",
			"3 (5 µs)"};

	// Drain source overvoltage thresholds
	const char VDSOV[8][12] = {
			"0 (100 mV)",
			"1 (150 mV)",
			"2 (200 mV)",
			"3 (250 mV)",
			"4 (300 mV)",
			"5 (400 mV)",
			"6 (500 mV)",
			"7 (600 mV)"};

	// Current sense amplifier gain
	const char CSAG[8][14] = {
			"0 (10 V/V)",
			"1 (15  V/V)",
			"2 (20 V/V)",
			"3 (25 V/V)",
			"4 (31.5 V/V)",
			"5 (35 V/V)",
			"6 (40 V/V)",
			"7 (47.7 V/V)"};

	// Overcurrent detection threshold
	const char OCTH[4][22] = {
			"0 (VDD/2 +- 0.1 VDD)",
			"1 (VDD/2 +- 0.2 VDD)",
			"2 (VDD/2 +- 0.25 VDD)",
			"3 (VDD/2 +- 0.3 VDD)"};

	// Overcurrent thresholds for 50 µOhm shunt in [A] by OCTH and CSAG setting
	const char OC[4][8][8] = {
			{"660 A", "440 A", "330 A", "264 A", "210 A", "189	 A", "165 A", "138 A"},
			{"1320 A", "880 A", "660 A", "528 A", "419 A", "377 A", "330 A", "277 A"},
			{"1650 A", "1100 A", "825 A", "660 A", "524 A", "471 A", "413 A", "346 A"},
			{"1980 A", "1320 A", "990 A", "792 A", "629 A", "566 A", "495 A", "415 A"}};

	uartPrintf("\r\n"); // new line
	uartPrintf("---------------------------  VBAT ------------------------------\r\n");

	// print VBAT undervoltage restart time UVRT
	data = getRegisterData(VBATOVUVRST); // get current register data
	data &= VBATUVARST_MASK;			 // clear non UVRT bits
	data = (data >> VBATUVARST_POS);	 // shift bits to LSB position
	uartPrintf("VBAT undervoltage restart time....(uvrt): %s\r\n", UVRT[data]);

	// print VBAT overvoltage restart time OVRT
	data = getRegisterData(VBATOVUVRST); // get current register data
	data &= VBATOVARST_MASK;			 // clear non OVRT bits
	data = (data >> VBATOVARST_POS);	 // shift bits to LSB position
	uartPrintf("VBAT overvoltage restart time.....(ovrt): %s\r\n", OVRT[data]);

	uartPrintf("-------------------------  Channel A  --------------------------\r\n");

	// print VDS overvoltage threshold, channel A
	data = getRegisterData(VDSTHA_B); // get current register data
	data &= VDSTH_A_MASK;			  // clear non FLT_A bits
	data = (data >> VDSTH_A_POS);	  // shift bits to LSB position
	uartPrintf("VDS overvoltage threshold A.....(dsov a): %s\r\n", VDSOV[data]);

	// print VDS overvoltage safe state, channel A
	data = getRegisterData(VDSTHA_B); // get current register data
	if (data & VDSA_SS)				  // bit = 1
	{
		uartPrintf("VDS overvoltage safe state A.....(sst a): 1 (stay activated)\r\n");
	}
	else // bit = 0
	{
		uartPrintf("VDS overvoltage safe state A.....(sst a): 0 (deactivated)\r\n");
	}

	// print Mosfet voltage blanking time BLK, channel A
	data = getRegisterData(MOSFLTBLKA_B); // get current register data
	data &= MOSBLK_A_MASK;				  // clear non BLK_A bits
	data = (data >> MOSBLK_A_POS);		  // shift bits to LSB position
	uartPrintf("Mosfet voltage blanking time A...(blk a): %s\r\n", BLK[data]);

	// print Mosfet voltage filter time FLT, channel A
	data = getRegisterData(MOSFLTBLKA_B); // get current register data
	data &= MOSFLT_A_MASK;				  // clear non FLT_A bits
	data = (data >> MOSFLT_A_POS);		  // shift bits to LSB position
	uartPrintf("Mosfet voltage filter time A.....(flt a): %s\r\n", FLT[data]);

	uartPrintf("-------------------------  Channel B  --------------------------\r\n");

	// print drain source overvoltage threshold, channel B
	data = getRegisterData(VDSTHA_B); // get current register data
	data &= VDSTH_B_MASK;			  // clear non FLT_A bits
	data = (data >> VDSTH_B_POS);	  // shift bits to LSB position
	uartPrintf("VDS overvoltage threshold B.....(dsov b): %s\r\n", VDSOV[data]);

	// print VDS overvoltage safe state, channel B
	data = getRegisterData(VDSTHA_B); // get current register data
	if (data & VDSB_SS)				  // safe state channel B = 1 (ON)
	{
		uartPrintf("VDS overvoltage safe state B.....(sst b): 1 (stay activated)\r\n");
	}
	else // safe state channel B = 0 (OFF)
	{
		uartPrintf("VDS overvoltage safe state B.....(sst b): 0 (deactivated)\r\n");
	}

	// print Mosfet voltage blanking time BLK, channel B
	data = getRegisterData(MOSFLTBLKA_B); // get current register data
	data &= MOSBLK_B_MASK;				  // clear non BLK_A bits
	data = (data >> MOSBLK_B_POS);		  // shift bits to LSB position
	uartPrintf("Mosfet voltage blanking time B...(blk b): %s\r\n", BLK[data]);

	// print Mosfet voltage filter time FLT, channel B
	data = getRegisterData(MOSFLTBLKA_B); // get current register data
	data &= MOSFLT_B_MASK;				  // clear non FLT_A bits
	data = (data >> MOSFLT_B_POS);		  // shift bits to LSB position
	uartPrintf("Mosfet voltage filter time B.....(flt b): %s\r\n", FLT[data]);

	// print precharge time
	uartPrintf("Precharge time.....................(prt): %d ms\r\n", getPrechargeTime());

	uartPrintf("----------------------------------------------------------------\r\n");

	// print channel cross control
	data = getRegisterData(MOS_CHS_CTRL); // get current register data
	if (data & CHCRCTRL)				  // bit = 1
	{
		uartPrintf("Channel cross control.............(xctr): 1 (activated)\r\n");
	}
	else // bit = 0
	{
		uartPrintf("Channel cross control.............(xctr): 0 (deactivated)\r\n");
	}

	uartPrintf("-----------------------  Current Sense  ------------------------\r\n");

	// print hall sensor sensitivity
	uartPrintf("Hall sensitivity...................(hss): %d µV/A\r\n", getHallSensitivity());

	// print current sense location
	data = getRegisterData(CSAG_OCTH); // get current register data
	if (data & CSA_HSS)				   // bit = 1
	{
		uartPrintf("Current sense shunt location......(cshs): 1 (high side)\r\n");
	}
	else // bit = 0
	{
		uartPrintf("Current sense shunt location......(cshs): 0 (low side)\r\n");
	}

	// print current sense load
	data = getRegisterData(CSAG_OCTH); // get current register data
	if (data & CSA_COUTSEL)			   // bit = 1
	{
		uartPrintf("Current sense output load.........(csld): 1 (>100 pF)\r\n");
	}
	else // bit = 0
	{
		uartPrintf("Current sense output load.........(csld): 0 (<100pF)\r\n");
	}

	// print current sense amplifier gain CSAG
	data = getRegisterData(CSAG_OCTH); // get current register data
	data &= CSAG_MASK;				   // clear non CSAG bits
	csag_setting = (data >> CSAG_POS); // shift bits to LSB position
	uartPrintf("Current sense amplifier gain......(csag): %s\r\n", CSAG[csag_setting]);

	// print overcurrent detection threshold OCTH
	data = getRegisterData(CSAG_OCTH); // get current register data
	data &= OCTH_MASK;				   // clear non OCTH bits
	octh_setting = (data >> OCTH_POS); // shift bits to LSB position
	uartPrintf("Overcurrent detection threshold...(octh): %s\r\n", OCTH[octh_setting]);

	// print overcurrent threshold value in Ampere
	uartPrintf("Overcurrent threshold [A]...............: %s\r\n", OC[octh_setting][csag_setting]);

	uartPrintf("----------------------------------------------------------------");
}

//****************************************************************************
// checkButtons
//
// get the status of the push buttons on the driver board and act
//
// button 1 is same as commands "on" and "off"
// in case of a failure, pressing button 1 will clear the failure
//
// button 2 is same as "s" (safe state enable)
//****************************************************************************
void checkButtons(void)
{
	static uint8_t status_button1 = 0;
	static uint8_t status_button2 = 0;

	// Button 1 is pressed -> "falling edge" at button 1
	// -> switch on with precharge or switch off
	if (status_button1 == 0 && !DIGITAL_IO_GetInput(&BUTTON1))
	{
		if (DIGITAL_IO_GetInput(&ENABLE)) // driver is enabled
		{
			if ((getRegisterData(STDIAG) & FAILURE)) // a failure is present
			{
				clearFailures();
			}
			else // no failure present
			{
				if ((getRegisterData(MOS_CHS_CTRL) & MOSONCH_A)) // channel A is on
				{
					switchOff();
				}
				else // channel A is off
				{
					switchOnWithPrecharge();
				}
			}
		}
		else // driver is disabled
		{
			switchOnWithPrecharge();
		}

		status_button1 = 1;
	}

	// Button 1 is released -> "rising edge" at button 1
	// -> reset status
	if (status_button1 == 1 && DIGITAL_IO_GetInput(&BUTTON1))
	{
		status_button1 = 0;
	}

	// Button 2 is pressed -> "falling edge" at button 2
	// -> safestate enable
	if (status_button2 == 0 && !DIGITAL_IO_GetInput(&BUTTON2))
	{
		if (DIGITAL_IO_GetInput(&ENABLE)) // driver is enabled
		{
			if ((getRegisterData(DIAG) & SAFESTATEN)) // safe state bit is set
			{
				DIGITAL_IO_SetOutputHigh(&SAF);
				clearFailures();
			}
			else // safe state bit is not set
			{
				DIGITAL_IO_SetOutputLow(&SAF);
			}
		}

		status_button2 = 1;
	}

	// Button 2 is released -> "rising edge" at button 2
	// -> reset status
	if (status_button2 == 1 && DIGITAL_IO_GetInput(&BUTTON2))
	{
		status_button2 = 0;
	}
}

//****************************************************************************
// setLEDs
// set status LEDs (channel A: LED1, channel B: LED2)
//****************************************************************************
void setLEDs(void)
{
	static uint8_t channel_a_on;
	static uint8_t channel_b_on;
	static uint8_t error_flag;

	// get status of error flag and outputs
	// if driver is disabled clear status  (led's off)
	if (DIGITAL_IO_GetInput(&ENABLE))
	{
		channel_a_on = getRegisterData(MOS_CHS_CTRL) & MOSONCH_A;
		channel_b_on = getRegisterData(MOS_CHS_CTRL) & MOSONCH_B;
		error_flag = getRegisterData(STDIAG) & FAILURE;
	}
	else
	{
		channel_a_on = 0;
		channel_b_on = 0;
		error_flag = 0;
	}

	// set LED_CHA and LED1
	if (channel_a_on)
	{
		DIGITAL_IO_SetOutputHigh(&LED_CHA);
		DIGITAL_IO_SetOutputHigh(&LED1);
	}
	else
	{
		DIGITAL_IO_SetOutputLow(&LED_CHA);
		DIGITAL_IO_SetOutputLow(&LED1);
	}

	// set LED_CHB
	if (channel_b_on)
	{
		DIGITAL_IO_SetOutputHigh(&LED_CHB);
	}
	else
	{
		DIGITAL_IO_SetOutputLow(&LED_CHB);
	}

	// set LED_ERR
	if (error_flag)
	{
		DIGITAL_IO_SetOutputHigh(&LED_ERR);
	}
	else
	{
		DIGITAL_IO_SetOutputLow(&LED_ERR);
	}

	// set LED2
	if ((getRegisterData(DIAG) & SAFESTATEN)) // safe state bit is set
	{
		DIGITAL_IO_SetOutputHigh(&LED2);
	}
	else // safe state bit is not set
	{
		DIGITAL_IO_SetOutputLow(&LED2);
	}
}

//****************************************************************************
//
// Interrupt Service Routines
//
//****************************************************************************

//****************************************************************************
// ISR_timerChannelA
//
// gets called when TIMER_A is expired
//****************************************************************************
void ISR_timerChannelA(void)
{
	// stop the timer
	TIMER_Stop(&TIMER_A);
	// clear interrupt event so next event is considered as new
	TIMER_ClearEvent(&TIMER_A);
	// switch off channel A
	clearBits(MOS_CHS_CTRL, MOSONCH_A);
}

//****************************************************************************
// ISR_timerChannelB
//
// gets called when TIMER_B is expired
//****************************************************************************
void ISR_timerChannelB(void)
{
	// stop the timer
	TIMER_Stop(&TIMER_B);
	// clear interrupt event so next event is considered as new
	TIMER_ClearEvent(&TIMER_B);
	// switch off channel B
	clearBits(MOS_CHS_CTRL, MOSONCH_B);
}

//****************************************************************************
// ISR_timerPrecharge
//
// gets called when TIMER_PRECHARGE is expired
//****************************************************************************
void ISR_timerPrecharge(void)
{
	// stop the timer
	TIMER_Stop(&TIMER_PRECHARGE);
	// clear interrupt event so next event is considered as new
	TIMER_ClearEvent(&TIMER_PRECHARGE);
	// force update of shadow registers
	readAllRegisters();
	reportChanges();
	// switch on channel A
	setBits(MOS_CHS_CTRL, MOSONCH_A);
	// wait
	wait(100000);
	// switch off channel B
	// force update of shadow registers
	readAllRegisters();
	reportChanges();
	clearBits(MOS_CHS_CTRL, MOSONCH_B);
}

//****************************************************************************
// ISR_INT
//
// gets called when INT pin of 2ED4820 goes high
//****************************************************************************
void ISR_INT(void)
{
	driverInterruptOccurred = 1; // set interrupt flag
}

//****************************************************************************
// ISR_OCD1
//
// gets called when OCD1 pin of TLE4920 goes low
//****************************************************************************
void ISR_OCD1(void)
{
	uartPrintf("\r\n"); // new line
	uartPrintf("OCD1 was triggered by TLE4972");
	uartPrintf("\r\n>"); // new line and prompt
}
