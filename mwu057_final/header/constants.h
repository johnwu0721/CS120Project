#ifndef CONSTANTS_H
#define CONSTANTS_H
/*
unsigned char Rows[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //LED Rows
unsigned char Cols[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; //LED Cols



struct Arrows {
   unsigned char Row;
   unsigned char Col;
} up,left,down,right;

unsigned char allRows = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
unsigned char allCols = Cols[0] & Cols[1] & Cols[2] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7];

struct Arrows up = {.Row = allRows, .Col = Cols[6] & Cols[7]};
struct Arrows left = {.Row = Rows[0] | Rows[1], .Col = allCols};
struct Arrows down = {.Row = allRows, .Col = Cols[0] & Cols[1]};
struct Arrows right = {.Row = Rows[6] | Rows[7], .Col = allCols};




struct Arrows ledArr[4] = {
   {Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7], Cols[6] & Cols[7]}, //up
   {Rows[0] | Rows[1], Cols[0] & Cols[1] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7]}, //left
   {Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7], Cols[0] & Cols[1]}, //down
   {Rows[6] | Rows[7], Cols[0] & Cols[1] & Cols[3] & Cols[4] & Cols[5] & Cols[6] & Cols[7]}, //right
}
*/
//up.Rows[] = Rows[0] | Rows[1] | Rows[2] | Rows[3] | Rows[4] | Rows[5] | Rows[6] | Rows[7];
//up.Cols[] = Cols[6] & Cols[7]; 



unsigned char player1[8] = {
   0x0A,
   0x04,
   0x0A,
   0x1F,
   0x0E,
   0x04,
   0x0A,
   0x11,
};

#endif
