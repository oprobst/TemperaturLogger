
#ifndef UART_H_
#define UART_H_


void uart_init(void);

void uart_transmit_integer(int32_t value);

void uart_transmit_string(char *data);

unsigned char uart_receive( void );

void uart_transmit( unsigned char data );

uint16_t read_config_value ();

#endif /* UART_H_ */