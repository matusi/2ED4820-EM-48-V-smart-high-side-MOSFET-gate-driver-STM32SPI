/*
 * analog.h
 *
 *  Created on: 10 june 2023
 *      Author: Chokri
 */

#ifndef ANALOG_H_
#define ANALOG_H_

void getAdcResults();
double getNtcTemperature();
double getVbat();
double getVld();
double getIshunt();
double getIhall();

double getCsoReferenceVoltage(void);
void setCsoReferenceVoltage(void);


void calculateAnalogValues(void);


#endif /* ANALOG_H_ */
