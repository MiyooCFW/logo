#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "assets.h"
#include "logo.h"

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
#define ENDDELAY 300

//speed at which the logo moves (unit: pixels per frame)
#define ANIMSPEED 1

//---------------------------------------------------//

int main(int argc, char* argv[]) {
	memset(&argint, 0, sizeof(argint));
	if (argc == 1) {
		args = false;
	} else if (argc == 4) {
		for (size_t i = 1; i < argc; ++i) {
			argint[i] = strtof(argv[i], &str);
			if (*str != '\0') {
				fprintf(stderr, "Invalid argument: %s\n", argv[i]);
				if (args) args = false;
			}
		}
	} else {
		std::cout << "Provided bad number of arguments with: ";
		for (int i = 1; i < argc; i++) {
			std::cout << argv[i] << ' ';
		}
		std::cout << '\n';
		args = false;
	}

	if (args) {
		animdel = argint[1] * 60;
		enddel = argint[2] * 60;
		animspeed = argint[3];
	} else {
		animdel = ANIMDELAY;
		enddel = ENDDELAY;
		animspeed = ANIMSPEED;
		printf("Usage: %s <logo_start[sec]> <logo_ending[sec]> <logo_speed[ppf]>\nRunning default setup: %s", argv[0], argv[0]);
		if ((int)animdel%60 != 0 || (int)enddel%60 != 0)
			printf(" %i.%i %i.%i %i\n", (int)animdel/60, (int)animdel%60, (int)enddel/60, (int)enddel%60, animspeed);
		else
			printf(" %i %i %i\n", (int)animdel/60, (int)enddel/60, animspeed);
	}

	homepath = getenv("HOME");
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

	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_SWSURFACE);
	SDL_ShowCursor(SDL_DISABLE);

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

	if (FILE *f = fopen(logobg_path, "r")) {
		logobg = IMG_Load(logobg_path);
		blitbg = true;
	} else {
		printf("Didn't find PNG in %s - using default background\n", logobg_path);
	}

	color = SDL_MapRGB(screen->format, R, G, B);
	dest_y = (int)(screen->h - logoimg->h) / 2;
	curr_time = old_time = SDL_GetTicks();
	for (int i = 0 - logoimg->h - animdel; i <= dest_y && (!quit_app); i = i + animspeed) {
		input_poll();
		rect.x = 0;
		rect.y = 0;
		rect.w = screen->w;
		rect.h = screen->h;
		if (blitbg) {
			SDL_BlitSurface(logobg, NULL, screen, &rect);
		} else {
			SDL_FillRect(screen, &rect, color);
		}
		dstrect.x = (int)(screen->w - logoimg->w) / 2;
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
	
	while(Mix_Playing(-1) && (!quit_app)) {
		input_poll();
	}
	
	for (int j = 0 ; j < (int)(sqrt(2+8*enddel*10)-1)/2 && (!quit_app); j++){
		input_poll();
		SDL_Delay(j);
	}

	quit();

	return 0;
}

void quit() {
	SDL_FreeRW(RWops);
	SDL_FreeRW(RWops2);
	SDL_FreeSurface(logoimg);
	SDL_FreeSurface(screen);
	Mix_FreeChunk(logosound);
	Mix_CloseAudio();
	SDL_Quit();
}

void input_poll() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN: case SDL_QUIT:
				quit_app = true;
				break;	
		}
	}
}
