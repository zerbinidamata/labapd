#include <msp430.h>

void pin_setup()
{
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    P5SEL |= BIT2 | BIT3 |BIT4 |BIT5;

    UCSCTL1 = DCORSEL_0;
    UCSCTL2 = FLLD_0 |194;
    UCSCTL3 = SELREF_0| FLLREFDIV_5;
    UCSCTL4 = SELA_1 | SELS_3 | SELM_5; //aclock, smclock, mclock
    UCSCTL5 = DIVM_2;
    UCSCTL6 = 0;
}

void set_timer()
{
    TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR;
    TA0EX0 = TAIDEX_0;
    TA0CTL &= ~TACLR;
    TA0CCR0 = 9999;
}

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    pin_setup();
    set_timer();

    int j;
    for(; ;)
    {
        for(j = 0; j < 20 ; )
        {
            if((TA0CTL & TAIFG ) != 0 )
            {
                j++;
                TA0CTL &= ~TAIFG;
            }

        }
        P4OUT ^= BIT7;
    }
	return 0;
}
