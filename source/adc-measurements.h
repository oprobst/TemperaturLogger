#ifndef ADCMEASUREMENTS_H_
#define ADCMEASUREMENTS_H_


void init_adc ();

uint16_t measure_temperature_sensor (volatile uint16_t supplyVoltage);
	
uint16_t measure_internal_temperature ();

uint16_t measure_supply_voltage(void);

void init_adc ();

#endif /* ADCMEASUREMENTS_H_ */