#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include "assets.h"

//------------------- PARAMETERS --------------------//

// screen size
#define WIDTH  320
#define HEIGHT 240

// background color (RGB)
#define R 96
#define G 148
#define B 8

// delay until the logo starts being visible (unit: frames) (60 frames = 1 sec)
#define ANIMDELAY 60

// time from the moment the logo stops moving and sound is played until the logo app closes (unit: frames) (60 frames = 1 sec)
#define ENDDELAY 3000

//speed at which the logo moves (unit: pixels per frame)
#define ANIMSPEED 1

//---------------------------------------------------//

void quit() {
	Mix_CloseAudio();
	SDL_Quit();
}

void exit_event() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				quit();
				break;
		}
	}
}

int main(int argc, char* argv[]) {
	char* homepath = getenv("HOME");
	char logoimg_path[256], logosound_path[256], logobg_path[256];
	if (homepath == NULL) {
		printf("$HOME has not been defined in env");
	} else {
		sprintf(logoimg_path, "%s/logo.png", getenv("HOME"));
		sprintf(logosound_path, "%s/logo.wav", getenv("HOME"));
		sprintf(logobg_path, "%s/logobg.png", getenv("HOME"));
	}
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}
	SDL_Surface *screen;

	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_SWSURFACE);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_RWops *RWops;
	SDL_Surface *logoimg;

	if (FILE *f = fopen(logoimg_path, "r")) {
	 	RWops = SDL_RWFromFile(logoimg_path, "rb");
	} else {
		printf("Didn't find PNG in %s - using default logoimg\n", logoimg_path);
#ifdef VERSION_POCKETGO
		RWops = SDL_RWFromConstMem(png_logo_pocketgo, sizeof(png_logo_pocketgo));
#elif VERSION_POWKIDDY
		RWops = SDL_RWFromConstMem(png_logo_powkiddy, sizeof(png_logo_powkiddy));
#elif VERSION_BITTBOY
		RWops = SDL_RWFromConstMem(png_logo_bittboy, sizeof(png_logo_bittboy));
#elif VERSION_GENERIC
		RWops = SDL_RWFromConstMem(png_logo_generic, sizeof(png_logo_generic));
#else
		RWops = SDL_RWFromConstMem(png_logo_miyoo, sizeof(png_logo_miyoo));
#endif
	}
	logoimg = IMG_LoadPNG_RW(RWops);
	if (!logoimg) {
		printf("Error loading logo: %s\n", IMG_GetError());
		return -1;
	}

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024);
	Mix_AllocateChannels(2);
	SDL_RWops *RWops2;
	Mix_Chunk *logosound;

	if (FILE *f = fopen(logosound_path, "r")) {
		RWops2 = SDL_RWFromFile(logosound_path, "rb");
	} else {
		printf("Didn't find WAV in %s - using default logosound\n", logosound_path);
		RWops2 = SDL_RWFromConstMem(wav_logosound, sizeof(wav_logosound));
	}
	logosound = Mix_LoadWAV_RW(RWops2, 0);
	if (!logosound) {
		printf("Error loading sound: %s\n", Mix_GetError());
		return -1;
	}

	int dest_y = (screen->h - logoimg->h) / 2;
	uint32_t curr_time = SDL_GetTicks();
	uint32_t old_time = curr_time;
	uint32_t color = SDL_MapRGB(screen->format, R, G, B);
	bool blitbg = false;
	SDL_Rect rect;
	SDL_Rect dstrect;
	SDL_Event event;
	SDL_Surface* logobg;
	if (FILE *f = fopen(logobg_path, "r")) {
		logobg = IMG_Load(logobg_path);
		blitbg = true;
	} else {
		printf("Didn't find PNG in %s - using default background\n", logobg_path);
	}
	for (int i = 0 - logoimg->h - ANIMDELAY; i <= dest_y; i = i + ANIMSPEED) {
		exit_event();
		rect.x = 0;
		rect.y = 0;
		rect.w = screen->w;
		rect.h = screen->h;
		if (blitbg) {
			SDL_BlitSurface(logobg, NULL, screen, &rect);
		} else {
			SDL_FillRect(screen, &rect, color);
		}
		dstrect.x = (screen->w - logoimg->w) / 2;
		dstrect.y = i;
		dstrect.w = logoimg->w;
		dstrect.h = logoimg->h;
		SDL_BlitSurface(logoimg, NULL, screen, &dstrect);
		if (i == dest_y) {
			Mix_PlayChannel(-1, logosound, 0);
		}
		while (curr_time < old_time + 16) {
			curr_time = SDL_GetTicks();
		}
		old_time = curr_time;
		SDL_Flip(screen);
	}
	
	while(Mix_Playing(-1)) {
		exit_event();
	}
	
	for (int j = 0 ; j < (sqrt(2+8*ENDDELAY)-1)/2; j++){
		SDL_Delay(j);
		exit_event();
	}

	SDL_FreeRW(RWops);
	SDL_FreeRW(RWops2);
	SDL_FreeSurface(logoimg);
	Mix_FreeChunk(logosound);
	quit();

	return 0;
}
