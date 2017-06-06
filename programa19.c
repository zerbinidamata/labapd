#include <msp430.h>

int cont = 10;
/*
 * main.c
 */

// P3.6 --> RS
// P3.5 --> E
// P6.3 --> D7
// P6.2 --> D6
// P6.1 --> D5
// P6.0 --> D4

// Definições para acessar pinos usados para o LCD
#define RS_BIT          BIT6
#define RS              P3OUT
#define Enable_BIT      BIT5
#define Enable          P3OUT
#define LCD_NIB         0XF
#define LCD_NIB_OUT     P6OUT
#define PINO            BIT5                    //???
#define PINO_OUT        P2OUT

#define ATZ_100us   3                       //Medido 104 useg
#define ATZ_500us   (9*ATZ_100us)       //Medido 513 useg
#define ATZ_1ms     (2.1*ATZ_500us)     //Medido 1,08 mseg
#define ATZ_2ms     2*ATZ_1ms
#define ATZ_5ms     (5.2*ATZ_1ms)       //Medido 5,07 mseg
#define ATZ_15ms    (3.1*ATZ_5ms)       //Medido 15,6 mseg

int decimos = 0,horas= 0,minutos = 0,segundos = 0;
int estadoSw1 = 0;

void count(void);
void timer(void);
void change_line(int line);
void lcd_init(void);
void lcd_wr_byte(char byte);
void lcd_wr_nib(char nib);
void pin_init(void);
void atraso(unsigned long valor);
void lap_clear(void);
void start_stop();


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    pin_init();
    lcd_init();
    timer();
    __enable_interrupt();
    //__low_power_mode_0 ();//Entrar LPM0
    while(1);
    return 0;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void sec(void)
{
    if(cont-- == 0)
    {
        segundos++;
        if( (segundos & 0xf) == 0xA)     // segundos = x9
        {
            segundos &= ~(BIT0|BIT1|BIT2|BIT3);     // Limpa nibble lsb
            segundos  += 0x10;      // Soma 1 no nibble msb

            if( ((segundos>>4) & 0xf) == 0x6)         // Segundos chegou em 60
            {
                minutos++;
                segundos = 0;

                if( (minutos & 0xf) == 0xA)     // minutos = x9
                    {
                        minutos &= ~(BIT0|BIT1|BIT2|BIT3);     // Limpa nibble lsb
                        minutos  += 0x10;           // Soma 1 no nibble msb

                        if( ((minutos>>4) & 0xf) == 0x6)         // Minutos chegou em 60
                        {
                              horas++;
                              minutos = 0;

                              if( (horas & 0xf) == 0xA)     // horas = x9
                              {
                                      horas &= ~(BIT0|BIT1|BIT2|BIT3);     // Limpa nibble lsb
                                      horas  += 0x10;      // Soma 1 no nibble msb

                                      if( ((horas>>4) & 0xf) == 0x2 && (horas & 0xf) == 0x4)         // Horas chegou em 24
                                      {

                                          horas = 0;
                                      }
                              }
                        }
                    }
             }
        }
        cont = 10;
    }
    decimos++;
    if((decimos &  0xf) == 0xA)
        decimos = 0;
    count();

}

// INTERRUPÇÕES DAS CHAVES

#pragma vector = PORT1_VECTOR;
__interrupt void port1(void)
{
    __delay_cycles(60000);      // deboucing

    int n;
    n = __even_in_range(P1IV,0x10);
    switch(n)
    {
        case 0x4: lap_clear(); break;   // P1.1
        default:   break;
    }

}

#pragma vector = PORT2_VECTOR;
__interrupt void port2(void)
{
    __delay_cycles(60000);      // deboucing

    int n;
    n = __even_in_range(P2IV,0x10);
    switch(n)
    {
        case 0x4: start_stop(); break;       // P2.1
        default:   break;
    }


}

void start_stop()
{
    if(estadoSw1 == 0)         // Parar
        {
            TA0CCTL0 = 0;   //disabilita interrupção
            estadoSw1 = 1;
        }
    else if(estadoSw1 == 1)       // Continuar
            {
                TA0CCTL0 = CCIE;
                estadoSw1 = 0;
            }

}

void lap_clear(void)
{
    if(estadoSw1 == 0)  // cronometro rodando
            lap();
    else{
        segundos = 0;
        minutos = 0;
        horas = 0;
        decimos = 0;
        count();
    }


}

