/*
****************************CREATED BY DO ANH TU - D11DTVT2 - EPU***************************************
-------------------NRF24L01+ 
-------------------
*/
#include "stm32f10x.h"
#include "NRF24L01.h"
#include "USART.h"
//*************Struct***********
extern GPIO_InitTypeDef  					GPIO_InitStructure;
extern EXTI_InitTypeDef 					EXTI_InitStructure; // EXTI struct
extern NVIC_InitTypeDef 					NVIC_InitStructure; // NVIC struct
extern SPI_InitTypeDef   				SPI_InitStructure;//SPU struct
//------------------------------

//*************Variable*********
uint8_t SPI_MASTER_Buffer_Tx[BufferSize];
uint8_t SPI_MASTER_Buffer_Rx[BufferSize];																					
uint8_t M_Rx = 0;
//------------------------------
//*************Function***********

//SPI1 Master function
void nRF24L01_configuration(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE);// cap xung cho GPIOA va SPI1

		GPIO_InitStructure.GPIO_Pin = nrf24l01_SCK| nrf24l01_MOSI;							//SCK: pin5 GPIOA, MOSI: pin7 GPIOA
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = nrf24l01_MISO;														//MISO: pin6 GPIOA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = nrf24l01_CSN| nrf24l01_CE;								//CS: pin4 GPIOA, CE pin1 GPIOA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;			//fullDuplex
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;														//SPI master mode
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;												//8bit data size
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;															
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;														//2bit on 1 egde
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;			//bo chia tan so (toc do < 10Mb/s)
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;											//truyen MSB
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI1, &SPI_InitStructure);		
		
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);												//interrupt on RX SPI (master receive)
		SPI_Cmd(SPI1, ENABLE);	
		
				/* Configure and enable SPI_MASTER interrupt -------------------------------*/
		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		nrf24l01_CE_LOW;
}

//-------------------------------------------------------------------------------------
//read register command: read a specifed register  from nrf24l01
	//param: reg: register address
void Nrf24l01_cmd_read(uint8_t reg)
{
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);	
	nrf24l01_CS_LOW;
	
	if(reg == nRF24_REG_RX_ADDR_P0 || reg == nRF24_REG_RX_ADDR_P1 || reg == nRF24_REG_TX_ADDR){
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
		SPI_I2S_SendData(nrf24l01_SPI,reg);
		rf24l01_delay_ms(T_delay);
		for(int i = 0;i<5;i++){
			SPI_I2S_SendData(nrf24l01_SPI,nRF24_CMD_NOP);
			rf24l01_delay_ms(T_delay);
		}//end for
	}//end if reg
	else{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
		SPI_I2S_SendData(nrf24l01_SPI,reg);
		rf24l01_delay_ms(T_delay);
		SPI_I2S_SendData(nrf24l01_SPI,nRF24_CMD_NOP);
		rf24l01_delay_ms(T_delay);
	}//end else
	nrf24l01_CS_HIGH;
	M_Rx = 0;
	//SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);	
}

//-------------------------------------------------------------------------------------
//Write register command: write data to specifed register in nrf24l01
void Nrf24l01_cmd_write(uint8_t reg, uint8_t data){
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);	
	reg = reg & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	nrf24l01_CS_LOW;
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
	SPI_I2S_SendData(nrf24l01_SPI,reg);
	rf24l01_delay_ms(T_delay);
	SPI_I2S_SendData(nrf24l01_SPI,data);
	rf24l01_delay_ms(T_delay);
		
	nrf24l01_CS_HIGH;
		
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);	
	M_Rx = 0;
}

