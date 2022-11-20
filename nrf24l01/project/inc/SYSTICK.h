#ifndef __SYSTICK_H__
#define __SYSTICK_H__

//**********include************
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_it.h"

//*************define**********

//-----------------------------

//**********prototype**********
void Delay_ms_sys(uint16_t time);
void SysTick_Configuration(void);
void Dms_sys_noint(uint16_t time);
//-----------------------------

#endif 

