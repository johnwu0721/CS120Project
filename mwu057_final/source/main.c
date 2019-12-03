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
#include <stdlib.h>
#include "timer.h"
#include "scheduler.h"
#include <util/delay.h>
#endif

#define up (~PINB & 0x08)    // up button
#define left (~PINB & 0x10)  // left button
#define down (~PINB & 0x20)  // down button
#define right (~PINB & 0x04) // right button

unsigned char gameStatus = 0; // game status
uint8_t highScore;
int score = 0; //score of the game

char Rows[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // LED Matrix
char Cols[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; // LED Matrix

enum MENU_STATES { WAIT, START, END, BUTPRESS, RESET } m_state ;
enum LED_STATES { WAITBUT,RELBUT, INIT, ONE, TWO, THREE, FOUR, FIN } state;

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
         break;
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
         //displayLED(score);
         displayScore(score);
         break;
      case END:
         displayHighScore(highScore);
         break;
      case RESET:
         break;
      default:
         break;
   }
}

void startGame() { //starting message
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "up to start");
   LCD_Cursor(17);
   LCD_createChar(0,player1);
   LCD_WriteData(0x00);
   return;
}

void displayScore() {
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "score: ");
   LCD_Cursor(7);
   updateScore(score);
   LCD_WriteData(score + '0');
}

void displayHighScore() {
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "highscore: ");
   LCD_Cursor(11);
   if (score > EEPROM_Read(0xFF)) {
      EEPROM_Write(0xFF, score);
     highScore = score;
   }
   else {
     highScore = eeprom_read_byte((uint8_t*)20);
   }
   LCD_WriteData(highScore);
}
//enum LED_STATES = { WAITBUT,RELBUT, INIT, 1, 2, 3, 4, FIN };

void led_tick(int state) {
   int random = 0;
   switch(state) { //led transitions
      case WAITBUT:
         if (up && !down && !right && !left) {
            state = RELBUT;
         }
         else {
            state = WAITBUT;
         }
         break;
      case RELBUT:
         if(!up) {
            state = INIT;
         }
         else {
            state = WAITBUT;
         }
         break;
      case INIT:
         random = genRandom();
         if (gameStatus == 1) {
            state = FIN;
         }
         else {
            state = INIT;
         }
         break;
      case ONE:      //up
         if (up && down && right && left) {
            state = WAITBUT;
         }
         else {
            state = INIT;
         }
         break;
      case TWO:      //down
         if (up && down && left && right) {
            state = WAITBUT;
         }
         else {
            state = INIT;
         }
         break;
      case THREE:      //left
         if (up && down && left && right) {
            state = WAITBUT;
         }
         else {
            state = INIT;
         }
         break;
      case FOUR:      //right
         if (up && down && left && right) {
            state = WAITBUT;
         }
         else {
            state = INIT;
         }
         break;
      case FIN:
         if (gameStatus == 0) {
            state = INIT;
         }
         else {
            state = FIN;
         }
         break;
      default:
         state = WAITBUT;
         break;
   }
   switch(state) {       //LED ACTIONS
      case WAITBUT:
         PORTA = 0x00;
         PORTC = 0x00;
         break;
      case RELBUT:
         break;
      case INIT:
         //random = genRandom();
         break;
      case ONE: //up
         PORTA = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
         PORTC = Cols[6] & Cols[7];
         if (up) {
            score++;
         }
         break;
      case TWO: //down
         PORTA = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
         PORTC = Cols[0] & Cols[1];
         if (down) {
            score++;
         } 
         break;
      case THREE: //left
         PORTA = Rows[0] | Rows[1];
         PORTC = Cols[0] & Cols[1] & Cols[2] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7];
         if (left) {
            score++;
         }
         break;
      case FOUR: //right
         PORTA = Rows[6] | Rows[7];
         PORTC = Cols[0] & Cols[1] & Cols[2] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7];
         if (right) {
            score++;
         }
         break;
      case FIN:
         displayHighScore(score);
         break;
      default:
         break;
   }
}
/*
int updateScore(int score) {
   score = score + 1;
   return score;
}
*/
int genRandom() {
   return (rand() % 4) + 1;
}

void EEPROM_Write(unsigned char address, unsigned char data) {
   eeprom_write_byte(address, data);
}

unsigned char EEPROM_Read(unsigned char address) {
   return eeprom_read_byte(address);
}


int main(void) {
    DDRA = 0xFF; PORTA = 0x00; // LED Matrix Rows  
    DDRC = 0xFF; PORTC = 0x00; // LED Matrix Cols
    DDRB = 0x43; PORTB = 0x9C; // Output: LCD Input: buttons
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
     
    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks)/ sizeof(task*);
    unsigned char i;
    unsigned long GCD = tasks[0] -> period;
    for (i = 1; i < numTasks; i++) {
       GCD = findGCD(GCD,tasks[i]->period);
    }

    task1.state = WAIT; //menu
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = & menu_tick;

    task2.state = WAITBUT; //LED
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = & led_tick;
    
    
    TimerSet(GCD);
    TimerOn();
    while (1) {       
       menu_tick(m_state);
       led_tick(state);
       
       for (i = 0; i < numTasks; i++) {
          if(tasks[i] -> elapsedTime == tasks[i] -> period) {
             tasks[i] -> state = tasks[i] -> TickFct(tasks[i] -> state);
             tasks[i] -> elapsedTime = 0;
          }
          tasks[i] -> elapsedTime += 1;
       }
       
       while (!TimerFlag) {
          TimerFlag = 0;
       }
    }

    return 0;
}
