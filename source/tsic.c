/*
 Library for TSIC digital Temperature Sensor Type 206/306 and may more.
 using ZACwire? Communication Protocol for the TSic?
 Manufatorer:    IST AG, Hygrosens, ZMD
 rewritten by:    Rolf Wagner
 Date:        2013.05.09
 Original Code by Rogger (http://www.mikrocontroller.net/topic/159774)
       ________
GND     ---| TSIC  |
Signal  ---|206/306  |
Vcc-Pin  ---|________|

 Der Anschluss erfolgt mit GND and GND, die Pins VCC und Signal werden jeweils an einem schaltbaren uC Pin angeschlossen.
 Es werden also 2 Pins benötigt.
 Zwischen VCC und Masse wird ein 100nF Kondensator angeschlossen und der Pin von VCC läuft über einen 220 Ohm Widerstand (wie im Datenblatt beschrieben).

 */

#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <stdint.h>
#include <util/delay.h>

#include "tsic.h"
 
void TSIC_INIT(void) {
  TSIC_PORT_DDR_Power |= TSCI_POWER_PIN;    // Ausgang
  TSIC_PORT_DDR &= ~TSIC_SIGNAL_PIN;      // Eingang
}

uint8_t readSens(uint16_t *temp_value){
  uint16_t strobelength = 0;
  uint16_t strobetemp = 0;
  uint16_t dummy = 0;
  uint16_t timeout = 0;

  while (TSIC_SIGNAL_HIGH)
    ; // wait until start bit starts
  // Measure strobe time
  strobelength = 0;
  timeout = 0;
  while (TSIC_SIGNAL_LOW) {    // wait for rising edge
    strobelength++;
    timeout++;
    ABORT();
  }
  for (uint8_t i=0; i<9; i++) {
    // Wait for bit start
    timeout = 0;
    while (TSIC_SIGNAL_HIGH) { // wait for falling edge
      timeout++;
      ABORT();
    }
    // Delay strobe length
    timeout = 0;
    dummy = 0;
    strobetemp = strobelength;
    while (strobetemp--) {
      timeout++;
      dummy++;
      ABORT();
    }
    *temp_value <<= 1;
    // Read bit
    if (TSIC_SIGNAL_HIGH) {
      *temp_value |= 1;
    }
    // Wait for bit end
    timeout = 0;
    while (TSIC_SIGNAL_LOW) {    // wait for rising edge
      timeout++;
      ABORT();
    }
  }
  return 1;
}

uint8_t checkParity(uint16_t *temp_value) {
  uint8_t parity = 0;

  for (uint8_t i = 0; i < 9; i++) {
    if (*temp_value & (1 << i))
      parity++;
  }
  if (parity % 2)
    return 0;  // Parityfehler
  *temp_value >>= 1;               // Parity Bit löschen
  return 1;
}

uint8_t getTSicTemp(uint16_t *temp_value16) {
    uint16_t temp_value1 = 0;
    uint16_t temp_value2 = 0;

    TSIC_ON();
    _delay_us(500);     // wait for stabilization

    readSens(&temp_value1);      // 1. Byte einlesen
    readSens(&temp_value2);      // 2. Byte einlesen
    checkParity(&temp_value1);    // Parity vom 1. Byte prüfen
    checkParity(&temp_value2);    // Parity vom 2. Byte prüfen

    TSIC_OFF();    // Sensor ausschalten
    *temp_value16 = (temp_value1 << 8) + temp_value2;
    return 1;
}
