#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "init.h"
#include "textures.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "boxes.h"

void onExit();
void renderGame();
void renderMenu();
void renderInstruction();
void renderPause();

//TO DO
void renderWin();
void renderLose();

int main( int argc, char* args[] ) {
    //inicjalizacja SDL, TTF, stworzenie okna i renderu
    if(init()!=0) return 1;

    //czcionki
    font72 = TTF_OpenFont("font.ttf", 72); font_outline72 = TTF_OpenFont("font.ttf", 72);
    font60 = TTF_OpenFont("font.ttf", 60); font_outline60 = TTF_OpenFont("font.ttf", 60);
    font45 = TTF_OpenFont("font.ttf", 45); font_outline45 = TTF_OpenFont("font.ttf", 45);
    font30 = TTF_OpenFont("font.ttf", 30); font_outline30 = TTF_OpenFont("font.ttf", 30);
    font25 = TTF_OpenFont("font.ttf", 25); font_outline25 = TTF_OpenFont("font.ttf", 25);
    font20 = TTF_OpenFont("font.ttf", 20); font_outline20 = TTF_OpenFont("font.ttf", 20);

    //zainicjowanie animacji
    allAnimationInit();
    //stworzenie potrzebnych tekstur
    createTexturesAndRects();
    //zainicjowanie statystyk wrogów
    allEnemyInit();
    //zainicjowanie wartości pocisków
    allBulletsInit();
    //zainicjowanie skrzynek
    allBoxesInit();
    //losowanie
    srand(time(NULL));

    // <<-- GŁÓWNA PĘTLA PROGRAMU -->>
    while(program){
        switch(programStatus){
        case 1:
            renderMenu();
            break;
        case 2:
            renderGame();
            break;
        case 3:
            renderInstruction();
            break;
        case 4:
            renderPause();
            break;
        case 5:
            break;
        }
        //główne renderowanie
        SDL_RenderPresent(rend);

        //printf("%d\n",frames_counter%60);
        //licznik klatek
        frames_counter++;
        //klatki na sekunde
        SDL_Delay(1000/FPS);
    }

    onExit();
    return 0;
}

