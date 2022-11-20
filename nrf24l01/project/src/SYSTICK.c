//*NOTE SysTick_Config(SystemCoreClock/1000); // goi ham nay trong ham main neu co dung ngat voi systick
/* ham nay o trong file stm32f10x_it.c, ngat se xay ra sau 1ms khi dung lenh SysTick_Config(SystemCoreClock/1000) tren va dung bien Timingdelay 
	 de thay doi thoi gian ma ta muon delay, bien Timing delay se bi giam di (Timingdelay--) sau 1ms o ham phuc vu ngat
// 2 ham tren se duoc dung khi khoi tao systick co dung ngat
void SysTick_Handler(void)
{
	Timingdelay--;
}
*/
#include "stm32f10x.h"
#include "SYSTICK.h"
//*************Struct***********
//------------------------------

//*************Variable*********
uint32_t Timingdelay;							//bien delay dung trong systick de tao delay chuan, bien nay dung trong ngat SysTick_Handler(void)

//------------------------------

//*************Function***********
//delay systick chuan dung voi ngat, phai khai bao SysTick_Config(SystemCoreClock/1000); va thiet lap SysTick_Handler(void) de dung ham nay
void Delay_ms_sys(uint16_t time)
	{
	Timingdelay = time;						// gan bien dem bang tham so truyen vao
	while(Timingdelay!=0);				// ham cho doi bien dem bang 0.
	
	}
//--------------------------------
// ham config systick khi khong dung ngat, ham nay khai bao trong main, neu dung ham nay thi ko dung SysTick_Config(SystemCoreClock/1000); nua
void SysTick_Configuration(void)
	{
		SysTick->CTRL = 0x00000005; //ENABLE = 1,TICKINT =0,CLKSOURCE=1
		SysTick->VAL =0;
		SysTick->LOAD = 72000-1;    //gia tri thuc cua bo dem=gia tri truyen vao +1

	}
//--------------------------------	
// ham delay khong dung ngat
void Dms_sys_noint(uint16_t time)
	{
		Timingdelay = time;
		while(Timingdelay!=0)
			{
				while( (SysTick->CTRL&0x10000)==0x10000) // kiem tra xem COUNT FLAG o thanh ghi SYSTICK CTRL co set 1 ko
					{
						Timingdelay--;
					}
			}
	}

