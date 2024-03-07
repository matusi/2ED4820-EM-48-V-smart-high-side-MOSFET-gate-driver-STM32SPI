/*
 * registers.c
 *
 *  Created on: 21 Feb 2020
 */


#include <DAVE.h>   //Declarations from DAVE Code Generation (includes SFR declaration)
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> // includes strtol
#include "uart.h"
#include "registers.h"

//****************************************************************************
// global buffers for register data
//****************************************************************************
static uint8_t g_reg_data[11];         // buffer for 2ED4820 register data
static uint8_t g_prev_reg_data[11];    // buffer for previous register data
static uint8_t g_init_reg_data[11];    // buffer for initialisation register data

// Global flags for interrupt and failure bit
uint8_t driverInterruptOccurred = 0;
uint8_t failureBitWasSet = 0;

//****************************************************************************
// global variables for register and bit names
//****************************************************************************
const char reg_name[11][7]={
        "STDIAG",
        "CHDIAG",
        "DIAG",
        "MOSCTR",
        "FCLEAN",
        "VDSTH",
        "FLTBLK",
        "CSAOCT",
        "VBRES",
        "RESETS",
        "SPARE"
};

//
// [11 registers][8 bit data][10 char. max]
// bit names are shortened vs. specification
// for space reasons and to make it easier to read
//
const char bit_name[11][8][10]={
        { //STDIAG
                "VBATOV",    // LSB : VBAT undervoltage failure (not latched)
                "VBATUV",    // VBAT overvoltage failure (not latched)
                "VDDUV",     // VDD undervoltage propagated in the first command and indicates an under voltage event
                "TSD",       // Chip in Overtemperature (latched)
                "OTWARN",    // Temperature warning
                "MEM",       // Error in the memory, trimming not possible (flag cannot be cleaned)
                "VCPRDY",    // Charge pump is ready
                "FAIL"       // MSB : Main Failure indication, 1 when there is a failure
        },
        { //CHDIAG
                "VSOVA",     // LSB : Source overvoltage - Channel_A
                "VDSOVA",    // Drain to Source overvoltage - Channel_A (latched)
                "VGSUVA",    // Gate-Source undervoltage - Channel_A (latched)
                "VSOVB",     // Source overvoltage - Channel_B
                "VDSOVB",    // Drain to Source overvoltage - Channel_B (latched)
                "VGSUVB",    // Gate-Source undervoltage - Channel_B (latched)
                "OC",        // Overcurrent failure (latched)
                "VCPUV"      // MSB : Charge pump undervoltage
        },
        { //DIAG
                "SST_EN",    // LSB : SAFESTATEN
                "ADD_NA",    // Address not available
                "LOG_CP",    // Loss of charge pump ground
                "LOG_D",     // Loss of digital ground
                "LOG_A",     // Loss of analog ground
                "D5",        // Not assigned
                "D6",        // Not assigned
                "D7"         // Not assigned
        },
        { //MOS_CHS_CTRL
                "CHA_ON",    // LSB : Switch on channel A
                "VDSCLA",    // Clear VDS flag Channel A
                "VGSCLA",    // Clear VGS flag Channel A
                "CHB_ON",    // Switch on channel B
                "VDSCLB",    // Clear VDS flag Channel B
                "VGSCLB",    // Clear VGS flag Channel B
                "ITRPCL",    // Clear ITRIP flag
                "XCTRL"      // MSB : Channel cross control activation
        },
        {  // FAILURE_CLEAN
                "VBUVCL",    // LSB : Clear VBAT undervoltage failure
                "VBOVCL",    // Clear VBAT overvoltage failure
                "VDDUCL",    // Clear Vdd undervoltage failure
                "TSD_CL",     // Clear overtemperature failure
                "INT_CL",     // Clear interrupt
                "SST_CL",     // Safe State clear
                "D6",        // Not assigned
                "CPUVCL"     // MSB : VCP under voltage clear
        },
        { // VDSTHA_B
                "VDS_A0",     // LSB : Drain-Source overvoltage threshold Channel_A
                "VDS_A1",     // Drain-Source overvoltage threshold Channel_A
                "VDS_A2",     // Drain-Source overvoltage threshold Channel_A
                "SST_A",    // VDS Channel A safe state: ON when 0, OFF when 1 (default)
                "VDS_B0",     // Drain-Source overvoltage threshold Channel_B
                "VDS_B1",     // Drain-Source overvoltage threshold Channel_B
                "VDS_B2",     // Drain-Source overvoltage threshold Channel_B
                "SST_B"     // MSB : VDS Channel B safe state: ON when 0, OFF when 1 (default)
        },
        { // MOSFLTBLKA_B
                "FLT_A0",     // LSB : MOS voltage filter Channel_A
                "FLT_A1",     // MOS voltage filter Channel_A
                "BLK_A0",     // MOS voltage blank time Channel_A
                "BLK_A1",     // MOS voltage blank time Channel_A
                "FLT_B0",     // MOS voltage filter Channel_B
                "FLT_B1",     // MOS voltage filter Channel_B
                "BLK_B0",     // MOS voltage blank time Channel_B
                "BLK_B1"      // MSB : MOS voltage blank time Channel_B
        },
        { // CSAG_OCTH
                "CSAG0",     // LSB : Current sense amplifier gain GDIFF
                "CSAG1",     // Current sense amplifier gain GDIFF
                "CSAG2",     // Current sense amplifier gain GDIFF
                "OCTH0",     // Over current detection thresholds
                "OCTH1",     // Over current detection thresholds
                "HSS",       // Control signal to select the CSA configuration LSS or HSS(or bidirectional)
                "COUT",      // Configures opamp output stage vs load cap
                "D7"         // MSB : Not assigned
        },
        { // VBATOVOVRST
                "OVRT0",     // LSB : VBAT overvoltage auto-restart time
                "OVRT1",     // VBAT overvoltage auto-restart time
                "UVRT0",     // VBAT undervoltage auto-restart time
                "UVRT1",     // VBAT undervoltage auto-restart time
                "D4",        // Not assigned
                "D5",        // Not assigned
                "D6",        // Not assigned
                "D7"         // Not assigned
        },
        {
                "SRES0",     // LSB : Software Reset 0
                "FRES0",     // Reset Fails registers 0
                "GENINT",    // Generate interrupt signal
                "D3",        // Not assigned
                "SRES1",     // Software Reset 1
                "FRES1",     // Reset Fails registers 1
                "D6",        // Not assigned
                "D7"         // Not assigned
        },
        {
                "SPARE0",    // LSB : Spare register bit 0
                "SPARE1",    // Spare register bit 1
                "SPARE2",    // Spare register bit 2
                "SPARE3",    // Spare register bit 3
                "SPARE4",    // Spare register bit 4
                "SPARE5",    // Spare register bit 5
                "SPARE6",    // Spare register bit 6
                "SPARE7"     // MSB : Spare register bit 7
        }

};