void onExit(){
    //wyczyszczenie zasobów przed wyjściem
    for(int i=0;i<ENEMY_TYPES;i++)
        SDL_DestroyTexture(enemyTypes[i].texture);
    for(int i=0;i<BULLET_TYPES;i++)
        SDL_DestroyTexture(bulletTypes[i].texture);
    SDL_DestroyTexture(scoreTextTexture);
    SDL_DestroyTexture(startTextTexture);
    SDL_DestroyTexture(helpTextTexture);
    SDL_DestroyTexture(endTextTexture);
    SDL_DestroyTexture(pauseTextTexture);
    SDL_DestroyTexture(yesTextTexture);
    SDL_DestroyTexture(noTextTexture);
    SDL_DestroyTexture(animationTexture);
    SDL_DestroyTexture(iconFreezeTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(hudTexture);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
}

void renderMenu(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        //zatrzymanie programu
        if(event.type == SDL_QUIT)
            program = 0;
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_ESCAPE:
                program = 0;
                break;
            case SDL_SCANCODE_LEFT:
                selectedOption--;
                if(selectedOption<1) selectedOption=3;
                break;
            case SDL_SCANCODE_RIGHT:
                selectedOption++;
                if(selectedOption>3) selectedOption=1;
                break;
            case SDL_SCANCODE_UP:
                selectedOption--;
                if(selectedOption<1) selectedOption=3;
                break;
            case SDL_SCANCODE_DOWN:
                selectedOption++;
                if(selectedOption>3) selectedOption=1;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_Q:
                switch(selectedOption){
                case 1:
                    programStatus = 2;
                    deadTimer = DEAD_TIMER;
                    gameEnd = 0;
                    frames_counter = 0;
                    //załadowanie podstawowych wartości gracza
                    addPlayerBasicStats();
                    bg_scrolling_speed = 2;
                    cleanMap();
                    selectedOption=1;
                    break;
                case 2:
                    programStatus = 3;
                    break;
                case 3:
                    program = 0;
                    break;
                }
                break;
            default:
                break;
            }
        }
    }

    //wyczyszczenie okna
    SDL_RenderClear(rend);

    SDL_RenderCopy(rend, bgTexture, NULL, &bgRect);
    bgRect.y+=bg_scrolling_speed;
    if(bgRect.y>=0)
        bgRect.y=WINDOW_HEIGHT-bgRect.h;

    //rozpocznij
    if(startTextRect.x < WINDOW_WIDTH/2-startTextRect.w/2)
        startTextRect.x+=13;
    else
        startTextRect.x = WINDOW_WIDTH/2-startTextRect.w/2;

    if(selectedOption==1)
        SDL_RenderCopy(rend, selectedStartTextTexture, NULL, &startTextRect);
    else
        SDL_RenderCopy(rend, startTextTexture, NULL, &startTextRect);

    //instrukcja
    if(helpTextRect.x > WINDOW_WIDTH/2-helpTextRect.w/2)
        helpTextRect.x-=13;
    else
        helpTextRect.x = WINDOW_WIDTH/2-helpTextRect.w/2;

    if(selectedOption==2)
        SDL_RenderCopy(rend, selectedHelpTextTexture, NULL, &helpTextRect);
    else
    SDL_RenderCopy(rend, helpTextTexture, NULL, &helpTextRect);

    //wyjście
    if(endTextRect.x < WINDOW_WIDTH/2-endTextRect.w/2)
        endTextRect.x+=13;
    else
        endTextRect.x = WINDOW_WIDTH/2-endTextRect.w/2;

    if(selectedOption==3)
        SDL_RenderCopy(rend, selectedEndTextTexture, NULL, &endTextRect);
    else
    SDL_RenderCopy(rend, endTextTexture, NULL, &endTextRect);
}