void lap()
{
    change_line(2);
    RS |= RS_BIT;            //RS = 1
    lcd_wr_byte(((horas>>4) & 0xf) + 48);          // Escreve nibble msb
    lcd_wr_byte((horas & 0xf) + 48);

    lcd_wr_byte(58); // char :

    lcd_wr_byte(((minutos>>4) & 0xf ) + 48);          // Escreve nibble msb
    lcd_wr_byte((minutos & 0xf) + 48);                 // Escreve nibble lsb

    lcd_wr_byte(58); // char :

    lcd_wr_byte(((segundos>>4) & 0xf ) + 48);          // Escreve nibble msb
    lcd_wr_byte((segundos & 0xf) + 48);                 // Escreve nibble lsb

    lcd_wr_byte(44); // char ,

    lcd_wr_byte(decimos + 48);

    segundos = 0;
    minutos = 0;
    horas = 0;
    decimos = 0;
}


// Gerar atraso
void atraso(unsigned long valor)
{
    volatile unsigned long cont=valor;
    while(cont != 0)    cont--;
}

void pin_init(void)
{
    // Inicializando pinos

    // Pinos chaves
    P2DIR &= ~BIT1;        //P2.1 = entrada
    P2REN |= BIT1;         //Habilita pullup
    P2OUT |= BIT1;         //Setar BIT1 ;S1 com pullup/dw
    P1DIR &= ~BIT1;        //P1.1 = entrada
    P1REN |= BIT1;         //Habilita pullup
    P1OUT |= BIT1;         //Setar BIT1 ;S2 com pullup/dw

    // Interrupção dos pinos
    P2IES |= BIT1;
    P2IE  |= BIT1;
    P1IES |= BIT1;
    P1IE  |= BIT1;

    // Pinos LCD
    P6DIR |= BIT0|BIT1|BIT2|BIT3;
    P3DIR |= BIT5|BIT6;
    P6OUT = 0;
    P3OUT = 0;
}

// Escreve nibble no bus e pulsa E
void lcd_wr_nib(char nib)
{
    LCD_NIB_OUT = LCD_NIB_OUT & (~LCD_NIB);          //Zerar os 4 nibbles
    LCD_NIB_OUT = LCD_NIB_OUT | (nib & 0xf);         //Escrever os 4 nibbles
    Enable = Enable |Enable_BIT;                    // E=1
    atraso(ATZ_2ms);
    Enable = Enable & (~Enable_BIT);                // E=0
}

// Escreve um byte no bus (não altera RS)
void lcd_wr_byte(char byte)
{
    lcd_wr_nib( (byte>>4) & 0xf);           // nibble msb
    lcd_wr_nib( byte & 0xf);                //nibble lsb
}

void lcd_init(void)
{
    atraso(ATZ_15ms);
    RS &= (~RS_BIT);         //RS = 0
    atraso(ATZ_2ms);

    lcd_wr_nib(3);                      //4bits only
    lcd_wr_byte(2);                    //cursor return home instruction
    lcd_wr_byte(0x28);                // 4 bits, 2 linhas, 5x8
    lcd_wr_byte(0x8);                // Display off
    lcd_wr_byte(0x1);               //Clear Display
    lcd_wr_byte(0x6);              //Incr AC a cada escrita
    lcd_wr_byte(0xf);             //Display on, cursor pisc
}

void change_line(int line)
{
    RS &= (~RS_BIT);         //RS = 0
    if(line == 1)
    {
        lcd_wr_nib(0);      // posição do cursor
        lcd_wr_nib(0x2);
    }
    if(line == 2)
    {
        lcd_wr_nib(12);
        lcd_wr_nib(0);
    }
}

void timer(void)
{
    TA0CCR0 = 32767/10;
    TA0EX0 = TAIDEX_3;                      // Divide por 4
    TA0CTL = TASSEL_2|ID_3|MC_1|TACLR;      // SMCLK | divide por 8 | Modo up | clear
    TA0CCTL0 = CCIE;                        // Habilita
    TA0CTL &= ~TACLR;
}

void count(void)        // Escreve no LCD
{
    change_line(1);
    RS |= RS_BIT;            //RS = 1

    lcd_wr_byte(((horas>>4) & 0xf) + 48);          // Escreve nibble msb
    lcd_wr_byte((horas & 0xf) + 48);

    lcd_wr_byte(58); // char :

    lcd_wr_byte(((minutos>>4) & 0xf ) + 48);          // Escreve nibble msb
    lcd_wr_byte((minutos & 0xf) + 48);                 // Escreve nibble lsb

    lcd_wr_byte(58); // char :

    lcd_wr_byte(((segundos>>4) & 0xf ) + 48);          // Escreve nibble msb
    lcd_wr_byte((segundos & 0xf) + 48);                 // Escreve nibble lsb

    lcd_wr_byte(44); // char ,

    lcd_wr_byte(decimos + 48);
}


