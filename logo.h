#ifndef LOGO_H_
#define LOGO_H_
SDL_RWops *RWops;
SDL_Surface *logoimg;
SDL_Surface *screen;
SDL_RWops *RWops2;
SDL_Rect rect;
SDL_Rect dstrect;
SDL_Event event;
SDL_Surface* logobg;
Mix_Chunk *logosound;

bool quit_app = false;

bool args = true;
int animdel, enddel;
int animspeed;
char* str;
float argint[3];

void quit();
void input_poll();

char* homepath;
char logoimg_path[256], logosound_path[256], logobg_path[256];

int dest_y;
uint32_t curr_time, old_time;
uint32_t color;
bool blitbg = false;
#endif /*LOGO_H_*/
