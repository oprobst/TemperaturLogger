
#include <stdint.h>
#include <avr/eeprom.h>

#include "eeprom-store.h";

uint16_t eeprom_curr_pos = 0;
uint8_t end_of_buffer = FALSE;

void write_next_value (uint8_t value){
	if (end_of_buffer == TRUE){
		return;
	} 
	eeprom_update_byte((uint8_t *) eeprom_curr_pos, value);	
	if (++eeprom_curr_pos == EEPROM_SIZE-1){
		end_of_buffer = TRUE;
	} else {
		eeprom_update_byte((uint8_t *) eeprom_curr_pos + 1, 0xFF);
	}
}

int read_next_value (void){
	if (end_of_buffer == TRUE){
		return -1;
	}
	uint8_t value = eeprom_read_byte((uint8_t *) eeprom_curr_pos);
	if (++eeprom_curr_pos == EEPROM_SIZE-1){
		end_of_buffer = TRUE;
	} else if (value == 0xFF){
		end_of_buffer = TRUE;
		return -1;
	}
	return value;	
}

uint8_t is_buffer_full (void){
	return end_of_buffer;
}

void eeprom_reset_current_address (void){
	eeprom_curr_pos = 0;
	end_of_buffer = FALSE;
}

