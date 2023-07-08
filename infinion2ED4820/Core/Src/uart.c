/*
 * uart.c
 *
 *  Created on: 4 Mar 2020
 *      Author: schwarzg
 */

#include <DAVE.h>   //Declarations from DAVE Code Generation (includes SFR declaration)
#include <stdio.h>
#include <stdarg.h> // for va list in uartPrintff
// #include <stdlib.h> // includes strtol

//****************************************************************************
// uartPrintf
//
// printf function for output to UART
//****************************************************************************
void uartPrintf(const char *p_frm, ...)
{
    va_list p_arg;
    static char buf[100];

    va_start(p_arg, p_frm);
    vsprintf(buf,p_frm,p_arg);
    va_end(p_arg);
    UART_Transmit(&UART_0, (uint8_t*)buf, strlen(buf));
}


//****************************************************************************
// uartGetChar
//
// Read one character from UART buffer
// Returns 1 if character was present, otherwise 0
//****************************************************************************
int uartGetChar(char *uart_char)
{
	if(!UART_IsRXFIFOEmpty(&UART_0)) // check if receive buffer contains any data
	{
		*uart_char = (char)UART_GetReceivedWord(&UART_0);  // get one character from FIFO
		return 1;
	}
	else
		return 0;
}




//****************************************************************************
// uartPutChar (direct mode)
//
// write one character to UART
//****************************************************************************
void uartPutChar(char uart_char)
{
    if(uart_char >= 0x20 && uart_char <= 0x7D) //printable ASCII characters
    {
    	UART_TransmitWord(&UART_0,(uint8_t) uart_char);
    }
}