//****************************************************************************
// xtoi
//
// Returns int value for hexadecimal string
//
// Copyright (c) Leigh Brasington 2012.  http://www.leighb.com/xtoi.htm
//****************************************************************************
int xtoi(char *hexstring)
{
    int i = 0;
    char c = 0;

    // get rid of '0x'
    if ((*hexstring == '0') && (*(hexstring+1) == 'x'))
        hexstring += 2;

    while(*hexstring)
    {
        c = toupper((unsigned char)*hexstring++);
        if ((c < '0') || (c > 'F') || ((c > '9') && (c < 'A')))
            break;
        c -= '0';
        if (c > 9)
            c -= 7;
        i = (i << 4) + c;
    }
    return i;
}

//****************************************************************************
// spiRead
//
// sends read command for register adress
// gets data of previous SPI access
//****************************************************************************
int spiRead(uint8_t adr, uint8_t *data)
{
    uint8_t tx_buf[2]; // transmit buffer
    uint8_t rx_buf[2]; // receive buffer

    *data = 0;

    // read only if driver is enabled (ENABLE pin high)
//    if(DIGITAL_IO_GetInput(&ENABLE))
//    {
        if(adr >=0 && adr <= 10)
        {
            tx_buf[0] = adr; // R/W bit is 0 anyway with address <= 10
            tx_buf[1] = 0;   // data is "don't care"
            uint16_t packedData = CombineBytes(tx_buf[0], tx_buf[1]);
            SplitBytes(packedData, &tx_buf[0], &tx_buf[1]);
//            SPI_MASTER_Transfer(&SPI, tx_buf, , 2);
            HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, 1, HAL_MAX_DELAY);  // SPI Transceive

            *data = rx_buf[1];
            return 1;
        }
        else
        {
        	return 0;
//            uartPrintf("[ERROR]: Can't read register - driver is disabled");
        }
