#define F_CPU 1000000
#define BAUD 4800

#define MEASUREMENT_INTERVALL 2000 * 60 //every two minutes

#include <stdint.h>

#include <util/delay.h>
#include <avr/sleep.h>

#include "adc-measurements.h"
#include "eeprom-store.h"
#include "uart.h"
 

void power_down(){
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();	
	sleep_cpu();
	sleep_disable();
}

/* Main - a simple test program*/
int main( void )
{
	//LED - output
	DDRB |= ((1 << PB0) );
	
	uart_init();
	
	init_adc();
	
	uint16_t supplyVolt = measure_supply_voltage();
	
	if (supplyVolt < 4500){
		while(1) {
			
			PORTB ^= (1<<PB0);
			uint16_t temp = measure_temperature_sensor(measure_supply_voltage());
			uint8_t write = (temp / 10.0);
			write_next_value(write);
			PORTB ^= (1<<PB0);
			_delay_ms(MEASUREMENT_INTERVALL);
			if (is_buffer_full()){
				while(1)
				{
					power_down();
				}
			}
			
		}
	} else{
		for (int i = 0;  i < 10;i++){
			_delay_ms(100);
			PORTB ^= (1<<PB0);
		}
		while(1)
		{
			
			uart_transmit_string("Ready for sending EEPROM! \n\r");
			uart_receive();
			int nextValue = 0;
			do{
				nextValue = read_next_value();
				if (nextValue != -1){
					uart_transmit_string("\n\r");
					uart_transmit_integer(nextValue);
				}
			} while (nextValue != -1);
			
			uart_transmit_string(" \n\r End of EEPROM... \n\r");
			eeprom_reset_current_address();
			uart_receive();
			
			/*
			USART0_Transmit_String("\n\r");
			USART0_Transmit_String("U=");
			USART0_Transmit_Integer(supplyVolt);
			USART0_Transmit_String("mV ");
			
			uint16_t temp = readTemperatur(supplyVolt);
			USART0_Transmit_String(" T=");
			
			USART0_Transmit_Integer(temp);
			
			temp = readInternalTemperatur(supplyVolt);
			USART0_Transmit_String(" Ti=");
			
			USART0_Transmit_Integer(temp);
			*/
			
			//USART0_Transmit_String(c);
			
			//UART_Transmit_float(123.4f);
			//USART0_Transmit(USART0_Receive()); /* Echo the received character */
			//  USART0_Receive();
			//_delay_ms(1000);
		}
	}
}




