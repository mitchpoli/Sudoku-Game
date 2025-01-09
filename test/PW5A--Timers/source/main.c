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

//########################TIMER###########################
int newy;
int min, sec, msec;
int timer_running = 1;
u16* gfx;

void configureSprite() {
    VRAM_D_CR = VRAM_ENABLE | VRAM_D_SUB_SPRITE;
    oamInit(&oamSub, SpriteMapping_1D_32, false);
    gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    swiCopy(spritePal, SPRITE_PALETTE, spritePalLen/2);
    swiCopy(spriteTiles, gfx, spriteTilesLen/2);
	SPRITE_PALETTE[0]=ARGB16(1,31,31,31);
	SPRITE_PALETTE[1]=ARGB16(1,31,31,31);
}


void updateCrono() {
	if (timer_running) {
		msec += 1000/60;
		if (msec>=1000) {
			msec -= 1000;
			sec++;
			if (sec>=60) {
				sec -= 60;
				min++;
			}
		}
	}
}


int isBetterTime(int curr_min, int curr_sec, int curr_msec, int file_min, int file_sec, int file_msec) {
	if (curr_min < file_min) return 1;
	if (curr_min == file_min && curr_sec < file_sec) return 1;
	if (curr_min == file_min && curr_sec == file_sec && curr_msec < file_msec) return 1;
	return 0;
}


// Funzione per salvare il miglior tempo su result.txt
void saveBestTime(int min, int sec, int msec) {
	FILE *file = fopen("/result.txt", "w");
	if (file) {
		fprintf(file, "%02d:%02d.%03d\n", min, sec, msec);
		fclose(file);
	} else {
		iprintf("Error: Unable to write to result.txt\n");
	}
}


// Funzione per leggere il tempo da result.txt
int loadBestTime(int *file_min, int *file_sec, int *file_msec) {
	FILE *file = fopen("/result.txt", "r");
	if (file) {
		int result = fscanf(file, "%d:%d.%d", file_min, file_sec, file_msec);
		fclose(file);
		return result == 3; // True if successfully read three values
	}
	return 0; // No file or unable to read
}





int main(void) {

	fatInitDefault();
	consoleDemoInit();

	initChronoDisp_Main(); // initialize the main screen
	initSudokuDisp_Sub(); // initialize the sub screen

	uint16_t currentColor = GREEN; // Colore iniziale
	changeColorDisp_Main(GREEN);
	min = sec = msec = 0;
	newy = 169;

	// Configura il TIMER_0
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024;
	TIMER_DATA(0) = 0;

	// Initialize the sudoku
	sudokuInit(BG_MAP_RAM_SUB(25), staticMatrix);


	int file_min = 99, file_sec = 59, file_msec = 999; // Uso valori alti per simulare il "peggior tempo"
	int best_time_loaded = loadBestTime(&file_min, &file_sec, &file_msec);
	if (best_time_loaded == 0) { // Se il file non esiste o non contiene un tempo valido
		saveBestTime(99, 59, 999); // Inizializza il file con un tempo "infinito"
	}

	// Initialize the audio
	Audio_Init();
	// Create the background continous music
	Audio_PlayMusic();

	// Initialize the sprite
	configureSprite();
	// Set the sprite initial position
	int x = 216;
	int y = 9;
	newy = 169;


	while (1) {
		y = displaceSprite(newy, y);

		oamSet(&oamSub,0,x,y,0,0,SpriteSize_16x16,SpriteColorFormat_256Color,gfx,-1,false,false,false,false,false);
		oamUpdate(&oamSub);


		scanKeys();
		int keys = keysDown();
		
		// If the key A is pressed the game stops
		if (keys & KEY_X) {
			if (timer_running != 0) {
				timer_running = 0;

			}
		}
		

		// If the key X is pressed the game restart
		if (keys & KEY_A) {
			if (timer_running != 1) {
				timer_running = 1;

			}
		}

		// If key UP is pressed the best time is reset to the maximum time possible
		if (keys & KEY_UP) {
			file_min = 99;
			file_sec = 59;
			file_msec = 999;
			saveBestTime(file_min, file_sec, file_msec);
			changeColorDisp_Main(GREEN);
		}


		if (timer_running) {
			handleTouchPad();
		}

		// Wait for the screen to refresh
		swiWaitForVBlank(); 

		// Update and print the crono
		updateCrono();
		updateChronoDisp_Main(min, sec, msec);


		//Change the color of the timer if we passed the record
		if (currentColor != RED) {
			if (!isBetterTime(min, sec, msec, file_min, file_sec, file_msec)) {
				changeColorDisp_Main(RED);
				currentColor = RED;
			}
		}
		

		// The game is finished with the correct solution
		if (memcmp(dynamicMatrix, answerMatrix, sizeof(dynamicMatrix)) == 0) {
			timer_running = 0; // Ferma il cronometro

			// Confronta il tempo corrente con il tempo nel file
			if (isBetterTime(min, sec, msec, file_min, file_sec, file_msec)) {
				saveBestTime(min, sec, msec); // Salva il nuovo tempo migliore
			}
		}	
	}
}