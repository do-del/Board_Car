#include "stc32g.h"
#include "intrins.h"

#define LED_PIN P53

void main(void)
{
	EAXFR = 1;
	CKCON = 0x00;
	WTST = 0x00;
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	
	P5M0 = 0x00;
	P5M1 = 0x00;
	
	LED_PIN = 0;
	
	while(1)
	{
	}
}