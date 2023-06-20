#ifndef LED_H
#define LED_H

#include "stc32g.h"
#include "intrins.h"

#define LED_PIN P53

#define Led_On() do{P53=0;}while(0)
#define Led_Off() do{P53=1;}while(0)

#endif