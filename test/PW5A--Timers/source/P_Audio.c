#include "P_Audio.h"

void Audio_Init()
{
	//Init the sound library
	//Parte della libreria maxmod e serve per inizializzare il sistema audio. 
	//Specifica che il sistema utilizzerà la memoria predefinita per accedere ai dati audio
	//La funzione accetta come parametro un puntatore all'indirizzo del banco sonoro (soundbank), che contiene le definizioni delle tracce, effetti sonori.
	//soundbank_bin è un array binario generato da un tool (come il convertitore di dati audio fornito con maxmod).
	//(mm_addr) è un cast al tipo mm_addr, che è definito nella libreria maxmod,
	//assicura che l'indirizzo passato sia interpretato correttamente come un puntatore valido per maxmod.
	//La riga dice al sistema audio di maxmod di caricare il banco sonoro a partire dall'indirizzo specificato da soundbank_bin
	mmInitDefaultMem((mm_addr)soundbank_bin);

	//Load module
	//Si trova in soundbank.h creato al momento della compilazione da FlatOutLies.mod
	//MOD_FLATOUTLIES è un 32-bit index
	mmLoad(MOD_FLATOUTLIES);

	//Load effect
	//Si trova in soundbank.h creato al momento della compilazione da swish.wav e clunk.wav
	mmLoadEffect(SFX_SWISH);
	mmLoadEffect(SFX_CLUNK);
}

void Audio_PlaySoundEX( int i )
{
	//Declare a sound effect
	//Struttura di tipo mm_sound_effect per configurare e personalizzare la riproduzione di un effetto sonoro.
	mm_sound_effect sound;

	//Set the id of the sound effect with the input parameter
	//Determina quale effetto sonoro riprodurre (es. SFX_SWISH o SFX_CLUNK).
	sound.id = i;

	//Set the rate to the default one (1024)
	//Controlla la velocità di riproduzione
	sound.rate = 1024;
	
	//Set the volume to the maximum (range 0...255)
	sound.volume = 255;
	
	//Set the panning depending on the effect (0-left....255-right)
	//Controlla la posizione stereo dell'audio
	if(i == SFX_SWISH)
		sound.panning = 0;
	if(i == SFX_CLUNK)
		sound.panning = 255;

	//Play the effect using the sound structure
	mmEffectEx(&sound);
}

void Audio_PlaySound( int sound )
{
	//Play the effect
	mmEffect(sound);
}

void Audio_PlayMusic()
{
	//Start playing music in a loop
	mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP); // si poteva usare anche MM_PLAY_ONCE
	//Set module volume to 512 (range 0...1024) using the function mmSetVolume(...)
	mmSetModuleVolume(512);
}
