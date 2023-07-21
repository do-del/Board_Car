#ifndef MOTOR_H
#define MOTOR_H

#include "stc32g.h"
#include "intrins.h"
#include "sys.h"

#include "pwm.h"
#include "math.h"

void motor_init(void);
void motor_run(unsigned char vel);
void motor_turn(unsigned char vel);

#endif