//---------------------------------------------------------------------------------------
//NRF24 initialization: all register at reset value
void Nrf24l01_init(void)
{
	Nrf24l01_cmd_write(nRF24_REG_CONFIG,     0x08);
	Nrf24l01_cmd_write(nRF24_REG_EN_AA,      0x3F);
	Nrf24l01_cmd_write(nRF24_REG_EN_RXADDR,  0x03);
	Nrf24l01_cmd_write(nRF24_REG_SETUP_AW,   0x03);
	Nrf24l01_cmd_write(nRF24_REG_SETUP_RETR, 0x03);
	Nrf24l01_cmd_write(nRF24_REG_RF_CH,      0x02);
	Nrf24l01_cmd_write(nRF24_REG_RF_SETUP,   0x0E);
	Nrf24l01_cmd_write(nRF24_REG_STATUS,     0x00);
	Nrf24l01_cmd_write(nRF24_REG_RX_PW_P0,   0x00);
	Nrf24l01_cmd_write(nRF24_REG_RX_PW_P1,   0x00);
	Nrf24l01_cmd_write(nRF24_REG_RX_PW_P2,   0x00);
	Nrf24l01_cmd_write(nRF24_REG_RX_PW_P3,   0x00);
	Nrf24l01_cmd_write(nRF24_REG_RX_PW_P4,   0x00);
	Nrf24l01_cmd_write(nRF24_REG_RX_PW_P5,   0x00);
	Nrf24l01_cmd_write(nRF24_REG_DYNPD,      0x00);
	Nrf24l01_cmd_write(nRF24_REG_FEATURE,    0x00);
}
//---------------------------------------------------------------------------------------
//enable/disable enhanced shockburst for each pipe, enable auto ACK compatible with nRF24l01+
//param: pipe: bit[0:5] = pipe [0:5]
				 // 1 =ENABLE
				 // 0 =DISABLE
void Nrf24l01_EN_AA(uint8_t pipe)
{
	uint8_t reg = nRF24_REG_EN_AA & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	pipe = pipe & 0x3F;
	
	Nrf24l01_cmd_write(reg,pipe);
}
//---------------------------------------------------------------------------------------
//set RF channel: RF channel = 2400Mhz + (uint8_t channel)
//param: channel = bit[0:6] = [2400:2527]Mhz = [0-127]DEC
void Nrf24l01_SetRFchannel(uint8_t channel)
{
	uint8_t reg = nRF24_REG_RF_CH & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	channel= channel & 0x7F;
	Nrf24l01_cmd_write(reg,channel);
}

//---------------------------------------------------------------------------------------
//choose data rate for nrf24
/*param: dataRate: nRF24_DR_250kbps
									 nRF24_DR_1Mbps
									 nRF24_DR_2Mbps
*/
void Nrf24l01_SetDataRate(uint8_t dataRate)
{

	uint8_t reg = nRF24_REG_RF_SETUP & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_read(nRF24_REG_RF_SETUP);
	uint8_t dat = SPI_MASTER_Buffer_Rx[1] & 0xD7;
	dat = dat | dataRate;
	Nrf24l01_cmd_write(reg,dat);
}

//---------------------------------------------------------------------------------------
////Set RF output power in TX mode
/*param: power: nRF24_PWR_n18dBm = -18dBm
								nRF24_PWR_n12dBm = -12dBm
								nRF24_PWR_n6dBm  = -6dBm
								nRF24_PWR_0dBm   =  0dBm
*/
void Nrf24l01_SetTxPower(uint8_t power)
{

	uint8_t reg = nRF24_REG_RF_SETUP & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_read(nRF24_REG_RF_SETUP);
	uint8_t dat = SPI_MASTER_Buffer_Rx[1] & 0xF9;
	dat = dat | power;
	Nrf24l01_cmd_write(reg,dat);
}

//---------------------------------------------------------------------------------------
//Set Power mode
/*param: mode: nRF24_PWR_UP
							 nRF24_PWR_DOWN
*/
void Nrf24l01_PowerMode(uint8_t mode)
{

	uint8_t reg = nRF24_REG_CONFIG & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_read(nRF24_REG_CONFIG);
	uint8_t dat = SPI_MASTER_Buffer_Rx[1] & 0xFD;
	dat = dat | mode;
	Nrf24l01_cmd_write(reg,dat);
}

//---------------------------------------------------------------------------------------
//Set operation mode at PTX or PRX
/*param: mode: nRF24_MODE_TX
							 nRF24_MODE_RX
*/
void Nrf24l01_OperationMode(uint8_t mode)
{

	uint8_t reg = nRF24_REG_CONFIG & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_read(nRF24_REG_CONFIG);
	uint8_t dat = SPI_MASTER_Buffer_Rx[1] & 0xFE;
	dat = dat | mode;
	Nrf24l01_cmd_write(reg,dat);
}

