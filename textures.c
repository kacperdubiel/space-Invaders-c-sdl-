#include "textures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

float bg_scrolling_speed = 0.5;

Animation *animations[MAX_ANIMATIONS];
AnimationType animationTypes[ANIMATION_TYPES];
Texture menuTextTextures[4];
Texture pauseTextTextures[3];
Texture instructionTextures[12];
Texture rankingTextTextures[4+RANKING_TOP];
Texture cooldownsTextTextures[BOX_TYPES];
Texture endTextTextures[10];
Texture hotkeysTextTextures[3];
Texture iconTextures[3];
Texture spellsCdTextures[3];

int heartX = -300;

//kolor czcionki
SDL_Color colorBlack = {0, 0, 0};
SDL_Color colorWhite = {255, 255, 255};
SDL_Color colorGold  = {255, 222, 0};

SDL_Texture* bgTexture, *hudTexture, *heartTexture, *tempTexture, *scoreTextTexture;
SDL_Texture* playerTexture, *playerBulletTexture;
SDL_Texture* animationTexture;
SDL_Texture* textTexture;
SDL_Texture* lifebarBackground, *lifebar, *lifebarPercent;

SDL_Rect bgRect, hudRect1, hudRect2, heartRect, scoreTextRect;
SDL_Rect playerRect, playerBulletRect, animationRect, frame;
SDL_Rect boxRect, boxRect2, textRect;
SDL_Rect lifebarBackgroundRect, lifebarRect, lifebarPercentRect;

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
    SDL_Surface* textSur2 = TTF_RenderUTF8_Blended(font, text, fontColor);
    SDL_Rect textRect = {outline, outline, textSur2->w, textSur2->h};

    SDL_SetSurfaceBlendMode(textSur2, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(textSur2, NULL, textSur, &textRect);
    SDL_Texture* createdTexture = SDL_CreateTextureFromSurface(rend,textSur);

    SDL_FreeSurface(textSur2);
    SDL_FreeSurface(textSur);
    return createdTexture;
}

