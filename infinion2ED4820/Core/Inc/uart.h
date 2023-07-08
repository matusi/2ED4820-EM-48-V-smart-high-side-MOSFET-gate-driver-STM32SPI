/*
 * uart.h
 *
 *  Created on: 4 Mar 2020
 *      Author: schwarzg
 */

#ifndef UART_H_
#define UART_H_

int uartGetChar(char *input_char);
void uartPutChar(char input_char);
void uartPrintf(const char *p_frm, ...);

#endif /* UART_H_ */
