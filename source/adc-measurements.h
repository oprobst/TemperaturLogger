#ifndef ADC-MEASUREMENTS_H_
#define ADC-MEASUREMENTS_H_


void init_adc (){

uint16_t measure_temperature_sensor (volatile uint16_t supplyVoltage);
	
uint16_t measure_internal_temperature ();

uint16_t measure_supply_voltage(void);

void init_adc ();

#endif /* ADC-MEASUREMENTS_H_ */