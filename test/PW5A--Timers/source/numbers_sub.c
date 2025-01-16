//numbers_sub.c
#include "numbers_sub.h"
#include <string.h>  // Include for memcpy

#define TILE_SIZE_8x8 64  // Ogni tile 8x8 è 64 byte
#define TILE_SIZE_16x16 256 // Ogni tile 16x16 è 256 byte
#define NUM_TILES 10  // Total number of tiles including the empty tile
#define MATRIX_SIZE 9

// Define the matrices
// This is the initial matrix used in the sudoku game
int staticMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 0, 0, 2, 0, 0, 0, 4, 6},
    {5, 6, 2, 0, 0, 0, 0, 8, 0},
    {7, 0, 0, 0, 0, 0, 0, 5, 0},
    {3, 0, 0, 0, 0, 9, 0, 6, 1},
    {0, 7, 0, 3, 0, 5, 0, 2, 0},
    {1, 9, 0, 4, 0, 0, 0, 0, 3},
    {0, 4, 0, 0, 0, 0, 0, 0, 2},
    {0, 5, 0, 0, 0, 0, 8, 3, 4},
    {8, 1, 0, 0, 0, 2, 0, 0, 0}
};

// This is the matrix representing the sudoku solution
int answerMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {9, 8, 1, 2, 5, 7, 3, 4, 6},
    {5, 6, 2, 9, 3, 4, 1, 8, 7},
    {7, 3, 4, 1, 6, 8, 2, 5, 9},
    {3, 2, 5, 8, 7, 9, 4, 6, 1},
    {4, 7, 6, 3, 1, 5, 9, 2, 8},
    {1, 9, 8, 4, 2, 6, 5, 7, 3},
    {6, 4, 9, 5, 8, 3, 7, 1, 2},
    {2, 5, 7, 6, 9, 1, 8, 3, 4},
    {8, 1, 3, 7, 4, 2, 6, 9, 5}
};

// This is the dynamic matrix used in the game. It is initialized like the staticMatrix, 
// and is updated throughout the game. It is therefore a dynamic matrix
int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 0, 0, 2, 0, 0, 0, 4, 6},
    {5, 6, 2, 0, 0, 0, 0, 8, 0},
    {7, 0, 0, 0, 0, 0, 0, 5, 0},
    {3, 0, 0, 0, 0, 9, 0, 6, 1},
    {0, 7, 0, 3, 0, 5, 0, 2, 0},
    {1, 9, 0, 4, 0, 0, 0, 0, 3},
    {0, 4, 0, 0, 0, 0, 0, 0, 2},
    {0, 5, 0, 0, 0, 0, 8, 3, 4},
    {8, 1, 0, 0, 0, 2, 0, 0, 0}
};


/*
// easy static matrix for fast check the code, jsut write a 5 in the empty box
int staticMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {9, 8, 1, 2, 5, 7, 3, 4, 6},
    {5, 6, 2, 9, 3, 4, 1, 8, 7},
    {7, 3, 4, 1, 6, 8, 2, 5, 9},
    {3, 2, 5, 8, 7, 9, 4, 6, 1},
    {4, 7, 6, 3, 1, 5, 9, 2, 8},
    {1, 9, 8, 4, 2, 6, 5, 7, 3},
    {6, 4, 9, 5, 8, 3, 7, 1, 2},
    {2, 5, 7, 6, 9, 1, 8, 3, 4},
    {8, 1, 3, 7, 4, 2, 6, 9, 0}
};

int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {9, 8, 1, 2, 5, 7, 3, 4, 6},
    {5, 6, 2, 9, 3, 4, 1, 8, 7},
    {7, 3, 4, 1, 6, 8, 2, 5, 9},
    {3, 2, 5, 8, 7, 9, 4, 6, 1},
    {4, 7, 6, 3, 1, 5, 9, 2, 8},
    {1, 9, 8, 4, 2, 6, 5, 7, 3},
    {6, 4, 9, 5, 8, 3, 7, 1, 2},
    {2, 5, 7, 6, 9, 1, 8, 3, 4},
    {8, 1, 3, 7, 4, 2, 6, 9, 0}
};
*/