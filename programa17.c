#include <msp430.h>

unsigned int estado,PWM;
unsigned int contador;

void pins(void)
{
    // Inicializando pinos
    P4DIR |= BIT7;      //P4.7 SW2
    P4OUT &= ~(BIT7) ;     //P4.7 começa em 0

    P2DIR &= ~BIT1;     //P2.1 = entrada
    P2REN |= BIT1;   //Habilita pullup
    P2OUT |= BIT1;   //Setar BIT1 ;S1 com pullup/dw
    P1DIR &= ~BIT1;     //P2.1 = entrada
    P1REN |= BIT1;   //Habilita pullup
    P1OUT |= BIT1;   //Setar BIT1 ;S1 com pullup/dw

    P5SEL |= BIT2 | BIT3 |BIT4 |BIT5;

    UCSCTL1 = DCORSEL_0;
       UCSCTL2 = FLLD_0 |194;
       UCSCTL3 = SELREF_0| FLLREFDIV_5;
       UCSCTL4 = SELA_1 | SELS_3 | SELM_5; //aclock, smclock, mclock
       UCSCTL5 = DIVM_2;
       UCSCTL6 = 0;
}

void timer(void)
{
    TB0CCR0 = 800;                 // TB0 DEVE TER PERÍODO DE 20 ms
    TB0EX0 = TAIDEX_0;
    TB0CTL = TASSEL_2|ID_0|MC_1|TACLR;
    TB0CCTL0 = CCIE;                    //Habilita
    TB0CTL &= ~TBCLR;
}


int main(void) {
WDTCTL = WDTPW | WDTHOLD;// Stop watchdogtimer

PWM = 0;   // modulação
contador = 1;
estado = 0;

    pins();
    timer();
__enable_interrupt();

while(1)
{

    if ((P2IN & BIT1) != BIT1 && PWM < 100)  // Apertou S1
    {
              __delay_cycles(30000);
              while((P2IN & BIT1) != BIT1);    // espera soltar
              PWM += 5;         //Aumenta a largura do pulso em 5%
    }

    if ((P1IN & BIT1) != BIT1 && PWM > 0)  // Apertou S2
    {
            __delay_cycles(30000);
           while((P1IN & BIT1) != BIT1);    // espera soltar
           PWM -= 5;    //Diminui a largura do pulso em 5%
    }

}

    return 0;
}

#pragma vector = 59     // TIMER0_B0_VECTOR
__interrupt void pwm(void){
if(--contador == 0)
{
    if(estado == 0)     // Primeira parte da modulação
    {
        contador = (101 - PWM) ;
        estado = 1;
        P4OUT &= ~(BIT7);       //Desliga o led
    }
    else                // Segunda parte da modulação
    {
        contador = (PWM + 1);
        estado = 0;
        P4OUT |= BIT7;       //Liga o led

    }
}

}
