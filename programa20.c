#include <msp430.h> 

//chaves
#define SW1_PRESS   ((P2IN & BIT1) != BIT1);
#define SW2_PRESS   ((P1IN & BIT1) != BIT1);

//leds
#define LED1_ON     P1OUT |= BIT0;
#define LED1_OFF    P1OUT &= ~BIT0;
#define LED2_ON     P4OUT |= BIT7;
#define LED2_OFF    P4OUT &= ~BIT7;

//receiver
#define RECEIVER        BIT5
#define RECEIVER_OUT    P2OUT

//sender
#define SENDER        BIT4
#define SENDER_OUT    P2OUT

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer



}





void set_timerA(void)
{

}
