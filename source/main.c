// main.c
#include <nds.h>
#include <stdio.h>
#include <math.h>

#include "string.h"
#include "chrono_display.h"
#include "sudoku_display.h"
#include "numbers.h"
#include "Template.h"
#include "sudoku_init.h"
#include "controls.h"
#include "numbers_sub.h"
#include "P_Audio.h"
#include "sprite.h"

#include <sys/dir.h>
#include <fat.h>
#include <dirent.h>

int newy; // Target y-coordinate for sprite movement
int min, sec, msec; // Variables to store timer values
int timer_running = 1; // Timer state: 1 = running, 0 = paused
u16* gfx; // Pointer to sprite graphics
u16 prev_timer_value; // Stores the previous timer value for calculating elapsed time

//######################## SPRITE ###########################
/**
 * @brief Configures the sprite system and loads sprite graphics.
 */
void configureSprite() {
    VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE; // Enable VRAM for sub-screen sprites
    oamInit(&oamSub, SpriteMapping_1D_32, false); // Initialize sub-screen Object Attribute Memory (OAM)
    gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color); // Allocate graphics memory for the sprite

    // Copy sprite palette and graphics to VRAM
    swiCopy(spritePal, SPRITE_PALETTE, spritePalLen / 2);
    swiCopy(spriteTiles, gfx, spriteTilesLen / 2);

    // Set palette colors
    SPRITE_PALETTE[0] = ARGB16(1, 0, 0, 0); // Black
    SPRITE_PALETTE[1] = ARGB16(1, 31, 31, 31); // White
}

//######################## TIMER ###########################
/**
 * @brief Updates the timer using the hardware timer and calculates elapsed time in milliseconds.
 */
void updateCrono() {
    if (timer_running) {
        u16 current_timer_value = TIMER_DATA(0); // Read the current value of TIMER_0
        u16 elapsed_ticks = current_timer_value - prev_timer_value; // Calculate the ticks since the last update
        prev_timer_value = current_timer_value; // Update the previous timer value

        // Convert ticks to milliseconds (assuming TIMER_DIV_1024)
        msec += (elapsed_ticks * 1024) / (BUS_CLOCK / 1000);
        if (msec >= 1000) {
            msec -= 1000;
            sec++;
            if (sec >= 60) {
                sec -= 60;
                min++;
            }
        }
    }
}

//######################## SECONDARY STORAGE ###########################
/**
 * @brief Compares the current time with the stored best time.
 * @return 1 if the current time is better, 0 otherwise.
 */
int isBetterTime(int curr_min, int curr_sec, int curr_msec, int file_min, int file_sec, int file_msec) {
    if (curr_min < file_min) return 1;
    if (curr_min == file_min && curr_sec < file_sec) return 1;
    if (curr_min == file_min && curr_sec == file_sec && curr_msec < file_msec) return 1;
    return 0;
}

/**
 * @brief Saves the best time to a file.
 */
void saveBestTime(int min, int sec, int msec) {
    FILE *file = fopen("/result.txt", "w"); // Opens the file in write mode
    if (file) {
        fprintf(file, "%02d:%02d.%03d\n", min, sec, msec); // Write the time in MM:SS.mmm format
        fclose(file);
    } else {
        iprintf("Error: Unable to write to result.txt\n");
    }
}

/**
 * @brief Loads the best time from a file.
 * @return 1 if successful, 0 otherwise.
 */
int loadBestTime(int *file_min, int *file_sec, int *file_msec) {
    FILE *file = fopen("/result.txt", "r"); // Opens the file in read mode
    if (file) {
        int result = fscanf(file, "%d:%d.%d", file_min, file_sec, file_msec); // Read the time in MM:SS.mmm format
        fclose(file);
        return result == 3; // Return true if three values were successfully read
    }
    return 0; // File not found or read error
}

int main(void) {
    // Initialize the FAT file system and the console for debug output
    fatInitDefault();
    consoleDemoInit();

    // Initialize the main and sub screens
    initChronoDisp_Main();
    initSudokuDisp_Sub();

    // Set the initial timer color and values
    uint16_t currentColor = GREEN;
    changeColorDisp_Main(GREEN);
    min = sec = msec = 0;
    newy = 169;

    // Configure hardware timer (TIMER_0) with a 1024 divider
    TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024;
    TIMER_DATA(0) = 0;
    prev_timer_value = 0;

    // Initialize the Sudoku grid
    sudokuInit(BG_MAP_RAM_SUB(25), staticMatrix);

    // Load the best time from file or initialize it
    int file_min = 99, file_sec = 59, file_msec = 999;
    int best_time_loaded = loadBestTime(&file_min, &file_sec, &file_msec);
    if (!best_time_loaded) {
        saveBestTime(99, 59, 999); // Initialize the file with a "maximum" time
    }

    // Initialize audio and start background music
    Audio_Init();
    Audio_PlayMusic();

    // Configure and initialize the sprite
    configureSprite();
    int x = 216, y = 9;

    while (1) {
        // Update sprite position
        y = displaceSprite(newy, y);
        oamSet(&oamSub, 0, x, y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx, -1, false, false, false, false, false);
        oamUpdate(&oamSub);

        // Handle user input
        scanKeys();
        int keys = keysDown();

        if (keys & KEY_X) timer_running = 0; // Pause timer
        if (keys & KEY_A) timer_running = 1; // Resume timer

        if (keys & KEY_UP) { // Reset best time
            file_min = 99;
            file_sec = 59;
            file_msec = 999;
            saveBestTime(file_min, file_sec, file_msec);
            changeColorDisp_Main(GREEN);
        }

        if (timer_running) handleTouchPad();

        // Wait for screen refresh
        swiWaitForVBlank();

        // Update and display the timer
        updateCrono();
        updateChronoDisp_Main(min, sec, msec);

        // Change timer color if the record is surpassed
        if (currentColor != RED && !isBetterTime(min, sec, msec, file_min, file_sec, file_msec)) {
            changeColorDisp_Main(RED);
            currentColor = RED;
        }

        // Check if the Sudoku puzzle is solved
        if (memcmp(dynamicMatrix, answerMatrix, sizeof(dynamicMatrix)) == 0) {
            timer_running = 0; // Stop the timer
            if (isBetterTime(min, sec, msec, file_min, file_sec, file_msec)) {
                saveBestTime(min, sec, msec); // Save the new best time
            }
        }
    }
}
