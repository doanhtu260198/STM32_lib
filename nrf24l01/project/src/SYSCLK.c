#include "stm32f10x.h"
#include "SYSCLK.h"
//*************Struct***********
ErrorStatus 							HSEStartUpStatus; // trang thai clock
//-----------------------------


//*************Function***********
//System clock source HSE,Extenal clock source
void Clock_HSE_Configuration(uint32_t clock_in){
	RCC_DeInit(); // reset lai gia tri clock ve default ban dau
	//Enable HSE( su dung thach anh ngoai 8Mhz de co duoc tan so MAX 72Mhz)
	RCC_HSEConfig(RCC_HSE_ON);
	//wait until HSE ready, doi den khi HSE khoi dong xong
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus==SUCCESS){
		//Enable Prefetch Buffer
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		// tao do tre 2 latency circle
		FLASH_SetLatency(FLASH_Latency_2);
		//HCLK=SYSCLK,o day AHB clock = SYSCLK/1/2/4/8... 
		RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHB clock = SYSCLK/1
		//APB2PeriphClock, PCLK2 = HCLK, bo chia tan cho cac pin GPIO ngoai vi, APB2 clock = HCLK/1/2/4/8...
		RCC_PCLK2Config(RCC_HCLK_Div1);
		//APB1PeriphClock, PCLK1 = HCLK/2, bo chia tan cho cac pin GPIO ngoai vi, APB1 clock = HCLK/1/2/4/8...
		RCC_PCLK1Config(RCC_HCLK_Div2);
		//PLLCLK = clock_in,HSE/1 x PLLmul
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,clock_in);
		//Enale PLL
		RCC_PLLCmd(ENABLE);
		//wait until PLL is ready
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
    /* Select PLL as system clock source */
		// co 3 tham so, su dung PLLCLK,HSI hoac HSE
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08){ }
	}//end if HSEStartUpStatus
	else{ while (1){}}
}
//--------------------------------
//System clock source HSI, intenal clock source
void Clock_HSI_Configuration(uint32_t clock_in){
	RCC_DeInit(); // reset lai gia tri clock ve default ban dau
	RCC_HSICmd(ENABLE);
	//HCLK=SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//APB2PeriphClock, PCLK2 = HCLK, bo chia tan cho cac pin GPIO ngoai vi, APB2 clock = HCLK/1/2/4/8...
	RCC_PCLK2Config(RCC_HCLK_Div2);
	//APB1PeriphClock, PCLK1 = HCLK/2, bo chia tan cho cac pin GPIO ngoai vi, APB1 clock = HCLK/1/2/4/8...
	RCC_PCLK1Config(RCC_HCLK_Div2);
	//PLLCLK = clock_in,HSE/1 x PLLmul
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,clock_in);
	//Enale PLL
	RCC_PLLCmd(ENABLE);
	//wait until PLL is ready
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
	/* Select PLL as system clock source */
	// co 3 tham so, su dung PLLCLK,HSI hoac HSE
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	/* Wait till PLL is used as system clock source */
	while(RCC_GetSYSCLKSource() != 0x08){ }
}
//--------------------------------

