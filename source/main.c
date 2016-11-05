#define F_CPU 1000000
#define BAUD 4800

//#define MEASUREMENT_INTERVALL 2000 * 60 //every two minutes
#define MEASUREMENT_INTERVALL 1000 //every second

#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "adc-measurements.h"
#include "eeprom-store.h"
#include "uart.h"
 
void power_down();
void measure_mode();
void output_mode ();
void init_interrupts ();

volatile uint32_t secondsSinceLastMeasurement = 0;


ISR (TIMER1_COMPA_vect) {
	secondsSinceLastMeasurement++;
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main( void )
{
	//LED - output
	DDRB |= ((1 << PB0) );
	
	set_start_temperature(0);
	set_end_temperature(30);
	set_resolution(1);
	set_intervall(2);
	
	uart_init();
	
	init_adc();
	init_interrupts ();
	
	sei();
	
	uint16_t supplyVolt = measure_supply_voltage();
	
	measure_mode();
	//output_mode ();
	    
	if (supplyVolt > 4500){
		measure_mode();
	} else{
	    output_mode ();
	}
}



void power_down(){
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_cpu();
	sleep_disable();
}

void idle(){
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	sleep_cpu();
	sleep_disable();
}


void measure_mode(){
	while(1) {
		
		if (secondsSinceLastMeasurement >= get_intervall()){
			PORTB ^= (1<<PB0);
			uint16_t temp = measure_temperature_sensor(measure_supply_voltage());
			uint8_t write = (temp / 10.0);
			write_next_value(write);
			PORTB ^= (1<<PB0);
		
			if (is_buffer_full()){
				while(1)
				{
					power_down();
				}
			}
			secondsSinceLastMeasurement = 0;
			idle ();
		}
		
	}
}

void output_mode (){
	for (int i = 0;  i < 10;i++){
		_delay_ms(100);
		PORTB ^= (1<<PB0);
	}
	while(1)
	{
		
		uart_transmit_string("Ready for sending EEPROM! \n\r");
		uart_receive();
		uart_transmit_string("\n\r Resolution: ");
		uart_transmit_integer(get_resolution());
		uart_transmit_string("\n\r From range: ");
		uart_transmit_integer(get_start_temperature());
		uart_transmit_string("\n\r To range: ");
		uart_transmit_integer(get_end_temperature());
		uart_transmit_string("\n\r ");
		int nextValue = 0;
		while(! is_buffer_full()){
			nextValue = read_next_value();			
				uart_transmit_string(",");
				uart_transmit_integer(nextValue);		
		}
		
		uart_transmit_string(" \n\r End of EEPROM... \n\r");
		eeprom_reset_current_address();
		uart_receive();
		
	}
}




void init_interrupts (){
	//every second
		
	TCCR1B |= (1 << WGM12); // CTC Modus
	TIMSK1 |= (1<<OCIE1A); // Allow interrupt
	//TCCR1B |= (1<<CS12) | (0<<CS11) | (0<<CS10); // Prescaler 64
	TCCR1B |= (0<<CS12) | (1<<CS11) | (1<<CS10); // Prescaler 64 
	OCR1A = 15624; 
	
}

