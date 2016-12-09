#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 1000000 

#define POWER_HI PORTB |= (1<<PB1);
#define POWER_LO PORTB ^= (1<<PB1);

//#define MEASUREMENT_INTERVALL 2000 * 60 //every two minutes
#define MEASUREMENT_INTERVALL 1000 //every second

#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "adc-measurements.h"
#include "eeprom-store.h"
#include "uart.h"
#include "temperatur-converter.h"
#include "tsic.h"




#define TRUE				1
#define FALSE				0

void power_down();
void measure_mode();
void output_mode ();
void init_interrupts ();
void print_help();
void show_configuration();

#endif /* MAIN_H_ */