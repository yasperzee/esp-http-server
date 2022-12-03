#ifndef EEPROM_H
#define EEPROM_H

/*************************** eeprom.h ************************************
  Description:  
*******************************************************************************/
/*------------------------------------------------------------------------------
  Version 0.1     Yasperzee   12'22     Cleaning and refactoring
 
------------------------------------------------------------------------------*/
#include "setup.h"
#include <EEPROM.h>

void write_eeprom( int addr, int value);
int read_eeprom(int address);
void clear_eeprom();

#endif // EEPROM_H