//---------------------------------------------------------------------------------------
//Set CRC scheme 
/*param: nobyte: nRF24_CRC_1BYTE
							   nRF24_CRC_2BYTE
*/
void Nrf24l01_CRCscheme(uint8_t nobyte)
{

	uint8_t reg = nRF24_REG_CONFIG & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_read(nRF24_REG_CONFIG);
	uint8_t dat = SPI_MASTER_Buffer_Rx[1] & 0xFB;
	dat = dat | nobyte;
	Nrf24l01_cmd_write(reg,dat);
}

//---------------------------------------------------------------------------------------
//Set address widths (common for all pipe),TX/RX address field width
/*param: nobyte: nRF24_AW_3bytes
							   nRF24_AW_4bytes
								 nRF24_AW_5bytes
*/
void Nrf24l01_AddWidth(uint8_t nobyte)
{
	uint8_t reg = nRF24_REG_SETUP_AW & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_write(reg,nobyte);
}

//---------------------------------------------------------------------------------------
//Set address for specified pipe
/*param: pipe:  nRF24_REG_TX_ADDR (this is NOT a pipe)
								nRF24_REG_RX_ADDR_P0
							  nRF24_REG_RX_ADDR_P1
								nRF24_REG_RX_ADDR_P2
								nRF24_REG_RX_ADDR_P3
								nRF24_REG_RX_ADDR_P4
								nRF24_REG_RX_ADDR_P5
				addr:		user put address here as an array
NOTE: must set add width first before use this function,void Nrf24l01_AddWidth(uint8_t nobyte)
*/
void Nrf24l01_SetAddr(uint8_t pipe,uint8_t* addr)
{
		uint8_t addrWidth = Nrf24l01_GetAddWidth();
	
		uint8_t reg = pipe & nRF24_MASK_REG_MAP;
		reg = reg | nRF24_CMD_W_REGISTER;
	
		switch(pipe){
			case nRF24_REG_TX_ADDR: //(not a pipe)
			case nRF24_REG_RX_ADDR_P0:
			case nRF24_REG_RX_ADDR_P1:
				nrf24l01_CS_LOW;
	
				SPI_I2S_SendData(nrf24l01_SPI,reg);
				rf24l01_delay_ms(T_delay);
			
				while(addrWidth--){
					SPI_I2S_SendData(nrf24l01_SPI,*addr);
					rf24l01_delay_ms(T_delay);
					//printf("\nd: %x count:%d",*addr,addrWidth);
					addr++;
				}//end while
				M_Rx = 0;
				nrf24l01_CS_HIGH;
				break;
			case nRF24_REG_RX_ADDR_P2:
			case nRF24_REG_RX_ADDR_P3:
			case nRF24_REG_RX_ADDR_P4:
			case nRF24_REG_RX_ADDR_P5:
				Nrf24l01_cmd_write(reg,*addr);
				break;
		}
	
}

//---------------------------------------------------------------------------------------
//Set retransmition delay and retransmition No.count
/*param: ARD: nRF24_ARD_250us ==> nRF24_ARD_4000us 
				 ARC:	nRF24_ARC_DISABLE ==> nRF24_ARC_15TIME ==> if exceed ==> MAX_RT in STATUS reg will be set
*/
void Nrf24l01_AutoRETR(uint8_t ARD,uint8_t ARC)
{
	uint8_t reg = nRF24_REG_SETUP_RETR & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	uint8_t dat = ARD | ARC;
	Nrf24l01_cmd_write(reg,dat);
}

