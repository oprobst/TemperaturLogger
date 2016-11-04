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

/*
 * Write a value to the next free byte in eeprom
 */
void write_next_value (uint8_t value);

/*
 * Read next byte in eeprom
 */
int read_next_value (void);

/*
 * Return 1 if read or write came to the end of eeprom or last byte in read mode
 */
uint8_t is_buffer_full (void);

/*
 * reset pointer to the begining of eeprom, usually only for testing purposes.
 */
void eeprom_reset_current_address (void);

#endif

 