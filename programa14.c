#include <msp430.h>
#define INIT_DELAY 15000
#define EN_UP_DELAY 300
#define EN_DOWN_DELAY 100
#define DELAY 1500




/*
 * main.c
 */


void pin_setup(); //configuração dos pinos
void lcd_init(); //inicialização do lcd
void send_instructions(); // mandar as instruçoes pro lcd, tanto de inicialização como de escrita, alterar valor do rs
void send_char(const unsigned char); //escrever um caracter no lcd
void send_string(); //usar a send_char para escrever uma string
void line(); //mover a linha do lcd

int main(void)
{
    const char nome[] = {"RAFAEL"};
    const char nome2[] = {"MARIANA"};
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //inicializar ↓
    __delay_cycles(INIT_DELAY); // apos ligar espera 15ms
    pin_setup();
    lcd_init();

    //escrever ↓
    line(1);
    send_string(nome);
    line(2);
    send_string(nome2);

    while(1);



}

void line(int line)
{
    if (line == 1)
    {
       send_instructions(0,0);
       send_instructions(1,0);
       __delay_cycles(DELAY);
       return;
    }

    if(line == 2)
    {
        send_instructions(0xc,0);
        send_instructions(0x0,0);
        __delay_cycles(DELAY);
    }
    return;
}

void send_string(const char * str) //recebe array como entrada
{
    while(*str){
            send_char(*str++);
            __delay_cycles(DELAY);
        }
        return;
}


void send_char(const unsigned char c){
    unsigned char b = c >> 4;
    send_instructions((b&0xF),1);
    send_instructions((c&0xF),1);
    return;
}


void send_instructions(int instruction,int rs){
    P3OUT = 0;
    P6OUT = instruction; //dados
    if(rs){//diferenciar inicializaçao de escrita
        P3OUT |= BIT6;
    }
    __delay_cycles(EN_DOWN_DELAY);
    P3OUT |= BIT5; //mudar valor enable
    __delay_cycles(EN_UP_DELAY);
    P3OUT &= ~BIT5;
    __delay_cycles(EN_DOWN_DELAY);
    return;
}


void lcd_init()
{
        send_instructions(BIT1|BIT0,0);
        __delay_cycles(DELAY);
        send_instructions(BIT1|BIT0,0);
        __delay_cycles(DELAY);
        send_instructions(BIT1|BIT0,0);
        __delay_cycles(DELAY);
        send_instructions(BIT1,0);
        __delay_cycles(DELAY);
        send_instructions(BIT1,0);
        __delay_cycles(DELAY);
        send_instructions(BIT3,0);
        __delay_cycles(DELAY);
        send_instructions(0,0);
        send_instructions(BIT3,0);
        __delay_cycles(DELAY);
        send_instructions(0,0);
        send_instructions(BIT0,0);
        __delay_cycles(DELAY);
        send_instructions(0,0);
        send_instructions(BIT2|BIT1,0);
        __delay_cycles(DELAY);
        send_instructions(0,0);
        send_instructions(BIT3|BIT2|BIT1,0);
        __delay_cycles(DELAY);
        return;

}

void pin_setup(){
    P6DIR |= BIT0|BIT1|BIT2|BIT3;
    P3DIR |= BIT5|BIT6;
    P6OUT = 0;
    P3OUT = 0;
    return;
}

