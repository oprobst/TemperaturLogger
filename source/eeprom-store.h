

#include <stdint.h>

#ifndef EEPROM_STORE_H
#define EEPROM_STORE_H

#define EEPROM_SIZE 512

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


void write_next_value (uint8_t value);

int read_next_value (void);

uint8_t is_buffer_full (void);

void eeprom_reset_current_address (void);

#endif

 