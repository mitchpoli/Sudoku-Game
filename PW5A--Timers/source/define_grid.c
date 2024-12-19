// define_grid.c
#include "define_grid.h"
#include <string.h>  // Include for memcpy

// Define the matrices
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


int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE]; // Initially empty

// Implementation of initializeMatrices
void initializeMatrices(void) {
    memcpy(dynamicMatrix, staticMatrix, sizeof(staticMatrix));

}