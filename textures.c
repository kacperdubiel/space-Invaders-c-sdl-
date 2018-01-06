#include "textures.h"

#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"

float bg_scrolling_speed = 0.5;

int heartX = -300;
//czcionka
TTF_Font* font;
//kolor czcionki
SDL_Color colorBlack = {0, 0, 0};
SDL_Color colorWhite = {255, 255, 255};
SDL_Color colorGold = {255, 222, 0};

SDL_Texture* bgTexture, *hudTexture, *heartTexture, *tempTexture, *scoreTextTexture;
SDL_Texture* playerTexture, *playerBulletTexture;
SDL_Texture* explosionTexture;
SDL_Texture* startTextTexture, *selectedStartTextTexture, *helpTextTexture, *selectedHelpTextTexture, *endTextTexture, *selectedEndTextTexture;
SDL_Texture* pauseTextTexture, *yesTextTexture, *selectedYesTextTexture, *noTextTexture, *selectedNoTextTexture;

SDL_Rect bgRect, hudRect1, hudRect2, heartRect, scoreTextRect;
SDL_Rect playerRect, playerBulletRect, explosionRect, frame;
SDL_Rect startTextRect, helpTextRect, endTextRect;
SDL_Rect pauseTextRect, yesTextRect, noTextRect;

SDL_Rect hudRect1 = {0,WINDOW_HEIGHT,WINDOW_WIDTH,HUD_HEIGHT};
SDL_Rect hudRect2 = {0,-45,WINDOW_WIDTH,HUD_HEIGHT};
SDL_Rect heartRect = {0,WINDOW_HEIGHT-35,35,30};

