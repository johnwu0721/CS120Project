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
#endif

char Rows = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}
char Cols = {

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;
    DDRC = 0x00; PORTC = 0xFF;
    /* Insert your solution below */
    while (1) {
       PORTA = 
    }
    return 1;
}
