//controls.c
#include "controls.h"
#include "numbers_sub.h"
//#include "globals.h"
#include <stdio.h>
#include "P_Audio.h"
#include "sudoku_display.h"

GameState state = STATE_DELETE;  // Default state or any other initial state

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
		if(touch.px>23 && touch.px<170 && touch.py>23 && touch.py<168){
            // x and y indicate the selected box, x and y element of [0,8] 
            x = (touch.px-24)/16;
            y = (touch.py-23)/16;

            // Update the matrix only if the static matrix allows writing
            if(staticMatrix[y][x] == 0){  // Ensure the spot is writable
                if(state == STATE_DELETE){
                    dynamicMatrix[y][x] = 0;  // Clear the cell if delete state is active
                    clearNumber(BG_MAP_RAM_SUB(25),x,y);
                    
                } else if(dynamicMatrix[y][x] == 0) {
                    dynamicMatrix[y][x] = state;  // Only write if the cell is empty
                    printNumber(BG_MAP_RAM_SUB(25), state, x,y);
                }
            }
            // Play the left sound effect
            Audio_PlaySoundEX(SFX_SWISH);
        }

        // Selection bar
        if(touch.px>200 && touch.px<255){
            // Delete
            // Avoid instant update of the cell
            x = -1;
            if(touch.py>164){
                state = STATE_DELETE;
                newy = 169;
            }
            // Numbers
            else{
                y = (touch.py-9)/16;
                if(y>=9){
                    y = 8;
                }
                state = (GameState)(STATE_NUMBER_1 + y);
                // y is now the selected number
                newy = (state-1)*16+9;
            }
             // Play the right sound effect
            Audio_PlaySoundEX(SFX_CLUNK);
        }
    }
}