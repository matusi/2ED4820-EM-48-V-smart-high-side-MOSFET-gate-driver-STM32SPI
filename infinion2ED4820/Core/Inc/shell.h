/*
 * shell.h
 *
 *  Created on: 20 Feb 2020
 *      Author: schwarzg
 */

#ifndef SHELL_H_
#define SHELL_H_

#define CMD_BUF_LEN 	25		// maximum length of command string
#define CMD_BUF_NUM 	10		// number of commands for command history


void shellInit(void);
void processHandler(void);
void commandLineHandler(void);
void prechargeMonitor(void);


#endif /* SHELL_H_ */
