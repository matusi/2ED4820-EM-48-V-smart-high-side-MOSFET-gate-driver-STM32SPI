/*
croc * shell.c
 *
 *  Created on: 20 Feb 2020
 *      Author: schwarzg
 */

#include <DAVE.h>   //Declarations from DAVE Code Generation (includes SFR declaration)
#include <stdio.h>
#include <stdarg.h> // for va list in uartPrintff
#include <stdlib.h> // includes strtol
#include <ctype.h>
#include "shell.h"
#include "uart.h"
#include "registers.h"
#include "commands.h"
#include "analog.h"


//****************************************************************************
// shell_init
//
// initialises the shell environment
//****************************************************************************
void shellInit(void)
{
    // print welcome message
    uartPrintf("\r\n");
    uartPrintf("-------------------------------------------------------------------\r\n");
    uartPrintf("48V Battery Switch command line interface, Rev. 1.0, 2022-02-02\r\n");
    uartPrintf("To display the list of available commands please enter '?'\r\n");
    uartPrintf("-------------------------------------------------------------------\r\n>");

    setRegisterDefaultValues(); // reset state
    loadConfig(); // read configuration from EEPROM emulation in flash memory
	saveInitConfig(); // store it for first initialisation
}




//****************************************************************************
// commandLineHandler
//
// command line interface handler for terminal on virtual COM port
// gets called every 1 ms by SYSTIMER
// reads a character from the UART and appends it to the command string
// calls executeCommand when the enter key is pressed
//****************************************************************************
void commandLineHandler(void)
{
    static char input_char;
    static char junk; // for purging the uart fifo
    static char cmd_buffer[CMD_BUF_NUM][CMD_BUF_LEN]; // command buffer
    static char* cmd_string; // pointer to command buffer
    static uint8_t cmd_num = 0;
    static uint8_t len;
    static uint32_t count; // for wait loop
    static bool first_call_of_function = 1;

    // if command line handler is called  for the first time
    // initialise command buffer
    if(first_call_of_function)
    {
    	for (count = 0; count <CMD_BUF_NUM; count++ )
    	{
    		cmd_buffer[count][0] = '\0';
    	}

        first_call_of_function = 0;
    }


    cmd_string = cmd_buffer[cmd_num];
    len = strlen(cmd_string);

    // read input from UART
    if(uartGetChar(&input_char)) // get one character
    {
        switch(input_char)
        {
        case '\r':  // Return / Enter
        case '\n':  // New Line
            executeCommand(cmd_string);       // analyze command and execute

			if(cmd_num < CMD_BUF_NUM - 1)         // select next command buffer
			{
				cmd_num = cmd_num + 1;
			}
			else
			{
				cmd_num = 0;
			}

			cmd_buffer[cmd_num][0] = '\0';    // clear command buffer
            uartPrintf("\r\n>");              // goto new line + print prompt
            break;

        case '\b':  // Delete last character
            uartPrintf("\b \b"); // delete: step back one character, print space, step back again
            if(len > 0) len--;
            cmd_string[len]= '\0';
            break;

            // Arrow Keys
            // Up: '\e[A', Down: '\e[B', Right:'\e[C', Left:'\e[D'
        case '\e':   // Escape

        	// allow some time to receive the next character
        	count = 1000; while(count--) __NOP();

        	// get next character
        	uartGetChar(&input_char); // exspecting '['

        	if(input_char == '[') // just to make sure we got the right character
        	{
            	// allow some time to receive the next character
            	count = 1000; while(count--) __NOP();

            	// get next character
       		    uartGetChar(&input_char); // get direction character (A, B, C or D)

        		if(input_char == 'A') // Up - restore last command
        		{
        			// delete current command string
        			while(len > 0)
        			{
        				uartPrintf("\b \b"); // delete: step back one character, print space, step back again
        				len--;
        			}

        			// replace command string by last command

        			if(cmd_num > 0)
        				cmd_num = cmd_num - 1;
        			else
        				cmd_num = CMD_BUF_NUM - 1;

        			cmd_string = cmd_buffer[cmd_num];
        			uartPrintf("%s", cmd_string);
        			len = strlen(cmd_string);
        		}
        		else if(input_char == 'B') // Down
        		{
        			// delete current command string
        			while(len > 0)
        			{
        				uartPrintf("\b \b"); // delete: step back one character, print space, step back again
        				len--;
        			}

        			// replace command string by last command

        			if(cmd_num < (CMD_BUF_NUM - 1))
        				cmd_num = cmd_num + 1;
        			else
        				cmd_num = 0;

        			cmd_string = cmd_buffer[cmd_num];
        			uartPrintf("%s", cmd_string);
        			len = strlen(cmd_string);
        		}
        		else if(input_char == 'C') // Right
        		{
        			// Same as Space
        			if(len < CMD_BUF_LEN-1)
        			{
        				input_char = 0x20; //Space
        				uartPutChar(input_char);
        				// append character to command string
        				strncat(cmd_string, &input_char, 1);
        				len++;
        			}
        		}
        		else if(input_char == 'D') // Left
        		{
        			// Same as Delete
        			if(len > 0)
        			{
        				uartPrintf("\b \b"); // delete: step back one character, print space, step back again
        				len--;
        				cmd_string[len]= '\0';
        			}
        		}
        		else // in any other case
        		{
        			// read the rest of the escape string and disregard
        			while(uartGetChar(&junk))
        			{
        	        	// allow some time to receive the next character
        	        	count = 1000; while(count--) __NOP();

        				// loop ends when uartGetChar() returns 0
        			}
        		}
        	}
    		else // some unexpected escape sequence
    		{
    			// read the rest of the escape string and disregard
    			while(uartGetChar(&junk))
    			{
    	        	// allow some time to receive the next character
    	        	count = 1000; while(count--) __NOP();

    				// loop ends when uartGetChar() returns 0
    			}
    		}
        	break;

        default:
        	if(len < CMD_BUF_LEN-1)
        	{
        		// append character to command string
        		if(input_char >= 0x20 && input_char <= 0x7D) //printable ASCII characters
        		{
        			strncat(cmd_string, &input_char, 1);
        			len++;

        			// echoing the input character
        			// (=> please disable local echo in terminal program)
        			uartPutChar(input_char);
        		}

        	}
        	else
        	{
        		cmd_string[len]= '\0';
        	}
        }
    }

    // check status of XMC4700 Relax Kit push buttons
    checkButtons();
    // Because the buttons are connected to ports without access to the ERU
    // they can't create interrupts. Therefore they need to be polled
    // We do this here simply because it's convenient ;-)

}



