/*	Author: mwu057
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
#endif

char Rows[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // LED Matrix
char Cols[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; // LED Matrix

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00; // LED Matrix 
    DDRC = 0xFF; PORTC = 0x00; // LED Matrix
    DDRB = 0xFF; PORTB = 0x00; // LCD data lines
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
    
    LCD_init();
    LCD_DisplayString(1,"Hello");

    while (1) {
       PORTA = Rows[0] | Rows[1];
       PORTC = Cols[0] & Cols[1];
    }
    return 1;
}