//    }
}
//****************************************************************************
// spiWrite
//
// writes 8 bit data to register adress
//****************************************************************************
int spiWrite(uint8_t adr, uint8_t data)
{
    uint8_t tx_buf[2]; // transmit buffer


        if(adr >= 0 && adr <= 10)
        {
            tx_buf[0] = adr;
            tx_buf[0] |= (1 << 7); // set R/W bit to "1"
            tx_buf[1] = data;

            SPI_MASTER_Transmit(&SPI, tx_buf, 2);
            return 1; // Return 1 for success
        }

    else
    {
        uartPrintf("\r\n"); // new line
        uartPrintf("[ERROR]: Can't write register - driver is disabled");
    }
    return 0; // Return 0 for failure
}



//****************************************************************************
// setRegisterDefaultValues()
//
// fill the global register data buffers with default values
// to avoid "register changed" message after enabling the driver
//****************************************************************************
void setRegisterDefaultValues(void)
{
    g_reg_data[STDIAG] = 0x00;
    g_reg_data[CHDIAG] = 0x00;
    g_reg_data[DIAG] = 0x00;
    g_reg_data[MOS_CHS_CTRL] = 0x80;
    g_reg_data[FAILURECLEAN] = 0x00;
    g_reg_data[VDSTHA_B] = 0xAA;
    g_reg_data[MOSFLTBLKA_B] = 0x11;
    g_reg_data[CSAG_OCTH] = 0x1D;
    g_reg_data[VBATOVUVRST] = 0x00;
    g_reg_data[RESETS] = 0x00;
    g_reg_data[SPARE_REG] = 0xF0;

    for(int reg_num = 0; reg_num < 11; reg_num++)
    {
        g_prev_reg_data[reg_num] = g_reg_data[reg_num];
    }
}

//****************************************************************************
// getRegisterName
//****************************************************************************
const char* getRegisterName(uint8_t reg_num)
{
    if(reg_num >= 0 && reg_num <= 10)
    {
        return reg_name[reg_num];
    }
    else
    {
        return "[ERROR]: Invalid Register Number";
    }
}



//****************************************************************************
// setRegisterData
//****************************************************************************
uint8_t setRegisterData(uint8_t reg_num, uint8_t reg_data)
{
    if(reg_num >= 0 && reg_num <= 10)
    {
        g_reg_data[reg_num] = reg_data;
        return 1;
    }
    else
    {
        return 0;
    }
}

//****************************************************************************
// getRegisterData
//****************************************************************************
uint8_t getRegisterData(uint8_t reg_num)
{
    if(reg_num >= 0 && reg_num <= 10)
    {
        return g_reg_data[reg_num];
    }
    else
    {
        return 0;
    }
}

//****************************************************************************
// setInitRegisterData
//****************************************************************************
uint8_t setInitRegisterData(uint8_t reg_num, uint8_t reg_data)
{
    if(reg_num >= 0 && reg_num <= 10)
    {
    	g_init_reg_data[reg_num] = reg_data;
        return 1;
    }
    else
    {
        return 0;
    }
}

//****************************************************************************
// getInitRegisterData
//****************************************************************************
uint8_t getInitRegisterData(uint8_t reg_num)
{
    if(reg_num >= 0 && reg_num <= 10)
    {
        return g_init_reg_data[reg_num];
    }
    else
    {
        return 0;
    }
}


