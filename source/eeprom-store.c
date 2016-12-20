
#include <stdint.h>
#include <avr/eeprom.h>

#include "eeprom-store.h"

#define EEPROM_DATA_STARTPOS 6

uint16_t eeprom_curr_pos = EEPROM_DATA_STARTPOS;
uint8_t end_of_buffer = FALSE;


void write_next_value (int32_t value){
	if (end_of_buffer == TRUE){
		return;
	} 
	uint32_t cPointerVal = 1;
	while (get_protected_mode() == 1 &&  cPointerVal != 0 &&  eeprom_curr_pos < EEPROM_SIZE-1){		
		eeprom_read_block (& cPointerVal, (void *) eeprom_curr_pos, get_resolution());			
		if (cPointerVal != 0){
			eeprom_curr_pos = eeprom_curr_pos + get_resolution();
		}	
	}
		
	//eeprom_update_byte((void *) eeprom_curr_pos, value);	
	eeprom_update_block( & value, (void *) eeprom_curr_pos, get_resolution());
    eeprom_curr_pos = eeprom_curr_pos + get_resolution();
	if (eeprom_curr_pos >= EEPROM_SIZE-1){
		end_of_buffer = TRUE;
	} else {
		eeprom_update_word((uint16_t *) (eeprom_curr_pos), 0);
	}
}

uint16_t read_next_value (void){
	if (end_of_buffer == TRUE){
		return 0x00;
	}
	
	int16_t value = 0;
	eeprom_read_block (& value, (void *) eeprom_curr_pos, get_resolution());
	eeprom_curr_pos = eeprom_curr_pos + get_resolution();
	if (eeprom_curr_pos >= EEPROM_SIZE-1){
		end_of_buffer = TRUE;
	} 
	return value;	
}

uint8_t clear(void){
	eeprom_curr_pos = EEPROM_DATA_STARTPOS;
	while(1){
	  
	  eeprom_update_word((uint16_t *) (eeprom_curr_pos), 0);
	  eeprom_curr_pos = eeprom_curr_pos + get_resolution();
	  if (eeprom_curr_pos >= EEPROM_SIZE-1){
		  end_of_buffer = TRUE;
		  return;
	  }
	}
}

uint8_t is_buffer_full (void){
	return end_of_buffer;
}

void eeprom_reset_current_address (void){
	eeprom_curr_pos = EEPROM_DATA_STARTPOS;
	end_of_buffer = FALSE;
}



void set_resolution (uint8_t res){
	eeprom_update_byte((uint8_t *) 0, res);
}

uint8_t get_resolution (){
	uint8_t res = eeprom_read_byte((uint8_t *) 0);
	if (res != 1 && res != 2 && res != 4){
		res = 1;
	}
	return res;
}

void set_start_temperature (int8_t startTemp){
	eeprom_update_byte((uint8_t *) 1, startTemp);
}

int8_t get_start_temperature (){
	int8_t ret = eeprom_read_byte((uint8_t *) 1);
	if (ret == 0xFF){
		ret = 0;
	}
	return ret;
}

void set_end_temperature (int8_t endTemp){
	eeprom_update_byte((uint8_t *) 2, endTemp);
}

int8_t get_end_temperature (){
		int8_t ret = eeprom_read_byte((uint8_t *) 2);
		if (ret == 0xFF){
			ret = 100;
		}
		return ret;
}


void set_intervall (uint16_t value){
	eeprom_update_word((uint16_t *) 3, value);
}

uint16_t get_interval (){
	uint16_t ret = eeprom_read_word((uint16_t *) 3);
	if (ret == 0xFFFF){
		ret = 1;
	}
	return ret;
}

void set_protected_mode (int8_t val){
	eeprom_update_byte((uint8_t *) 4, val);
}

int8_t get_protected_mode (){
	int8_t ret = eeprom_read_byte((uint8_t *) 4);
	if (ret != 1 && ret != 0){
		return 0;
	}
	return ret;
}