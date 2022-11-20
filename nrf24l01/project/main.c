#include "stm32f10x.h"
#include "SYSCLK.h"
#include "SYSTICK.h"
#include "USART.h"
#include "NRF24L01.h"
#include <string.h>
////*************define***********
// Define what part of demo will be compiled:
//   0 : disable
//   1 : enable
#define DEMO_RX_SINGLE      0 // Single address receiver (1 pipe)
#define DEMO_RX_MULTI       0 // Multiple address receiver (3 pipes)
#define DEMO_RX_SOLAR       0 // Solar temperature sensor receiver
#define DEMO_TX_SINGLE      0 // Single address transmitter (1 pipe)
#define DEMO_TX_MULTI       0 // Multiple address transmitter (3 pipes)
#define DEMO_RX_SINGLE_ESB  0 // Single address receiver with Enhanced ShockBurst (1 pipe)
#define DEMO_TX_SINGLE_ESB  1 // Single address transmitter with Enhanced ShockBurst (1 pipe)


//--------------------------------
//*************prototype**********
void Delay_ms(uint16_t time);			//ham delay tuong doi
void Delay_ms_sys(uint16_t time); //ham delay chuan dung systick

//--------------------------------

//*************variable***********
uint32_t clock_source=0;					//bien doc gia tri tan so clock

extern uint8_t SPI_MASTER_Buffer_Tx[BufferSize];
extern uint8_t SPI_MASTER_Buffer_Rx[BufferSize];																					
extern uint8_t M_Rx;
char *datpl = "hellyeah motherfucker hellfire m";
//uint8_t datpl2[]={0x01,0x05,0x03,0x2c,0x35};
uint8_t RXadd[5]={0x01,0x05,0x03,0x2c,0x35};



//--------------------------------

