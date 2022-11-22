

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

void main(){

	while(1);
}
