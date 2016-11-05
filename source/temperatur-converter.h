#ifndef TEMPERATURCONVERTER_H_
#define TEMPERATURCONVERTER_H_

/*
 Converts a real temperatur to a fictive 'unit'. 
 
 A unit is calculated by unit= (end_temperatur_range - start_temperatur_range) / 2 ^ (8 * resolution)
 
*/
void init_converter ();

uint16_t to_units(int32_t temperatur);

int32_t to_temperatur(uint16_t units);

#endif /* TEMPERATUR-CONVERTER_H_ */