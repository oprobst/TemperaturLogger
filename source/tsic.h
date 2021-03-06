#ifndef tsic_H
#define tsic_H
#include "Main.h"
/******************************************************************************/
// TSIC Signal-Pin
#define TSIC_PORT             PORTA  // Port to use
#define TSIC_PIN              PINA
#define TSIC_PORT_DDR         DDRA
#define TSIC_SIGNAL_PIN       (1<<PA0)

// TSIC VCC
#define TSIC_PORT_Power       PORTA
#define TSIC_PORT_DDR_Power   DDRA
#define TSCI_POWER_PIN        (1<<PA3)
/******************************************************************************/

#define TSIC_SIGNAL_HIGH	TSIC_PIN & TSIC_SIGNAL_PIN				// High Signal of the TSIC-Sensor
#define TSIC_SIGNAL_LOW		!( TSIC_PIN & TSIC_SIGNAL_PIN)			// Low Signal of the TSIC-Sensor
#define TSIC_ON()			TSIC_PORT_Power |=  TSCI_POWER_PIN		// Power up the TSIC-Sensor
#define TSIC_OFF()			TSIC_PORT_Power &= ~TSCI_POWER_PIN		// Power down the TSIC-Sensor
#define Abbruch()			if (timeout==0){return 0;}				// Abbruch, falls der sensor abgezogen wird

void TSIC_INIT(void);
uint8_t getTSicTemp(int32_t *temp_value16);
uint8_t readSens(uint16_t *temp_value);
uint8_t checkParity(uint16_t *temp_value);



#endif