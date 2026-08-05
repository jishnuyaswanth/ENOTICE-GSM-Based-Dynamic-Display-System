
#define FOSC 12000000           //Oscillator Frequency- 12MHz

#define CCLK (5*FOSC)           //CPU Clock- 60MHz

#define PCLK (CCLK/4)           //Peripheral Clock - 15MHz

#define BAUD 9600                       //Baud Rate

#define DIVISOR (PCLK/(16*BAUD))           //

#define UART0_CHNO 6       //Interrupt Channel No

#define TXD0_EN (1<<0)

#define RXD0_EN (1<<2)

#define _8BIT 3

#define WORD_LEN_SEL _8BIT       //Data Length

#define DLAB_BIT 7                       //

#define DR_BIT 0

#define THRE_BIT 5

#define TEMT_BIT 6

#define RBR_INTERRUPT_EN (1<<0)           //Enable Receive Interrupt

#define THRE_INTERRUPT_EN (1<<1)           //Enable Transmit Interrupt
