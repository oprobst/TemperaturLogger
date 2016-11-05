

#ifndef EEPROM_STORE_H
#define EEPROM_STORE_H

#define EEPROM_SIZE 500

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*
 * Write a value to the next free byte in eeprom
 */
void write_next_value (int32_t value);

/*
 * Read next byte in eeprom
 */
uint64_t read_next_value (void);

/*
 * Return 1 if read or write came to the end of eeprom or last byte in read mode
 */
uint8_t is_buffer_full (void);

/*
 * reset pointer to the begining of eeprom, usually only for testing purposes.
 */
void eeprom_reset_current_address (void);

void set_start_temperature (int8_t startTemp);
int8_t get_start_temperature ();

void set_end_temperature (int8_t endTemp);
int8_t get_end_temperature ();

void set_resolution (uint8_t res);
uint8_t get_resolution ();

void set_intervall (uint16_t value);
uint16_t get_intervall ();
#endif

 