//****************************************************************************
// getBitName
//****************************************************************************
const char* getBitName(uint8_t reg_num, uint8_t bit_num)
{

    if(reg_num >= 0 && reg_num <= 10)
    {
        if(bit_num >= 0 && bit_num <= 7)
        {
            return bit_name[reg_num][bit_num];
        }
        else
        {
            return "[ERROR]: Invalid bit number";
        }
    }
    else
    {
        return "[ERROR]: Invalid register number";
    }
}


//****************************************************************************
// setBits
//
// sets the bits in given register and writes register
//****************************************************************************
void setBits(uint8_t reg_num, uint8_t bitmask)
{
    uint8_t data;

    data = g_reg_data[reg_num]; // get current register data
    data |= bitmask;            // set bits

    if(DIGITAL_IO_GetInput(&ENABLE))
    	spiWrite(reg_num, data);    // write new register data
	else
		setRegisterData(reg_num, data); // update shadow register
}

//****************************************************************************
// clearBits
//
// clears the bits in given register and writes register
//****************************************************************************
void clearBits(uint8_t reg_num, uint8_t bitmask)
{
    uint8_t data;

    data = g_reg_data[reg_num]; // get current register data
    data &= ~bitmask;           // clear bits

    if(DIGITAL_IO_GetInput(&ENABLE))
     	spiWrite(reg_num, data);    // write new register data
	else
		setRegisterData(reg_num, data); // update shadow register
}


//****************************************************************************
// readRegister
//****************************************************************************
void readRegister(char *arg)
{
    uint8_t reg_num;   // register number

    // all registers are already read every 10 ms
    // by the function readAllRegisters()
    // and the data is stored in global variable g_reg_data[]
    // so here we just have to print out the content

    // print only if driver is enabled (ENABLE pin high)
    if(DIGITAL_IO_GetInput(&ENABLE))
    {
        if (!strcmp(arg,"all") || !strcmp(arg,"a") || !strcmp(arg,""))
        {
            for(reg_num = 0; reg_num <=10; reg_num++)
            {
                printRegister(reg_num);
            }
        }
        else
        {
            reg_num = atoi(arg); // convert string to integer

            if(reg_num >= 0 && reg_num <= 10)
            {
                printRegister(reg_num);
            }
            else
            {
                uartPrintf("\r\n"); // new line
                uartPrintf("[ERROR]: Invalid register number");
            }
        }
    }
    else
    {
        uartPrintf("\r\n"); // new line
        uartPrintf("[ERROR]: Can't read register - driver is disabled");
    }

}

//****************************************************************************
// readAllRegisters
//
// reads all registers and stores data in global variables
//****************************************************************************
void readAllRegisters(void)
{
    uint8_t reg_num; // register number
    uint8_t rx_data; // received data

    // read only if driver is enabled (ENABLE pin high)
    if(DIGITAL_IO_GetInput(&ENABLE))
    {
        for(reg_num = 0; reg_num < 11; reg_num++)
        {
            spiRead(reg_num, &rx_data);

            //skip first response - rx data is response to previous frame
            if (reg_num > 0)
            {
                g_reg_data[reg_num-1] = rx_data; // store data in global variable
            }
        }

        //repeat last frame to get data for last register (Reg. #10)
        spiRead(10, &rx_data);
        g_reg_data[10] = rx_data;
    }
}

//****************************************************************************
// writeRegister
//****************************************************************************
void writeRegister(char *adr, char *dat)
{
    uint8_t reg_num;    // register number
    uint8_t reg_data;       // register data

    // write only if driver is enabled (ENABLE pin high)
    if(DIGITAL_IO_GetInput(&ENABLE))
    {
        reg_num = atoi(adr); // convert string to integer

        if(reg_num >= 0 && reg_num <= 10)
        {
            // convert data string from hex format to uint8_t
            if (strlen(dat)<=2) // expecting up to 2 hex characters
            {
                reg_data = xtoi(dat);
                spiWrite(reg_num, reg_data);
            }
            else
            {
                uartPrintf("\r\n"); // new line
                uartPrintf("[ERROR]: expecting max. 2 hex characters (e.g. '2A')");
            }
        }
        else
        {
            uartPrintf("\r\n"); // new line
            uartPrintf("[ERROR]: Invalid register number");
        }

    }
    else
    {
        uartPrintf("\r\n"); // new line
        uartPrintf("[ERROR]: Can't write register - driver is disabled");
    }
}


