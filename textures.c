#include "textures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init.h"
#include "enemy.h"
#include "bullet.h"
#include "boxes.h"
#include "player.h"

Animation *animations[MAX_ANIMATIONS];
AnimationType animationTypes[ANIMATION_TYPES];

float bg_scrolling_speed = 0.5;

int heartX = -300;


//kolor czcionki
SDL_Color colorBlack = {0, 0, 0};
SDL_Color colorWhite = {255, 255, 255};
SDL_Color colorGold = {255, 222, 0};

SDL_Texture* bgTexture, *hudTexture, *heartTexture, *tempTexture, *scoreTextTexture;
SDL_Texture* playerTexture, *playerBulletTexture;
SDL_Texture* animationTexture;
SDL_Texture* startTextTexture, *selectedStartTextTexture, *helpTextTexture, *selectedHelpTextTexture, *endTextTexture, *selectedEndTextTexture;
SDL_Texture* pauseTextTexture, *yesTextTexture, *selectedYesTextTexture, *noTextTexture, *selectedNoTextTexture;
SDL_Texture* iconFreezeTexture, *iconFreezeCdTexture, *freezeCdTexture, *freezeHtkTexture;
SDL_Texture* textTexture;
SDL_Texture* arrowsTexture, *qTexture, *wTexture, *eTexture;

SDL_Rect bgRect, hudRect1, hudRect2, heartRect, scoreTextRect;
SDL_Rect playerRect, playerBulletRect, animationRect, frame;
SDL_Rect startTextRect, helpTextRect, endTextRect;
SDL_Rect pauseTextRect, yesTextRect, noTextRect;
SDL_Rect iconFreezeRect, freezeCdRect, freezeHtkRect;
SDL_Rect boxRect, textRect;
SDL_Rect arrowsRect, keyRect;

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