//****************************************************************************
// processHandler
//
// gets called every 100 ms by SYSTIMER
//****************************************************************************
void processHandler(void)
{
    // read content of all registers
    readAllRegisters();

    // compare register setting with previous state and report changes
    reportChanges();

    // set status LEDs on adaptor board
    setLEDs();

    // calculate all analog values for displaying them with Micrium GUI
    calculateAnalogValues();
}


//****************************************************************************
// prechargeMonitor
//
// gets called every 10 ms by SYSTIMER
//****************************************************************************
void prechargeMonitor(void)
{
    static double vbat = 0;      // battery voltage
    static double vld = 0;       // load voltage
    static double vres = 0;      // voltage across resistor (vbat - vld)
    static double power = 0;     // dissipated power [W]
    static double energy = 0;    // dissipated energy [Ws]
    static double time = 0;      // expired time [s]
    static double timeStep = 0.01;  // [s] function gets called every 10 ms

	// DIGITAL_IO_ToggleOutput(&LED_CHB); // for debugging

    // if driver is enabled and channel B is on
    if(DIGITAL_IO_GetInput(&ENABLE) && (getRegisterData(MOS_CHS_CTRL) & MOSONCH_B))
    {

		vbat = getVbat();
        vld = getVld();
    	vres = vbat - vld;
    	power = vres * vres / 10.0; // P = U²/R

    	// if more than 5 W are dissipated start integrating
    	// the power to get the energy in the resistor
    	if(power > 5.0)
    	{

    		energy += power * timeStep;
    		time += timeStep;

    		// switch off channel B when the dissipated energy exceeds 100 Ws
    		if(energy > 100.0)
    		{
    			clearBits(MOS_CHS_CTRL, MOSONCH_B); // switch off chan B

    	        uartPrintf("\b"); // backspace to delete prompt
    	        uartPrintf("[NOTE]: Channel B switched off due to precharge protection\r\n");
    	        uartPrintf("        Elapsed pulse time = %4.0f ms\r\n>", (time*1000));

    	        time = 0; // clear time counter
    		}
    	}
    }
    else // channel B is off
    {
    	time = 0; // clear time counter

     	// decrease energy counter by 5 J/s to avoid
    	// accumulation of energy by a series of pulses
    	// (20 s to cool off before next full precharge pulse)
    	// (values to be verified ...)
    	if(energy > 0.0)
    	{
    		energy -= 0.05;
    	}
    }

}