//****************************************************************************
// printRegister
//****************************************************************************
void printRegister(uint8_t reg_num)
{
    int bit_num;
    const char *reg_name;
    const char *bit_name;

    if(reg_num >= 0 && reg_num <= 10)
    {
        reg_name = getRegisterName(reg_num);

        uartPrintf("\r\n");
        uartPrintf("%2d: %6s: %02X |", reg_num, reg_name, g_reg_data[reg_num]);

        for(bit_num = 7; bit_num >= 0; bit_num--)
        {
           bit_name = getBitName(reg_num, (uint8_t)bit_num);
           if((g_reg_data[reg_num]) & (1 << bit_num)) // if bit is set (high)
            {
                 uartPrintf("\033[7m%6s\033[m|", bit_name); // inverse
            }
           else // if bit is not set (low)
           {
               uartPrintf("%6s|", bit_name);
           }
        }
    }
    else
    {
        uartPrintf("\r\n"); // new line
        uartPrintf("[ERROR]: Invalid register number");
    }
}

//****************************************************************************
// reportChanges
//
// compare register setting with previous state and report changes
//****************************************************************************
void reportChanges()
{
	uint8_t reg_num; // register number
	int bit_num;
	const char *reg_name;
	const char *bit_name;
	uint8_t reg_data_changed = 0;   // flag to indicate changed register content
	uint8_t bits_were_set = 0;      // flag to indicate if any bit was set
	uint8_t bits_were_cleared = 0;  // flag to indicate if any bit was cleared
	uint8_t changed_bits[11];       // bits that changed for each register
	uint8_t set_bits[11];           // bits that were set for each register
	uint8_t cleared_bits[11];       // bits that were cleared for each register


	if(DIGITAL_IO_GetInput(&ENABLE)) // only if driver is enabled
	{

		// check if anything changed
		for(reg_num = 0; reg_num < 11; reg_num++)
		{
			changed_bits[reg_num] = 0;
			set_bits[reg_num] = 0;
			cleared_bits[reg_num] = 0;

			if(g_reg_data[reg_num] != g_prev_reg_data[reg_num]) // register content changed
			{
				reg_data_changed = 1;
				changed_bits[reg_num] = g_reg_data[reg_num] ^ g_prev_reg_data[reg_num]; //XOR
				// bit was set if it changed and it is set now
				if ((set_bits[reg_num] = changed_bits[reg_num] & g_reg_data[reg_num]))
					bits_were_set = 1;
				// bit was cleared if it changed and it was set before
				if ((cleared_bits[reg_num] = changed_bits[reg_num] & g_prev_reg_data[reg_num]))
					bits_were_cleared = 1;
			}

			// store current register data for next comparison
			g_prev_reg_data[reg_num] = g_reg_data[reg_num];
		}

		// print changed bits
		if(reg_data_changed)
		{
			uartPrintf("\b"); // backspace to delete prompt

			if(bits_were_set)
			{
				uartPrintf("Bits Set:");

				for(reg_num = 0; reg_num <= 10; reg_num++)
				{
					reg_name = getRegisterName(reg_num);

					if(set_bits[reg_num])
					{
						for(bit_num = 7; bit_num >= 0; bit_num--)
						{
							if(set_bits[reg_num] & (1 << bit_num))
							{
								bit_name = getBitName(reg_num, (uint8_t)bit_num);
								uartPrintf(" %s,", bit_name);
							}
						}
					}
				}

				uartPrintf("\b \r\n"); // '\b ' = delete last comma

				if(set_bits[STDIAG] & FAILURE)
				{
					failureBitWasSet = 1;
				}
			}


			if(bits_were_cleared)
			{
				uartPrintf("Bits Cleared:", reg_num, reg_name);

				for(reg_num = 0; reg_num <= 10; reg_num++)
				{
					reg_name = getRegisterName(reg_num);

					if(cleared_bits[reg_num])
					{
						for(bit_num = 7; bit_num >= 0; bit_num--)
						{
							if(cleared_bits[reg_num] & (1 << bit_num))
							{
								bit_name = getBitName(reg_num, (uint8_t)bit_num);
								uartPrintf(" %s,", bit_name);
							}
						}
					}
				}

				uartPrintf("\b \r\n"); // '\b ' = delete last comma
			}

			uartPrintf(">"); // prompt

		}

		// notify user about driver interrupt
		if(driverInterruptOccurred)
		{
			uartPrintf("\b[INTERRUPT]\r\n>");
		}

		// report failures if failure bit was set or interrupt occurred
		if(failureBitWasSet || driverInterruptOccurred)
		{
			reportFailures();
			//uartPrintf("\r\n>"); // new line and prompt

			// reset flags to avoid duplicate reporting
			driverInterruptOccurred = 0;
			failureBitWasSet = 0;
		}
	}
}


