#ifndef F_CPU
#define F_CPU 1000000
#endif

#ifndef BAUD
#define BAUD 4800
#endif 

#include <util/setbaud.h>
#include <avr/io.h>
#include <stdio.h> 

#include "uart.h"


void uart_init(void)
{
	REMAP = (1 << U0MAP);
	
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	UCSR0A &= ~(1 << U2X0);
	
	UCSR0B = ( ( 1 << RXEN0 ) | ( 1 << TXEN0 ) );
	UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);

}



void uart_transmit_integer(int32_t value)
{
	char data[16];
	sprintf(data, "%ld", value);
	uart_transmit_string(data);
}

void uart_transmit_string(char *data)
{
	while( *data != '\0' )
	uart_transmit (*data++);
	
}

/* Read and write functions */
unsigned char uart_receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) )	;
	return UDR0;
}

void uart_transmit( unsigned char data )
{
	while ( !(UCSR0A & (1<<UDRE0)) );
	
	UDR0 = data;
	
}