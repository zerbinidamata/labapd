#include <msp430.h>

int i;

int main(void) {
WDTCTL = WDTPW | WDTHOLD;// Stop watchdogtimer


   P5SEL |= BIT2 | BIT3 |BIT4 |BIT5;

   UCSCTL1 = DCORSEL_0;
   UCSCTL2 = FLLD_0 |194;
   UCSCTL3 = SELREF_0| FLLREFDIV_5;
   UCSCTL4 = SELA_1 | SELS_3 | SELM_5; //aclock, smclock, mclock
   UCSCTL5 = DIVM_2;
   UCSCTL6 = 0;

  P4DIR |= BIT7;
  P4SEL |= BIT7;
  TB0CCR0 = 40000;
  TB0CCTL1 = CLLD_1| OUTMOD_3;
  TB0CTL = TASSEL_2 | MC_1 | TACLR;

  int sentido = 0;

  PMAPKEYID = 0x02D52;
  P4MAP7 = PM_TB0CCR1A;

  int freq[] = {100, 94, 88, 83, 78, 73, 68, 63, 58, 54, 49, 45, 41, 37, 33, 29, 25, 22, 18, 15, 12, 9, 5, 2,0};


  while(1)
  {
      if(sentido == 0)
      {
          for( i = 24 ; i >= 0  ; i--)
          {
              __delay_cycles(5000);
              TB0CCR1  = 40000*freq[i]/100;
          }
          sentido = 1;
      }
      if(sentido ==1)
      {
          for(i = 0 ; i <= 24 ; i++)
          {
              __delay_cycles(5000);
              TB0CCR1  = 40000*freq[i]/100;
          }
          sentido = 0;
      }
  }

}