void createTexturesAndRects(){
    //czcionki
    font85 = TTF_OpenFont("font.ttf", 85); font_outline85 = TTF_OpenFont("font.ttf", 85);
    font72 = TTF_OpenFont("font.ttf", 72); font_outline72 = TTF_OpenFont("font.ttf", 72);
    font60 = TTF_OpenFont("font.ttf", 60); font_outline60 = TTF_OpenFont("font.ttf", 60);
    font50 = TTF_OpenFont("font.ttf", 50); font_outline50 = TTF_OpenFont("font.ttf", 50);
    font45 = TTF_OpenFont("font.ttf", 45); font_outline45 = TTF_OpenFont("font.ttf", 45);
    font30 = TTF_OpenFont("font.ttf", 30); font_outline30 = TTF_OpenFont("font.ttf", 30);
    font25 = TTF_OpenFont("font.ttf", 25); font_outline25 = TTF_OpenFont("font.ttf", 25);
    font20 = TTF_OpenFont("font.ttf", 20); font_outline20 = TTF_OpenFont("font.ttf", 20);
    font15 = TTF_OpenFont("font.ttf", 15); font_outline15 = TTF_OpenFont("font.ttf", 15);

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
        char pathBuffor[70] = "img/enemies/enemy";
        char intBuffor[3];
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,".png");

        tempTexture = createTexture(pathBuffor);
        enemyTypes[i].texture = tempTexture;
        SDL_QueryTexture(enemyTypes[i].texture, NULL, NULL, &enemyTypes[i].rect.w, &enemyTypes[i].rect.h);
    }

    // <<-- PASEK ŻYCIA BOSSA -->>
    lifebarBackground = createTexture("img/misc/lifebar_background.png");
    SDL_QueryTexture(lifebarBackground, NULL, NULL, &lifebarBackgroundRect.w, &lifebarBackgroundRect.h);
    lifebarBackgroundRect.x = WINDOW_WIDTH/2-lifebarBackgroundRect.w/2;
    lifebarBackgroundRect.y = HUD_HEIGHT/2-lifebarBackgroundRect.h/2;

    lifebar = createTexture("img/misc/lifebar.png");
    SDL_QueryTexture(lifebar, NULL, NULL, &lifebarRect.w, &lifebarRect.h);
    lifebarRect.x = WINDOW_WIDTH/2-lifebarRect.w/2;
    lifebarRect.y = HUD_HEIGHT/2-lifebarBackgroundRect.h/2;


    // <<-- GRACZ -->>
    playerTexture = createTexture("img/player.png");
    //pobierz wymiary gracza
    SDL_QueryTexture(playerTexture, NULL, NULL, &playerRect.w, &playerRect.h);

    // <<-- IKONY -->>
    for(int i=0; i<3; i++){
        char pathBuffor[50] = "img/icons/icon";
        char intBuffor[2];
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,".png");

        tempTexture = createTexture(pathBuffor);
        iconTextures[i].second_texture = tempTexture;

        strcpy(pathBuffor, "img/icons/icon");
        sprintf(intBuffor, "%d", i);
        strcat(pathBuffor,intBuffor);        strcat(pathBuffor,"off.png");

        tempTexture = createTexture(pathBuffor);
        iconTextures[i].texture = tempTexture;

        SDL_QueryTexture(iconTextures[i].texture, NULL, NULL, &iconTextures[i].rect.w, &iconTextures[i].rect.h);
    }


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

    // <<-- NAPISY -->>
    //MENU
        //rozpocznij
        menuTextTextures[0].texture = createTextTexture(font72, "ROZPOCZNiJ", colorWhite, 0, font_outline72, colorBlack);
        menuTextTextures[0].second_texture = createTextTexture(font72, "ROZPOCZNiJ", colorGold, 0, font_outline72, colorBlack);
        SDL_QueryTexture(menuTextTextures[0].texture, NULL, NULL, &menuTextTextures[0].rect.w, &menuTextTextures[0].rect.h);
        menuTextTextures[0].rect.x = -200;
        menuTextTextures[0].rect.y = 80;

        //instrukcja
        menuTextTextures[1].texture = createTextTexture(font72, "iNSTRUKCJA", colorWhite, 0, font_outline72, colorBlack);
        menuTextTextures[1].second_texture = createTextTexture(font72, "iNSTRUKCJA", colorGold, 0, font_outline72, colorBlack);
        SDL_QueryTexture(menuTextTextures[1].texture, NULL, NULL, &menuTextTextures[1].rect.w, &menuTextTextures[1].rect.h);
        menuTextTextures[1].rect.x = WINDOW_WIDTH+200;
        menuTextTextures[1].rect.y = 230;

        //ranking
        menuTextTextures[2].texture = createTextTexture(font72, "RANKiNG", colorWhite, 0, font_outline72, colorBlack);
        menuTextTextures[2].second_texture = createTextTexture(font72, "RANKiNG", colorGold, 0, font_outline72, colorBlack);
        SDL_QueryTexture(menuTextTextures[2].texture, NULL, NULL, &menuTextTextures[2].rect.w, &menuTextTextures[2].rect.h);
        menuTextTextures[2].rect.x = -800;
        menuTextTextures[2].rect.y = 380;

        //wyjście
        menuTextTextures[3].texture = createTextTexture(font72, "WYJŚCiE", colorWhite, 0, font_outline72, colorBlack);
        menuTextTextures[3].second_texture = createTextTexture(font72, "WYJŚCiE", colorGold, 0, font_outline72, colorBlack);
        SDL_QueryTexture(menuTextTextures[3].texture, NULL, NULL, &menuTextTextures[3].rect.w, &menuTextTextures[3].rect.h);
        menuTextTextures[3].rect.x = WINDOW_WIDTH+900;
        menuTextTextures[3].rect.y = 530;

    //GRA
        //przycisk ataku
        hotkeysTextTextures[0].texture = createTextTexture(font20, "Q", colorWhite, 2, font_outline20, colorBlack);
        SDL_QueryTexture(hotkeysTextTextures[0].texture, NULL, NULL, &hotkeysTextTextures[0].rect.w, &hotkeysTextTextures[0].rect.h);
        //przycisk zamrażania
        hotkeysTextTextures[1].texture = createTextTexture(font20, "W", colorWhite, 2, font_outline20, colorBlack);
        SDL_QueryTexture(hotkeysTextTextures[1].texture, NULL, NULL, &hotkeysTextTextures[1].rect.w, &hotkeysTextTextures[1].rect.h);
        //przycisk bomby
        hotkeysTextTextures[2].texture = createTextTexture(font20, "E", colorWhite, 2, font_outline20, colorBlack);
        SDL_QueryTexture(hotkeysTextTextures[2].texture, NULL, NULL, &hotkeysTextTextures[2].rect.w, &hotkeysTextTextures[2].rect.h);


    //PAUZA
        //potwierdzenie
        pauseTextTextures[0].texture = createTextTexture(font45, "CZY NA PEWNO CHCESZ WYJŚĆ?", colorWhite, 0, font_outline45, colorBlack);
        SDL_QueryTexture(pauseTextTextures[0].texture, NULL, NULL, &pauseTextTextures[0].rect.w, &pauseTextTextures[0].rect.h);
        pauseTextTextures[0].rect.x = WINDOW_WIDTH/2-pauseTextTextures[0].rect.w/2;
        pauseTextTextures[0].rect.y = 150;

        //tak
        pauseTextTextures[1].texture = createTextTexture(font60, "TAK", colorWhite, 0, font_outline60, colorBlack);
        pauseTextTextures[1].second_texture = createTextTexture(font60, "TAK", colorGold, 0, font_outline60, colorBlack);
        SDL_QueryTexture(pauseTextTextures[1].texture, NULL, NULL, &pauseTextTextures[1].rect.w, &pauseTextTextures[1].rect.h);
        pauseTextTextures[1].rect.x = (WINDOW_WIDTH/5)+pauseTextTextures[1].rect.w/2;
        pauseTextTextures[1].rect.y = 400;

        //nie
        pauseTextTextures[2].texture = createTextTexture(font60, "NiE", colorWhite, 0, font_outline60, colorBlack);
        pauseTextTextures[2].second_texture = createTextTexture(font60, "NiE", colorGold, 0, font_outline60, colorBlack);
        SDL_QueryTexture(pauseTextTextures[2].texture, NULL, NULL, &pauseTextTextures[2].rect.w, &pauseTextTextures[2].rect.h);
        pauseTextTextures[2].rect.x = (WINDOW_WIDTH/5)*3;
        pauseTextTextures[2].rect.y = 400;

}

