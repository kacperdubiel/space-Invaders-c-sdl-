#include "init.h"

#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "enemy.h"

int unsigned frames_counter = 0;
int unsigned current_time = 0;
int unsigned last_time = 0;
int program = 1;
int programStatus = 1;
int selectedOption = 1;
int instructionTexturesCreated = 0;

int deadTimer = DEAD_TIMER;
int gameEnd = 0;

int bossFlag = 0;
int bossLifeFlag = 0;
int bossBoxFlag = 0;
int bossPointsTimer = BOSS_BONUS_POINTS_TIME;

int lifeBonusPoints = 0;
int bossBonusPoints = 0;

SDL_Window* win;
SDL_Renderer* rend;

char textBuffer[70];

int init(){
    // inicjalizacja SDL
    if(SDL_Init( SDL_INIT_VIDEO ) != 0){
        printf(">> Error! << - %s\n",SDL_GetError());
        return 1;
    }

    // inicjalizacja TTF
    if(TTF_Init() != 0){
        return 1;
    }

    //stworzenie okna
    win = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if(!win){
        printf(">> Error! << - %s\n",SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //stworzenie rendera
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    rend = SDL_CreateRenderer(win, -1, render_flags);
    if(!rend){
        printf(">> Error! << - %s\n",SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    return 0;

}
