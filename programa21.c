#include <msp430.h>

#define SW1     (P2IN&BIT1)
#define SW2     (P1IN&BIT1)


void pin_setup();
void send(char);

unsigned volatile char rcv_char;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  pin_setup();
  __enable_interrupt();

  char flag = 3; //state
      while(1){
          if(!SW1 && (flag&BIT0)) {
              __delay_cycles(1000);
              send(0x1);
              flag &= ~BIT0;
              while(!SW1);
              __delay_cycles(1000);

          }
          if(!SW1 && !(flag&BIT0)){
              __delay_cycles(1000);
              send(0xa);
              flag |= BIT0;
              while(!SW1);
              __delay_cycles(1000);


          }
          if(!SW2 && (flag&BIT1)){
              __delay_cycles(1000);
              send(0x10);
              flag &= ~BIT1;
              while(!SW2);
              __delay_cycles(1000);
          }
          if(!(SW2) && !(flag&BIT1)){
              __delay_cycles(1000);
              send(0xa0);
              flag |= BIT1;
              while(!SW2);
              __delay_cycles(1000);
          }
          switch(rcv_char){ //leds
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
    P2DIR = 0;
    P2REN = BIT1;
    P2OUT = BIT1;

    P1DIR = BIT0;
    P1REN = BIT1;
    P1OUT = BIT1;

    P4DIR = BIT7;

    P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 6;                              // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
                                              // over sampling
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void send(char val){
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = val;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) //interrupt USCI
{
  switch(__even_in_range(UCA0IV,4)) //The __even_in_range intrinsic provides a hint to the compiler when generating switch statements for interrupt vector routines.
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    rcv_char = UCA0RXBUF;                   // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
