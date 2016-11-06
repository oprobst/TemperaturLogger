# TemperaturLogger

Einfacher Temperatur Logger über ATTiny 841 und Knopfzelle.
Gemessen wird über einen TSic 506, wofür die Spannung mittels DCDC Boost auf 5V gesetzt wird.

![schaltplan](https://github.com/oprobst/TemperaturLogger/blob/master/Schaltplan.png)

TODO:
Sobald der Logger an USB angeschlossen wird, kann er per Bootloader neu programmiert werden oder aber die Daten ausgelesen werden.

### Bootloader TinySafeBoot
http://jtxp.org/tech/tinysafeboot.htm
gute Anleitung unter https://ladvien.github.io/robots/tsb/

./tsb tn841 b2a7

tsb COM5:4800 fw TemperaturLogger.hex