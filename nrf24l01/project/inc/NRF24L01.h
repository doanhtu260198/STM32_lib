/*
****************************CREATED BY DO ANH TU - D11DTVT2 - EPU***************************************
-------------------NRF24L01+ 
-------------------
*/
#ifndef __NRF24L01_H__
#define __NRF24L01_H__

//**********include************
#include "stm32f10x.h"


//*************define**********
//ENABLE/DISABLE
#define ENB  1
#define DIS 0
//ENABLE/DISABLE EN_AA and EN_RX_ADD
#define ENAA_ENRX  	11
#define ENAA_DISRX 	10
#define DISAA_ENRX 	01
#define DISAA_DISRX 00
//define delay parameter
#define N_delay 10
#define T_delay 1
//buffer size
#define BufferSize 32
//define map pin
#define nrf24l01_PORT_MOSI GPIOA
#define nrf24l01_PORT_MISO GPIOA
#define nrf24l01_PORT_SCK GPIOA
#define nrf24l01_PORT_SCN GPIOA
#define nrf24l01_PORT_CE  GPIOA

#define nrf24l01_MOSI GPIO_Pin_7
#define nrf24l01_MISO GPIO_Pin_6
#define nrf24l01_SCK	 GPIO_Pin_5
#define nrf24l01_CSN  GPIO_Pin_4
#define nrf24l01_CE  GPIO_Pin_1

#define nrf24l01_SPI SPI1

//define pin ctrl
#define nrf24l01_CS_LOW nrf24l01_PORT_SCN->BRR |= nrf24l01_CSN 
#define nrf24l01_CS_HIGH nrf24l01_PORT_SCN->BSRR |= nrf24l01_CSN 
#define nrf24l01_CE_LOW nrf24l01_PORT_CE->BRR |= nrf24l01_CE 
#define nrf24l01_CE_HIGH nrf24l01_PORT_CE->BSRR |= nrf24l01_CE

// nRF24L0 instruction definitions
//#define nRF24_CMD_R_REGISTER       (uint8_t)0x00 // Register read
#define nRF24_CMD_W_REGISTER       (uint8_t)0x20 // Register write
#define nRF24_CMD_R_RX_PAYLOAD     (uint8_t)0x61 // Read RX payload
#define nRF24_CMD_W_TX_PAYLOAD     (uint8_t)0xA0 // Write TX payload
#define nRF24_CMD_FLUSH_TX         (uint8_t)0xE1 // Flush TX FIFO
#define nRF24_CMD_FLUSH_RX         (uint8_t)0xE2 // Flush RX FIFO
#define nRF24_CMD_REUSE_TX_PL      (uint8_t)0xE3 // Reuse TX payload
#define nRF24_CMD_R_RX_PL_WID      (uint8_t)0x60 // Read RX payload width in RF FIFO
#define nRF24_CMD_W_ACK_PAYLOAD    (uint8_t)0xA8 // Write payload to transmit with ACK (use in RX mode), 3 last bit need to be config !!!
#define nRF24_CMD_LOCK_UNLOCK      (uint8_t)0x50 // Lock/unlock exclusive features
#define nRF24_CMD_NOP              (uint8_t)0xFF // No operation (used for reading status register)

