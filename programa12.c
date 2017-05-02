#include <msp430.h> 

/*
 * main.c
 */

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}


int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR = 0x01; //bit 1 como output

    P1OUT = 0x00; //

    for(;;)
    {
    P1OUT ^= 0x01;
    waitFor(0.5);

    }

    return 0;
}
