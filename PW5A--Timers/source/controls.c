//controls.c
#include "controls.h"
//#include "globals.h"
#include <stdio.h>

GameState state = STATE_DELETE;  // Default state or any other initial state

extern int dynamicMatrix[MATRIX_SIZE][MATRIX_SIZE];   
extern int staticMatrix[MATRIX_SIZE][MATRIX_SIZE];
extern u16* numbers;
int x;
int y;   
void handleTouchPad()
{
    // Read the touch position.
	touchPosition touch;
	touchRead(&touch);
	//Only if px or py are different to 0 handle the touch
	if(touch.px || touch.py)
	{
        // inside the grid
		if(touch.px>29 && touch.px<174 && touch.py>23 && touch.py<168){
            // x and y indicate the selected box, x and y element of [0,8] 
            x = (touch.px-29)/16;
            y = (touch.py-23)/16;

            // update the matrix only if the static matrix allows writing
            if(staticMatrix[x][y] == 0){  // Ensure the spot is writable
                if(state == STATE_DELETE){
                    dynamicMatrix[x][y] = 0;  // Clear the cell if delete state is active
                    numbers[x*2*32+y*2]=dynamicMatrix[x][y] | (1<<12); //write with the grey palette

                } else if(dynamicMatrix[x][y] == 0) {
                    dynamicMatrix[x][y] = state;  // Only write if the cell is empty
                    numbers[x*2*32+y*2]=dynamicMatrix[x][y] | (1<<12); //write with the grey palette
                }
            }
        }

        // selection bar
        if(touch.px>205 && touch.px<255 && touch.py>8 && touch.py<191){
            // delete
            // avoid instant update of the cell
            x = -1;
            if(touch.py>164){
                state = STATE_DELETE;
            }
            // numbers
            else{
                y = (touch.py-9)/15;
                if(y>=9){
                    y = 8;
                }
                state = (GameState)(STATE_NUMBER_1 + y);
                // y is now the selected number
            }
        }
    }
}
