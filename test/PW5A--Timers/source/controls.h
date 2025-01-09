//controls.h
#include <nds.h>
#define TILE_SIZE 64  // Each tile is 8x8 pixels, and each pixel is 1 byte
#define NUM_TILES 10  // Total number of tiles including the empty tile
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
extern u8 tileData[NUM_TILES][TILE_SIZE];
extern int staticMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern u16* numbers_matrix;

extern u16* map;
extern int x;
extern int y;
extern int newy;

void handleTouchPad();
void tiles_draw();