//---------------------------------------------------------------------------------------
//Enable auto ack for EN_AA register and RX pipe for ERX register
/*param: EEAA: nRF24_AA_ERX_PIPE0 ==> nRF24_AA_ERX_PIPE5
				 ERX:	 nRF24_AA_ERX_PIPE0 ==> nRF24_AA_ERX_PIPE5
				 EN:		ENAA_ENRX		: enable auto ack, enable specified pipe
								ENAA_DISRX	:	enable auto ack, disable specified pipe
								DISAA_ENRX	:	disable auto ack, enable specified pipe
								DISAA_DISRX :	disable auto ack, disable specified pipe
				nobyte: No.byte in RX payload on specified pipe depends on ERX pipe
*/
void Nrf24l01_SetRX(uint8_t EEAA,uint8_t ERX,uint8_t EN,uint8_t nobyte)
{

	
	uint8_t datAA;
	uint8_t datERX;
	//----------------------------------------------------------------------------
	//enable/disable auto ack on EN_AA register and RX pipe for ERX register
	if(EN==ENAA_ENRX){
		Nrf24l01_cmd_read(nRF24_REG_EN_AA);
		datAA = SPI_MASTER_Buffer_Rx[1] | EEAA;
		Nrf24l01_cmd_read(nRF24_REG_EN_RXADDR);
		datERX = SPI_MASTER_Buffer_Rx[1] | ERX;
	}
	else if(EN==ENAA_DISRX){
		Nrf24l01_cmd_read(nRF24_REG_EN_AA);
		datAA = SPI_MASTER_Buffer_Rx[1] | EEAA;
		Nrf24l01_cmd_read(nRF24_REG_EN_RXADDR);
		datERX = SPI_MASTER_Buffer_Rx[1] & (~ERX);
	}
	else if(EN==DISAA_ENRX){
		Nrf24l01_cmd_read(nRF24_REG_EN_AA);
		datAA = SPI_MASTER_Buffer_Rx[1] & (~EEAA);
		Nrf24l01_cmd_read(nRF24_REG_EN_RXADDR);
		datERX = SPI_MASTER_Buffer_Rx[1] | ERX;
	}
	else{
		Nrf24l01_cmd_read(nRF24_REG_EN_AA);
		datAA = SPI_MASTER_Buffer_Rx[1] & (~EEAA);
		Nrf24l01_cmd_read(nRF24_REG_EN_RXADDR);
		datERX = SPI_MASTER_Buffer_Rx[1] & (~ERX);
	}
	//----------------------------------------------------------------------------
	//write data ack and pipe to nrf24
	uint8_t reg = nRF24_REG_EN_AA & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	Nrf24l01_cmd_write(reg,datAA);
	
	reg = nRF24_REG_EN_RXADDR & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;	
	Nrf24l01_cmd_write(reg,datERX);
	//----------------------------------------------------------------------------
	//write No.byte in RX payload on specified pipe depends on ERX pipe
	switch(ERX){
		case nRF24_AA_ERX_PIPE0:
			Nrf24l01_cmd_write(nRF24_REG_RX_PW_P0,nobyte);
			break;
		case nRF24_AA_ERX_PIPE1:
			Nrf24l01_cmd_write(nRF24_REG_RX_PW_P1,nobyte);
			break;
		case nRF24_AA_ERX_PIPE2:
			Nrf24l01_cmd_write(nRF24_REG_RX_PW_P2,nobyte);
			break;	
		case nRF24_AA_ERX_PIPE3:
			Nrf24l01_cmd_write(nRF24_REG_RX_PW_P3,nobyte);
			break;	
		case nRF24_AA_ERX_PIPE4:
			Nrf24l01_cmd_write(nRF24_REG_RX_PW_P4,nobyte);
			break;		
		case nRF24_AA_ERX_PIPE5:
			Nrf24l01_cmd_write(nRF24_REG_RX_PW_P5,nobyte);
			break;
	}
}

//---------------------------------------------------------------------------------------
//Enable/disable dynamic payload length on specifed pipe 
/*param: pipe: nRF24_DPL_PIPE0 ==> nRF24_DPL_PIPE5
				 EN:	 EN:enable/DIS:disable
*NOTE: IF ENABLE DLP, IT MUST REQUIRE EE_AA ENABLE ON SPECIFIED PIPE
			 Bit EN_DPL in FEATURE REGISTER will be set in this function to ensure function working because of it's requirement
*/
void Nrf24l01_EN_DPL(uint8_t pipe,uint8_t EN)
{
	uint8_t dat;
	//-------------------------------------------------------------------------------------
	//Enable bit EN_DPL in FEATURE REGISTER
	uint8_t reg = nRF24_REG_FEATURE & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_read(nRF24_REG_FEATURE);
	dat = SPI_MASTER_Buffer_Rx[1] | 0x04;
	
	Nrf24l01_cmd_write(reg,dat);
	
	//-------------------------------------------------------------------------------------
	//Enable/Disable DPL on specified pipe
	Nrf24l01_cmd_read(nRF24_REG_DYNPD);
	if(EN==ENB){

		dat = SPI_MASTER_Buffer_Rx[1] | pipe;
	}
	else{
		dat = SPI_MASTER_Buffer_Rx[1] & (~pipe);
	}
	
	reg = nRF24_REG_DYNPD & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	Nrf24l01_cmd_write(reg,dat);
}

