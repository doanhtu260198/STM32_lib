#ifndef __SYSCLK_H__
#define __SYSCLK_H__

//**********include************
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_rcc.h"

//*************define**********
#define clock_16Mhz  0
#define clock_24Mhz  0x00040000
#define clock_32Mhz  0x00080000
#define clock_40Mhz  0x000C0000
#define clock_48Mhz  0x00100000
#define clock_56Mhz  0x00140000
#define clock_64Mhz  0x00180000
#define clock_72Mhz  0x001C0000
//-----------------------------

//**********prototype**********
void Clock_HSE_Configuration(uint32_t clock_in);		//config thach anh ngoai
void Clock_HSI_Configuration(uint32_t clock_in);		//config thach anh noi
//-----------------------------

#endif 


