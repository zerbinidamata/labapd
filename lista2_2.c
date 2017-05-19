#include <msp430.h>

unsigned int cont;
unsigned int conta;

void pin_setup()
{
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    P5SEL |= BIT2 | BIT3 |BIT4 |BIT5;

    UCSCTL1 = DCORSEL_0;
    UCSCTL2 = FLLD_0 |194;
    UCSCTL3 = SELREF_0| FLLREFDIV_5;
    UCSCTL4 = SELA_1 | SELS_3 | SELM_5; //aclock, smclock, mclock
    UCSCTL5 = DIVS_2;
    UCSCTL6 = 0;
}

void set_timer()
{
    TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;
    TA0EX0 = TAIDEX_0;
    TA0CCR0 = 10485;
    TA0CCTL0 = CCIE;
    TA0CTL &= ~TACLR;
}
/*
 * main.c
 */

#pragma vector = 53;

__interrupt void isr_ccifg0()
{
    cont = 1;
    while(cont != 0)
    {

        if((TA0CTL & TAIFG ) != 0 )
        {
            cont --;
            TA0CTL &= ~TAIFG;
        }

    }
    P1OUT ^= BIT0;
    cont = 1;

    int conta = 3;
    while(conta != 0)
        {
            if((TA0CTL & TAIFG ) != 0 )
            {
                conta --;
                TA0CTL &= ~TAIFG;
            }
        }
        P1OUT ^= BIT0;
        cont = 3;

    int cont_2 = 12;
    while(cont_2 != 0)
        {
            if((TA0CTL & TAIFG ) != 0 )
            {
                cont_2 --;
                TA0CTL &= ~TAIFG;
            }
        }
                P4OUT ^= BIT7;
                cont_2 = 12;

    int conta_2 = 20;
    while(conta_2 != 0)
        {
            if((TA0CTL & TAIFG ) != 0 )
            {
                conta_2 --;
                TA0CTL &= ~TAIFG;
            }
        }
            P4OUT ^= BIT7;
            conta_2 = 20;

}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    pin_setup();
    set_timer();

    __enable_interrupt();
    while(1);
}
