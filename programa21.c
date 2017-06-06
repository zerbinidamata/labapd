#include <msp430.h> 

//keys
#define SW1         BIT1
#define SW1_IN      P2IN
#define SW2         BIT1
#define SW2_IN      P1IN

//leds
#define LED1        BIT0
#define LED1_OUT    P1OUT
#define LED2        BIT7
#define LED2_OUT    P4OUT

//variables
    const int turn_red_on = 0x01;
    const int turn_red_off = 0x0A;
    const int turn_green_on = 0x10;
    const int turn_green_off = 0xA0;

    int state;

    int state_sw1 = 0;
    int state_sw2 = 0;

//functions
void pin_setup();

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    pin_setup();

            UCA0CTL1 = UCSSEL_2 |      //SMCLK
                  UCSWRST;        //UCSI em Reset
            UCA0CTL0 = UCSPB;           //2 Stops
            UCA0STAT = UCLISTEN;        //Loopback <== <==

        // Baudrate
            UCA0BRW = 6;                //Divisor
            UCA0MCTL = UCBRF_13 | UCBRS_0; //Moduladores

        // Configurar portas
            P3DIR &= ~BIT3;     //P3.3 saída
            P3REN |= BIT3;      //Hab resistor
            P3OUT |= BIT3;      //Pull up
            P3SEL |= BIT3;      //Rx serial

            P3DIR |= BIT4;      //P3.4 entrada
            P3SEL |= BIT4;      //Tx serial

            UCA0CTL1 &= ~UCSWRST;        //Sair do Reset

            while(1)
            {

                 while ((UCA0IFG & UCTXIFG) == 0);  //Esperar TX
                     UCA0IFG &= ~UCTXIFG;

                 while ((UCA0IFG & UCRXIFG) == 0);  //Esperar Rx
                     UCA0IFG &= ~UCRXIFG;

                state =  UCA0TXBUF;

                if(state == turn_red_on)
                    LED1_OUT |= LED1;
                if(state == turn_red_off)
                    LED1_OUT &= ~LED1;
                if(state == turn_green_on)
                    LED2_OUT |= LED2;
                if(state == turn_green_off)
                    LED2_OUT &= ~LED2;
            }
}
//interruptions
#pragma vector = PORT1_VECTOR;
__interrupt void port1(void)
{
    if(state_sw1 == 0)
    {
      UCA0TXBUF  = turn_red_on ;
      state_sw1 = 1;
    }
    if(state_sw1 == 1)
    {
      UCA0TXBUF  = turn_red_off ;
      state_sw1 = 0;
    }
}

#pragma vector = PORT2_VECTOR;
__interrupt void port2(void)
{
    if(state_sw2 == 0)
    {
        UCA0TXBUF  = turn_green_on ;
        state_sw2 = 1;

    }
    if(state_sw2 == 1)
    {

        UCA0TXBUF  = turn_green_off ;
        state_sw2 = 0;

    }
}

void pin_setup()
{
        P2DIR &= ~SW1;
        P2REN |= SW1;
        P2OUT |= SW1;

        P1DIR &= ~SW2;
        P1REN |= SW2;
        P1OUT |= SW2;

        P1DIR |= LED1;
        LED1_OUT &= ~LED1;
        P4DIR |= LED2;
        LED2_OUT &= ~LED2;
}
