#include "chrono_display.h"

/**
 * @brief Prints a digit on a 2D tile map.
 * 
 * @param map Pointer to the tile map.
 * @param number The number to represent (0-11, where 10 represents "." and 11 represents ":").
 * @param x X-coordinate on the map (starting column).
 * @param y Y-coordinate on the map (starting row).
 */
void printDigit(u16* map, int number, int x, int y)
{
    int i, j;

    if (number < 10) { // Numbers from 0 to 9
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 4; j++) {
                if (number >= 0) {
                    map[(i + y) * 32 + j + x] = (u16)(i * 4 + j) + 32 * number;
                } else {
                    map[(i + y) * 32 + j + x] = 32; // Empty tile
                }
            }
        }
    }

    if (number == 10) { // Decimal point "."
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 2; j++) {
                map[(i + y) * 32 + j + x] = (u16)(i * 4 + j) + 32 * 10 + 2;
            }
        }
    }

    if (number == 11) { // Colon ":"
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 2; j++) {
                map[(i + y) * 32 + j + x] = (u16)(i * 4 + j) + 32 * 10;
            }
        }
    }
}

/**
 * @brief Updates the tile map to display minutes, seconds, and milliseconds.
 * 
 * @param map Pointer to the tile map.
 * @param min Minutes (0-59).
 * @param sec Seconds (0-59).
 * @param msec Milliseconds (0-999).
 */
void updateChronoDisp(u16* map, int min, int sec, int msec)
{
    int x = 0, y = 0;
    int number;

    /***** MINUTES *****/
    number = min;
    if (min > 59) min = number = -1; // Invalid value

    // First digit
    x = 0; y = 8;
    if (min >= 0) number = min / 10;
    printDigit(map, number, x, y);

    // Second digit
    x = 4; y = 8;
    if (min >= 0) number = min % 10;
    printDigit(map, number, x, y);

    /***** POINTS (".") *****/
    x = 8; y = 8;
    number = 10;
    printDigit(map, number, x, y);

    /***** SECONDS *****/
    number = sec;
    if (sec > 59) sec = number = -1; // Invalid value

    // First digit
    x = 10; y = 8;
    if (sec >= 0) number = sec / 10;
    printDigit(map, number, x, y);

    // Second digit
    x = 14; y = 8;
    if (sec >= 0) number = sec % 10;
    printDigit(map, number, x, y);

    /***** COLON (":") *****/
    x = 18; y = 8;
    number = 11;
    printDigit(map, number, x, y);

    /***** MILLISECONDS *****/
    number = msec;
    if (msec > 999) msec = number = -1; // Invalid value

    // First digit
    x = 20; y = 8;
    if (msec >= 0) number = msec / 100;
    printDigit(map, number, x, y);

    // Second digit
    x = 24; y = 8;
    if (msec >= 0) number = (msec % 100) / 10;
    printDigit(map, number, x, y);

    // Third digit
    x = 28; y = 8;
    if (msec >= 0) number = (msec % 10) % 10;
    printDigit(map, number, x, y);
}

/**
 * @brief Wrapper to update the main display with minutes, seconds, and milliseconds.
 * 
 * @param min Minutes.
 * @param sec Seconds.
 * @param msec Milliseconds.
 */
void updateChronoDisp_Main(int min, int sec, int msec)
{
    updateChronoDisp(BG_MAP_RAM(0), min, sec, msec);
}

/**
 * @brief Initializes the main display for showing the clock.
 */
void initChronoDisp_Main()
{
    // Configure video memory and display mode for the main screen
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
    REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;

    // Set up the background control register for a 32x32 tile map, 256 colors
    BGCTRL[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

    // Copy tile data (numbers) and palette to VRAM
    swiCopy(numbersTiles, BG_TILE_RAM(1), numbersTilesLen); // Load number graphics
    swiCopy(numbersPal, BG_PALETTE, numbersPalLen);         // Load palette data

    // Set default colors for tiles
    BG_PALETTE[0] = ARGB16(1, 0, 31, 31); // Default foreground color
    BG_PALETTE[1] = ARGB16(1, 0, 0, 0);   // Background color

    // Initialize the tile map with empty tiles
    memset(BG_MAP_RAM(0), 32, 32 * 32 * 2); // Fill the map with empty tile indices

    // Initialize the display with default (empty) values
    updateChronoDisp_Main(-1, -1, -1); // Clear display initially
}


/**
 * @brief Changes the primary color of the display.
 * 
 * @param c Color in ARGB16 format.
 * 
 * The colors are defined in the .h file
 */
void changeColorDisp_Main(uint16 c)
{
    BG_PALETTE[0] = c;
}
