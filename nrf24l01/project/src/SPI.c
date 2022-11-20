#include "stm32f10x.h"
#include "SPI.h"
//*************Struct***********
extern GPIO_InitTypeDef  					GPIO_InitStructure;
extern EXTI_InitTypeDef 					EXTI_InitStructure; // EXTI struct
extern NVIC_InitTypeDef 					NVIC_InitStructure; // NVIC struct
SPI_InitTypeDef   				SPI_InitStructure;//SPU struct
//-----------------------------


//*************Function***********
//SPI1 Master function
void SPI1_Master_Configuration(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE);// cap xung cho GPIOA va SPI1

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;										//SCK: pin5 GPIOA, MOSI: pin7 GPIOA
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																//MISO: pin6 GPIOA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;																//CS: pin4 GPIOA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			//fullDuplex
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;														//SPI master mode
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;												//8bit data size
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;															
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;														//2bit on 1 egde
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;			//bo chia tan so (toc do 9Mb/s)
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;											//truyen LSB
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI1, &SPI_InitStructure);		
		
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);												//interrupt on RX SPI (master receive)
		SPI_Cmd(SPI1, ENABLE);	
		
				/* Configure and enable SPI_MASTER interrupt -------------------------------*/
		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	
//SPI2 Slave function	
void SPI2_Slave_Configuration(void)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);									// cap xung cho SPI2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);									// cap xung cho GPIOB

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;														//MISO: pin14 GPIOB
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;								//SCK: pin13 GPIOB MOSI:pin15 GPIOB
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;														//CS: pin12 GPIOB
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //fullDuplex
		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;												 //SPI Slave mode
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										 //8bit data size
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;												 //2bit on 1 egde			
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	 //bo chia tan so (toc do 9Mb/s)
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;									 //truyen LSB
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI2, &SPI_InitStructure);				
		
		SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);										 //interrupt on RX SPI (slave receive)
		SPI_Cmd(SPI2, ENABLE);	
		
		/* Configure and enable SPI_SLAVE interrupt --------------------------------*/
		NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

