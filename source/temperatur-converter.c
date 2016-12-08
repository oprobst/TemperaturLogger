 
 #include <stdint.h>
 
 #include "temperatur-converter.h"
 #include "eeprom-store.h"
 
 uint32_t range ;
 double unit ; 
 
void init_converter (){
	  range = get_end_temperature() * 100 - get_start_temperature() * 100;
	  uint32_t res = 1;
	  for (uint8_t i = 0; i < 8 * get_resolution(); i++)
		res = res * 2;
	  unit = (double)  range /(double) res  + .5;
	  if (unit == 0)
		unit = 1;
}

 uint16_t to_units(int32_t temperatur){
	uint32_t temp_wo_offset = temperatur - get_start_temperature() * 100; 
	uint16_t rest = (uint32_t)(temp_wo_offset + (0.5 * unit)) % (uint16_t) (unit + 0.5) ;
	uint32_t units = temp_wo_offset - rest;
	return units/unit;
 }

 int32_t to_temperatur(uint16_t units){
	 
	 return units * unit + get_start_temperature() * 100;
 }