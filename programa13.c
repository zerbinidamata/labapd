	#include <msp430.h> 

/*
 * main.c
 */
void waitFor (unsigned int secs) {

    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

void pinos()
{
        P1DIR |= BIT0;//P1.0 SAIDA
        P1DIR &= ~BIT1;//P1.1 ENTRADA
        P2DIR &= ~BIT1;//P2.1 ENTRADA
        P4DIR |= BIT7;//P4.7 SAIDA

        P1REN |= BIT1;//P1.1 ENABLE RESISTOR
        P2REN |= BIT1;//P2.1 ENABLE RESISTOR

        P2OUT |= BIT1;//P2.1 PULL-UP RESISTOR
        P1OUT |= BIT1;//P1.1 PULL-UP RESISTOR

        return;
}

void pisca()
{
    char p1 = P1OUT; // guardar estado de inicio do p1out
    char p4 = P4OUT; //guardar estado de inicio do p4out


    P1OUT |= BIT0; //
    P4OUT &= ~BIT7; //

    while(!(P2IN&BIT1)&&(!(P1IN&BIT1))){ //enquanto os dois nao estiver pressionados
        waitFor(0.5);
        P1OUT ^= BIT0;
        P4OUT ^= BIT7;
    }

    P1OUT = p1;
    P4OUT = p4; //usar os estados anteriores para fazer ele piscar
}

int debounce(char *aponta,char  bit)
{
    if(!(*(aponta)&bit)){ //se ele nao aponta para o bit, cria se um delay para ver se a mudança no estado
        waitFor(1);

        if(!(*(aponta)&bit)) ; //se ele nao muda de estado, retorna 1 e termina a funçao
            return 1;
    }
    return 0;
}




int main ()
{
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    pinos();

    while(1)
    {
     if(debounce(&P1IN,BIT1)) // ve se ta pressionado
     {
         P4OUT ^= BIT7 ; //inverte estado
         while(!(P1IN&BIT1)){ //enquanto nao soltar
             if(debounce(&P2IN,BIT1)) // ve se a outra chave tambem está pressionada
                 pisca();
         }
      }

     if(debounce(&P2IN,BIT1))
     {

         P1OUT ^= BIT0 ;
             while(!(P2IN&BIT1)){
                      if(debounce(&P1IN,BIT1))
                          pisca();
                  }

     }
    }

}
