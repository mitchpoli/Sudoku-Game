//numbers_sub.h
#include <stdint.h>  // For using uint8_t
#include <nds.h>
#define TILE_SIZE 64  // Each tile is 8x8 pixels, and each pixel is 1 byte
#define NUM_TILES 10  // Total number of tiles including the empty tile
#define MATRIX_SIZE 9

// Define the tiles as a two-dimensional array
extern u8 tileData[NUM_TILES][TILE_SIZE];
extern int staticMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern int answerMatrix[MATRIX_SIZE][MATRIX_SIZE];

void initializeMatrices();