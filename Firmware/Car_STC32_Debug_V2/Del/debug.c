#include "debug.h"
#include "stc.h"
#include "usb.h"

void Debug_Init(void)
{
	#if DEBUG_ENABLE
	//====== USB ≥ı ºªØ ======
	P3M0 &= ~0x03;
	P3M1 |= 0x03;
	
	IRC48MCR = 0x80;
	while (!(IRC48MCR & 0x01));
	
	USBCLK = 0x00;
	USBCON = 0x90;
	//========================
	#endif
}

