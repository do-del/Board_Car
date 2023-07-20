#ifndef PWM_H
#define PWM_H

#include "stc32g.h"
#include "intrins.h"

#define PWM1P_PIN P10
#define PWM1N_PIN P11
#define PWM4P_PIN P16
#define PWM4N_PIN P17

void pwm1p_config(void);
void pwm1n_config(void);

void pwm_init(void);
void pwma_setcycle(unsigned int cycle);
void pwm1_setduty(unsigned int duty);
void pwm4_setduty(unsigned int duty);
void pwm1p_enable(void);
void pwm1n_enable(void);
void pwm4p_enable(void);
void pwm4n_enable(void);
void pwm1p_disable(void);
void pwm1n_disable(void);
void pwm4p_disable(void);
void pwm4n_disable(void);

#endif