//*************Struct***********
RCC_ClocksTypeDef 				RCC_ClockFreq;		
//--------------------------------
//=================================================================================================================================
//=================================================================================================================================
//*******************************************************MAIN**********************************************************************
int main(void)
{
	Clock_HSE_Configuration(clock_72Mhz);	// goi chuong trinh cau hinh clock HSE
	UART_Configuration();									// cau hinh cho UART 2 tren pin 2 TX va pin 3 RX GPIOA
	nRF24L01_configuration();							// cau hinh nrf24l01 SPI 
	RCC_GetClocksFreq(&RCC_ClockFreq);		// goi ham get clock source de debug va quan sat
	clock_source = RCC_ClockFreq.SYSCLK_Frequency;
	SysTick_Config(SystemCoreClock/1000);
	Delay_ms_sys(100);
	printf("frequency: %d",clock_source);
	printf("hello\n");
	Delay_ms_sys(3);
	Nrf24l01_init();
	Delay_ms_sys(3);
	nrf24l01_CE_LOW;
	//----------------------------------------------------------------------------------------------
	//only one demo is choosen
	if((DEMO_RX_SINGLE + DEMO_RX_MULTI + DEMO_RX_SOLAR + DEMO_TX_SINGLE + DEMO_TX_MULTI + DEMO_RX_SINGLE_ESB + DEMO_TX_SINGLE_ESB) !=1){
		printf("\nERROR");
		while(1);
	}	
	
	//##############################################################################################
	//DEMO RX SINGLE PIPE
	#if ( DEMO_RX_SINGLE )
		Nrf24l01_SetRFchannel(115);														//channel 2.490MHz
		Nrf24l01_SetDataRate(nRF24_DR_250kbps);									//data rate 1Mps
		//Nrf24l01_SetTxPower(nRF24_PWR_0dBm);								//TX power 0 dBm
		Nrf24l01_CRCscheme(nRF24_CRC_2BYTE);									//CRC 2byte
		Nrf24l01_AddWidth(nRF24_AW_5bytes);										//address width 5 bytes
		Nrf24l01_SetAddr(nRF24_REG_RX_ADDR_P1,RXadd); 				//addresing pipe
		Nrf24l01_SetRX(nRF24_AA_ERX_PIPE1,nRF24_AA_ERX_PIPE1,DISAA_ENRX,5);
		Nrf24l01_OperationMode(nRF24_MODE_RX);
		//enter standby mode
		Nrf24l01_PowerMode(nRF24_PWR_UP);
		//enter RXmode
		nrf24l01_CE_HIGH;
	
		Nrf24l01_cmd_read(nRF24_REG_RX_ADDR_P1);
		uint8_t status = Nrf24l01_Get_Status_RXFIFO();
		printf("\n status = %d",status);
		uint8_t status2 = Nrf24l01_Get_Status_TXFIFO();
		printf("\n status2 = %d",status2);
		uint8_t statusreg= Nrf24l01_Get_Status();
		printf("\n statusreg = %x",statusreg);
		uint8_t statusIRQ= nRF24_GetIRQFlags();
		printf("\n statusIRQ = %x",statusIRQ);
		uint8_t statuspipe= nRF24_GetRXSource();
		printf("\n statuspipe = %x",statuspipe);
		uint8_t pipe =0;
		uint8_t payloadLength;
		uint8_t RX_data[BufferSize];
		uint8_t RPD;
		while(1){
				Nrf24l01_cmd_read(nRF24_REG_RPD);
				RPD = SPI_MASTER_Buffer_Rx[1];
				printf("RPD: %X",RPD);
				Delay_ms_sys(500);
				printf("\n--------------------");
				//Nrf24l01_cmd_read(nRF24_REG_FIFO_STATUS);
				if(Nrf24l01_Get_Status_RXFIFO()!=nRF24_STATUS_RXFIFO_EMPTY){
					
					pipe = Nrf24l01_Read_PL(&payloadLength);			
					printf("\ndata on pipe: %d",pipe);				
					printf("\npayload: %s ",(char*)SPI_MASTER_Buffer_Rx);
					for(int i = 0;i<5;i++){
						RX_data[i]=SPI_MASTER_Buffer_Rx[i];
					}
					printf("\npayload data: %s ",(char*)RX_data);
					Nrf24l01_ClearIRQflag();
					Delay_ms_sys(10);
				}//end if

		}//end while
	#endif
	//END DEMO RX SINGLE PIPE
	//##############################################################################################	

	//##############################################################################################
	//DEMO TX SINGLE PIPE
	#if ( DEMO_TX_SINGLE )
		Nrf24l01_EN_AA(0x00);																	//disable enhanced shockburst
		Nrf24l01_SetRFchannel(115);														//channel 2.490MHz
		Nrf24l01_SetDataRate(nRF24_DR_250kbps);									//data rate 1Mps
		Nrf24l01_SetTxPower(nRF24_PWR_0dBm);									//TX power 0 dBm
		Nrf24l01_CRCscheme(nRF24_CRC_2BYTE);									//CRC 2byte
		Nrf24l01_AddWidth(nRF24_AW_5bytes);										//address width 5 bytes
		Nrf24l01_SetAddr(nRF24_REG_TX_ADDR,RXadd); 						//addresing pipe
		//Nrf24l01_SetRX(nRF24_AA_ERX_PIPE1,nRF24_AA_ERX_PIPE1,DISAA_ENRX,5);
		Nrf24l01_OperationMode(nRF24_MODE_TX);
		//enter standby mode
		Nrf24l01_ClearIRQflag();	
		Nrf24l01_PowerMode(nRF24_PWR_UP);
		//enter RXmode
		nrf24l01_CE_LOW;
		uint8_t payload[5] = "hello";
		uint8_t payloadLength=5;
		uint8_t statusReg;
		int wait = 0x00FFFFFF;
		while(1){
			nrf24l01_CE_LOW;
			Nrf24l01_FlushTX();
			Nrf24l01_cmd_write_PL(payload,payloadLength);
			nrf24l01_CE_HIGH;
			Delay_ms(100);
			
			
			//-----------------------------------------------------------------------------------------
			//check TX_DS(means the packet has been transmitted) or MAX_RT (means the maximum number of TX retransmits happened) flag in Status Reg
			do{
				statusReg= Nrf24l01_Get_Status();
				if(statusReg&(nRF24_FLAG_TX_DS|nRF24_FLAG_MAX_RT)){
						break;
				}
				
			}while(wait--);
			nrf24l01_CE_LOW;
			//-----------------------------------------------------------------------------------------
			//check if time out
			printf("\n ==wait = %d",wait);
			if(wait<0){
				printf("\nTIME OUT!!!");
				wait = 0x00FFFFFF;
			}
			
			printf("\n STATUS REG = %X",statusReg);
			Nrf24l01_ClearIRQflag();
			if(statusReg & nRF24_FLAG_TX_DS){
				printf("\n ******transmition successed!!!******");
				
			}
			else if(statusReg & nRF24_FLAG_MAX_RT){
				printf("\n #####MAXIMUM TRANSMITION!!!#####");
			}
			else{
				printf("\n #####ERROR!!!#####");
				Nrf24l01_FlushTX();
			}
			
			
			Delay_ms(500);
		}//end while
	#endif
	//END DEMO TX SINGLE PIPE
	//##############################################################################################	
		
	//##############################################################################################
	//DEMO_RX_SINGLE_ESB
	#if ( DEMO_RX_SINGLE_ESB )
		Nrf24l01_SetRFchannel(115);														//channel 2.515MHz
		Nrf24l01_SetDataRate(nRF24_DR_2Mbps);									//data rate 2Mps
		Nrf24l01_SetTxPower(nRF24_PWR_0dBm);									//TX power 0 dBm for auto ack transmit
		Nrf24l01_CRCscheme(nRF24_CRC_2BYTE);									//CRC 2byte
		Nrf24l01_AddWidth(nRF24_AW_5bytes);										//address width 5 bytes
		Nrf24l01_SetAddr(nRF24_REG_RX_ADDR_P1,RXadd); 				//addresing pipe
		Nrf24l01_SetRX(nRF24_AA_ERX_PIPE1,nRF24_AA_ERX_PIPE1,ENAA_ENRX,10);
		Nrf24l01_OperationMode(nRF24_MODE_RX);
		//enter standby mode
		Nrf24l01_PowerMode(nRF24_PWR_UP);
		//enter RXmode
		nrf24l01_CE_HIGH;
		
		uint8_t pipe =0;
		uint8_t payloadLength;
		uint8_t RX_data[BufferSize];
		uint8_t RPD;
		printf("\nESB TESTING");
		while(1){
			Nrf24l01_cmd_read(nRF24_REG_RPD);
			RPD = SPI_MASTER_Buffer_Rx[1];
			printf("RPD: %X",RPD);
			Delay_ms(500);
			printf("\n--------------------");
			//Nrf24l01_cmd_read(nRF24_REG_FIFO_STATUS);
			if(Nrf24l01_Get_Status_RXFIFO()!=nRF24_STATUS_RXFIFO_EMPTY){
				
				pipe = Nrf24l01_Read_PL(&payloadLength);			
				printf("\ndata on pipe: %d",pipe);				
				printf("\npayload: %s ",(char*)SPI_MASTER_Buffer_Rx);
				for(int i = 0;i<payloadLength;i++){
					RX_data[i]=SPI_MASTER_Buffer_Rx[i];
				}
				printf("\npayload data: %s ",(char*)RX_data);
				Nrf24l01_ClearIRQflag();
				Delay_ms(10);
			}//end if
		}
	#endif
	//END DEMO_RX_SINGLE_ESB
	//##############################################################################################

	//##############################################################################################
	//DEMO_TX_SINGLE_ESB
	#if ( DEMO_TX_SINGLE_ESB )
		Nrf24l01_EN_AA(0x01);																	//enable enhanced shockburst for pipe 0 (for auto ack packet)
		Nrf24l01_SetRFchannel(115);														//channel 2.515MHz
		Nrf24l01_SetDataRate(nRF24_DR_2Mbps);									//data rate 2Mps
		Nrf24l01_SetTxPower(nRF24_PWR_0dBm);									//TX power 0 dBm
		Nrf24l01_CRCscheme(nRF24_CRC_2BYTE);									//CRC 2byte
		Nrf24l01_AddWidth(nRF24_AW_5bytes);										//address width 5 bytes
		Nrf24l01_SetAddr(nRF24_REG_TX_ADDR,RXadd); 						//addresing pipe
		Nrf24l01_SetAddr(nRF24_REG_RX_ADDR_P0,RXadd); 				//addresing pipe P0 as same as TXpipe for receiving auto-ACK
		//Nrf24l01_SetRX(nRF24_AA_ERX_PIPE1,nRF24_AA_ERX_PIPE1,DISAA_ENRX,5);
		Nrf24l01_OperationMode(nRF24_MODE_TX);								//Mode TX
		Nrf24l01_AutoRETR(nRF24_ARD_2500us,nRF24_ARC_15TIME);	//set time retransmit and counter
		//enter standby mode
		Nrf24l01_ClearIRQflag();	
		Nrf24l01_PowerMode(nRF24_PWR_UP);
		
		//TESTING get information PLOS and ARC
		printf("//-------TEST------");
		uint8_t info = nRF24_GetReTInfo(); 
		printf("\ninfor = %X",info);
		uint8_t PLOS = info >> 4;
		uint8_t ARC = info & 0x0F;
		printf("\nPLOS: counter lost packet = %d",PLOS);
		printf("\nARC: counter transmited packet = %d",ARC);
		
		nrf24l01_CE_LOW;
		uint8_t payload[10] = "ESBtest...";
		uint8_t payloadLength=10;
		
		uint8_t statusReg;
		while(1){
			//transmit packet
			//--------------------------------------------------
			nrf24l01_CE_LOW;
			Nrf24l01_FlushTX();
			Nrf24l01_cmd_write_PL(payload,payloadLength);
			nrf24l01_CE_HIGH;
			//Delay_ms(100);
			//--------------------------------------------------	
			
			statusReg= Nrf24l01_Get_Status();
			Delay_ms(10);
			uint8_t TX_DS = statusReg & 0x20;
			Delay_ms(10);
			uint8_t MAX_RT = statusReg & 0x08;
			Delay_ms(10);
			printf("\nTX_DS: %d",TX_DS);
			Delay_ms(10);
			printf("\nMAX_RT: %d",MAX_RT);
			
			info = nRF24_GetReTInfo(); 
			PLOS = info >> 4;
			ARC = info & 0x0F;
			printf("\nPLOS: counter lost packet = %d",PLOS);
			printf("\nARC: counter transmited packet = %d",ARC);
			Delay_ms(500);
			Nrf24l01_ClearIRQflag();
		}
	#endif
	//END DEMO_TX_SINGLE_ESB
	//##############################################################################################		
		
}//end MAIN
//=================================================================================================================================
//=================================================================================================================================

//--------------------------------
//*************Function***********
// delay
void Delay_ms(uint16_t time){
	//tang bien dem len 12000 lan
	uint32_t time_n = time*12000;
	// bang 0 thi thoat lap
	while(time_n!=0){time_n--;}
}

//ham xu ly ngat nhan SPI1
void SPI1_IRQHandler(void)
{
	
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
		{
				SPI_MASTER_Buffer_Rx[M_Rx]=SPI_I2S_ReceiveData(SPI1);
				printf("\ndata:%X,%d",SPI_MASTER_Buffer_Rx[M_Rx],M_Rx);
				M_Rx++;
		}
	SPI_I2S_ClearFlag(SPI1, SPI_I2S_IT_RXNE);
	SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);

}


