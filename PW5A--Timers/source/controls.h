//controls.h
#include <nds.h>

#define MATRIX_SIZE 9

// if defined here it is accessible by controls.c and main.c
typedef enum {
    STATE_DELETE = 0, // Start from 0 to align with the number values 
    STATE_NUMBER_1,
    STATE_NUMBER_2,
    STATE_NUMBER_3,
    STATE_NUMBER_4,
    STATE_NUMBER_5,
    STATE_NUMBER_6,
    STATE_NUMBER_7,
    STATE_NUMBER_8,
    STATE_NUMBER_9 
} GameState;

extern GameState state;  // Declaration of the state variable

// Declare the matrices as external to allow them to be accessed from other files
extern int staticMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE];

extern int x;
extern int y;

void handleTouchPad();
