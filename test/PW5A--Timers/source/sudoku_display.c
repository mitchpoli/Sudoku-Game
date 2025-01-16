// sudoku_display.c
#include "sudoku_display.h"
#include "Template.h"
#include "numbers_sub.h"
#include <nds.h>

//######################## Template_SUB ###########################
/**
 * @brief Configures video memory and display mode for the sub-screen.
 */
void P_Graphics_setup() {
    // Configure video memory for sub-screen
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

    // Set display mode for the sub-screen
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG0_ACTIVE; 
}

// External pointer to numbers matrix for managing the display.
u16* numbers_matrix;

/**
 * @brief Configures BG0 in tile mode for the sub-screen.
 */
void BG0_Sub() {
    // Configure BG0 for tile-based rendering
    BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(25) | BG_TILE_BASE(6) | BG_PRIORITY(0);

    // Copy tile graphics and palette to sub-screen memory
    swiCopy(smallNumbersTiles, BG_TILE_RAM_SUB(6), smallNumbersTilesLen);
    swiCopy(smallNumbersPal, BG_PALETTE_SUB, smallNumbersPalLen);

    // Set up palette colors
    BG_PALETTE_SUB[1] = ARGB16(1, 31, 31, 31); // Foreground color (white)
    BG_PALETTE_SUB[0] = ARGB16(0, 0, 0, 0);   // Background color (transparent)

    // Initialize background as transparent
    memset(BG_MAP_RAM_SUB(25), 0, 32 * 32 * 2);
}

/**
 * @brief Configures BG2 in rotoscale mode for the sub-screen.
 */
void BG2_Sub() {
    // Configure BG2 for bitmap-based rendering with affine transformations
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B8_256x256 | BG_PRIORITY(3);

    // Set up affine transformation matrix
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;

    // Copy template palette and bitmap to sub-screen memory
    swiCopy(TemplatePal, BG_PALETTE_SUB, TemplatePalLen / 2);
    swiCopy(TemplateBitmap, BG_GFX_SUB, TemplateBitmapLen / 2);
}

/**
 * @brief Initializes the Sudoku display for the sub-screen.
 * 
 * Combines graphics setup, tile configuration, and bitmap rendering
 * for proper visualization of the Sudoku game.
 */
void initSudokuDisp_Sub() {
    P_Graphics_setup();
    BG0_Sub(); 
    BG2_Sub();  
}

/**
 * @brief Prints a number on the specified tile map.
 * 
 * @param map Pointer to the tile map.
 * @param number Number to print (1-9).
 * @param x X-coordinate on the tile map.
 * @param y Y-coordinate on the tile map.
 */
void printNumber(u16* map, int number, int x, int y) {
    int offset = 3;
    x = x * 2 + offset; 
    y = y * 2 + offset;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            map[(i + y) * 32 + j + x] = (u16)i * 2 + j + 4 * (number - 1);
        }
    }
}

/**
 * @brief Initializes the Sudoku grid with a given matrix.
 * 
 * @param map Pointer to the tile map.
 * @param matrix 9x9 Sudoku matrix containing numbers to display.
 */
void sudokuInit(u16* map, int matrix[9][9]) {
    int number;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            number = matrix[i][j];
            if (number != 0) {
                printNumber(map, number, j, i);
            }
        }
    }
}

/**
 * @brief Clears a number from the specified tile map.
 * 
 * @param map Pointer to the tile map.
 * @param x X-coordinate on the tile map.
 * @param y Y-coordinate on the tile map.
 */
void clearNumber(u16* map, int x, int y) {
    int offset = 3;
    x = x * 2 + offset; 
    y = y * 2 + offset;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            map[(i + y) * 32 + j + x] = 0;
        }
    }
}

/**
 * @brief Displaces a sprite vertically towards a target position.
 * 
 * @param newy Target Y-coordinate.
 * @param y Current Y-coordinate.
 * @return Updated Y-coordinate.
 */
int displaceSprite(int newy, int y) {
    if (newy != y) {
        if (newy > y) {
            y += 3; // Increment downwards
            if (y > newy) {
                y = newy; // Prevent overshooting
            }
        } else if (newy < y) {
            y -= 3; // Decrement upwards
            if (y < newy) {
                y = newy; // Prevent overshooting
            }
        }
    }
    return y;
}
