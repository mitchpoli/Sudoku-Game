// define_grid.h
#ifndef GAME_MATRICES_H
#define GAME_MATRICES_H

#define MATRIX_SIZE 9

// Declare the matrices as external to allow them to be accessed from other files
extern int staticMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern int answerMatrix[MATRIX_SIZE][MATRIX_SIZE];

// Declare the function to initialize matrices
void initializeMatrices(void);

#endif // GAME_MATRICES_H