void renderGame(){
    //zdarzenia
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        //zatrzymanie programu
        if(event.type == SDL_QUIT)
            program = 0;
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_ESCAPE:
                programStatus  = 4;
                selectedOption = 1;
                break;
            case SDL_SCANCODE_UP:
                player.up = 1;
                break;
            case SDL_SCANCODE_DOWN:
                player.down = 1;
                break;
            case SDL_SCANCODE_RIGHT:
                player.right = 1;
                break;
            case SDL_SCANCODE_LEFT:
                player.left = 1;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_Q:
                player.fire = 1;
                break;
            case SDL_SCANCODE_W:
                if(player.freezeCooldown == 0){
                    addAnimation(player.x+playerRect.w/2, player.y+playerRect.h/2, 0, 2);
                    player.freezeCooldown = FREEZE_COOLDOWN;
                    player.freezeTime = FREEZE_DURATION;
                    for(int i=0;i<MAX_ENEMIES;i++){
                        if(enemies[i]){
                            int type = enemies[i]->enemyType;
                            enemies[i]->freeze = 1;
                            addAnimation(enemies[i]->x+enemyTypes[type].rect.w/2, enemies[i]->y+enemyTypes[type].rect.h/2, rand()%360, 4);
                        }
                    }
                    for(int i=0;i<MAX_BULLETS;i++){
                        if(bullets[i]){
                            int bType = bullets[i]->bulletType;
                            if(bType!=0){
                                bullets[i]->freeze = 1;
                                addAnimation(bullets[i]->x+bulletTypes[bType].rect.w/2, bullets[i]->y+bulletTypes[bType].rect.h/2, rand()%360, 4);
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
        if(event.type == SDL_KEYUP){
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_UP:
                player.up = 0;
                break;
            case SDL_SCANCODE_DOWN:
                player.down = 0;
                break;
            case SDL_SCANCODE_RIGHT:
                player.right = 0;
                break;
            case SDL_SCANCODE_LEFT:
                player.left = 0;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_Q:
                player.fire = 0;
                break;
            default:
                break;
            }
        }
    }
    //wyczyszczenie okna
    SDL_RenderClear(rend);

    //koniec gry;
    if(player.life<=0){
        gameEnd = 1;
        player.x = -100;
        player.y = -100;
        player.attCooldown = 0;
        player.freezeCooldown = -1;
    }
    if(gameEnd){
        deadTimer--;
        if(deadTimer<=0){
            programStatus=1;
            selectedOption=1;
        }
    }

    //ładowanie broni gracza i wrogów
    player.attCooldown++;
    for(int i=0;i<MAX_ENEMIES;i++)
        if(enemies[i] && enemies[i]->cooldown>0 && !enemies[i]->freeze) enemies[i]->cooldown--;

    // <<-- LICZNIK -->>
    current_time = SDL_GetTicks();
    if(current_time > last_time+1000){
        if(player.freezeCooldown > 0)
            player.freezeCooldown--;

        if(player.freezeTime > 0)
            player.freezeTime--;
        else{
            for(int i=0;i<MAX_ENEMIES;i++)
                if(enemies[i] && enemies[i]->freeze) enemies[i]->freeze = 0;
            for(int i=0;i<MAX_BULLETS;i++)
                if(bullets[i] && bullets[i]->freeze) bullets[i]->freeze = 0;
        }

        if(player.timers[1] > 0){
            player.timers[1]--;
            player.attack_speed = 10;
        }else
            player.attack_speed = 25;

        if(player.timers[2] > 0){
            player.timers[2]--;
            player.damage = 3;
        }else
            player.damage = 1;

        if(player.timers[3] > 0){
            player.timers[3]--;
            player.bullet_amplitude = 5;
        }else
            player.bullet_amplitude = 35;

        if(player.timers[4] > 0){
            player.timers[4]--;
            player.bullet_speed = 20;
        }else
            player.bullet_speed = 5;

        printf("FPS: %d\n",frames_counter);
        frames_counter=0;
        last_time = current_time;
    }

    //ruch gracza
    if(player.right)
        player.x += player.speed;
    if(player.left)
        player.x -= player.speed;
    if(player.up)
        player.y -= player.speed;
    if(player.down && player.y < WINDOW_HEIGHT-playerRect.h-HUD_HEIGHT-5)
        player.y += player.speed;

    //kolizja gracza z ekranem
    if(player.x <= 0)
        player.x = 0;
    if(player.x >= WINDOW_WIDTH-playerRect.w)
        player.x = WINDOW_WIDTH-playerRect.w;
    if(player.y <= 45)
        player.y = 45;
    if(player.y >= WINDOW_HEIGHT-playerRect.h-HUD_HEIGHT){
        player.y-=3;
        player.x = (WINDOW_WIDTH - playerRect.w) / 2;
        player.attCooldown = 0;
    }

    //atak gracza
    if(player.fire && player.attCooldown>=player.attack_speed){
        addBullet(player.x+playerRect.w/2-5,player.y-playerBulletRect.h/2,0);
        player.attCooldown=0;
    }

    //aktualizacja pozycji gracza
    playerRect.x = player.x;
    playerRect.y = player.y;

    //rysowanie poszczególnych elementów
    //tło
    SDL_RenderCopy(rend, bgTexture, NULL, &bgRect);
    bgRect.y+=bg_scrolling_speed;
    if(bgRect.y>=0)
        bgRect.y=WINDOW_HEIGHT-bgRect.h;

    //skrzynki
    for(int i=0; i<MAX_BOXES; i++){
        if(boxes[i]){
            int type = boxes[i]->boxType;
            boxRect.x = boxes[i]->x;
            boxRect.y = boxes[i]->y;

            SDL_RenderCopy(rend, boxTypes[type].texture, NULL, &boxRect);

            boxes[i]->y+=1;
            if(boxes[i]->y >= WINDOW_HEIGHT){
                removeBox(i);
                continue;
            }

            collisionUpdate();
            int m = player.collisionBoxesCount;

            if(collisionCheck(1, boxes[i]->collisionBoxes, m, player.collisionBoxes)){
                if(boxes[i]->boxType == 0){
                    if(player.life < MAX_LIFE) player.life++;
                }else{
                    player.timers[type] = BONUS_DURATION;
                }
                removeBox(i);
            }
        }
    }

    //gracz
    if(player.life>0)
        SDL_RenderCopy(rend, playerTexture, NULL, &playerRect);

    //ruch pocisków gracza
    for(int i=0;i<MAX_BULLETS;i++){
        if(bullets[i] && bullets[i]->bulletType == 0){
            int bType = bullets[i]->bulletType;

            bulletTypes[bType].rect.x = bullets[i]->x;
            bulletTypes[bType].rect.y = bullets[i]->y;
            SDL_RenderCopy(rend, bulletTypes[bType].texture, NULL, &bulletTypes[bType].rect);

            bullets[i]->x = bullets[i]->x_start + sin(bullets[i]->y_helper)*bullets[i]->bullet_amplitude;
            bullets[i]->y_helper += SINUS_ACCURACY;
            bullets[i]->y -= bullets[i]->bullet_speed;

            if(bullets[i]->y<=0){
                removeBullet(i);
                continue;
            }

            for(int j=0;j<MAX_ENEMIES;j++){
                if(enemies[j]){
                    int type = enemies[j]->enemyType;

                    collisionUpdate();
                    int n = bulletTypes[bType].collisionBoxesCount;
                    int m = enemyTypes[type].collisionBoxesCount;

                    if(collisionCheck(n, bullets[i]->collisionBoxes, m, enemies[j]->collisionBoxes)){
                        enemies[j]->life-=bullets[i]->bullet_damage;
                        addAnimation(bullets[i]->x+bulletTypes[bType].rect.w/2, bullets[i]->y+bulletTypes[bType].rect.h/2, rand()%360, 0);
                        removeBullet(i);
                        if(enemies[j]->life<=0){
                            player.score += enemyTypes[type].score;
                            addAnimation(enemies[j]->x+enemyTypes[type].rect.w/2, enemies[j]->y+enemyTypes[type].rect.h/2, rand()%360, 0);

                            int k = rand()%BOX_TYPES;
                            int random = rand()%1000;
                            if(random < boxTypes[k].dropChance*enemyTypes[type].dropChanceMultiplier){
                                addBox(enemies[j]->x+enemyTypes[type].rect.w/2, enemies[j]->y+enemyTypes[type].rect.h/2, k);
                            }
                            removeEnemy(j);
                        }
                        break;
                    }
                }
            }
        }
    }

    //stworzenie wrogów gdy plansza jest pusta
    int stageCleared = 1;
    for(int i=0;i<MAX_ENEMIES;i++){
        if(enemies[i]!=NULL){
            stageCleared = 0;
        }
    }

    // <<-- KOLEJNE FALE POTWORÓW -->>
    if(stageCleared && player.stage<=LAST_STAGE+1){
        if(player.stage>LAST_STAGE){
            deadTimer = 120;
            gameEnd = 1;
        }
        addEnemies(player.stage);
        printf(">> Stage %d! <<\n",player.stage);
        player.stage++;
        bg_scrolling_speed += BG_SCROLLING_SPEED_INCREASE;
    }

    //rysowanie i ruch wrogów
    int change = 0;
    for(int i = 0;i<MAX_ENEMIES;i++){
        if(enemies[i]){
            int type = enemies[i]->enemyType;
            enemyTypes[type].rect.x = enemies[i]->x;
            enemyTypes[type].rect.y = enemies[i]->y;

            collisionUpdate();
            int n = enemyTypes[type].collisionBoxesCount;
            int m = player.collisionBoxesCount;

            if(collisionCheck(n, enemies[i]->collisionBoxes, m, player.collisionBoxes)){
                player.life-=enemyTypes[type].damage;
                addAnimation(enemies[i]->x+enemyTypes[type].rect.w/2, enemies[i]->y+enemyTypes[type].rect.h/2, rand()%360, 0);
                if(player.life==0)
                    addAnimation(player.x+playerRect.w/2, player.y+playerRect.h/2, 0, 3);
                else
                    addAnimation(player.x+playerRect.w/2, player.y+playerRect.h/2, rand()%360, 0);
                addAnimation(heartRect.x+heartRect.w/2, heartRect.y+heartRect.h/2, rand()%360, 1);
                removeEnemy(i);
                continue;
            }else
                SDL_RenderCopy(rend, enemyTypes[type].texture, NULL, &enemyTypes[type].rect);

            if(enemies[0] && enemies[0]->y<60){
                for(int j=0;j<MAX_ENEMIES;j++){
                    if(enemies[j] && !enemies[j]->freeze) enemies[j]->y+=0.3;
                }
            }else{
                if((enemies[i]->cooldown<=0) && !enemies[i]->freeze){
                    int roll;
                    for(int l=0;l<5;l++)
                        roll = rand();
                    roll=roll%2000+1;
                    if(roll<=enemyTypes[type].bulletChance){
                        addBullet(enemies[i]->x+enemyTypes[type].rect.w/2, enemies[i]->y+enemyTypes[type].rect.h-5, enemyTypes[type].bulletType);
                        enemies[i]->cooldown = enemyTypes[type].cooldownTime;
                    }
                }
                if(!enemies[i]->freeze)
                    enemies[i]->x += enemies[i]->x_vel;
                if((enemies[i]->x+enemyTypes[type].rect.w >= WINDOW_WIDTH || enemies[i]->x <= 0) && !enemies[i]->freeze){
                    change = 1;
                    enemies[i]->x -= enemies[i]->x_vel;
                    break;
                }
            }
        }
    }
    if(change){
        for(int i = 0;i<MAX_ENEMIES;i++){
            if(enemies[i]){
                enemies[i]->x_vel*=-1;
                enemies[i]->y+=enemies[i]->y_vel;
                if(enemies[i]->y>=WINDOW_HEIGHT-hudRect1.h)
                    removeEnemy(i);
            }
        }
    }

    //ruch pocisków wrogów
    for(int i=0;i<MAX_BULLETS;i++){
        if(bullets[i] && bullets[i]->bulletType > 0){
            int bType = bullets[i]->bulletType;

            bulletTypes[bType].rect.x = bullets[i]->x;
            bulletTypes[bType].rect.y = bullets[i]->y;
            SDL_RenderCopyEx(rend,bulletTypes[bType].texture,NULL,&bulletTypes[bType].rect,bullets[i]->bullet_angle,NULL,SDL_FLIP_NONE);

            if(!bullets[i]->freeze)
                bullets[i]->bullet_angle += bulletTypes[bType].spin_speed;
            if(bullets[i]->bullet_angle>=360){
                bullets[i]->bullet_angle-=360;
            }
            if(!bullets[i]->freeze){
                bullets[i]->x = bullets[i]->x_start - sin(bullets[i]->y_helper)*bullets[i]->bullet_amplitude;
                bullets[i]->y_helper += SINUS_ACCURACY;
                bullets[i]->y += bullets[i]->bullet_speed;
            }
            if(bullets[i]->y>=WINDOW_HEIGHT){
                removeBullet(i);
                continue;
            }

            collisionUpdate();
            int n = bulletTypes[bType].collisionBoxesCount;
            int m = player.collisionBoxesCount;

            if(collisionCheck(n, bullets[i]->collisionBoxes, m, player.collisionBoxes)){
                player.life-=bulletTypes[bType].bullet_damage;
                addAnimation(bullets[i]->x+bulletTypes[bType].rect.w/2, bullets[i]->y+bulletTypes[bType].rect.h/2, rand()%360, 0);
                if(player.life==0)
                    addAnimation(player.x+playerRect.w/2, player.y+playerRect.h/2, 0, 3);
                addAnimation(heartRect.x+heartRect.w/2, heartRect.y+heartRect.h/2, rand()%360, 1);
                removeBullet(i);
                break;
            }
        }
    }

    //hud
    if(hudRect1.y>WINDOW_HEIGHT-HUD_HEIGHT)
        hudRect1.y-=3;
    else
        hudRect1.y=WINDOW_HEIGHT-HUD_HEIGHT;

    if(hudRect2.y<0)
        hudRect2.y+=3;
    else
        hudRect2.y=0;

    SDL_RenderCopy(rend, hudTexture, NULL, &hudRect1);

    // <<-- IKONY -->>
    //zamrażanie
    iconFreezeRect.x = WINDOW_WIDTH-iconFreezeRect.w-10;
    iconFreezeRect.y = WINDOW_HEIGHT-iconFreezeRect.h-5;
    if(player.freezeCooldown == 0){
        SDL_RenderCopy(rend, iconFreezeTexture, NULL, &iconFreezeRect);
        freezeHtkRect.x = iconFreezeRect.x+iconFreezeRect.w/2-freezeHtkRect.w/2;
        freezeHtkRect.y = iconFreezeRect.y+iconFreezeRect.h-freezeHtkRect.h;
        SDL_RenderCopy(rend, freezeHtkTexture, NULL, &freezeHtkRect);
    }else if(player.freezeCooldown == -1){
        SDL_RenderCopy(rend, iconFreezeCdTexture, NULL, &iconFreezeRect);
    }else{
        SDL_RenderCopy(rend, iconFreezeCdTexture, NULL, &iconFreezeRect);

        sprintf(textBuffer, "%d", player.freezeCooldown);

        freezeCdTexture = createTextTexture(font45, textBuffer, colorWhite, 3, font_outline45, colorBlack);
        SDL_QueryTexture(freezeCdTexture, NULL, NULL, &freezeCdRect.w, &freezeCdRect.h);

        freezeCdRect.x = iconFreezeRect.x+iconFreezeRect.w/2-freezeCdRect.w/2;
        freezeCdRect.y = iconFreezeRect.y+iconFreezeRect.h/2-freezeCdRect.h/2;
        SDL_RenderCopy(rend, freezeCdTexture, NULL, &freezeCdRect);

        SDL_DestroyTexture(freezeCdTexture);
    }

    //serca
    heartRect.x = heartX;
    if(heartX<5)
        heartX+=4;
    else
        heartX = 5;

    for(int i=0;i<player.life;i++){
        SDL_RenderCopy(rend, heartTexture, NULL, &heartRect);
        if(i+1<player.life)
            heartRect.x += 40;
    }

    //zmiana klatek animacji
    for(int i=0;i<MAX_ANIMATIONS;i++){
        if(animations[i]){
            int type = animations[i]->animationType;
            animationTypes[type].rect.x = animations[i]->x;
            animationTypes[type].rect.y = animations[i]->y;

            SDL_RenderCopyEx(rend, animationTypes[type].texture, &animations[i]->frameRect, &animationTypes[type].rect, animations[i]->angle, NULL, SDL_FLIP_NONE);

            animations[i]->frameRect.x += animations[i]->frameRect.w;
            if(animations[i]->frameRect.x >= animationTypes[type].textureW){
                animations[i]->frameRect.x = 0;
                animations[i]->frameRect.y += animations[i]->frameRect.h;
                if(animations[i]->frameRect.y >= animationTypes[type].textureH){
                    removeAnimation(i);
                    continue;
                }
            }

        }
    }

    SDL_RenderCopy(rend, hudTexture, NULL, &hudRect2);

    //ikony bonusów
    boxRect.x = 10;
    boxRect.y = 8;
    for(int i = 1; i<BOX_TYPES; i++){
        if(player.timers[i]>0){
            SDL_RenderCopy(rend, boxTypes[i].texture, NULL, &boxRect);

            sprintf(textBuffer, "%d", player.timers[i]);

            textTexture = createTextTexture(font20, textBuffer, colorWhite, 1, font_outline20, colorBlack);
            SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
            textRect.x = boxRect.x+boxRect.w/2;
            textRect.y = boxRect.y+boxRect.h/2-3;
            SDL_RenderCopy(rend, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }else{
            SDL_RenderCopy(rend, boxTypes[i].offTexture, NULL, &boxRect);
        }
        boxRect.x += boxRect.w+10;
    }

    //punkty (napis)
    sprintf(textBuffer, "Punkty: %d", player.score);

    scoreTextTexture = createTextTexture(font30, textBuffer, colorGold, 1, font_outline30, colorBlack);
    SDL_QueryTexture(scoreTextTexture, NULL, NULL, &scoreTextRect.w, &scoreTextRect.h);
    scoreTextRect.x = WINDOW_WIDTH-scoreTextRect.w-10;
    scoreTextRect.y = 0;
    SDL_RenderCopy(rend, scoreTextTexture, NULL, &scoreTextRect);
    SDL_DestroyTexture(scoreTextTexture);

}

void renderInstruction(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        //zatrzymanie programu
        if(event.type == SDL_QUIT)
            program = 0;
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_ESCAPE:
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_Q:
                startTextRect.x = WINDOW_WIDTH/2-startTextRect.w/2;
                helpTextRect.x = WINDOW_WIDTH/2-helpTextRect.w/2;
                endTextRect.x = WINDOW_WIDTH/2-endTextRect.w/2;
                programStatus=1;
                break;
            default:
                break;
            }
        }
    }
    //wyczyszczenie okna
    SDL_RenderClear(rend);

    //tło
    SDL_RenderCopy(rend, bgTexture, NULL, &bgRect);
    bgRect.y+=bg_scrolling_speed;
    if(bgRect.y>=0)
        bgRect.y=WINDOW_HEIGHT-bgRect.h;

    //strzałki
    arrowsRect.x = 30;
    arrowsRect.y = 160;
    SDL_RenderCopy(rend, arrowsTexture, NULL, &arrowsRect);

    textTexture = createTextTexture(font25, "RUCH GRACZA", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = arrowsRect.x+arrowsRect.w+20;
    textRect.y = arrowsRect.y+arrowsRect.h/2-textRect.h/2;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //Q
    SDL_QueryTexture(qTexture, NULL, NULL, &keyRect.w, &keyRect.h);
    keyRect.x = arrowsRect.x+arrowsRect.w/2-keyRect.w/2;
    keyRect.y = arrowsRect.y+arrowsRect.h+30;
    SDL_RenderCopy(rend, qTexture, NULL, &keyRect);

    textTexture = createTextTexture(font25, "ATAK", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.y = keyRect.y+keyRect.h/2-textRect.h/2;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //W
    SDL_QueryTexture(wTexture, NULL, NULL, &keyRect.w, &keyRect.h);
    keyRect.y += keyRect.h+30;
    SDL_RenderCopy(rend, wTexture, NULL, &keyRect);

    textTexture = createTextTexture(font25, "ZAMRAŻANiE", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.y += keyRect.h+30;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //E
    SDL_QueryTexture(eTexture, NULL, NULL, &keyRect.w, &keyRect.h);
    keyRect.y += keyRect.h+30;
    SDL_RenderCopy(rend, eTexture, NULL, &keyRect);

    textTexture = createTextTexture(font25, "BOMBA", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.y += keyRect.h+30;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);


    //życie
    SDL_Rect instructionIconRect = {510, 120, 60, 60};
    SDL_RenderCopy(rend, boxTypes[0].texture, NULL, &instructionIconRect);

    textTexture = createTextTexture(font25, "DODATKOWE ŻYCiE", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = instructionIconRect.x+instructionIconRect.w+20;
    textRect.y = instructionIconRect.y+instructionIconRect.h/2-textRect.h/2;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //bonusy
    sprintf(textBuffer, "BONUSY (na %d Sekund):", BONUS_DURATION);
    textTexture = createTextTexture(font25, textBuffer, colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = instructionIconRect.x;
    textRect.y = instructionIconRect.y+instructionIconRect.h+50;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //prędkość ataku
    instructionIconRect.y = textRect.y+textRect.h+20;
    SDL_RenderCopy(rend, boxTypes[1].texture, NULL, &instructionIconRect);

    textTexture = createTextTexture(font25, "PRĘDKOŚĆ ATAKU x2", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = instructionIconRect.x+instructionIconRect.w+20;
    textRect.y = instructionIconRect.y+instructionIconRect.h/2-textRect.h/2;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //obrażenia
    instructionIconRect.y += instructionIconRect.h+20;
    SDL_RenderCopy(rend, boxTypes[2].texture, NULL, &instructionIconRect);

    textTexture = createTextTexture(font25, "OBRAŻENiA x3", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = instructionIconRect.x+instructionIconRect.w+20;
    textRect.y = instructionIconRect.y+instructionIconRect.h/2-textRect.h/2;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //usunięcie amplitudy
    instructionIconRect.y += instructionIconRect.h+20;
    SDL_RenderCopy(rend, boxTypes[3].texture, NULL, &instructionIconRect);

    textTexture = createTextTexture(font25, "PROSTY TOR POCiSKU", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = instructionIconRect.x+instructionIconRect.w+20;
    textRect.y = instructionIconRect.y+instructionIconRect.h/2-textRect.h/2;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    //obrażenia
    instructionIconRect.y += instructionIconRect.h+20;
    SDL_RenderCopy(rend, boxTypes[4].texture, NULL, &instructionIconRect);

    textTexture = createTextTexture(font25, "PRĘDKOŚĆ POCiSKÓW x4", colorWhite, 1, font_outline25, colorBlack);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.y = instructionIconRect.y+instructionIconRect.h/2-textRect.h/2;
    SDL_RenderCopy(rend, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void renderPause(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        //zatrzymanie programu
        if(event.type == SDL_QUIT)
            program = 0;
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_ESCAPE:
                programStatus = 2;
                break;
            case SDL_SCANCODE_LEFT:
                selectedOption--;
                if(selectedOption<1) selectedOption=2;
                break;
            case SDL_SCANCODE_RIGHT:
                selectedOption++;
                if(selectedOption>2) selectedOption=1;
                break;
            case SDL_SCANCODE_UP:
                selectedOption--;
                if(selectedOption<1) selectedOption=2;
                break;
            case SDL_SCANCODE_DOWN:
                selectedOption++;
                if(selectedOption>2) selectedOption=1;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_Q:
                switch(selectedOption){
                case 1:
                    programStatus=2;
                    selectedOption=1;
                    break;
                case 2:
                    startTextRect.x = WINDOW_WIDTH/2-startTextRect.w/2;
                    helpTextRect.x = WINDOW_WIDTH/2-helpTextRect.w/2;
                    endTextRect.x = WINDOW_WIDTH/2-endTextRect.w/2;
                    programStatus=1;
                    selectedOption=1;
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
    //wyczyszczenie okna
    SDL_RenderClear(rend);

    //tło
    SDL_RenderCopy(rend, bgTexture, NULL, &bgRect);
    bgRect.y+=bg_scrolling_speed;
    if(bgRect.y>=0)
        bgRect.y=WINDOW_HEIGHT-bgRect.h;

    //pauza
    SDL_RenderCopy(rend, pauseTextTexture, NULL, &pauseTextRect);

    //NIE
    if(selectedOption==1)
        SDL_RenderCopy(rend, selectedNoTextTexture, NULL, &noTextRect);
    else
    SDL_RenderCopy(rend, noTextTexture, NULL, &noTextRect);

    //TAK
    if(selectedOption==2)
        SDL_RenderCopy(rend, selectedYesTextTexture, NULL, &yesTextRect);
    else
    SDL_RenderCopy(rend, yesTextTexture, NULL, &yesTextRect);
}

void renderWin(){
    //TO DO
}

void renderLose(){
    //TO DO
}
