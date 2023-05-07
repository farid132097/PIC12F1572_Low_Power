/*
 * File:   main.c
 * Author: farid
 *
 * Created on May 8, 2023, 12:04 AM
 */

#define  _XTAL_FREQ 8000000UL
#include <xc.h>
#include "configbits.h"


volatile int state = 0;


void TIM2_Wait(void){
    T2CON=0x07;
    for(int i=0; i<40; i++){
        TMR2=0;
        while(TMR2<255);
    }
    T2CON=0x00;
}


void main(void) {
    
    
    TIM2_Wait();
    
    DACCON0 = 0;
    CM1CON0  = 0;
    ADCON0 = 0;
    FVRCON = 0;
    VREGCON |= (1<<1);
    OPTION_REG = 0x7f;
    
    
    TRISA |= (1<<0);
    ANSELA&=~(1<<0);
    WPUA  |= (1<<0);
    
    TRISA &=~ (1<<1)|(1<<2)|(1<<4)|(1<<5);
    ANSELA &=~ ((1<<1)|(1<<2)|(1<<4)|(1<<5));
    LATA |= (1<<1)|(1<<2)|(1<<4)|(1<<5);
    WPUA |= (1<<3);
    
    
    IOCAP  &=~(1<<0);
    IOCAN  |= (1<<0);
    INTCON |= (1<<3);   //IOCIE
    INTCON |= (1<<7);   //GIE
    IOCAF  &=~(1<<0);   //Individual Flag
    INTCON &=~(1<<0);   //IOCIF
    
    while(1){
        
        if(state == 1){
            //active
            TIM2_Wait();
            state = 0;
        }else{
            //sleep
            SLEEP();
        }
        
    }
    
    return;
}

void interrupt ISR(void){
    if( INTCON & (1<<0) ){
        if( IOCAF & (1<<0) ){
            state = 1;
            IOCAF  &=~(1<<0);
        }
        INTCON &=~(1<<0);
    }
}