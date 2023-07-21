#include "spi.h"

unsigned char SPI_RW(unsigned char byte)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		if(byte & 0x80)
			MOSI = 1;
		else
			MOSI = 0;
		byte <<= 1;
		SCK = 1;
		byte |= MISO;
		SCK = 0;
	}
	return byte;
}