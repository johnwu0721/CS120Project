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
#include "io.c"
#include "functions.h"
#include <avr/interrupt.h>
//#include <stdlib.h>
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
int score = 0; //score of the game

char Rows[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // LED Matrix
char Cols[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; // LED Matrix

enum MENU_STATES { WAIT, START, END, BUTPRESS, RESET } m_state ;

void menu_tick() {
   switch(m_state) { //transitions
      case WAIT:
         if (up && !down && !right && !left) { // up to start
            m_state = BUTPRESS;
         }
         else {
            m_state = WAIT;
         }
         break;
      case BUTPRESS:
         if (!up) {
            m_state = START;
         }
         else {
            m_state = WAIT;
         }
      case START:
         if (up && down && left && right) { //reset
            m_state = RESET;
         }
         else if (gameStatus) {
            m_state = END;
         }
         else {
            m_state = START;
         }
         //m_state = (gameStatus) ? END : START; // gameStat = 1 means end
         break;
      case END:
         if (up && down && left && right) { //reset
            m_state = RESET;
         }
         else if (gameStatus) {
            m_state = END;
         }
         else {
            m_state = WAIT;
         }
         //m_state = (gameStatus) ? END : WAIT;
         break;
      case RESET:
         m_state = WAIT;
         break;
      default:
         m_state = WAIT;
         break;
   }
   switch(m_state) { //actions
      case WAIT:
         startGame();
         break;
      case BUTPRESS:
         break;
      case START:
         gameStatus = 0;
         displayScore(score); 
         displayLED();
         break;
      case END:
         displayHighScore(highScore);
         break;
      case RESET:
         startGame();
         PORTA = 0x00;
         PORTC = 0x00;
         break;
      default:
         break;
   }
}

void startGame() { //starting message
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "press up to start");
   return;
}

int displayScore(int score) {
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "score: ");
   LCD_Cursor(7);
   updateScore(score);
   LCD_WriteData(score + '0');
   return score;
}

int displayHighScore(int score) {
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "highscore: ");
   LCD_Cursor(11);
   LCD_WriteData(highScore); //LCD_WriteData
   return score;
}

void displayLED() { //arrows for the game
   unsigned char rand = 0;
   rand = genRandom();
   if (rand == 1) { //up
      PORTA = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
      PORTC = Cols[6] & Cols[7];
   }
   else if (rand == 2) { //down
      PORTA = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
      PORTC = Cols[0] & Cols[1];
   }
   else if (rand == 3) { //left
      PORTA = Rows[0] | Rows[1];
      PORTC = Cols[0] & Cols[1] & Cols[2] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7];
   }
   else if (rand == 4) { //right
      PORTA = Rows[6] | Rows[7];
      PORTC = Cols[0] & Cols[1] & Cols[2] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7];
   }
   updateScore(score);
   return;
}

int updateScore(int score) {
   if (rand == 1 && up) {
      score = score + 100;
   }
   else if (rand == 2 && down) {
      score = score + 100;
   }
   else if (rand == 3 && left) {
      score = score + 100;
   }
   else if (rand == 4 && right) {
      score = score + 100;
   }
   return score;
}

int genRandom() {
   return (rand() % 4) + 1;
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
    DDRA = 0xFF; PORTA = 0x00; // LED Matrix Rows  
    DDRC = 0xFF; PORTC = 0x00; // LED Matrix Cols
    DDRB = 0x43; PORTB = 0x9C; // Output: LCD, speaker Input: buttons
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
    
    TimerOn();
    TimerSet(1000);
    PWM_on();
    //startGame();
    m_state = WAIT;

    while (1) {
       //PORTA = Rows[0] | Rows[1];
       //PORTC = Cols[2] & Cols[5];
       menu_tick();
       while (!TimerFlag) {
          TimerFlag = 0;
       }
    }

    return 0;
}
