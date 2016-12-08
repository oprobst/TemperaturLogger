#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "adc-measurements.h"


uint16_t readADCsamples( uint8_t nsamples );
uint16_t readSingleADC(void);

void init_adc (){

	//ADC enabled, prescaler
	ADCSRA =
	(1 << ADEN)  |
	(0 << ADPS2) |
	(1 << ADPS1) |
	(1 << ADPS0);
		
}


uint32_t measure_temperature_sensor (volatile uint16_t supplyVoltage) {
	ADMUXB=
	(0 << REFS2) |
	(0 << REFS1) |
	(0 << REFS0) |
	(0 << GSEL1) |
	(0 << GSEL0);


	ADMUXA =
	(0 << MUX3)  |
	(0 << MUX2)  |
	(0 << MUX1)  |
	(0 << MUX0);
	
	uint16_t measure = readADCsamples(5);
	return (((measure / 1023.0) * supplyVoltage / 1000.0) - 2.7315) * 10000.0;
}



uint16_t measure_internal_temperature (volatile uint16_t supplyVoltage) {
	ADMUXB=
	(0 << REFS2) |
	(0 << REFS1) |
	(0 << REFS0) |
	(0 << GSEL1) |
	(0 << GSEL0);


	ADMUXA =
	(1 << MUX3)  |
	(1 << MUX2)  |
	(1 << MUX1)  |
	(1 << MUX0);
	
	uint16_t measure = readADCsamples(5);
	return (((measure / 1023.0) * supplyVoltage / 1000.0) - 2.7315) * 100.0;
}


// measure supply voltage in mV
uint16_t measure_supply_voltage(void)
{
	ADMUXB= (0 << REFS2)|(0 << REFS1)|(0 << REFS0); //VCC as reference
	ADMUXA= ((1 << MUX3)|(1 << MUX2)|(0 << MUX1)|(1 << MUX0)); // measure internal 1.1V

	_delay_us(500);

	uint32_t sum = 0;
	uint8_t nsamples = 3;
	_delay_us(500);
	for (uint8_t i = 0; i < nsamples; ++i ) {
		ADCSRA |= (1 << ADSC);        // start conversion
		while (ADCSRA & (1 << ADSC)); // wait to finish
		sum += (1100UL*1023/ADC);     // AVcc = Vbg/ADC*1023 = 1.1V*1023/ADC
	}
	
	
	return (uint16_t)( sum / nsamples );
	/*
	uint16_t vcc =(uint16_t)( sum / nsamples );
	ADMUXA= ((0 << MUX3)|(1 << MUX2)|(1 << MUX1)|(1 << MUX0)); // measure ADC7 = battery voltage
	
	sum = 0;
	nsamples = 3;
	_delay_us(500);
	for (uint8_t i = 0; i < nsamples; ++i ) {
		ADCSRA |= (1 << ADSC);        // start conversion
		while (ADCSRA & (1 << ADSC)); // wait to finish
		sum +=  (((double)vcc)/1023.0*ADC);     // AVcc = Vbg/ADC*1023 = 1.1V*1023/ADC
	}
	return (uint16_t)( sum / nsamples );
	*/
	
}

uint16_t readADCsamples( uint8_t nsamples )
{
	uint32_t sum = 0;
	_delay_us(500);
	for (uint8_t i = 0; i < nsamples; ++i ) {
		sum += readSingleADC( );
	}
	
	return (uint16_t)( sum / nsamples );
}





uint16_t readSingleADC(void)
{
	
	uint8_t adc_lobyte; // to hold the low byte of the ADC register (ADCL)
	uint16_t raw_adc;
	
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete

	// for 10-bit resolution:
	adc_lobyte = ADCL; // get the sample value from ADCL
	raw_adc = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte

	return raw_adc;
}