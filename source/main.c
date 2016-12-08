
#include "main.h"

volatile uint32_t secondsSinceLastMeasurement = 0;


ISR (TIMER1_COMPA_vect) {
	secondsSinceLastMeasurement++;
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main( void )
{
	//Outputs:
	//PB0: LED 
	//PB1: 5V supply power enable 
	DDRB |= (1 << PB0 | 1 << PB1);
	//PA3: Power up supply sensor
	DDRA |= (1 << PA3);
	
	//Clock on PB2
	//CLKCR |= (1 << CKOUTC);
	
	set_start_temperature(5);
	set_end_temperature(30);
	set_resolution(1);
	set_intervall(3);
	
	init_adc();
	init_interrupts ();
	init_converter();
	TSIC_INIT();	// Init TSIC Temperatursensor
	
	sei();
	
	uint16_t supplyVolt = measure_supply_voltage();
	
	//measure_mode(); //outcomment me to debug on pc		 		   
		    
	if (supplyVolt < 4500 && supplyVolt > 1000){
		// Set UART pins to 0, to prevent reverse voltage from ft232 VCC pin (~ 1V if RX/TX = Hi)
		DDRA |= (1 << PA1 | 1 << PA2);
		PORTA |= (0 << PA1  | 0 << PA2);
		measure_mode();
	} else{
		uart_init();
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
	ACSR1A |= ((1 << ACD1) ); //disable ac
	ACSR1A |= ((1 << ACD1) ); //disable ac
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	sleep_cpu();
	sleep_disable();
}


void measure_mode(){
	for (uint8_t i = 0; i < 10; i++){
		PORTB ^= (1<<PB0);
		_delay_ms(200);
	}
	while(1) {
		
		if (secondsSinceLastMeasurement >= get_interval()){
			POWER_HI
			_delay_ms(2); 
			PORTB ^= (1<<PB0);
			ACSR1A |= ((0 << ACD1) ); //enable ac
			ACSR0A |= ((0 << ACD1) ); //enable ac
			uint16_t temp;
			getTSicTemp(&temp);			
			uint32_t write = to_units(temp);		
		    write_next_value(write);
			POWER_LO
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
uint16_t temperature = 0;
void output_mode (){
    uart_transmit_string("ENTERING OUTPUT MODE");
	while(1)
	{
		char command = uart_receive();
		if (command == 'T'){
			_delay_ms(0);
			if (getTSicTemp(&temperature)){ //turn the TSIC-Sensor ON -> messure -> OFF
				//temperature = (( temperature * 250L) >> 8) - 500;				// Temperatur *10 also 26,4 = 264
			    temperature = (temperature * 200.0/2048.0 -50)* 10.0 ;
				uart_transmit_string("T");
				uart_transmit_integer(temperature);		
				uart_transmit_string("t");		
				uart_transmit_string("\n\rU");
				uart_transmit_integer(to_units(temperature));	
				uart_transmit_string(" => ");
					uart_transmit_integer(to_temperatur(to_units(temperature)));	
					uart_transmit_string("u\n\r");
			}
		} else if (command == 'M'){
			
			
		} else if (command == 'V'){
		if (getTSicTemp(&temperature)){ //turn the TSIC-Sensor ON -> messure -> OFF
			temperature = ((temperature * 250L) >> 8) - 500;				// Temperatur *10 also 26,4 = 264
			uart_transmit_string("Current temperature:");
			uart_transmit_integer(temperature);
			uart_transmit_string("\n\r");
			 
		}
		
		uart_transmit_string("Sending EEPROM! \n\r");
		uart_transmit_string("\n\r Resolution: ");
		uart_transmit_integer(get_resolution());
		uart_transmit_string("\n\r From range: ");
		uart_transmit_integer(get_start_temperature());
		uart_transmit_string("\n\r To range: ");
		uart_transmit_integer(get_end_temperature());
		uart_transmit_string("\n\r Interval : ");
		uart_transmit_integer(get_interval());
		uart_transmit_string(" sec.\n\r ");
				uart_transmit_string("\n\r Voltage : ");
				uart_transmit_integer(measure_supply_voltage());
				uart_transmit_string(" mV\n\r ");
		
		uint16_t nextValue = 0;
		while(! is_buffer_full()){
			    nextValue = read_next_value();			
				uint16_t output = to_temperatur(nextValue);				
				uart_transmit_string(",");
				uart_transmit_integer(output);		
		}
		
		uart_transmit_string(" \n\r End of EEPROM... \n\r");
		eeprom_reset_current_address();
		} else {
			uart_transmit_string("Unknown command \n\r");
		}
		
	}
}




void init_interrupts (){
	//every second
		
	TCCR1B |= (1 << WGM12); // CTC Modus
	TIMSK1 |= (1<<OCIE1A); // Allow interrupt
	TCCR1B |= (0<<CS12) | (1<<CS11) | (1<<CS10); // Prescaler 64 
	OCR1A = 15624; 
	
}