// nRF24L0 register definitions
#define nRF24_REG_CONFIG           (uint8_t)0x00 // Configuration register
#define nRF24_REG_EN_AA            (uint8_t)0x01 // Enable "Auto acknowledgment"
#define nRF24_REG_EN_RXADDR        (uint8_t)0x02 // Enable RX addresses
#define nRF24_REG_SETUP_AW         (uint8_t)0x03 // Setup of address widths
#define nRF24_REG_SETUP_RETR       (uint8_t)0x04 // Setup of automatic retransmit
#define nRF24_REG_RF_CH            (uint8_t)0x05 // RF channel
#define nRF24_REG_RF_SETUP         (uint8_t)0x06 // RF setup register
#define nRF24_REG_STATUS           (uint8_t)0x07 // Status register
#define nRF24_REG_OBSERVE_TX       (uint8_t)0x08 // Transmit observe register
#define nRF24_REG_RPD              (uint8_t)0x09 // Received power detector
#define nRF24_REG_RX_ADDR_P0       (uint8_t)0x0A // Receive address data pipe 0
#define nRF24_REG_RX_ADDR_P1       (uint8_t)0x0B // Receive address data pipe 1
#define nRF24_REG_RX_ADDR_P2       (uint8_t)0x0C // Receive address data pipe 2
#define nRF24_REG_RX_ADDR_P3       (uint8_t)0x0D // Receive address data pipe 3
#define nRF24_REG_RX_ADDR_P4       (uint8_t)0x0E // Receive address data pipe 4
#define nRF24_REG_RX_ADDR_P5       (uint8_t)0x0F // Receive address data pipe 5
#define nRF24_REG_TX_ADDR          (uint8_t)0x10 // Transmit address
#define nRF24_REG_RX_PW_P0         (uint8_t)0x11 // Number of bytes in RX payload in data pipe 0
#define nRF24_REG_RX_PW_P1         (uint8_t)0x12 // Number of bytes in RX payload in data pipe 1
#define nRF24_REG_RX_PW_P2         (uint8_t)0x13 // Number of bytes in RX payload in data pipe 2
#define nRF24_REG_RX_PW_P3         (uint8_t)0x14 // Number of bytes in RX payload in data pipe 3
#define nRF24_REG_RX_PW_P4         (uint8_t)0x15 // Number of bytes in RX payload in data pipe 4
#define nRF24_REG_RX_PW_P5         (uint8_t)0x16 // Number of bytes in RX payload in data pipe 5
#define nRF24_REG_FIFO_STATUS      (uint8_t)0x17 // FIFO status register
#define nRF24_REG_DYNPD            (uint8_t)0x1C // Enable dynamic payload length
#define nRF24_REG_FEATURE          (uint8_t)0x1D // Feature register

//define mask for special command
#define nRF24_MASK_REG_MAP         (uint8_t)0x1F // Mask bits[4:0] for CMD_RREG and CMD_WREG commands
#define nRF24_MASK_DATARATE        (uint8_t)0x28 // Mask RD_DR_[5,3] bits in RF_SETUP register
#define nRF24_FLAG_TX_DS           (uint8_t)0x20 // TX_DS bit (data sent TX FIFO interrupt)
#define nRF24_FLAG_MAX_RT          (uint8_t)0x10 // MAX_RT bit (maximum number of TX retransmits interrupt)
//-----------------------------
//*************Variable**********
//Data rate
enum {
	nRF24_DR_250kbps = (uint8_t)0x20, // 250kbps data rate
	nRF24_DR_1Mbps   = (uint8_t)0x00, // 1Mbps data rate
	nRF24_DR_2Mbps   = (uint8_t)0x08  // 2Mbps data rate
};
//TX power
enum {
	nRF24_PWR_n18dBm = (uint8_t)0x00, // 250kbps data rate
	nRF24_PWR_n12dBm = (uint8_t)0x02, // 1Mbps data rate
	nRF24_PWR_n6dBm  = (uint8_t)0x04,  // 2Mbps data rate
	nRF24_PWR_0dBm   = (uint8_t)0x06
};
//power mode
enum {
	nRF24_PWR_UP   = (uint8_t)0x02, // Power up
	nRF24_PWR_DOWN = (uint8_t)0x00  // Power down
};
//Operation Mode
enum {
	nRF24_MODE_TX   = (uint8_t)0x00, // PTX
	nRF24_MODE_RX 	= (uint8_t)0x01  // PRX
};
//CRC schmeme
enum {
	nRF24_CRC_1BYTE = (uint8_t)0x00, // 1byte CRC
	nRF24_CRC_2BYTE = (uint8_t)0x04  // 2byte CRC
};
//Address Widths 
enum {
	nRF24_AW_3bytes = (uint8_t)0x01, // 3byte add
	nRF24_AW_4bytes = (uint8_t)0x02, // 4byte add
	nRF24_AW_5bytes = (uint8_t)0x03  // 5byte add
};
//Retransmit ARD(delay) vs ARC(count) 
enum {
	nRF24_ARD_250us  = (uint8_t)0x00,
	nRF24_ARD_500us  = (uint8_t)0x10,
	nRF24_ARD_750us  = (uint8_t)0x20,
	nRF24_ARD_1000us = (uint8_t)0x30,
	nRF24_ARD_1250us = (uint8_t)0x40,
	nRF24_ARD_1500us = (uint8_t)0x50,
	nRF24_ARD_1750us = (uint8_t)0x60,
	nRF24_ARD_2000us = (uint8_t)0x70,
	nRF24_ARD_2250us = (uint8_t)0x80,
	nRF24_ARD_2500us = (uint8_t)0x90,
	nRF24_ARD_2750us = (uint8_t)0xA0,
	nRF24_ARD_3000us = (uint8_t)0xB0,
	nRF24_ARD_3250us = (uint8_t)0xC0,
	nRF24_ARD_3500us = (uint8_t)0xD0,
	nRF24_ARD_3750us = (uint8_t)0xE0,
	nRF24_ARD_4000us = (uint8_t)0xF0,
	
