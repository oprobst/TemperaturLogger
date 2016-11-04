#include <avr/io.h>

#define F_CPU 1000000
#define BAUD 4800
#define MEASUREMENT_INTERVALL 2000 * 60 //every two minutes

#include <stdint.h>
#include <util/setbaud.h>
#include <util/delay.h>

#include "eeprom-store.h"

void uart_init(void)
{
	REMAP = (1 << U0MAP);
	
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	UCSR0A &= ~(1 << U2X0);
	
	UCSR0B = ( ( 1 << RXEN0 ) | ( 1 << TXEN0 ) );
	UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);

}
/* Prototypes */
uint16_t measure_supply(void);
void USART0_Init( unsigned int baudrate );
unsigned char USART0_Receive( void );
void USART0_Transmit( unsigned char data );
uint16_t readADCsamples( uint8_t nsamples );
uint16_t readSingleADC(void);
uint16_t readADC (volatile uint8_t channel) ;
void OutputFloat(char *data);
void USART0_Transmit_Integer(uint16_t value);
uint16_t readTemperatur (volatile uint16_t supplyVoltage);

uint16_t readInternalTemperatur (volatile uint16_t supplyVoltage);

/* Main - a simple test program*/
int main( void )
{
	//LED - output
	DDRB |= ((1 << PB0) );
	
	uart_init();
	
	initADC();
	
	uint16_t supplyVolt = measure_supply();
	
	if (supplyVolt < 4500){
		while(1) {
			
			PORTB ^= (1<<PB0);
			uint16_t temp = readTemperatur(measure_supply());
			uint8_t write = (temp / 10.0);
			write_next_value(write);
			PORTB ^= (1<<PB0);
			_delay_ms(MEASUREMENT_INTERVALL);
			if (is_buffer_full()){
				while(1)
				{
					_delay_ms(1000);
					PORTB ^= (1<<PB0);
					_delay_ms(20);
					PORTB ^= (1<<PB0);
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
			
			USART0_Transmit_String("Ready for sending EEPROM! \n\r");
			USART0_Receive();
			int nextValue = 0;
			do{
				nextValue = read_next_value();
				if (nextValue != -1){
					USART0_Transmit_String("\n\r");
					USART0_Transmit_Integer(nextValue);
				}
			} while (nextValue != -1);
			
			USART0_Transmit_String(" \n\r End of EEPROM... \n\r");
			eeprom_reset_current_address();
			USART0_Receive();
			
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



void USART0_Transmit_Integer(uint16_t value)
{
	char data[16];
	sprintf(data, "%d", value);
	USART0_Transmit_String(data);
}

void USART0_Transmit_String(char *data)
{
	while( *data != '\0' )
	USART0_Transmit (*data++);
	
}

/* Read and write functions */
unsigned char USART0_Receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) )	;
	return UDR0;
}

void USART0_Transmit( unsigned char data )
{
	while ( !(UCSR0A & (1<<UDRE0)) );
	
	UDR0 = data;
	
}

int blnk(void)
{
	
	
	
	while (1)
	{
		PORTB |= (1<<PB0);
		_delay_ms(1400);
		PORTB &= ~(1 << PB0);
		_delay_ms(1400);
	}
}





void initADC (){


	//ADC enabled, prescaler
	ADCSRA =
	(1 << ADEN)  |
	(0 << ADPS2) |
	(1 << ADPS1) |
	(1 << ADPS0);
	
}


uint16_t readTemperatur (volatile uint16_t supplyVoltage) {
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
	return (((measure / 1023.0) * supplyVoltage / 1000.0) - 2.7315) * 1000.0;
}



uint16_t readInternalTemperatur (volatile uint16_t supplyVoltage) {
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


uint16_t readADC (volatile uint8_t channel) {
	ADMUXB=
	(0 << REFS2) |
	(0 << REFS1) |
	(0 << REFS0) |
	(0 << GSEL1) |
	(0 << GSEL0);
	
	switch (channel){
		case 0: //Temp Sensor
		ADMUXA =
		(0 << MUX3)  |
		(0 << MUX2)  |
		(0 << MUX1)  |
		(0 << MUX0);
		return readADCsamples(3);
		case 1: //Voltage
		ADMUXA =
		(0 << MUX3)  |
		(0 << MUX2)  |
		(0 << MUX1)  |
		(1 << MUX0);
		return readADCsamples(3);
		case 2: //Voltage
		ADMUXA =
		(1 << MUX3)  |
		(1 << MUX2)  |
		(1 << MUX1)  |
		(1 << MUX0);
		return readADCsamples(3);
		
		default: //internal temp sensor
		ADMUXA =
		(1 << MUX3)  |
		(1 << MUX2)  |
		(0 << MUX1)  |
		(0 << MUX0);
		return readADCsamples(3);
		
	}
	
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


// measure supply voltage in mV
uint16_t measure_supply(void)
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
	
}


/*
* Do a single ADC read
*/
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