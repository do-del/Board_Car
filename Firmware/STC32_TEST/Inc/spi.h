#ifndef SPI_H
#define SPI_H

#include "stc32g.h"
#include "intrins.h"

#define MISO P24
#define MOSI P23
#define SCK P25

unsigned char SPI_RW(unsigned char byte);

#endif