	nRF24_ARC_DISABLE  = (uint8_t)0x00,
	nRF24_ARC_1TIME  	 = (uint8_t)0x01,
	nRF24_ARC_2TIME  	 = (uint8_t)0x02,
	nRF24_ARC_3TIME 	 = (uint8_t)0x03,
	nRF24_ARC_4TIME 	 = (uint8_t)0x04,
	nRF24_ARC_5TIME 	 = (uint8_t)0x05,
	nRF24_ARC_6TIME 	 = (uint8_t)0x06,
	nRF24_ARC_7TIME 	 = (uint8_t)0x07,
	nRF24_ARC_8TIME 	 = (uint8_t)0x08,
	nRF24_ARC_9TIME 	 = (uint8_t)0x09,
	nRF24_ARC_10TIME 	 = (uint8_t)0x0A,
	nRF24_ARC_11TIME 	 = (uint8_t)0x0B,
	nRF24_ARC_12TIME 	 = (uint8_t)0x0C,
	nRF24_ARC_13TIME 	 = (uint8_t)0x0D,
	nRF24_ARC_14TIME 	 = (uint8_t)0x0E,
	nRF24_ARC_15TIME 	 = (uint8_t)0x0F
	
};
//Enable EE_AA auto ack and ERX pipe on specify pipe
enum {
	nRF24_AA_ERX_PIPE0 = (uint8_t)0x01,  // enable auto ack on pipe 0
	nRF24_AA_ERX_PIPE1 = (uint8_t)0x02,  // enable auto ack on pipe 1
	nRF24_AA_ERX_PIPE2 = (uint8_t)0x03,  // enable auto ack on pipe 2
	nRF24_AA_ERX_PIPE3 = (uint8_t)0x04,  // enable auto ack on pipe 3
	nRF24_AA_ERX_PIPE4 = (uint8_t)0x05,	 // enable auto ack on pipe 4
	nRF24_AA_ERX_PIPE5 = (uint8_t)0x06,  // enable auto ack on pipe 5
	
};
//No.byte in RX payload on specified pipe depends on ERX pipe
enum {
	nRF24_RX_PW_1BYTE  = (uint8_t)1,  /* 1 byte payload length*/ 	nRF24_RX_PW_17BYTE = (uint8_t)17,  /* 1 byte payload length*/ 
	nRF24_RX_PW_2BYTE  = (uint8_t)2,  /* 2 byte payload length*/ 	nRF24_RX_PW_18BYTE = (uint8_t)18,  /* 1 byte payload length*/ 
	nRF24_RX_PW_3BYTE  = (uint8_t)3,  /* 3 byte payload length*/ 	nRF24_RX_PW_19BYTE = (uint8_t)19,  /* 1 byte payload length*/ 
	nRF24_RX_PW_4BYTE  = (uint8_t)4,  /* 4 byte payload length*/ 	nRF24_RX_PW_20BYTE = (uint8_t)20,  /* 1 byte payload length*/ 
	nRF24_RX_PW_5BYTE  = (uint8_t)5,	/* 5 byte payload length*/ 	nRF24_RX_PW_21BYTE = (uint8_t)21,  /* 1 byte payload length*/ 
	nRF24_RX_PW_6BYTE  = (uint8_t)6,  /* 6 byte payload length*/ 	nRF24_RX_PW_22BYTE = (uint8_t)22,  /* 1 byte payload length*/ 
	nRF24_RX_PW_7BYTE  = (uint8_t)7,  /* 1 byte payload length*/ 	nRF24_RX_PW_23BYTE = (uint8_t)23,  /* 1 byte payload length*/ 
	nRF24_RX_PW_8BYTE  = (uint8_t)8,  /* 1 byte payload length*/ 	nRF24_RX_PW_24BYTE = (uint8_t)24,  /* 1 byte payload length*/ 
	nRF24_RX_PW_9BYTE  = (uint8_t)9,  /* 1 byte payload length*/ 	nRF24_RX_PW_25BYTE = (uint8_t)25,  /* 1 byte payload length*/ 
	nRF24_RX_PW_10BYTE = (uint8_t)10, /* 1 byte payload length*/	nRF24_RX_PW_26BYTE = (uint8_t)26,  /* 1 byte payload length*/  
	nRF24_RX_PW_11BYTE = (uint8_t)11, /* 1 byte payload length*/  nRF24_RX_PW_27BYTE = (uint8_t)27,  /* 1 byte payload length*/ 
	nRF24_RX_PW_12BYTE = (uint8_t)12, /* 1 byte payload length*/  nRF24_RX_PW_28BYTE = (uint8_t)28,  /* 1 byte payload length*/ 
	nRF24_RX_PW_13BYTE = (uint8_t)13, /* 1 byte payload length*/  nRF24_RX_PW_29BYTE = (uint8_t)29,  /* 1 byte payload length*/ 
	nRF24_RX_PW_14BYTE = (uint8_t)14, /* 1 byte payload length*/  nRF24_RX_PW_30BYTE = (uint8_t)30,  /* 1 byte payload length*/ 
	nRF24_RX_PW_15BYTE = (uint8_t)15, /* 1 byte payload length*/  nRF24_RX_PW_31BYTE = (uint8_t)31,  /* 1 byte payload length*/ 
	nRF24_RX_PW_16BYTE = (uint8_t)16, /* 1 byte payload length*/  nRF24_RX_PW_32BYTE = (uint8_t)32   /* 1 byte payload length*/ 
};
//Enable/disable pipe has dynamic payload length
enum {
	nRF24_DPL_PIPE0 = (uint8_t)0x01,  // enable DPL on pipe 0
	nRF24_DPL_PIPE1 = (uint8_t)0x02,  // enable DPL on pipe 1
	nRF24_DPL_PIPE2 = (uint8_t)0x03,  // enable DPL on pipe 2
	nRF24_DPL_PIPE3 = (uint8_t)0x04,  // enable DPL on pipe 3
	nRF24_DPL_PIPE4 = (uint8_t)0x05,  // enable DPL on pipe 4
	nRF24_DPL_PIPE5 = (uint8_t)0x06		// enable DPL on pipe 5

};
// Status of the RX FIFO
enum {
	nRF24_STATUS_RXFIFO_DATA  = (uint8_t)0x00, // The RX FIFO contains data and available locations
	nRF24_STATUS_RXFIFO_EMPTY = (uint8_t)0x01, // The RX FIFO is empty
	nRF24_STATUS_RXFIFO_FULL  = (uint8_t)0x02, // The RX FIFO is full
	nRF24_STATUS_RXFIFO_ERROR = (uint8_t)0x03  // Impossible state: RX FIFO cannot be empty and full at the same time
};
// Status of the TX FIFO
enum {
	nRF24_STATUS_TXFIFO_DATA  = (uint8_t)0x00, // The TX FIFO contains data and available locations
	nRF24_STATUS_TXFIFO_EMPTY = (uint8_t)0x10, // The TX FIFO is empty
	nRF24_STATUS_TXFIFO_FULL  = (uint8_t)0x20, // The TX FIFO is full
	nRF24_STATUS_TXFIFO_ERROR = (uint8_t)0x30  // Impossible state: TX FIFO cannot be empty and full at the same time
};
// Result of RX FIFO reading
typedef enum {
	nRF24_RX_PIPE0  = (uint8_t)0x00, // Packet received from the PIPE#0
	nRF24_RX_PIPE1  = (uint8_t)0x01, // Packet received from the PIPE#1
	nRF24_RX_PIPE2  = (uint8_t)0x02, // Packet received from the PIPE#2
	nRF24_RX_PIPE3  = (uint8_t)0x03, // Packet received from the PIPE#3
	nRF24_RX_PIPE4  = (uint8_t)0x04, // Packet received from the PIPE#4
	nRF24_RX_PIPE5  = (uint8_t)0x05, // Packet received from the PIPE#5
	nRF24_RX_EMPTY  = (uint8_t)0xff  // The RX FIFO is empty
} nRF24_RXResult;
//**********prototype**********
void nRF24L01_configuration(void);																			//configuration SPI pinout, IRQ RX SPI, baudrate...
void Nrf24l01_cmd_read(uint8_t reg);																		//read register command: read a specifed register  from nrf24l01
void Nrf24l01_cmd_write(uint8_t reg, uint8_t data);											//Write register command: write data to specifed register in nrf24l01
void Nrf24l01_init(void);																								//NRF24 initialization
void Nrf24l01_EN_AA(uint8_t pipe);																			//enable/disable enhanced shockburst for each pipe, enable auto ACK compatible with nRF24l01+
void Nrf24l01_SetRFchannel(uint8_t channel);														//set RF channel: RF channel = 2400Mhz + (uint8_t channel)
void Nrf24l01_SetDataRate(uint8_t dataRate);														//choose data rate for nrf24
void Nrf24l01_SetTxPower(uint8_t power);																//Set RF output power in TX mode
void Nrf24l01_PowerMode(uint8_t mode);																	//Set Power mode
void Nrf24l01_OperationMode(uint8_t mode);															//Set operation mode at PTX or PRX
void Nrf24l01_CRCscheme(uint8_t nobyte);																//CRC scheme: to send to PRX		
void Nrf24l01_AddWidth(uint8_t nobyte);        													//Set address widths (common for all pipe),TX/RX address field width
void Nrf24l01_SetAddr(uint8_t pipe,uint8_t* addr);											//Set address for specified pipe
void Nrf24l01_AutoRETR(uint8_t ARD,uint8_t ARC);												//Set retransmition delay and retransmition No.count
void Nrf24l01_SetRX(uint8_t EEAA,uint8_t ERX,uint8_t EN,uint8_t nobyte);//Enable auto ack for EN_AA register and RX pipe for ERX register
void Nrf24l01_EN_DPL(uint8_t pipe,uint8_t EN);													//Enable/disable dynamic payload length on specifed pipe 
void Nrf24l01_FlushTX(void);																						//Flush TX FIFO, used in TX mode
void Nrf24l01_FlushRX(void);																						//Flush RX FIFO, used in RX mode
void Nrf24l01_ResetPLOS(void);																					//Reset PLOS counter
void Nrf24l01_ClearIRQflag(void);																				//clear pending bit IRQ
void Nrf24l01_cmd_write_PL(uint8_t *pbuf, uint8_t length);							//Write 32bytes to payload: TX FIFO, always start at byte 0 in payload
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t Nrf24l01_GetAddWidth(void);																			//Get address width
uint8_t Nrf24l01_Get_Status_RXFIFO(void);																//Get Status RX FIFO
uint8_t Nrf24l01_Get_Status_TXFIFO(void);																//Get Status TX FIFO
uint8_t Nrf24l01_Get_Status(void);																			//Get Status of STATUS register
uint8_t nRF24_GetIRQFlags(void);																				//Get pending IRQ flags
uint8_t nRF24_GetRXSource(void) ;																				//Get pipe number for the payload available for reading from RX FIFO
uint8_t nRF24_GetReTInfo(void); 																				//Get information about counting lost packets and retransmited packets
nRF24_RXResult Nrf24l01_Read_PL(uint8_t *length);												//Read data from payload
void rf24l01_delay_ms(uint16_t time);																		//approximataion delay between each time send SPI

//-----------------------------

#endif 

