#include "stm32f10x.h"
#include "USART.h"
//**************define**************
//define ben duoi dung de su dung ham prinf voi cac function co chuc nang hien thi du lieu len may tinh hay lcd...
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
  	/* e.g. write a character to the LCD */
	//lcd_Data_Write((u8)ch);
	USART_SendData(USART2,(u8)ch);
	
	/*Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
	{}

  	return ch;
}


//*************Struct***********
GPIO_InitTypeDef  					GPIO_InitStructure;		// GPIO struct, bo extern neu dung file rieng
NVIC_InitTypeDef 					NVIC_InitStructure; 	// NVIC struct, bo extern neu dung file rieng
USART_InitTypeDef			UART_InitStructure;								// USART struct
//-----------------------------


//*************Function***********
void UART_Configuration(void)
	{
		/*Cap clock cho USART và port su dung*/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);													//cap xung cho ngoai vi port A
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);												//cap xung cho USART2
		
			/* Cau Tx mode AF_PP, Rx mode FLOATING  */
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;																			//pin 2 GPIOA: TX
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																//alternate function
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;																			//pin3 GPIO3: RX
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;													//input tha noi
			GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		/*Cau hinh USART*/
			UART_InitStructure.USART_BaudRate = 115200;																		//baudrate
			UART_InitStructure.USART_WordLength = USART_WordLength_8b;	
			UART_InitStructure.USART_StopBits = USART_StopBits_1;
			UART_InitStructure.USART_Parity = USART_Parity_No;
			UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART2, &UART_InitStructure);
				
		/* Cau hinh vector ngat va muc uu tien */
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			/* xoa co ngat nhan cho lan dau su dung*/
			USART_ClearFlag(USART2, USART_IT_RXNE);																				//khong can thiet
			
			/* Cau hinh cho phep ngat nhan*/
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

			/* Cho phep UART hoat dong */
			USART_Cmd(USART2, ENABLE);
	
	}
//--------------------------------