void createInstructionTextures(){
    //INSTRUKCJA
        instructionTextures[0].texture = createTextTexture(font25, "RUCH GRACZA", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[0].second_texture = createTexture("img/instruction/arrow_keys.png");

        instructionTextures[1].texture = createTextTexture(font25, "ATAK", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[1].second_texture = createTexture("img/instruction/q.png");

        instructionTextures[2].texture = createTextTexture(font25, "ZAMRAŻANiE", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[2].second_texture = createTexture("img/instruction/w.png");

        instructionTextures[3].texture = createTextTexture(font25, "BOMBA", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[3].second_texture = createTexture("img/instruction/e.png");

        instructionTextures[4].texture  = createTextTexture(font25, "DODATKOWE ŻYCiE", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[5].texture  = createTextTexture(font25, "ŁADUNEK BOMBY", colorWhite, 1, font_outline25, colorBlack);

        sprintf(textBuffer, "BONUSY (na %d Sekund):", BONUS_DURATION);
        instructionTextures[6].texture  = createTextTexture(font25, textBuffer, colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[7].texture  = createTextTexture(font25, "PRĘDKOŚĆ ATAKU x2", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[8].texture  = createTextTexture(font25, "OBRAŻENiA x3", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[9].texture  = createTextTexture(font25, "PROSTY TOR POCiSKU", colorWhite, 1, font_outline25, colorBlack);
        instructionTextures[10].texture = createTextTexture(font25, "PRĘDKOŚĆ POCiSKÓW x4", colorWhite, 1, font_outline25, colorBlack);

        instructionTextures[11].texture = createTextTexture(font60, "POWRÓT", colorGold, 1, font_outline60, colorBlack);
        SDL_QueryTexture(instructionTextures[11].texture, NULL, NULL, &instructionTextures[11].rect.w, &instructionTextures[11].rect.h);
}

void createRankingTextures(){
    printf("CREATING RANKING TEXTURES...\n");
    //nazwa
    rankingTextTextures[0].texture = createTextTexture(font45, "NAZWA GRACZA", colorWhite, 0, font_outline45, colorBlack);
    SDL_QueryTexture(rankingTextTextures[0].texture, NULL, NULL, &rankingTextTextures[0].rect.w, &rankingTextTextures[0].rect.h);

    //punkty
    rankingTextTextures[1].texture = createTextTexture(font45, "PUNKTY", colorWhite, 0, font_outline45, colorBlack);
    SDL_QueryTexture(rankingTextTextures[1].texture, NULL, NULL, &rankingTextTextures[1].rect.w, &rankingTextTextures[1].rect.h);

    //powrót
    rankingTextTextures[2].texture = createTextTexture(font50, "POWRÓT", colorWhite, 0, font_outline50, colorBlack);
    rankingTextTextures[2].second_texture = createTextTexture(font50, "POWRÓT", colorGold, 0, font_outline50, colorBlack);
    SDL_QueryTexture(rankingTextTextures[2].texture, NULL, NULL, &rankingTextTextures[2].rect.w, &rankingTextTextures[2].rect.h);

    //reset
    rankingTextTextures[3].texture = createTextTexture(font50, "RESETUJ RANKiNG", colorWhite, 0, font_outline50, colorBlack);
    rankingTextTextures[3].second_texture = createTextTexture(font50, "RESETUJ RANKiNG", colorGold, 0, font_outline50, colorBlack);
    SDL_QueryTexture(rankingTextTextures[3].texture, NULL, NULL, &rankingTextTextures[3].rect.w, &rankingTextTextures[3].rect.h);

    readRanking();
    for(int i=0; i<RANKING_TOP;i++){
        rankingTextTextures[4+i].texture = createTextTexture(font30, ranking[i].name, colorWhite, 0, font_outline30, colorBlack);
        sprintf(textBuffer, "%d", ranking[i].score);
        rankingTextTextures[4+i].second_texture = createTextTexture(font30, textBuffer, colorWhite, 0, font_outline30, colorBlack);
        SDL_QueryTexture(rankingTextTextures[4+i].texture, NULL, NULL, &rankingTextTextures[4+i].rect.w, &rankingTextTextures[4+i].rect.h);
    }

}

void createEndTextures(){
    //koniec gry
    endTextTextures[0].texture = createTextTexture(font45, "KONiEC GRY", colorWhite, 0, font_outline45, colorBlack);
    SDL_QueryTexture(endTextTextures[0].texture, NULL, NULL, &endTextTextures[0].rect.w, &endTextTextures[0].rect.h);

    //punkty
    sprintf(textBuffer, "%d", player.score);
    endTextTextures[1].texture = createTextTexture(font45, textBuffer, colorWhite, 0, font_outline45, colorBlack);
    SDL_QueryTexture(endTextTextures[1].texture, NULL, NULL, &endTextTextures[1].rect.w, &endTextTextures[1].rect.h);

    sprintf(textBuffer, "BONUS ZA ŻYCiE: %d", lifeBonusPoints);
    endTextTextures[2].texture = createTextTexture(font30, textBuffer, colorWhite, 0, font_outline30, colorBlack);
    SDL_QueryTexture(endTextTextures[2].texture, NULL, NULL, &endTextTextures[2].rect.w, &endTextTextures[2].rect.h);

    sprintf(textBuffer, "BONUS CZASOWY WALKi fiNAŁOWEJ: %d", bossBonusPoints);
    endTextTextures[3].texture = createTextTexture(font30, textBuffer, colorWhite, 0, font_outline30, colorBlack);
    SDL_QueryTexture(endTextTextures[3].texture, NULL, NULL, &endTextTextures[3].rect.w, &endTextTextures[3].rect.h);

    sprintf(textBuffer, "%d", player.score+lifeBonusPoints+bossBonusPoints);
    endTextTextures[4].texture = createTextTexture(font85, textBuffer, colorGold, 0, font_outline85, colorBlack);
    SDL_QueryTexture(endTextTextures[4].texture, NULL, NULL, &endTextTextures[4].rect.w, &endTextTextures[4].rect.h);

    //potrzebna ilość punktów
    endTextTextures[5].texture = createTextTexture(font30, "ABY ZNALEŹĆ SiĘ W RANKiNGU MUSiSZ UZYSKAĆ ", colorWhite, 0, font_outline30, colorBlack);
    SDL_QueryTexture(endTextTextures[5].texture, NULL, NULL, &endTextTextures[5].rect.w, &endTextTextures[5].rect.h);

    sprintf(textBuffer, "WiĘCEJ NiŻ %d PUNKTÓW", ranking[4].score);
    endTextTextures[6].texture = createTextTexture(font30, textBuffer, colorGold, 0, font_outline30, colorBlack);
    SDL_QueryTexture(endTextTextures[6].texture, NULL, NULL, &endTextTextures[6].rect.w, &endTextTextures[6].rect.h);

    //wygrana
    endTextTextures[7].texture = createTextTexture(font30, "GRATULACJE, WPiSZ NAZWĘ GRACZA:", colorWhite, 0, font_outline30, colorBlack);
    SDL_QueryTexture(endTextTextures[7].texture, NULL, NULL, &endTextTextures[7].rect.w, &endTextTextures[7].rect.h);

    endTextTextures[8].texture = createTextTexture(font50, player.name, colorGold, 0, font_outline50, colorBlack);
    SDL_QueryTexture(endTextTextures[8].texture, NULL, NULL, &endTextTextures[8].rect.w, &endTextTextures[8].rect.h);

    endTextTextures[9].texture = createTextTexture(font30, "POTWiERDŹ NAZWĘ KLAWiSZEM ENTER", colorWhite, 0, font_outline30, colorBlack);
    SDL_QueryTexture(endTextTextures[9].texture, NULL, NULL, &endTextTextures[9].rect.w, &endTextTextures[9].rect.h);


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

void createAnimationTextures(){
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
}

void addAnimation(float x, float y, int angle, int type){
    int found = -1;
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
    if(type>=0 && type<ANIMATION_TYPES){
        animationTypes[type].verticalCount = verticalCount;
        animationTypes[type].horizontalCount = horizontalCount;
    }
}

void allAnimationInit(){
    animationInit(0, 10, 12);
    animationInit(1, 7, 8);
    animationInit(2, 11, 8);
    animationInit(3, 13, 10);
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
    for(int i=0;i<MAX_BOXES;i++){
        if(boxes[i]) removeBox(i);
    }
    for(int i=0;i<MAX_ANIMATIONS;i++){
        if(animations[i]) removeAnimation(i);
    }
}
