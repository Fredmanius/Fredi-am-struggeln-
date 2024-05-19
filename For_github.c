#include <Arduino.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "i2cmaster.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>


volatile int count_ms=0;
volatile int count_hs=0;


void delay_ms(unsigned int milliseconds);
void delay_hs(unsigned int hundred_milliseconds);

int main (void){


//initialize communication and board

uart_init();

i2c_init();


//Configuration of LEDS
 DDRD = 0xFF; 
 PORTD = 0x00;

//Set timer to be CTC (Clear timer on compare)
TCCR0A |=(1<<WGM01); 

//Set number you want to count to
OCR0A=249;

//16000/64presc=250khz -> 250khz*4ns=1ms
TCCR0B |= (1<<CS01) | (1<<CS00);

//Enable interrupts if timer has match
TIMSK0 |=(1<<OCIE0A);


//Enable all interrupts
sei();


while(1){
 

delay_ms(100);
PORTD=0b10000000;
delay_hs(5);
count_hs=0;
PORTD=PORTD<<1;
delay_hs(10);
count_hs=0;
PORTD=PORTD<<1;
delay_hs(20);
count_hs=0;
PORTD=PORTD<<1;
}

}




ISR(TIMER0_COMPA_vect){
count_ms++;

if(count_ms==100){
count_ms=0;
count_hs++;
}


}

//Function for delay_ms(100)
void delay_ms(unsigned int milliseconds){


while(count_ms<milliseconds);


}
void delay_hs(unsigned int hundred_milliseconds){

while(count_hs<hundred_milliseconds);

}