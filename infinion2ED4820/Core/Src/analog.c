/*
 * analog.c
 *
 *  Created on: 10 Mar 2020
 *      Author: schwarzg
 */

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <math.h>
#include <registers.h>
#include <commands.h>

/*
 * global variables for use with micrium GUI
 */

XMC_VADC_RESULT_SIZE_t adc_result_cso = 0;
XMC_VADC_RESULT_SIZE_t adc_result_aout = 0;
XMC_VADC_RESULT_SIZE_t adc_result_vref = 0;
XMC_VADC_RESULT_SIZE_t adc_result_vbat = 0;
XMC_VADC_RESULT_SIZE_t adc_result_vld = 0;
XMC_VADC_RESULT_SIZE_t adc_result_ntc = 0;

// XMC_VADC_RESULT_SIZE_t = uint16_t

double ntc_voltage = 0;
double ntc_resitance = 0;
double ntc_temperature = 0;

double battery_voltage = 0;
double load_voltage = 0;

double cso_voltage = 0;
double cso_reference_voltage = 1.65; // cso voltage at zero load current (typ. vdd/2)
double shunt_voltage = 0;
double shunt_current = 0;

double aout_voltage = 0;
double vref_voltage = 0;
double diff_voltage = 0; // aout - vref
double hall_current = 0;



//****************************************************************************
// Adc_Measurement_Handler
// called by interrupt
// read ADC values and store them in global variables
//****************************************************************************
void Adc_Measurement_Handler(void)
{
	 adc_result_cso = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_CSO);
	 adc_result_aout = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_AOUT);
	 adc_result_vref = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_VREF);
	 adc_result_vbat = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_VBAT);
	 adc_result_vld = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_VLD);
	 adc_result_ntc = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_NTC);
}

//****************************************************************************
// calculateAnalogValues
//
// central routine to calculate all analog measurements
// gets called every 100 ms by process handler
//****************************************************************************
void calculateAnalogValues(void)
{
	uint8_t driverIsEnabled;
	static double vdd = 3.3;    // supply voltage
	static double lsb;          // lsb in V
	static double div;         // Resistor divider ratio of vbat and vld inputs
	static double hall_sensitivity;

	// variables for getting the gain setting
    static uint8_t reg_data;
	static uint8_t csag_setting;
	static double gain[8] = {10, 15, 20, 25, 31.5, 35, 40, 47.7}; // gain in V/V
	
	static double r_shunt = 0.00005; // R_shunt = 50 µOhm

	// NTC parameters
	static double b = 3434.0;        // NTC B25/85 value from data sheet
	static double r_25 = 1e4;        // NTC resistance at 25 deg. C (10k)
	static double r_s = 1e4;         // value of resistor in series to NTC

	lsb = vdd/4096; // lsb in V, 12 bit ADC
	div = 53.7/2.7; // Resistor divider 51k over 2.7k

	// calculate NTC temperature
	ntc_voltage = lsb * (double)adc_result_ntc;
	if(ntc_voltage == 0) ntc_voltage = lsb; // to avoid division by 0
	ntc_resitance = (r_s/((vdd/ntc_voltage)-1));
	ntc_temperature = ((b * 298.15) / (b + 298.15 * (log(ntc_resitance/r_25)))); // Temperature in K
	ntc_temperature -= 273; // Temperature in °C

	// calculate vbat and vld
	battery_voltage = (double)adc_result_vbat * lsb * div;
	load_voltage = (double)adc_result_vld * lsb * div;

	// get current sense amplifier gain setting
	reg_data = getRegisterData(CSAG_OCTH);    // get register data
	reg_data &= CSAG_MASK;                    // clear non CSAG bits
	csag_setting = (reg_data >> CSAG_POS);    // shift bits to LSB position

	// calculate shunt current
	cso_voltage = lsb * (double)adc_result_cso;
	shunt_voltage = (cso_voltage - cso_reference_voltage) / gain[csag_setting];

	driverIsEnabled = DIGITAL_IO_GetInput(&ENABLE);

	if(driverIsEnabled)
	{
	    shunt_current = shunt_voltage / r_shunt;
	}
	else
	{
		shunt_current = 0.0;
	}

	// calculate hall current
	aout_voltage = lsb * (double)adc_result_aout;
	vref_voltage = lsb * (double)adc_result_vref;
	hall_sensitivity = (double)getHallSensitivity();

	diff_voltage = aout_voltage - vref_voltage;
	hall_current = diff_voltage / (hall_sensitivity*1e-6); // sensitivity in µV/A

	// because of the orientation of the hall sensor
	// the "positive" current direction would be from LD to BAT
	// therefore we need to reverse the sign
	hall_current *= -1.0;
}


double getNtcTemperature()
{
	return ntc_temperature;
}

double getVbat()
{
	return battery_voltage;
}

double getVld()
{
	return load_voltage;
}

double getIshunt()
{
	return shunt_current;
}

double getIhall()
{
	return hall_current;
}

double getCsoReferenceVoltage()
{
	return cso_reference_voltage;
}

void setCsoReferenceVoltage()
{
	static double vdd = 3.3;    // supply voltage
	static double lsb;          // lsb in V

	lsb = vdd/4096; // lsb in V, 12 bit ADC
	adc_result_cso = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_CSO);

	cso_voltage = lsb * (double)adc_result_cso;

	// if measured voltage is too far from vdd/2 there must be something wrong
	// set the reference voltage only if it is in a reasonable range
	if(cso_voltage > 1.6 && cso_voltage < 1.7)
	{
		cso_reference_voltage = cso_voltage;
	}

}