//****************************************************************************
// reportFailures
//
// check failure bits in diagnosis registers and report failures
//****************************************************************************
void reportFailures()
{
    uartPrintf("\b"); // delete prompt

	// STDIAG
	if((getRegisterData(STDIAG)) & VBAT_UV)
		uartPrintf("[FAILURE]: Vbat undervoltage\r\n");
	if((getRegisterData(STDIAG)) & VBAT_OV)
		uartPrintf("[FAILURE]: Vbat overvoltage\r\n");
	if((getRegisterData(STDIAG)) & VDD_UV)
		uartPrintf("[FAILURE]: Vdd undervoltage\r\n");
	if((getRegisterData(STDIAG)) & TSD)
		uartPrintf("[FAILURE]: Chip overtemperature\r\n");
	if((getRegisterData(STDIAG)) & OT_WARNING)
		uartPrintf("[WARNING]: Chip overtemperature\r\n");
	if((getRegisterData(STDIAG)) & MEM_FAIL)
		uartPrintf("[WARNING]: Memory Error\r\n");

	// CHDIAG
	if((getRegisterData(CHDIAG)) & VSOURCE_A)
		uartPrintf("[WARNING]: Source overvoltage channel A\r\n");
	if((getRegisterData(CHDIAG)) & VDSTRIP_A)
		uartPrintf("[FAILURE]: VDS overvoltage channel A\r\n");
	if((getRegisterData(CHDIAG)) & VGSTH_A)
		uartPrintf("[FAILURE]: VGS undervoltage channel A\r\n");
	if((getRegisterData(CHDIAG)) & VSOURCE_B)
		uartPrintf("[WARNING]: Source overvoltage channel B\r\n");
	if((getRegisterData(CHDIAG)) & VDSTRIP_B)
		uartPrintf("[FAILURE]: VDS overvoltage channel B\r\n");
	if((getRegisterData(CHDIAG)) & VGSTH_B)
		uartPrintf("[FAILURE]: VGS undervoltage channel B\r\n");
	if((getRegisterData(CHDIAG)) & ITRIP)
		uartPrintf("[FAILURE]: Overcurrent\r\n");
	if((getRegisterData(CHDIAG)) & VCP_UV)
		uartPrintf("[FAILURE]: Charge pump undervoltage\r\n");

    // DIAG
	if((getRegisterData(DIAG)) & SAFESTATEN)
		uartPrintf("[FAILURE]: SAFESTATE activated\r\n");
	if((getRegisterData(DIAG)) & ADD_NOT_AVAIL)
		uartPrintf("[WARNING]: SPI address not available\r\n");
	if((getRegisterData(DIAG)) & LOG_CP)
		uartPrintf("[WARNING]: Loss of charge pump ground\r\n");
	if((getRegisterData(DIAG)) & LOG_D)
		uartPrintf("[WARNING]: Loss of digital ground\r\n");
	if((getRegisterData(DIAG)) & LOG_A)
		uartPrintf("[WARNING]: Loss of analog ground\r\n");

    uartPrintf(">"); // prompt

}

