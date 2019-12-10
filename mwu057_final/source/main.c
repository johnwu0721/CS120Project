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
#include "eeprom.h"
#include "io.h"
#include "functions.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include "timer.h"
#include "scheduler.h"
#include <util/delay.h>


#define up (~PINB & 0x04)    // up button
#define left (~PINB & 0x08)  // left button
#define right (~PINB & 0x10)  // right button
#define down (~PINB & 0x20) // down button

unsigned char gameStatus = 0; // game status  0 in game 1 end game
uint8_t highScore;
int score; //score of the game
int LEDrandom; //random sequence of LED
int num;
int counter = 0;

char Rows[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // LED Matrix
char Cols[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; // LED Matrix

enum MENU_STATES { WAIT, BUTPRESS, START, END } m_state ;
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
            m_state = BUTPRESS;
         }
         break;
      case START:
         if (up && down && left && right) { //reset
            m_state = WAIT;
         }
         if (gameStatus == 1) {
            m_state = END;
         }
         else {
            m_state = START;
         }
         break;
      case END:
         if (up && down && left && right) { //reset
            m_state = WAIT;
         }
         if (gameStatus == 1) {
            m_state = END;
         }
         else {
            m_state = WAIT;
         }
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
         displayScore();
         break;
      case END:
         displayHighScore();
         break;
      default:
         break;
   }
}

void startGame() { //starting message
   LCD_init();
   LCD_ClearScreen();
   LCD_DisplayString(1, "up to start");
   LCD_createChar(0,p1);
   LCD_Cursor(17);
   LCD_WriteData(0x00);
   LCD_createChar(1,p2);
   LCD_Cursor(18);
   LCD_WriteData(0x01);
   LCD_createChar(2,p3);
   LCD_Cursor(19);
   LCD_WriteData(0x02);
   LCD_createChar(3,p4);
   LCD_Cursor(20);
   LCD_WriteData(0x03);
   LCD_createChar(4,p5);
   LCD_Cursor(21);
   LCD_WriteData(0x04);
   LCD_createChar(5,p3);
   LCD_Cursor(22);
   LCD_WriteData(0x05);
   LCD_createChar(6,p2);
   LCD_Cursor(23);
   LCD_WriteData(0x06);
   LCD_createChar(7,p6);
   LCD_Cursor(24);
   LCD_WriteData(0x07);
   EEPROM_write(0,5);
   num = EEPROM_read(0);
   LCD_Cursor(26);
   LCD_WriteData(num + '0');
}

void displayScore() {
   LCD_ClearScreen();
   LCD_DisplayString(1, "score: ");
   LCD_Cursor(7);
   LCD_WriteData(score + '0');
}

void displayHighScore() {
   LCD_ClearScreen();
   LCD_DisplayString(1, "highscore: ");
   LCD_Cursor(11);
   highScore = EEPROM_read(20);
   if (score > highScore) {
     EEPROM_write(20,score);
   }
   else {
     highScore = EEPROM_read(20);
   }
   LCD_WriteData(highScore + '0');
}

//enum LED_STATES = { WAITBUT,RELBUT, INIT, 1, 2, 3, 4, FIN };

void led_tick() {
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
            state = RELBUT;
         }
         break;
      case INIT:
         LEDrandom = genRandom();
         if (gameStatus == 1) {
            state = FIN;
         }
         else {
            state = LEDrandom;
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
            state = WAITBUT;
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
	 score = 0;
         if (counter >= 30) {
			 gameStatus = 1;
		 }
         break;
      case ONE: //up
         PORTA = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
         PORTC = Cols[6] & Cols[7];
		 counter++;
		 _delay_ms(1000);
		 if(up) {
		   score++;
                 }
         break;
      case TWO: //down
         PORTA = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
         PORTC = Cols[0] & Cols[1];
		 counter++;
		 _delay_ms(1000);
		 if(down) {
		   score++;
		 }
         break;
      case THREE: //left
         PORTA = Rows[0] | Rows[1];
         PORTC = Cols[0] & Cols[1] & Cols[2] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7];
		 counter++;
		 _delay_ms(1000);
		 if(left) {
		   score++;
		 }
         break;
      case FOUR: //right
         PORTA = Rows[6] | Rows[7];
         PORTC = Cols[0] & Cols[1] & Cols[2] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7];
                 counter++;
                 _delay_ms(1000);
	         if(right) {
	            score++;
	         }
         break;
      case FIN:
         displayHighScore();
		 counter = 0;
		 _delay_ms(10000);
		 gameStatus = 0;
         break;
      default:
         break;
   }
}


int genRandom() {
   return (rand() % 4) + 3;
}



int main(void) {
    DDRA = 0xFF; PORTA = 0x00; // LED Matrix Rows  
    DDRC = 0xFF; PORTC = 0x00; // LED Matrix Cols
    DDRB = 0xC3; PORTB = 0x3C; // Output: LCD Input: buttons
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
     
    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks)/ sizeof(task*);
    unsigned short i; //loop iterator
    unsigned long GCD = tasks[0] -> period;
    for (i = 1; i < numTasks; i++) {
       GCD = findGCD(GCD,tasks[i]->period);
    }

    task1.state = WAIT; //menu
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &menu_tick;

    task2.state = WAITBUT; //LED
    task2.period = 100;
    task2.elapsedTime = task2.period;
    task2.TickFct = &led_tick;
    
    
    TimerSet(GCD);
    TimerOn();
    while (1) {       
       if (gameStatus == 1) {
          PORTA = 0x00;
          PORTC = 0x00;
          displayHighScore();
       }
       if (gameStatus == 0) {
          for (i = 0; i < numTasks; i++) {
             if(tasks[i] -> elapsedTime == tasks[i] -> period) {
                tasks[i] -> state = tasks[i] -> TickFct(tasks[i] -> state);
                tasks[i] -> elapsedTime = 0;
             }
             tasks[i] -> elapsedTime += GCD;
          }
	   }
	   
       while (!TimerFlag);
          TimerFlag = 0;
    }

    return 0;
}
