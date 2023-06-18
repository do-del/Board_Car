#include "eeprom.h"

void eeprom_enable(void)
{
	IAP_CONTR = 0x80;
	IAP_TPS = IAP_TPS_VALUE;
}

void eeprom_disable(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0;
}

uint8_t eeprom_read(uint8_t addr_h,uint8_t addr_l)
{
	char dat;
	IAP_CMD = 1;
	IAP_ADDRL = addr_l;
	IAP_ADDRH = addr_h;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();
	return IAP_DATA;
}

void eeprom_write(uint8_t addr_h,uint8_t addr_l,uint8_t dat)
{
	IAP_CMD = 2;
	IAP_ADDRL = addr_l;
	IAP_ADDRH = addr_h;
	IAP_DATA = dat;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();
}

void eeprom_clean(uint8_t addr_h)
{
	IAP_CMD = 3;
	IAP_ADDRL = 0;
	IAP_ADDRH = addr_h;
	IAP_TRIG = 0x5a;
	IAP_TRIG = 0xa5;
	_nop_();
}

/*
uint8_t eeprom_test(uint8_t addr_h)
{
	uint8_t ret = 0;
	eeprom_enable();
	eeprom_clean(addr_h);
	eeprom_write(addr_h,0,0x88);
	if(eeprom_read(addr_h,0) == 0x88)
	{
		eeprom_clean(addr_h);
		eeprom_write(addr_h,0,0x55);
		if(eeprom_read(addr_h,0) == 0x55)
		{
			ret = 1;
		}
	}
	eeprom_disable();
	return ret;
}
*/