SDL_Texture* createTextTexture(TTF_Font* font, char *text, SDL_Color fontColor, int outline, TTF_Font* outlineFont, SDL_Color outlineColor){
    TTF_SetFontOutline(outlineFont, outline);

    SDL_Surface* textSur = TTF_RenderUTF8_Blended(outlineFont, text, outlineColor);
    if(!textSur) surfaceError(rend, win);
    SDL_Surface* textSur2 = TTF_RenderUTF8_Blended(font, text, fontColor);
    if(!textSur2) surfaceError(rend, win);
    SDL_Rect textRect = {outline, outline, textSur2->w, textSur2->h};

    SDL_SetSurfaceBlendMode(textSur2, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(textSur2, NULL, textSur, &textRect);
    SDL_Texture* createdTexture = SDL_CreateTextureFromSurface(rend,textSur);

    SDL_FreeSurface(textSur2);
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
        SDL_QueryTexture(bulletTypes[i].texture, NULL, NULL, &bulletTypes[i].rect.w, &bulletTypes[i].rect.h);
    }

    // <<-- WROGOWIE -->>
    for(int i=0; i<ENEMY_TYPES; i++){
        char pathBuffor[50] = "img/enemies/enemy";
        char intBuffor[2];
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,".png");

        tempTexture = createTexture(pathBuffor);
        enemyTypes[i].texture = tempTexture;
        SDL_QueryTexture(enemyTypes[i].texture, NULL, NULL, &enemyTypes[i].rect.w, &enemyTypes[i].rect.h);
    }

    // <<-- EKSPLOZJA -->>
    for(int i=0; i<ANIMATION_TYPES; i++){
        char pathBuffor[50] = "img/animations/animation";
        char intBuffor[2];
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,".png");

        tempTexture = createTexture(pathBuffor);
        animationTypes[i].texture = tempTexture;
        SDL_QueryTexture(animationTypes[i].texture, NULL, NULL, &animationTypes[i].textureW, &animationTypes[i].textureH);
        animationTypes[i].rect.w = animationTypes[i].textureW/animationTypes[i].verticalCount;
        animationTypes[i].rect.h = animationTypes[i].textureH/animationTypes[i].horizontalCount;
    }

    // <<-- GRACZ -->>
    playerTexture = createTexture("img/player.png");
    //pobierz wymiary gracza
    SDL_QueryTexture(playerTexture, NULL, NULL, &playerRect.w, &playerRect.h);

    // <<-- IKONY -->>
    //zamrażanie
    iconFreezeTexture = createTexture("img/icons/icon_freeze.png");
    iconFreezeCdTexture = createTexture("img/icons/icon_freeze_cd.png");
    SDL_QueryTexture(iconFreezeTexture, NULL, NULL, &iconFreezeRect.w, &iconFreezeRect.h);

    // <<-- SKRZYNKI -->>
    for(int i=0; i<BOX_TYPES; i++){
        char pathBuffor[50] = "img/boxes/box";
        char intBuffor[2];
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,".png");

        tempTexture = createTexture(pathBuffor);
        boxTypes[i].texture = tempTexture;

        tempTexture = createTexture("img/boxes/box_off.png");
        boxTypes[i].offTexture = tempTexture;
    }
    SDL_QueryTexture(boxTypes[0].texture, NULL, NULL, &boxRect.w, &boxRect.h);

    // <<-- INSTRUKCJA -->>
    arrowsTexture = createTexture("img/instruction/arrow_keys.png");
    SDL_QueryTexture(arrowsTexture, NULL, NULL, &arrowsRect.w, &arrowsRect.h);

    qTexture = createTexture("img/instruction/q.png");
    wTexture = createTexture("img/instruction/w.png");
    eTexture = createTexture("img/instruction/e.png");

    // <<-- NAPISY -->>
    //MENU
        //rozpocznij
        startTextTexture = createTextTexture(font72, "ROZPOCZNiJ", colorWhite, 0, font_outline72, colorBlack);
        selectedStartTextTexture = createTextTexture(font72, "ROZPOCZNiJ", colorGold, 0, font_outline72, colorBlack);
        SDL_QueryTexture(startTextTexture, NULL, NULL, &startTextRect.w, &startTextRect.h);
        startTextRect.x = -200;
        startTextRect.y = 150;

        //instrukcja
        helpTextTexture = createTextTexture(font72, "iNSTRUKCJA", colorWhite, 0, font_outline72, colorBlack);
        selectedHelpTextTexture = createTextTexture(font72, "iNSTRUKCJA", colorGold, 0, font_outline72, colorBlack);
        SDL_QueryTexture(helpTextTexture, NULL, NULL, &helpTextRect.w, &helpTextRect.h);
        helpTextRect.x = WINDOW_WIDTH+200;
        helpTextRect.y = 300;

        //wyjście
        endTextTexture = createTextTexture(font72, "WYJŚCiE", colorWhite, 0, font_outline72, colorBlack);
        selectedEndTextTexture = createTextTexture(font72, "WYJŚCiE", colorGold, 0, font_outline72, colorBlack);
        SDL_QueryTexture(endTextTexture, NULL, NULL, &endTextRect.w, &endTextRect.h);
        endTextRect.x = -800;
        endTextRect.y = 450;

    //GRA
        //freezeHtk
        freezeHtkTexture = createTextTexture(font30, "W", colorWhite, 2, font_outline30, colorBlack);
        SDL_QueryTexture(freezeHtkTexture, NULL, NULL, &freezeHtkRect.w, &freezeHtkRect.h);

    //PAUZA
        //potwierdzenie
        pauseTextTexture = createTextTexture(font45, "CZY NA PEWNO CHCESZ WYJŚĆ?", colorWhite, 0, font_outline45, colorBlack);
        SDL_QueryTexture(pauseTextTexture, NULL, NULL, &pauseTextRect.w, &pauseTextRect.h);
        pauseTextRect.x = WINDOW_WIDTH/2-pauseTextRect.w/2;
        pauseTextRect.y = 150;

        //tak
        yesTextTexture = createTextTexture(font60, "TAK", colorWhite, 0, font_outline60, colorBlack);
        selectedYesTextTexture = createTextTexture(font60, "TAK", colorGold, 0, font_outline60, colorBlack);
        SDL_QueryTexture(yesTextTexture, NULL, NULL, &yesTextRect.w, &yesTextRect.h);
        yesTextRect.x = (WINDOW_WIDTH/5)+yesTextRect.w/2;
        yesTextRect.y = 400;

        //nie
        noTextTexture = createTextTexture(font60, "NiE", colorWhite, 0, font_outline60, colorBlack);
        selectedNoTextTexture = createTextTexture(font60, "NiE", colorGold, 0, font_outline60, colorBlack);
        SDL_QueryTexture(noTextTexture, NULL, NULL, &noTextRect.w, &noTextRect.h);
        noTextRect.x = (WINDOW_WIDTH/5)*3;
        noTextRect.y = 400;

}

void collisionUpdate(){
    playerCollisionUpdate();
    enemyCollisionUpdate();
    bulletCollisionUpdate();
    boxCollisionUpdate();
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

void addAnimation(float x, float y, int angle, int type){
    int found;
    for(int i=0;i<MAX_ANIMATIONS;i++){
        if(animations[i]==NULL){
            found = i;
            break;
        }
    }
    if(found>=0){
        animations[found] = malloc(sizeof(Animation));
        animations[found]->x = x-animationTypes[type].rect.w/2;
        animations[found]->y = y-animationTypes[type].rect.h/2;
        animations[found]->angle = angle;
        animations[found]->animationType = type;
        animations[found]->frameRect.x = 0;
        animations[found]->frameRect.y = 0;
        animations[found]->frameRect.w = animationTypes[type].rect.w;
        animations[found]->frameRect.h = animationTypes[type].rect.h;
    }
}

void removeAnimation(int i){
    if(animations[i]){
        free(animations[i]);
        animations[i]=NULL;
    }
}

void animationInit(int type, int verticalCount, int horizontalCount){
    animationTypes[type].verticalCount = verticalCount;
    animationTypes[type].horizontalCount = horizontalCount;
}

void allAnimationInit(){
    animationInit(0, 10, 12);
    animationInit(1, 7, 8);
    animationInit(2, 11, 12);
    animationInit(3, 14, 15);
    animationInit(4, 12, 13);
    animationInit(5, 9, 10);
}

void cleanMap(){
    for(int i=0;i<MAX_BULLETS;i++){
        if(bullets[i]) removeBullet(i);
    }
    for(int i=0;i<MAX_ENEMIES;i++){
        if(enemies[i]) removeEnemy(i);
    }
}
