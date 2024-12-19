//main.c
#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "chrono_display.h"
#include "Template.h"
#include "controls.h"
#include "define_grid.h"
#include "tiles.h"
//#include "globals.h" 

//########################TIMER###########################


int min, sec, msec;
int timer_running = 1;

void ISR_TIMER0()
{
	if (timer_running) { 
		msec = (msec + 1) % 1000;
		if (msec == 0) {
			sec = (sec + 1) % 60;
			if (sec == 0)
				min++;
		}
	}
}

void ISR_VBlank()
{
	updateChronoDisp_Main(min, sec, msec);
}

//########################Tameplate_SUB###########################
void P_Graphics_setup() {
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG0_ACTIVE;  //Qui va aggiunto DISPLAY_BG2_ACTIVE ma se lo faccio mi escono cose strane, devo correggere
}

u16* numbers;
void P_Map16x16_configureBG0_Sub() {
    // Configura BG2 sulla sub screen
    BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(25) | BG_TILE_BASE(4);

    // Configura la trasformazione affina come identità (nessuna trasformazione)
    REG_BG2PA_SUB = 128; 
    REG_BG2PB_SUB = 0;  
    REG_BG2PC_SUB = 0;  
    REG_BG2PD_SUB = 128;

	// Configura i colori della palette
	u16 paletteWhite = {ARGB16(0, 0, 0, 0), ARGB16(1, 31, 31, 31)};  // White
	u16 paletteGrey = {ARGB16(0, 0, 0, 0), ARGB16(1, 25, 25, 25)};  // Grey

	dmaCopy(paletteWhite, &BG_PALETTE_SUB[128], 32);  // Copy first palette
	dmaCopy(paletteGrey, &BG_PALETTE_SUB[144] + 16, 32);  // Copy second palette, offset by 16 entries


	//Initialize pointer to the graphic memory
	numbers = (u16*)BG_MAP_RAM_SUB(25);
	dmaCopy(numbers, (u8*)BG_TILE_RAM(25), 64*10);
	// Set background offset to position the top-left corner of the grid
	REG_BG0HOFS = -34; // x
	REG_BG0VOFS = -28; // y

}

void P_Map16x16_configureBG2_Sub() {

	//Configure BG 2 Sub.
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;

    //Affine Marix Transformation
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;

    swiCopy(TemplatePal, BG_PALETTE_SUB, TemplatePalLen/2);
    swiCopy(TemplateBitmap, BG_GFX_SUB, TemplateBitmapLen/2);



}

void tiles_draw(){
    int i, j;

	for(i=0; i<32; i++){
		for(j=0; j<32; j++){
			if(i%2 == 0 && j%2 == 0 && i<17 && j<17){ // pick only even value for i and j
			// this because between each tile there is an empty tile
				numbers[i*32+j]=staticMatrix[i/2][j/2]; // gives the index of the tile that must be applied
			}
			else{
				numbers[i*32+j]=0;
			}
		}
	}
}




int main(void) {
	consoleDemoInit();

	initChronoDisp_Main();
	changeColorDisp_Main(WHITE);
	irqInit();
	min = sec = msec = 0;

	// Configura il TIMER_0
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(0) = TIMER_FREQ_1024(1000);

	// Configura e abilita l'IRQ per TIMER_0
	irqSet(IRQ_TIMER0, &ISR_TIMER0);
	irqEnable(IRQ_TIMER0);

	// Configura e abilita l'IRQ per VBLANK
	irqSet(IRQ_VBLANK, &ISR_VBlank);
	irqEnable(IRQ_VBLANK);

	P_Graphics_setup();
	P_Map16x16_configureBG2_Sub();
	P_Map16x16_configureBG0_Sub();


	initializeMatrices();  // Set up the matrices for game start
	tiles_draw();			// draw the starting position of the sudoku
	state = STATE_DELETE;
	while (1) {

		scanKeys();
		int keys = keysDown();
		// If the key A is pressed the game stops
		if (keys & KEY_A) {
			if (timer_running != 0) {
				timer_running = 0;
				printf("\nTime = %02d:%02d.%03d\n", min, sec, msec);
			}
			
		}
		
		// If the key X is pressed the game restart
		else if (keys & KEY_X) {
			if (timer_running != 1) {
				timer_running = 1;
				printf("\nTime = %02d:%02d.%03d\n", min, sec, msec);
			}
		}
		handleTouchPad();
		swiWaitForVBlank(); 
	}
}