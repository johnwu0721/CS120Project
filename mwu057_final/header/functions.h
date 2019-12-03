#ifndef __functions_h__
#define __functions_h__

void startGame();
void displayHighScore();
void displayScore();
int genRandom();
int updateScore(int score);
void EEPROM_Write(unsigned char address, unsigned char data);
unsigned char EEPROM_Read(unsigned char address);

#endif
