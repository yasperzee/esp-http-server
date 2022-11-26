#ifndef EEPROM_H
#define EEPROM_H

#include "setup.h"
#include <EEPROM.h>

void write_eeprom( int addr, int value);
int read_eeprom(int address);
void clear_eeprom();

#endif // EEPROM_H

