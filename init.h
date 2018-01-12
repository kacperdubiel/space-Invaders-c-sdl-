#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#define WINDOW_NAME   "Space Invaders"
#define WINDOW_WIDTH  (960)
#define WINDOW_HEIGHT (720)
#define FPS           (50)

#define FIRST_STAGE 1-1
#define LAST_STAGE  8

#define DEAD_TIMER 150

#include <SDL.h>

extern int unsigned frames_counter, current_time, last_time;
extern int program;
extern int programStatus;
extern int selectedOption;

extern int deadTimer;
extern int gameEnd;

extern int bossFlag;
extern int bossLifeFlag;
extern int bossBoxFlag;
extern int bossPointsTimer;

extern int lifeBonusPoints;
extern int bossBonusPoints;

extern char textBuffer[70];

extern SDL_Window* win;
extern SDL_Renderer* rend;

int init();

#endif // INIT_H_INCLUDED
