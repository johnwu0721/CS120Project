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
#include <avr/eeprom.h>
#include "io.h"
#include <avr/interrupt.h>
#include "timer.h"
#endif

#define up (~PINB & 0x08)    // up button
#define left (~PINB & 0x10)  // left button
#define down (~PINB & 0x20)  // down button
#define right (~PINB & 0x04) // right button
#define speaker (PINB & 0x40) // speaker

unsigned char gameStatus = 0; // game status
uint8_t highScore;
double frequency = 0.00; 
short score = 0; //score of the game

char Rows[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // LED Matrix
char Cols[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; // LED Matrix

enum MENU_STATES { WAIT, START, END } m_state ;

void menu_tick() {
   switch(m_state) { //transitions
      case WAIT:
         if (up && !down && !right && !left) { // up to start
            m_state = START;
         }
         else if (up && left && !right && !down) { // up & left to reset
            m_state = WAIT;
         }
         break;
      case START:
         m_state = (gameStatus) ? END : START; // gameStat = 1 means end
         break;
      case END:
         m_state = (gameStatus) ? END : WAIT;
         break;
   }
   switch(m_state) { //actions
      case WAIT:
         startGame();
         break;
      case START:
         gameStatus = 0;
         displayLED();
         break;
      case END:
         displayHighScore();
         break;
   }
}

void startGame() { //starting message
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "Press up to start");
}

int displayHighScore(int score) {
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "Highscore: ");
   LCD_Cursor(11);
   LCD_WriteData(highScore);
   return;
}

void displayLED() { //arrows for the game
     

}


void set_PWM(double frequency) {
   static double current_frequency;
   if (frequency != current_frequency) {
      if(!frequency) {TCCR3B &= 0x08; }
      else { TCCR3B |= 0x03; }
      if (frequency < 0.954) {OCR3A = 0xFFFF; }
      else if (frequency > 31250) {OCR3A = 0x0000; }
      else {OCR3A = (short)(8000000/ (128 * frequency)) - 1;}
      TCNT3 = 0;
      current_frequency = frequency;
   }
}

void PWM_on() {
   TCCR3A = (1 << COM3A0);
   TCCR3B = (1 << WGM32) | (1 < CS31) | (1 < CS30);
   set_PWM(0);
}

void PWM_off() {
   TCCR3A = 0x00;
   TCCR3B = 0x00;
}








int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00; // LED Matrix 
    DDRC = 0xFF; PORTC = 0x00; // LED Matrix
    DDRB = 0xE0; PORTB = 0x1F; // Output: LCD, speaker Input: buttons
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
    
    TimerOn();
    TimerSet(100);
    PWM_on();

    while (1) {
       PORTA = Rows[0] | Rows[1];
       PORTC = Cols[0] & Cols[1];
    }

    return 0;
}
