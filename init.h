#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <SDL.h>

#define WINDOW_NAME   "Space Invaders"
#define WINDOW_WIDTH  (960)
#define WINDOW_HEIGHT (720)
#define FPS           (60)

#define FISRT_STAGE 1
#define DEAD_TIMER 90

extern int unsigned global_timer;
extern int program;
extern int programStatus;
extern int selectedOption;

extern int deadTimer;
extern int gameEnd;

extern char scoreBuffer[70];

extern SDL_Window* win;
extern SDL_Renderer* rend;

int init();

#endif // INIT_H_INCLUDED
