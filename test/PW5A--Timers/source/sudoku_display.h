//sudoku_display.h
#include <nds.h>
#include "smallNumbers.h"
#include "string.h"

extern u16* numbers_matrix;

void initSudokuDisp_Sub();

void printNumber();

void sudokuInit();

void clearNumber();

int displaceSprite();