
#include <msp430.h> 

void pin_setup();
void send(char);

unsigned volatile char rcv_char;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    pin_setup();
   __enable_interrupt();
    char flag = 3;
    for(;;){
        if(!(P2IN&BIT1) && (flag&BIT0)) {
          // __delay_cycles(1000);
            send(0x1);
            flag &= ~BIT0;
            while(!(P2IN&BIT1));
          //  __delay_cycles(1000);
        }
       if(!(P2IN&BIT1) && !(flag&BIT0)){
       //__delay_cycles(1000);
            send(0xa);
            flag |= BIT0;
            while(!(P2IN&BIT1));
          //  __delay_cycles(1000);
        }
        if(!(P1IN&BIT1) && (flag&BIT1)){
            __delay_cycles(1000);
            send(0x10);
            flag &= ~BIT1;
            while(!(P1IN&BIT1));
            __delay_cycles(1000);
        }
        if(!(P1IN&BIT1) && !(flag&BIT1)){
            __delay_cycles(1000);
            send(0xa0);
            flag |= BIT1;
            while(!(P1IN&BIT1));
            __delay_cycles(1000);
        }
        switch(rcv_char){
        case 0x1:P1OUT |= BIT0;
        break;
        case 0xa:P1OUT &= ~BIT0;
        break;
        case 0x10:P4OUT |= BIT7;
        break;
        case 0xa0:P4OUT &= ~BIT7;
        break;
        default:break;
        }
    }
}


void pin_setup(){
    P2DIR = BIT4;
    P2REN = BIT1 + BIT5;
    P2OUT = BIT1 + BIT4 + BIT5;
    P2IES = BIT5;
    P2IE = BIT5;
    P2IFG = 0;

    P1DIR = BIT0;
    P1REN = BIT1;
    P1OUT = BIT1;

    P4DIR = BIT7;

    TBCTL = TBSSEL__SMCLK + ID_0 + MC__STOP + TBCLR;
}

void send(char val){
  // __disable_interrupt();
    unsigned int i = 0;
    TBCCR0 = 109;
    P2OUT &= ~ BIT4;
    TBCTL |= MC_1 + TBCLR;
    for(i = 0;i < 8;i++){
        while(!(TBCCTL0&BIT0));
        TBCCTL0 &= ~BIT0;
        if(val&BIT0) P2OUT |= BIT4;
        else P2OUT &= ~BIT4;
        val = val >> 1;
    }
    while(!(TBCCTL0&BIT0));
    TBCCTL0 &= ~BIT0;
    TBCTL &= ~MC_3;
    P2OUT |= BIT4;
 //   __enable_interrupt();
}


#pragma vector = PORT2_VECTOR
__interrupt void recv(void){
    unsigned int i = 0;
    unsigned char retval = 0;
    switch(P2IV){
    case P2IV_P2IFG5:
        TBCCR0 = 55;
        TBCTL |= MC_1 + TBCLR;
        while(!(TBCCTL0&BIT0));
        TBCCTL0 &= ~BIT0;
        TBCCR0 = 109;
        for(i = 0;i < 8;i++){
            while(!(TBCCTL0&BIT0));
            TBCCTL0 &= ~BIT0;
            retval = retval >> 1;
            if(P2IN&BIT5) retval += 0x80;
        }
        TBCTL &= ~MC_3;
        if(retval != 0xff)rcv_char = retval;
    default:
        return;
    }
}