//---------------------------------------------------------------------------------------
//Flush TX FIFO, used in TX mode
void Nrf24l01_FlushTX(void)
{
	nrf24l01_CS_LOW;
	
	SPI_I2S_SendData(nrf24l01_SPI,nRF24_CMD_FLUSH_TX);
	rf24l01_delay_ms(T_delay);
		
	nrf24l01_CS_HIGH;
}

//---------------------------------------------------------------------------------------
//Flush RX FIFO, used in RX mode
//NOTE: should not use when transmit ACK
void Nrf24l01_FlushRX(void)
{
	nrf24l01_CS_LOW;
	
	SPI_I2S_SendData(nrf24l01_SPI,nRF24_CMD_FLUSH_RX);
	rf24l01_delay_ms(T_delay);
		
	nrf24l01_CS_HIGH;
}

//---------------------------------------------------------------------------------------
//Reset PLOS counter
//NOTE: reset PLOS counter by writting in RF_CH
void Nrf24l01_ResetPLOS(void)
{
	Nrf24l01_cmd_read(nRF24_REG_RF_CH);
	uint8_t dat = SPI_MASTER_Buffer_Rx[1];
	
	uint8_t reg = nRF24_REG_RF_CH & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_write(reg,dat);
}
//---------------------------------------------------------------------------------------
//clear pending bit IRQ
void Nrf24l01_ClearIRQflag(void)
{
	Nrf24l01_cmd_read(nRF24_REG_STATUS);
	uint8_t dat = SPI_MASTER_Buffer_Rx[1]|0x70;
	
	uint8_t reg = nRF24_REG_STATUS & nRF24_MASK_REG_MAP;
	reg = reg | nRF24_CMD_W_REGISTER;
	
	Nrf24l01_cmd_write(reg,dat);
	
}

//-------------------------------------------------------------------------------------
//Write 32bytes to payload: TX FIFO, always start at byte 0 in payload
//Maximum 3 payload in TX FIFO, use Nrf24l01_FlushTX(void) to clear all data in payload
/*param: *pbuf : 	 data to put in payload
				 length:	 length of data

*/
void Nrf24l01_cmd_write_PL(uint8_t *pbuf, uint8_t length){
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
	nrf24l01_CS_LOW;
	
	SPI_I2S_SendData(nrf24l01_SPI,nRF24_CMD_W_TX_PAYLOAD);
	rf24l01_delay_ms(T_delay);
	
	while(length--){
		SPI_I2S_SendData(nrf24l01_SPI,*pbuf);
		rf24l01_delay_ms(T_delay);
		printf("\nd: %c count:%d",*pbuf,length);
		pbuf++;
	}//end while
	
	nrf24l01_CS_HIGH;
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);	
}

//---------------------------------------------------------------------------------------
//Get address width

/*return: 			 nRF24_AW_3bytes = 3
							   nRF24_AW_4bytes = 4
								 nRF24_AW_5bytes = 5
*/
uint8_t Nrf24l01_GetAddWidth(void)
{
	Nrf24l01_cmd_read(nRF24_REG_SETUP_AW);
	switch(SPI_MASTER_Buffer_Rx[1]){
		case nRF24_AW_3bytes:
			return 3;
			//break;
		case nRF24_AW_4bytes:
			return 4;
			//break;
		case nRF24_AW_5bytes:
			return 5;
			//break;
	}//end switch
	return 0;
}

//---------------------------------------------------------------------------------------
//Get Status RX FIFO

/*return: 	nRF24_STATUS_RXFIFO_DATA  // The RX FIFO contains data and available locations
						nRF24_STATUS_RXFIFO_EMPTY // The RX FIFO is empty
						nRF24_STATUS_RXFIFO_FULL  // The RX FIFO is full
						nRF24_STATUS_RXFIFO_ERROR // Impossible state: RX FIFO cannot be empty and full at the same time
*/
uint8_t Nrf24l01_Get_Status_RXFIFO(void)
{
	Nrf24l01_cmd_read(nRF24_REG_FIFO_STATUS);
	uint8_t status = SPI_MASTER_Buffer_Rx[1]& 0x03;
	return status;
}
//---------------------------------------------------------------------------------------
//Get Status TX FIFO
/*return: 	nRF24_STATUS_TXFIFO_DATA  // The RX FIFO contains data and available locations
						nRF24_STATUS_TXFIFO_EMPTY // The RX FIFO is empty
						nRF24_STATUS_TXFIFO_FULL  // The RX FIFO is full
						nRF24_STATUS_TXFIFO_ERROR // Impossible state: RX FIFO cannot be empty and full at the same time
*/
uint8_t Nrf24l01_Get_Status_TXFIFO(void)
{
	Nrf24l01_cmd_read(nRF24_REG_FIFO_STATUS);
	uint8_t status = SPI_MASTER_Buffer_Rx[1]& 0x30;
	return status;
}
//---------------------------------------------------------------------------------------
//Get Status of STATUS register
/*return: 	value of STATUS 

*/
uint8_t Nrf24l01_Get_Status(void)
{
	Nrf24l01_cmd_read(nRF24_REG_STATUS);
	uint8_t status = SPI_MASTER_Buffer_Rx[1];
	return status;
}

