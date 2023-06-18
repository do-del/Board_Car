#ifndef EEPROM_H
#define EEPROM_H

#include "stc8h.h"
#include "intrins.h"
#include "sys.h"

#define IAP_TPS_VALUE 12 //FOSC / 1000000

void eeprom_enable(void);
void eeprom_disable(void);
uint8_t eeprom_read(uint8_t addr_h,uint8_t addr_l);
void eeprom_write(uint8_t addr_h,uint8_t addr_l,uint8_t dat);
void eeprom_clean(uint8_t addr_h);

//uint8_t eeprom_test(uint8_t addr_h);

#endif
