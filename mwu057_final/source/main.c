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
#include <avr/epprom.h"
#include "io.h"
#endif

#define up (~PINB & 0x08)
#define left (~PINB & 0x10)
#define down (~PINB & 0x20)
#define right (~PINB & 0x40)

char Rows[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // LED Matrix
char Cols[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; // LED Matrix

enum MENU_STATES { WAIT, START, END } m_state ;

void menu_tick() {
   switch(m_state) { //transitions
      case WAIT:
         if (up && !down && !right && !left) {
            m_state = START;
         }
         else if (up && left && !right && !down) {
            m_state = WAIT;
         }
         break;
      case START:
         

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00; // LED Matrix 
    DDRC = 0xFF; PORTC = 0x00; // LED Matrix
    DDRB = 0xE0; PORTB = 0x1F; // Output: LCD, speaker Input: buttons
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
    
    LCD_init();
    LCD_DisplayString(1,"Hello");

    while (1) {
       PORTA = Rows[0] | Rows[1];
       PORTC = Cols[0] & Cols[1];
    }
    return 1;
}
