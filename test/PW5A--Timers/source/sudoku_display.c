//sudoku_display.c
#include "sudoku_display.h"
#include "Template.h"
#include "numbers_sub.h"
#include <nds.h>

//########################Tameplate_SUB###########################
void P_Graphics_setup() {
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG0_ACTIVE; 
}

//extern u16* numbers_matrix;
u16* numbers_matrix;

void BG0_Sub() { // tile mode
    // Configura BG0 sulla sub screen
    BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(25) | BG_TILE_BASE(6) | BG_PRIORITY(0);

    // Copia i tiles e la palette nella memoria della sub screen
    swiCopy(smallNumbersTiles, BG_TILE_RAM_SUB(6), smallNumbersTilesLen);
	swiCopy(smallNumbersPal, BG_PALETTE_SUB, smallNumbersPalLen);

    // Configura i colori della palette
	BG_PALETTE_SUB[1] = ARGB16(1,31,31,31);
	BG_PALETTE_SUB[0] = ARGB16(0,0,0,0);

    // Inizializza lo sfondo come trasparente
	memset(BG_MAP_RAM_SUB(25), 0,32*32*2);
}


void BG2_Sub() { // rotoscale mode

	//Configure BG 2 Sub.
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B8_256x256 | BG_PRIORITY(3);

    //Affine Marix Transformation
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;

    swiCopy(TemplatePal, BG_PALETTE_SUB, TemplatePalLen/2);
    swiCopy(TemplateBitmap, BG_GFX_SUB, TemplateBitmapLen/2);
    // TemplatePal defined in Template.h
}



void initSudokuDisp_Sub() {
    P_Graphics_setup();
    BG0_Sub(); 
    BG2_Sub();  
}



void printNumber(u16* map, int number, int x, int y) {
    int offset = 3;
    x = x*2 +offset; 
    y = y*2 + offset;
    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            map[(i + y)*32+j+x] = (u16)i*2+j+4*(number-1);
        }
    }
}


void sudokuInit(u16* map, int matrix[9][9]) {
    int number;
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            number = matrix[i][j];
            if (number != 0) {
                printNumber(map, number, j, i);
            }
        }
    }
}

void clearNumber(u16* map, int x, int y) {
    int offset = 3;
    x = x*2 +offset; 
    y = y*2 + offset;
    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            map[(i + y)*32+j+x] = 0;
        }
    }
}


int displaceSprite(int newy, int y) {
    if (newy != y) {
        if (newy > y) {
            y += 3; // Incrementa verso il basso
            if (y > newy) {
                y = newy; // Assicurati di non superare il limite
            }
        } else if (newy < y) {
            y -= 3; // Decrementa verso l'alto
            if (y < newy) {
                y = newy; // Assicurati di non superare il limite
            }
        }
    }
    return y;
}