void surfaceError(){
    printf(">> Error! << - %s\n",SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

SDL_Texture* createTexture(char *path){
    //załadowanie obrazka do pamięci
    SDL_Surface* imgSur = IMG_Load(path);
    if(!imgSur) surfaceError(rend, win);
    //załadowanie danych obrazka do pamięci karty graficznej
    SDL_Texture* createdTexture = SDL_CreateTextureFromSurface(rend, imgSur);
    SDL_FreeSurface(imgSur);
    return createdTexture;
}

SDL_Texture* createTextTexture(TTF_Font* font, char *text, SDL_Color color){
    //załadowanie obrazka do pamięci
    SDL_Surface* textSur = TTF_RenderUTF8_Solid(font, text, color);
    if(!textSur) surfaceError(rend, win);
    //załadowanie danych obrazka do pamięci karty graficznej
    SDL_Texture* createdTexture = SDL_CreateTextureFromSurface(rend,textSur);
    SDL_FreeSurface(textSur);
    return createdTexture;
}

void createTexturesAndRects(){
    // <<-- TŁO -->>
    bgTexture = createTexture("img/bg.png");
    SDL_QueryTexture(bgTexture, NULL, NULL, &bgRect.w, &bgRect.h);
    bgRect.y=WINDOW_HEIGHT-bgRect.h;

    // <<-- HUD -->>
    hudTexture = createTexture("img/hud.jpg");

    // <<-- SERCA -->>
    heartTexture = createTexture("img/heart.png");

    // <<-- POCISKI -->>
    for(int i=0; i<BULLET_TYPES; i++){
        char pathBuffor[50] = "img/bullets/bullet";
        char intBuffor[2];
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,".png");

        tempTexture = createTexture(pathBuffor);
        bulletTypes[i].texture = tempTexture;
        SDL_QueryTexture(bulletTypes[i].texture, NULL, NULL, &bulletTypes[i].bulletRect.w, &bulletTypes[i].bulletRect.h);
    }

    // <<-- WROGOWIE -->>
    for(int i=0; i<ENEMY_TYPES; i++){
        char pathBuffor[50] = "img/enemies/enemy";
        char intBuffor[2];
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,".png");

        tempTexture = createTexture(pathBuffor);
        enemyTypes[i].texture = tempTexture;
        SDL_QueryTexture(enemyTypes[i].texture, NULL, NULL, &enemyTypes[i].enemyRect.w, &enemyTypes[i].enemyRect.h);
    }

    // <<-- EKSPLOZJA -->>
    explosionTexture = createTexture("img/explosion.png");
    SDL_QueryTexture(explosionTexture, NULL, NULL, &explosionRect.w, &explosionRect.h);
    explosionRect.w = frame.w = explosionRect.w/5;
    explosionRect.h = frame.h = explosionRect.h/6;

    // <<-- GRACZ -->>
    playerTexture = createTexture("img/player.png");

    //pobierz wymiary gracza
    SDL_QueryTexture(playerTexture, NULL, NULL, &playerRect.w, &playerRect.h);

    // <<-- NAPISY -->>
    font = TTF_OpenFont("font.ttf", 72);
    //rozpocznij
    startTextTexture = createTextTexture(font, "ROZPOCZNiJ", colorWhite);
    selectedStartTextTexture = createTextTexture(font, "ROZPOCZNiJ", colorGold);
    SDL_QueryTexture(startTextTexture, NULL, NULL, &startTextRect.w, &startTextRect.h);
    startTextRect.x = -200;
    startTextRect.y = 150;

    //instrukcja
    helpTextTexture = createTextTexture(font, "iNSTRUKCJA", colorWhite);
    selectedHelpTextTexture = createTextTexture(font, "iNSTRUKCJA", colorGold);
    SDL_QueryTexture(helpTextTexture, NULL, NULL, &helpTextRect.w, &helpTextRect.h);
    helpTextRect.x = WINDOW_WIDTH+200;
    helpTextRect.y = 300;

    //wyjście
    endTextTexture = createTextTexture(font, "WYJŚCiE", colorWhite);
    selectedEndTextTexture = createTextTexture(font, "WYJŚCiE", colorGold);
    SDL_QueryTexture(endTextTexture, NULL, NULL, &endTextRect.w, &endTextRect.h);
    endTextRect.x = -800;
    endTextRect.y = 450;

    //pauza
    font = TTF_OpenFont("font.ttf", 46);
    pauseTextTexture = createTextTexture(font, "CZY NA PEWNO CHCESZ WYJŚĆ?", colorWhite);
    SDL_QueryTexture(pauseTextTexture, NULL, NULL, &pauseTextRect.w, &pauseTextRect.h);
    pauseTextRect.x = WINDOW_WIDTH/2-pauseTextRect.w/2;
    pauseTextRect.y = 150;

    //tak
    font = TTF_OpenFont("font.ttf", 60);
    yesTextTexture = createTextTexture(font, "TAK", colorWhite);
    selectedYesTextTexture = createTextTexture(font, "TAK", colorGold);
    SDL_QueryTexture(yesTextTexture, NULL, NULL, &yesTextRect.w, &yesTextRect.h);
    yesTextRect.x = (WINDOW_WIDTH/5)+yesTextRect.w/2;
    yesTextRect.y = 400;

    //nie
    noTextTexture = createTextTexture(font, "NiE", colorWhite);
    selectedNoTextTexture = createTextTexture(font, "NiE", colorGold);
    SDL_QueryTexture(noTextTexture, NULL, NULL, &noTextRect.w, &noTextRect.h);
    noTextRect.x = (WINDOW_WIDTH/5)*3;
    noTextRect.y = 400;

    TTF_CloseFont(font);
}

void collisionUpdate(){
    playerCollisionUpdate();
    enemyCollisionUpdate();
    bulletCollisionUpdate();
}

int collisionCheck(int n, SDL_Rect collBoxes[n], int m, SDL_Rect collBoxes2[m]){
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(collBoxes[i].x+collBoxes[i].w >= collBoxes2[j].x &&
               collBoxes[i].x <= collBoxes2[j].x+collBoxes2[j].w &&
               collBoxes[i].y+collBoxes[i].h >= collBoxes2[j].y &&
               collBoxes[i].y <= collBoxes2[j].y+collBoxes2[j].h)
                    return 1;
        }
    }
    return 0;
}

void cleanMap(){
    for(int i=0;i<MAX_BULLETS;i++){
        if(allBullets[i]) removeBullet(i);
    }
    for(int i=0;i<MAX_ENEMIES;i++){
        if(enemies[i]) removeEnemy(i);
    }
    for(int i=0;i<MAX_EXPLOSIONS;i++){
        if(explosions[i]) explosions[i]->frame=explosions[i]->lastFrame;
    }
}
