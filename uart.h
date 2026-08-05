#include"types.h"
void UART0_Init(void);

void UART0_Tx(u8);

void UART0_Str(u8 *);
//u8 UART0_Rx(void);
char UART0_Rx(void);

extern u8 rx_buf[400];