//---------------------------------------------------------------------------------------
// Get pending IRQ flags
// return: current status of RX_DR, TX_DS and MAX_RT bits of the STATUS register
uint8_t nRF24_GetIRQFlags(void)
{
	Nrf24l01_cmd_read(nRF24_REG_STATUS);
	uint8_t status = SPI_MASTER_Buffer_Rx[1] & 0x70;
	return status;
}

//---------------------------------------------------------------------------------------
// Get pipe number for the payload available for reading from RX FIFO
// return: pipe number or 0x07 if the RX FIFO is empty
uint8_t nRF24_GetRXSource(void) 
{
	Nrf24l01_cmd_read(nRF24_REG_STATUS);
	uint8_t pipe = SPI_MASTER_Buffer_Rx[1] & 0x0E;
	return ((nRF24_RXResult)pipe);
}

//---------------------------------------------------------------------------------------
// Get information about counting lost packets and retransmited packets
// return: bits include information
uint8_t nRF24_GetReTInfo(void) 
{
	Nrf24l01_cmd_read(nRF24_REG_OBSERVE_TX);
	uint8_t info = SPI_MASTER_Buffer_Rx[1];
	return info;
}


//---------------------------------------------------------------------------------------
//Read data from payload
/*param: length: variable length of data in payload
	return: 			 nRF24_RX_PIPE0 
							   nRF24_RX_PIPE1 
								 nRF24_RX_PIPE2 
								 nRF24_RX_PIPE3
								 nRF24_RX_PIPE4
								 nRF24_RX_PIPE4
								 nRF24_RX_EMPTY
*/
nRF24_RXResult Nrf24l01_Read_PL(uint8_t *length){
	uint8_t pipe;
	//get pipe number from STATUS register
	Nrf24l01_cmd_read(nRF24_REG_STATUS);
	//printf("STATUS REG = %x",SPI_MASTER_Buffer_Rx[1]);
	pipe = ((SPI_MASTER_Buffer_Rx[1]&0x0E))>>1;
	//printf("pipe = %d",pipe);
	if(pipe<6){
		//get length of payload in specified pipe
		Nrf24l01_cmd_read(nRF24_REG_RX_PW_P0 + pipe);
		*length = SPI_MASTER_Buffer_Rx[1];
		//*length = nRF24_REG_RX_PW_P0 + pipe;
		//printf("\nlength real = %d",*length);
		
		if(*length){
			nrf24l01_CS_LOW;
			
			SPI_I2S_SendData(nrf24l01_SPI,nRF24_CMD_R_RX_PAYLOAD);
			rf24l01_delay_ms(T_delay);
			M_Rx=0;
			while(*length!=0){
				SPI_I2S_SendData(nrf24l01_SPI,nRF24_CMD_NOP);
				rf24l01_delay_ms(T_delay);
				*length = *length-1;
			
			}//end while
			nrf24l01_CS_HIGH;
			//M_Rx=0;
			return ((nRF24_RXResult)pipe);
		}//end if *length
		
	}//end if pipe
	
	M_Rx=0;
	// The RX FIFO is empty
	*length = 0;
	return nRF24_RX_EMPTY;
	
	
}
//---------------------------------------------------------------------------------------
//approximataion delay between each time send SPI
	//param: time: delay time to mutilply
void rf24l01_delay_ms(uint16_t time)
{
	//tang bien dem len 12000 lan
	uint32_t time_n = time*N_delay;
	// bang 0 thi thoat lap
	while(time_n!=0){time_n--;}
}

