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
#include "enemy.h"
#include "bullet.h"
#include "boxes.h"
#include "player.h"
#include "ranking.h"

void onExit();
void renderGame();
void renderMenu();
void renderInstruction();
void renderPause();
void renderRanking();
void renderEnd();

int main( int argc, char* args[] ) {
    //inicjalizacja SDL, TTF, stworzenie okna i renderu
    if(init()!=0) return 1;

    //zainicjowanie animacji
    allAnimationInit();
    createAnimationTextures();
    //stworzenie potrzebnych tekstur
    createTexturesAndRects();
    //zainicjowanie statystyk wrogów
    allEnemyInit();
    //zainicjowanie wartości pocisków
    allBulletsInit();
    //zainicjowanie skrzynek
    allBoxesInit();
    //wczytanie rankingu
    readRanking();
    //losowanie
    srand(time(NULL));

    // <<-- GŁÓWNA PĘTLA PROGRAMU -->>
    while(program){
        //wyczyszczenie okna
        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, bgTexture, NULL, &bgRect);
        bgRect.y+=bg_scrolling_speed;
        if(bgRect.y>=0)
            bgRect.y=WINDOW_HEIGHT-bgRect.h;

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
            renderRanking();
            break;
        case 6:
            renderEnd(0);
            break;
        case 7:
            renderEnd(1);
            break;
        }
        //główne renderowanie
        SDL_RenderPresent(rend);

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
    for(int i=0;i<ANIMATION_TYPES;i++)
        SDL_DestroyTexture(animationTypes[i].texture);
    for(int i=0;i<BOX_TYPES;i++)
        SDL_DestroyTexture(boxTypes[i].texture);
    for(int i=0;i<BULLET_TYPES;i++)
        SDL_DestroyTexture(bulletTypes[i].texture);


    for(int i=0;i<4;i++){
        SDL_DestroyTexture(menuTextTextures[i].texture);        SDL_DestroyTexture(menuTextTextures[i].second_texture);
    }
    for(int i=0;i<3;i++){
        SDL_DestroyTexture(pauseTextTextures[i].texture);        SDL_DestroyTexture(pauseTextTextures[i].second_texture);
    }
    for(int i=0;i<12;i++){
        SDL_DestroyTexture(instructionTextures[i].texture);        SDL_DestroyTexture(instructionTextures[i].second_texture);
    }

    SDL_DestroyTexture(scoreTextTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(hudTexture);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(tempTexture);
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
                if(selectedOption<1) selectedOption=4;
                break;
            case SDL_SCANCODE_RIGHT:
                selectedOption++;
                if(selectedOption>4) selectedOption=1;
                break;
            case SDL_SCANCODE_UP:
                selectedOption--;
                if(selectedOption<1) selectedOption=4;
                break;
            case SDL_SCANCODE_DOWN:
                selectedOption++;
                if(selectedOption>4) selectedOption=1;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_RETURN:
                switch(selectedOption){
                case 1:
                    programStatus = 2;
                    deadTimer = DEAD_TIMER;
                    gameEnd = 0;
                    frames_counter = 0;
                    //wyzerowanie flag
                    bossFlag = 0;
                    bossLifeFlag = 0;
                    bossBoxFlag = 0;
                    bossPointsTimer = BOSS_BONUS_POINTS_TIME;
                    //wyzerowanie dodatkowych punktów
                    lifeBonusPoints = 0;
                    bossBonusPoints = 0;
                    //załadowanie podstawowych wartości gracza
                    addPlayerBasicStats();
                    bg_scrolling_speed = 2;
                    cleanMap();
                    selectedOption = 1;
                    break;
                case 2:
                    programStatus = 3;
                    if(!instructionTexturesCreated){
                        createInstructionTextures();
                        instructionTexturesCreated = 1;
                    }
                    break;
                case 3:
                    createRankingTextures();
                    selectedOption = 1;
                    programStatus = 5;
                    break;
                case 4:
                    program = 0;
                    break;
                }
                break;
            default:
                break;
            }
        }
    }

    //rozpocznij
    if(menuTextTextures[0].rect.x < WINDOW_WIDTH/2-menuTextTextures[0].rect.w/2)
        menuTextTextures[0].rect.x+=13;
    else
        menuTextTextures[0].rect.x = WINDOW_WIDTH/2-menuTextTextures[0].rect.w/2;

    if(selectedOption == 1)
        SDL_RenderCopy(rend, menuTextTextures[0].second_texture, NULL, &menuTextTextures[0].rect);
    else
        SDL_RenderCopy(rend, menuTextTextures[0].texture, NULL, &menuTextTextures[0].rect);

    //instrukcja
    if(menuTextTextures[1].rect.x > WINDOW_WIDTH/2-menuTextTextures[1].rect.w/2)
        menuTextTextures[1].rect.x-=13;
    else
        menuTextTextures[1].rect.x = WINDOW_WIDTH/2-menuTextTextures[1].rect.w/2;

    if(selectedOption == 2)
        SDL_RenderCopy(rend, menuTextTextures[1].second_texture, NULL, &menuTextTextures[1].rect);
    else
        SDL_RenderCopy(rend, menuTextTextures[1].texture, NULL, &menuTextTextures[1].rect);

    //ranking
    if(menuTextTextures[2].rect.x < WINDOW_WIDTH/2-menuTextTextures[2].rect.w/2)
        menuTextTextures[2].rect.x+=13;
    else
        menuTextTextures[2].rect.x = WINDOW_WIDTH/2-menuTextTextures[2].rect.w/2;

    if(selectedOption == 3)
        SDL_RenderCopy(rend, menuTextTextures[2].second_texture, NULL, &menuTextTextures[2].rect);
    else
        SDL_RenderCopy(rend, menuTextTextures[2].texture, NULL, &menuTextTextures[2].rect);

    //wyjście
    if(menuTextTextures[3].rect.x > WINDOW_WIDTH/2-menuTextTextures[3].rect.w/2)
        menuTextTextures[3].rect.x-=13;
    else
        menuTextTextures[3].rect.x = WINDOW_WIDTH/2-menuTextTextures[3].rect.w/2;

    if(selectedOption == 4)
        SDL_RenderCopy(rend, menuTextTextures[3].second_texture, NULL, &menuTextTextures[3].rect);
    else
        SDL_RenderCopy(rend, menuTextTextures[3].texture, NULL, &menuTextTextures[3].rect);
}

void renderGame(){
    //flagi
    int scoreFlag = 0;
    int timerFlag = 0;
    int bombsFlag = 0;

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
            case SDL_SCANCODE_Q:
                player.fire = 1;
                break;
            case SDL_SCANCODE_W:
                if(player.freezeCooldown == 0){
                    timerFlag = 1;
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
            case SDL_SCANCODE_E:
                if(player.bombs>0 && !player.bombsCooldown){
                    player.bombs--;
                    bombsFlag = 1;
                    addBullet(player.x+playerRect.w/2-bulletTypes[7].rect.w/2,player.y-bulletTypes[7].rect.h+30,7);
                    player.bombsCooldown = BOMBS_COOLDOWN;
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

    //koniec gry;
    if(player.life<=0){
        gameEnd = 1;
        player.x = WINDOW_WIDTH;
        player.y = WINDOW_HEIGHT;
        player.attCooldown = 0;
        player.freezeCooldown = -1;
        player.bombsCooldown = BOMBS_COOLDOWN;
    }

    if(gameEnd){
        deadTimer--;
        if(bg_scrolling_speed >= 0.5)
            bg_scrolling_speed -= 0.05;

        if(deadTimer<=0){
            if(player.life>5)
                lifeBonusPoints = player.score*(player.life-5)*0.05;
            else
                lifeBonusPoints = 0;

            if(player.life>0)
                bossBonusPoints = 2.5*enemyTypes[7].score*((float)bossPointsTimer/BOSS_BONUS_POINTS_TIME);

            if(player.score+lifeBonusPoints+bossBonusPoints>ranking[RANKING_TOP-1].score){
                programStatus = 7;
                selectedOption = 1;
            }else{
                programStatus = 6;
                selectedOption = 1;
            }
            createEndTextures();
            player.score += lifeBonusPoints+bossBonusPoints;

        }
    }

    //ładowanie broni gracza i wrogów
    player.attCooldown++;
    if(player.bombsCooldown>0)
        player.bombsCooldown--;
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

        if(enemies[0] && !enemies[0]->freeze && enemies[0]->enemyType == 7){
            bossFlag++;
            bossBoxFlag++;
        }
        if(enemies[0] && enemies[0]->enemyType == 7 && bossPointsTimer>0 && enemies[0]->y>=60){
            bossPointsTimer--;
        }

        timerFlag = 1;
        printf("FPS: %d\n",frames_counter);
        frames_counter = 0;
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
        player.bombsCooldown = 150;
        scoreFlag = 1;
        bombsFlag = 1;
    }

    //atak gracza
    if(player.fire && player.attCooldown>=player.attack_speed){
        addBullet(player.x+playerRect.w/2-5,player.y-playerBulletRect.h/2,0);
        player.attCooldown=0;
    }

    //aktualizacja pozycji gracza
    playerRect.x = player.x;
    playerRect.y = player.y;

    //skrzynki
    for(int i=0; i<MAX_BOXES; i++){
        if(boxes[i]){
            int type  = boxes[i]->boxType;
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
                }else if(boxes[i]->boxType == 5){
                    player.bombs++;
                    bombsFlag++;
                }else{
                    player.timers[type] = BONUS_DURATION;
                    timerFlag = 1;
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

            bullets[i]->x = bullets[i]->x_start + sin(bullets[i]->y_vel)*bullets[i]->bullet_amplitude;
            bullets[i]->y_vel += SINUS_ACCURACY;
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
                        if(type == 7 && enemies[0]->y>=59){
                            bossLifeFlag++;
                            enemies[j]->life-=bullets[i]->bullet_damage;
                        }else if(type != 7){
                            enemies[j]->life-=bullets[i]->bullet_damage;
                        }
                        addAnimation(bullets[i]->x+bulletTypes[bType].rect.w/2, bullets[i]->y+bulletTypes[bType].rect.h/2, rand()%360, 0);
                        removeBullet(i);
                        if(enemies[j]->life<=0){
                            scoreFlag = 1;
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

    //sprawdzenie czy plansza jest pusta
    int stageCleared = 1;
    for(int i=0;i<MAX_ENEMIES;i++){
        if(enemies[i]!=NULL) stageCleared = 0;
    }

    // <<-- KOLEJNE FALE POTWORÓW -->>
    if(stageCleared && player.stage<=LAST_STAGE){
        player.stage++;
        if(player.stage>LAST_STAGE){
            gameEnd = 1;
        }else{
            printf(">> Stage %d! <<\n",player.stage);
            addEnemies(player.stage);
            bg_scrolling_speed += BG_SCROLLING_SPEED_INCREASE;
        }
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
                if(player.life<=0)
                    addAnimation(player.x+playerRect.w/2, player.y+playerRect.h/2, 0, 3);
                else
                    addAnimation(player.x+playerRect.w/2, player.y+playerRect.h/2, rand()%360, 0);
                addAnimation(heartRect.x+heartRect.w/2, heartRect.y+heartRect.h/2, rand()%360, 1);
                if(type!=7)
                    removeEnemy(i);
                continue;
            }else
                SDL_RenderCopy(rend, enemyTypes[type].texture, NULL, &enemyTypes[type].rect);

            if(enemies[0] && enemies[0]->y<60){
                for(int j=0;j<MAX_ENEMIES;j++){
                    if(enemies[j] && !enemies[j]->freeze){
                        enemies[j]->y+=0.2;
                        if(enemies[j]->enemyType == 7 || enemies[j]->enemyType == 8){
                            enemies[j]->y+=0.5;
                            bossLifeFlag++;
                        }
                    }
                }
            }else{
                if((enemies[i]->cooldown<=0) && !enemies[i]->freeze){
                    int roll;
                    for(int l=0;l<5;l++)
                        roll = rand();
                    roll=roll%2000+1;
                    if(roll<=enemyTypes[type].bulletChance){
                        switch(type){
                        case 7:
                            addBullet(enemies[i]->x+enemyTypes[type].rect.w/2-5, enemies[i]->y+enemyTypes[type].rect.h-50, enemyTypes[type].bulletType);
                            break;
                        case 8:
                            addBullet(enemies[i]->x+enemyTypes[type].rect.w/2-20, enemies[i]->y+enemyTypes[type].rect.h-20, enemyTypes[type].bulletType);
                            break;
                        default:
                            addBullet(enemies[i]->x+enemyTypes[type].rect.w/2, enemies[i]->y+enemyTypes[type].rect.h-5, enemyTypes[type].bulletType);
                            break;
                        }

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

    //ruch pocisków wrogów i bomby gracza
    for(int i=0;i<MAX_BULLETS;i++){
        if(bullets[i] && bullets[i]->bulletType != 0){
            int bType = bullets[i]->bulletType;

            bulletTypes[bType].rect.x = bullets[i]->x;
            bulletTypes[bType].rect.y = bullets[i]->y;
            SDL_RenderCopyEx(rend,bulletTypes[bType].texture,NULL,&bulletTypes[bType].rect,bullets[i]->bullet_angle,NULL,SDL_FLIP_NONE);


            if(!bullets[i]->freeze || bType == 7)
                bullets[i]->bullet_angle += bulletTypes[bType].spin_speed;
            if(bullets[i]->bullet_angle>=360){
                bullets[i]->bullet_angle-=360;
            }
            if(bullets[i]->y>=WINDOW_HEIGHT){
                    removeBullet(i);
                    continue;
            }

            if(!bullets[i]->freeze || bType == 7){
                if(bType == 10){
                    bullets[i]->x += bullets[i]->x_vel;
                    bullets[i]->y += bullets[i]->y_vel;
                }else{
                    bullets[i]->x = bullets[i]->x_start - sin(bullets[i]->y_vel)*bullets[i]->bullet_amplitude;
                    bullets[i]->y_vel += SINUS_ACCURACY;
                    bullets[i]->y += bullets[i]->bullet_speed;
                }
            }

            collisionUpdate();
            int n = bulletTypes[bType].collisionBoxesCount;
            if(bType != 7){
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
            }else if(bType == 7){ //BOMBY
                for(int j=0; j<MAX_ENEMIES; j++){
                    if(enemies[j]){
                        int eType = enemies[j]->enemyType;
                        int m = enemyTypes[eType].collisionBoxesCount;

                        if(collisionCheck(n, bullets[i]->collisionBoxes, m, enemies[j]->collisionBoxes)){
                            if(eType == 7);
                                bossLifeFlag++;
                            for(int k = 0; k<MAX_ENEMIES;k++){
                                if(enemies[k]){
                                    int eType2 = enemies[k]->enemyType;
                                    enemies[k]->life-=bullets[i]->bullet_damage;
                                    addAnimation(enemies[k]->x+(enemyTypes[eType2].rect.w/2)*(0.4*(rand()%5-2)+1), enemies[k]->y+(enemyTypes[eType2].rect.h/2)*(0.4*(rand()%5-2)+1), rand()%360, 0);
                                    if(enemies[k]->life<=0){
                                        scoreFlag = 1;
                                        player.score += enemyTypes[eType2].score;

                                        int box = rand()%BOX_TYPES;
                                        int random = rand()%1000;
                                        if(random*1.5 < boxTypes[box].dropChance*enemyTypes[eType2].dropChanceMultiplier){
                                            addBox(enemies[k]->x+enemyTypes[eType2].rect.w/2, enemies[k]->y+enemyTypes[eType2].rect.h/2, box);
                                        }
                                        removeEnemy(k);
                                    }
                                }
                            }
                            addAnimation(bulletTypes[bType].rect.x+bulletTypes[bType].rect.w/2,bulletTypes[bType].rect.y, 0, 3);
                            removeBullet(i);
                            break;
                        }
                    }
                }
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
    //atak
    iconTextures[0].rect.x = WINDOW_WIDTH-3*iconTextures[0].rect.w-3*10;
    iconTextures[0].rect.y = WINDOW_HEIGHT-iconTextures[0].rect.h-5;
    if(player.attCooldown>=player.attack_speed)
        SDL_RenderCopy(rend, iconTextures[0].second_texture, NULL, &iconTextures[0].rect);
    else
        SDL_RenderCopy(rend, iconTextures[0].texture, NULL, &iconTextures[0].rect);

    hotkeysTextTextures[0].rect.x = iconTextures[0].rect.x+iconTextures[0].rect.w/2-hotkeysTextTextures[0].rect.w/2;
    hotkeysTextTextures[0].rect.y = iconTextures[0].rect.y+iconTextures[0].rect.h-hotkeysTextTextures[0].rect.h;
    SDL_RenderCopy(rend, hotkeysTextTextures[0].texture, NULL, &hotkeysTextTextures[0].rect);

    //zamrażanie
    iconTextures[1].rect.x = iconTextures[0].rect.x+iconTextures[0].rect.w+10;
    iconTextures[1].rect.y = iconTextures[0].rect.y;
    if(player.freezeCooldown == 0){
        SDL_RenderCopy(rend, iconTextures[1].second_texture, NULL, &iconTextures[1].rect);
        hotkeysTextTextures[1].rect.x = iconTextures[1].rect.x+iconTextures[1].rect.w/2-hotkeysTextTextures[1].rect.w/2;
        hotkeysTextTextures[1].rect.y = iconTextures[1].rect.y+iconTextures[1].rect.h-hotkeysTextTextures[1].rect.h;
        SDL_RenderCopy(rend, hotkeysTextTextures[1].texture, NULL, &hotkeysTextTextures[1].rect);
    }else if(player.freezeCooldown == -1){
        SDL_RenderCopy(rend, iconTextures[1].texture, NULL, &iconTextures[1].rect);
    }else{
        SDL_RenderCopy(rend, iconTextures[1].texture, NULL, &iconTextures[1].rect);

        if(timerFlag){
            SDL_DestroyTexture(spellsCdTextures[1].texture);
            sprintf(textBuffer, "%d", player.freezeCooldown);

            spellsCdTextures[1].texture = createTextTexture(font30, textBuffer, colorWhite, 2, font_outline30, colorBlack);
            SDL_QueryTexture(spellsCdTextures[1].texture, NULL, NULL, &spellsCdTextures[1].rect.w, &spellsCdTextures[1].rect.h);

            spellsCdTextures[1].rect.x = iconTextures[1].rect.x+iconTextures[1].rect.w/2-spellsCdTextures[1].rect.w/2;
            spellsCdTextures[1].rect.y = iconTextures[1].rect.y+iconTextures[1].rect.h/2-spellsCdTextures[1].rect.h/2;
        }
        SDL_RenderCopy(rend, spellsCdTextures[1].texture, NULL, &spellsCdTextures[1].rect);
    }

    //bomby
    iconTextures[2].rect.x = iconTextures[1].rect.x+iconTextures[1].rect.w+10;
    iconTextures[2].rect.y = iconTextures[1].rect.y;
    if(player.bombs>0)
        SDL_RenderCopy(rend, iconTextures[2].second_texture, NULL, &iconTextures[2].rect);
    else
        SDL_RenderCopy(rend, iconTextures[2].texture, NULL, &iconTextures[2].rect);

    hotkeysTextTextures[2].rect.x = iconTextures[2].rect.x+iconTextures[2].rect.w/2-hotkeysTextTextures[2].rect.w/2;
    hotkeysTextTextures[2].rect.y = iconTextures[2].rect.y+iconTextures[2].rect.h-hotkeysTextTextures[2].rect.h;
    SDL_RenderCopy(rend, hotkeysTextTextures[2].texture, NULL, &hotkeysTextTextures[2].rect);

    if(bombsFlag){
        SDL_DestroyTexture(spellsCdTextures[2].texture);
        sprintf(textBuffer, "%d", player.bombs);

        spellsCdTextures[2].texture = createTextTexture(font15, textBuffer, colorWhite, 2, font_outline15, colorBlack);
        SDL_QueryTexture(spellsCdTextures[2].texture, NULL, NULL, &spellsCdTextures[2].rect.w, &spellsCdTextures[2].rect.h);

        spellsCdTextures[2].rect.x = WINDOW_WIDTH-spellsCdTextures[2].rect.w-5;
        spellsCdTextures[2].rect.y = WINDOW_HEIGHT-spellsCdTextures[2].rect.h-3;
    }
    SDL_RenderCopy(rend, spellsCdTextures[2].texture, NULL, &spellsCdTextures[2].rect);

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

    // <<-- WALKA FINAŁOWA -->>
    if(player.stage == 8 && enemies[0] && enemies[0]->enemyType == 7 && enemies[0]->y >= 60){
        SDL_RenderCopy(rend, lifebarBackground, NULL, &lifebarBackgroundRect);

        if(bossLifeFlag){
            SDL_QueryTexture(lifebar, NULL, NULL, &lifebarRect.w, &lifebarRect.h);
            float lifePercent = (float)enemies[0]->life/enemyTypes[7].life;
            lifebarRect.w *= lifePercent;

            SDL_DestroyTexture(lifebarPercent);
            sprintf(textBuffer, "%3.f%%", lifePercent*100);
            lifebarPercent = createTextTexture(font25, textBuffer, colorWhite, 2, font_outline25, colorBlack);
            SDL_QueryTexture(lifebarPercent, NULL, NULL, &lifebarPercentRect.w, &lifebarPercentRect.h);
            lifebarPercentRect.x = lifebarBackgroundRect.x+lifebarBackgroundRect.w/2-lifebarPercentRect.w/2;
            lifebarPercentRect.y = 4;

            bossLifeFlag = 0;
        }

        SDL_RenderCopy(rend, lifebar, NULL, &lifebarRect);
        SDL_RenderCopy(rend, lifebarPercent, NULL, &lifebarPercentRect);

        if(bossFlag >= BOSS_BULLETS_COOLDOWN){
            addBullet(enemies[0]->x+16, enemies[0]->y+74, 9);
            addBullet(enemies[0]->x+115, enemies[0]->y+74, 9);
            bossFlag = 0;
        }

        if(bossBoxFlag >= BOSS_RANDOM_BOX_COOLDOWN){
            int box = rand()%BOX_TYPES;
            addBox((rand()%(WINDOW_WIDTH-80))+40, 0, box);

            bossBoxFlag = 0;
        }

    }

    //ikony bonusów
    boxRect.x = 10;
    boxRect.y = 8;
    for(int i = 1; i<BOX_TYPES-1; i++){
        if(player.timers[i]>0){
            SDL_RenderCopy(rend, boxTypes[i].texture, NULL, &boxRect);

            if(timerFlag){
                SDL_DestroyTexture(cooldownsTextTextures[i].texture);
                sprintf(textBuffer, "%d", player.timers[i]);

                cooldownsTextTextures[i].texture = createTextTexture(font20, textBuffer, colorWhite, 1, font_outline20, colorBlack);
                SDL_QueryTexture(cooldownsTextTextures[i].texture, NULL, NULL, &cooldownsTextTextures[i].rect.w, &cooldownsTextTextures[i].rect.h);
                cooldownsTextTextures[i].rect.x = boxRect.x+boxRect.w/2;
                cooldownsTextTextures[i].rect.y = boxRect.y+boxRect.h/2-3;
            }
            SDL_RenderCopy(rend, cooldownsTextTextures[i].texture, NULL, &cooldownsTextTextures[i].rect);

        }else{
            SDL_RenderCopy(rend, boxTypes[i].offTexture, NULL, &boxRect);
        }
        boxRect.x += boxRect.w+10;
    }

    //punkty (napis)
    if(scoreFlag){
        SDL_DestroyTexture(scoreTextTexture);
        sprintf(textBuffer, "Punkty: %d", player.score);
        scoreTextTexture = createTextTexture(font30, textBuffer, colorGold, 1, font_outline30, colorBlack);
        SDL_QueryTexture(scoreTextTexture, NULL, NULL, &scoreTextRect.w, &scoreTextRect.h);
        scoreTextRect.x = WINDOW_WIDTH-scoreTextRect.w-10;
        scoreTextRect.y = 0;
    }
    SDL_RenderCopy(rend, scoreTextTexture, NULL, &scoreTextRect);

    //licznik klatek
        frames_counter++;
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
                menuTextTextures[0].rect.x = WINDOW_WIDTH/2-menuTextTextures[0].rect.w/2;
                menuTextTextures[1].rect.x = WINDOW_WIDTH/2-menuTextTextures[1].rect.w/2;
                menuTextTextures[2].rect.x = WINDOW_WIDTH/2-menuTextTextures[2].rect.w/2;
                menuTextTextures[3].rect.x = WINDOW_WIDTH/2-menuTextTextures[3].rect.w/2;
                programStatus = 1;
                break;
            default:
                break;
            }
        }
    }

    //LEWA STRONA
    //ruch
    SDL_QueryTexture(instructionTextures[0].second_texture, NULL, NULL, &instructionTextures[0].rect.w, &instructionTextures[0].rect.h);
    instructionTextures[0].rect.x = 40;
    instructionTextures[0].rect.y = 120;
    SDL_RenderCopy(rend, instructionTextures[0].second_texture, NULL, &instructionTextures[0].rect);

    instructionTextures[0].rect.x += instructionTextures[0].rect.w+25;
    instructionTextures[0].rect.y += instructionTextures[0].rect.h/2-20;
    SDL_QueryTexture(instructionTextures[0].texture, NULL, NULL, &instructionTextures[0].rect.w, &instructionTextures[0].rect.h);
    SDL_RenderCopy(rend, instructionTextures[0].texture, NULL, &instructionTextures[0].rect);

    //Q
    SDL_QueryTexture(instructionTextures[1].second_texture, NULL, NULL, &instructionTextures[1].rect.w, &instructionTextures[1].rect.h);
    instructionTextures[1].rect.x = 105;
    instructionTextures[1].rect.y = 250;
    SDL_RenderCopy(rend, instructionTextures[1].second_texture, NULL, &instructionTextures[1].rect);

    instructionTextures[1].rect.x += instructionTextures[1].rect.w+85;
    instructionTextures[1].rect.y += instructionTextures[1].rect.h/2-20;
    SDL_QueryTexture(instructionTextures[1].texture, NULL, NULL, &instructionTextures[1].rect.w, &instructionTextures[1].rect.h);
    SDL_RenderCopy(rend, instructionTextures[1].texture, NULL, &instructionTextures[1].rect);

    //W
    SDL_QueryTexture(instructionTextures[2].second_texture, NULL, NULL, &instructionTextures[2].rect.w, &instructionTextures[2].rect.h);
    instructionTextures[2].rect.x = 105;
    instructionTextures[2].rect.y = 350;
    SDL_RenderCopy(rend, instructionTextures[2].second_texture, NULL, &instructionTextures[2].rect);

    instructionTextures[2].rect.x += instructionTextures[2].rect.w+85;
    instructionTextures[2].rect.y += instructionTextures[2].rect.h/2-20;
    SDL_QueryTexture(instructionTextures[2].texture, NULL, NULL, &instructionTextures[2].rect.w, &instructionTextures[2].rect.h);
    SDL_RenderCopy(rend, instructionTextures[2].texture, NULL, &instructionTextures[2].rect);

    //E
    SDL_QueryTexture(instructionTextures[3].second_texture, NULL, NULL, &instructionTextures[3].rect.w, &instructionTextures[3].rect.h);
    instructionTextures[3].rect.x = 105;
    instructionTextures[3].rect.y = 450;
    SDL_RenderCopy(rend, instructionTextures[3].second_texture, NULL, &instructionTextures[3].rect);

    instructionTextures[3].rect.x += instructionTextures[3].rect.w+85;
    instructionTextures[3].rect.y += instructionTextures[3].rect.h/2-20;
    SDL_QueryTexture(instructionTextures[3].texture, NULL, NULL, &instructionTextures[3].rect.w, &instructionTextures[3].rect.h);
    SDL_RenderCopy(rend, instructionTextures[3].texture, NULL, &instructionTextures[3].rect);

    //PRAWA STRONA
    //dodatkowe życie
    SDL_QueryTexture(boxTypes[0].texture, NULL, NULL, &boxRect2.w, &boxRect2.h);
    boxRect2.w *= 1.5;
    boxRect2.h *= 1.5;
    boxRect2.x = 530;
    boxRect2.y = 100;
    SDL_RenderCopy(rend, boxTypes[0].texture, NULL, &boxRect2);

    SDL_QueryTexture(instructionTextures[4].texture, NULL, NULL, &instructionTextures[4].rect.w, &instructionTextures[4].rect.h);
    instructionTextures[4].rect.x = 600;
    instructionTextures[4].rect.y = 105;
    SDL_RenderCopy(rend, instructionTextures[4].texture, NULL, &instructionTextures[4].rect);

    //ładunek bomby
    boxRect2.y += boxRect2.h+20;
    SDL_RenderCopy(rend, boxTypes[5].texture, NULL, &boxRect2);

    SDL_QueryTexture(instructionTextures[5].texture, NULL, NULL, &instructionTextures[5].rect.w, &instructionTextures[5].rect.h);
    instructionTextures[5].rect.x = instructionTextures[4].rect.x;
    instructionTextures[5].rect.y = instructionTextures[4].rect.y+boxRect2.h+20;
    SDL_RenderCopy(rend, instructionTextures[5].texture, NULL, &instructionTextures[5].rect);

    //bonusy
    SDL_QueryTexture(instructionTextures[6].texture, NULL, NULL, &instructionTextures[6].rect.w, &instructionTextures[6].rect.h);
    instructionTextures[6].rect.x = boxRect2.x;
    instructionTextures[6].rect.y = instructionTextures[5].rect.y+80;
    SDL_RenderCopy(rend, instructionTextures[6].texture, NULL, &instructionTextures[6].rect);

    //prędkość ataku
    boxRect2.y += boxRect2.h+95;
    SDL_RenderCopy(rend, boxTypes[1].texture, NULL, &boxRect2);

    SDL_QueryTexture(instructionTextures[7].texture, NULL, NULL, &instructionTextures[7].rect.w, &instructionTextures[7].rect.h);
    instructionTextures[7].rect.x = instructionTextures[5].rect.x;
    instructionTextures[7].rect.y = instructionTextures[6].rect.y+boxRect2.h+13;
    SDL_RenderCopy(rend, instructionTextures[7].texture, NULL, &instructionTextures[7].rect);

    //obrażenia
    boxRect2.y += boxRect2.h+20;
    SDL_RenderCopy(rend, boxTypes[2].texture, NULL, &boxRect2);

    SDL_QueryTexture(instructionTextures[8].texture, NULL, NULL, &instructionTextures[8].rect.w, &instructionTextures[8].rect.h);
    instructionTextures[8].rect.x = instructionTextures[7].rect.x;
    instructionTextures[8].rect.y = instructionTextures[7].rect.y+boxRect2.h+22;
    SDL_RenderCopy(rend, instructionTextures[8].texture, NULL, &instructionTextures[8].rect);

    //obrażenia
    boxRect2.y += boxRect2.h+20;
    SDL_RenderCopy(rend, boxTypes[3].texture, NULL, &boxRect2);

    SDL_QueryTexture(instructionTextures[9].texture, NULL, NULL, &instructionTextures[9].rect.w, &instructionTextures[9].rect.h);
    instructionTextures[9].rect.x = instructionTextures[8].rect.x;
    instructionTextures[9].rect.y = instructionTextures[8].rect.y+boxRect2.h+18;
    SDL_RenderCopy(rend, instructionTextures[9].texture, NULL, &instructionTextures[9].rect);

    //obrażenia
    boxRect2.y += boxRect2.h+20;
    SDL_RenderCopy(rend, boxTypes[4].texture, NULL, &boxRect2);

    SDL_QueryTexture(instructionTextures[10].texture, NULL, NULL, &instructionTextures[10].rect.w, &instructionTextures[10].rect.h);
    instructionTextures[10].rect.x = instructionTextures[9].rect.x;
    instructionTextures[10].rect.y = instructionTextures[9].rect.y+boxRect2.h+22;
    SDL_RenderCopy(rend, instructionTextures[10].texture, NULL, &instructionTextures[10].rect);

    //powrót
    instructionTextures[11].rect.x = WINDOW_WIDTH/2-instructionTextures[11].rect.w/2-20;
    instructionTextures[11].rect.y = WINDOW_HEIGHT-120;
    SDL_RenderCopy(rend, instructionTextures[11].texture, NULL, &instructionTextures[11].rect);
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
                switch(selectedOption){
                case 1:
                    programStatus=2;
                    selectedOption=1;
                    break;
                case 2:
                    bg_scrolling_speed = 0.5;
                    programStatus=1;
                    selectedOption=1;
                    menuTextTextures[0].rect.x = WINDOW_WIDTH/2-menuTextTextures[0].rect.w/2;
                    menuTextTextures[1].rect.x = WINDOW_WIDTH/2-menuTextTextures[1].rect.w/2;
                    menuTextTextures[2].rect.x = WINDOW_WIDTH/2-menuTextTextures[2].rect.w/2;
                    menuTextTextures[3].rect.x = WINDOW_WIDTH/2-menuTextTextures[3].rect.w/2;
                    break;
                }
                break;
            default:
                break;
            }
        }
    }

    //pauza
    SDL_RenderCopy(rend, pauseTextTextures[0].texture, NULL, &pauseTextTextures[0].rect);

    //NIE
    if(selectedOption==1)
        SDL_RenderCopy(rend, pauseTextTextures[2].second_texture, NULL, &pauseTextTextures[2].rect);
    else
    SDL_RenderCopy(rend, pauseTextTextures[2].texture, NULL, &pauseTextTextures[2].rect);

    //TAK
    if(selectedOption==2)
        SDL_RenderCopy(rend, pauseTextTextures[1].second_texture, NULL, &pauseTextTextures[1].rect);
    else
    SDL_RenderCopy(rend, pauseTextTextures[1].texture, NULL, &pauseTextTextures[1].rect);
}

void renderRanking(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        //zatrzymanie programu
        if(event.type == SDL_QUIT)
            program = 0;
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_ESCAPE:
                programStatus = 1;
                menuTextTextures[0].rect.x = WINDOW_WIDTH/2-menuTextTextures[0].rect.w/2;
                menuTextTextures[1].rect.x = WINDOW_WIDTH/2-menuTextTextures[1].rect.w/2;
                menuTextTextures[2].rect.x = WINDOW_WIDTH/2-menuTextTextures[2].rect.w/2;
                menuTextTextures[3].rect.x = WINDOW_WIDTH/2-menuTextTextures[3].rect.w/2;
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
                switch(selectedOption){
                case 1:
                    menuTextTextures[0].rect.x = WINDOW_WIDTH/2-menuTextTextures[0].rect.w/2;
                    menuTextTextures[1].rect.x = WINDOW_WIDTH/2-menuTextTextures[1].rect.w/2;
                    menuTextTextures[2].rect.x = WINDOW_WIDTH/2-menuTextTextures[2].rect.w/2;
                    menuTextTextures[3].rect.x = WINDOW_WIDTH/2-menuTextTextures[3].rect.w/2;
                    for(int i=0; i<4+RANKING_TOP; i++){
                        SDL_DestroyTexture(rankingTextTextures[i].texture);
                        SDL_DestroyTexture(rankingTextTextures[i].second_texture);
                    }
                    programStatus = 1;
                    selectedOption = 3;
                    break;
                case 2:
                    resetRanking();
                    saveRanking();
                    for(int i=0; i<4+RANKING_TOP; i++){
                        SDL_DestroyTexture(rankingTextTextures[i].texture);
                        SDL_DestroyTexture(rankingTextTextures[i].second_texture);
                    }
                    createRankingTextures();
                    break;
                }
                break;
            default:
                break;
            }
        }
    }

    //ranking
    SDL_Rect temp;
    SDL_QueryTexture(menuTextTextures[2].texture, NULL, NULL, &temp.w, &temp.h);
    temp.x = WINDOW_WIDTH/2-temp.w/2;
    temp.y = 50;
    SDL_RenderCopy(rend, menuTextTextures[2].texture, NULL, &temp);

    //nazwa gracza
    rankingTextTextures[0].rect.x = WINDOW_WIDTH/4-rankingTextTextures[0].rect.w/2;
    rankingTextTextures[0].rect.y = temp.y+temp.h+25;
    SDL_RenderCopy(rend, rankingTextTextures[0].texture, NULL, &rankingTextTextures[0].rect);

    //punkty
    rankingTextTextures[1].rect.x = 3*(WINDOW_WIDTH/4)-rankingTextTextures[1].rect.w/2;
    rankingTextTextures[1].rect.y = rankingTextTextures[0].rect.y;
    SDL_RenderCopy(rend, rankingTextTextures[1].texture, NULL, &rankingTextTextures[1].rect);

    //lista graczy
    rankingTextTextures[4].rect.x = rankingTextTextures[0].rect.x+30;
    rankingTextTextures[4].rect.y = rankingTextTextures[0].rect.y+75;
    temp.x = rankingTextTextures[1].rect.x+rankingTextTextures[1].rect.w/2-10;
    for(int i=0; i<RANKING_TOP; i++){
        if(i>0){
            rankingTextTextures[4+i].rect.x = rankingTextTextures[4+i-1].rect.x;
            rankingTextTextures[4+i].rect.y = rankingTextTextures[4+i-1].rect.y+55;
        }
        SDL_RenderCopy(rend, rankingTextTextures[4+i].texture, NULL, &rankingTextTextures[4+i].rect);
        SDL_QueryTexture(rankingTextTextures[4+i].second_texture, NULL, NULL, &temp.w, &temp.h);
        temp.y = rankingTextTextures[4+i].rect.y;
        SDL_RenderCopy(rend, rankingTextTextures[4+i].second_texture, NULL, &temp);
    }

    //powrót
    rankingTextTextures[2].rect.x = 70;
    rankingTextTextures[2].rect.y = WINDOW_HEIGHT-150;
    if(selectedOption == 1)
        SDL_RenderCopy(rend, rankingTextTextures[2].second_texture, NULL, &rankingTextTextures[2].rect);
    else
        SDL_RenderCopy(rend, rankingTextTextures[2].texture, NULL, &rankingTextTextures[2].rect);

    //resetuj ranking
    rankingTextTextures[3].rect.x = WINDOW_WIDTH-rankingTextTextures[3].rect.w-70;
    rankingTextTextures[3].rect.y = rankingTextTextures[2].rect.y;
    if(selectedOption == 2)
        SDL_RenderCopy(rend, rankingTextTextures[3].second_texture, NULL, &rankingTextTextures[3].rect);
    else
        SDL_RenderCopy(rend, rankingTextTextures[3].texture, NULL, &rankingTextTextures[3].rect);

}

void renderEnd(int mode){
    int nameFlag = 0;
    SDL_Event event;
    switch(mode){
    case 0:
        while(SDL_PollEvent(&event)){
            //zatrzymanie programu
            if(event.type == SDL_QUIT)
                program = 0;
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.scancode){
                case SDL_SCANCODE_ESCAPE:
                case SDL_SCANCODE_SPACE:
                case SDL_SCANCODE_RETURN:
                    for(int i=0; i<7; i++){
                        SDL_DestroyTexture(endTextTextures[i].texture);
                        SDL_DestroyTexture(endTextTextures[i].second_texture);
                    }
                    programStatus = 1;
                    selectedOption = 1;
                    break;
                default:
                    break;
                }
            }
        }

        //koniec gry
        endTextTextures[0].rect.x = WINDOW_WIDTH/2-endTextTextures[0].rect.w/2;
        endTextTextures[0].rect.y = 70;
        SDL_RenderCopy(rend, endTextTextures[0].texture, NULL, &endTextTextures[0].rect);

        //punkty
        endTextTextures[1].rect.x = WINDOW_WIDTH/2-endTextTextures[1].rect.w/2;
        endTextTextures[1].rect.y = endTextTextures[0].rect.y+70;
        SDL_RenderCopy(rend, endTextTextures[1].texture, NULL, &endTextTextures[1].rect);

        endTextTextures[2].rect.x = WINDOW_WIDTH/2-endTextTextures[2].rect.w/2;
        endTextTextures[2].rect.y = endTextTextures[1].rect.y+60;
        SDL_RenderCopy(rend, endTextTextures[2].texture, NULL, &endTextTextures[2].rect);

        endTextTextures[3].rect.x = WINDOW_WIDTH/2-endTextTextures[3].rect.w/2;
        endTextTextures[3].rect.y = endTextTextures[2].rect.y+50;
        SDL_RenderCopy(rend, endTextTextures[3].texture, NULL, &endTextTextures[3].rect);

        endTextTextures[4].rect.x = WINDOW_WIDTH/2-endTextTextures[4].rect.w/2;
        endTextTextures[4].rect.y = endTextTextures[3].rect.y+70;
        SDL_RenderCopy(rend, endTextTextures[4].texture, NULL, &endTextTextures[4].rect);

        //potrzebna ilość punktów
        endTextTextures[5].rect.x = WINDOW_WIDTH/2-endTextTextures[5].rect.w/2;
        endTextTextures[5].rect.y = endTextTextures[4].rect.y+170;
        SDL_RenderCopy(rend, endTextTextures[5].texture, NULL, &endTextTextures[5].rect);

        endTextTextures[6].rect.x = WINDOW_WIDTH/2-endTextTextures[6].rect.w/2;
        endTextTextures[6].rect.y = endTextTextures[5].rect.y+70;
        SDL_RenderCopy(rend, endTextTextures[6].texture, NULL, &endTextTextures[6].rect);

        break;
    case 1:
        while(SDL_PollEvent(&event)){
            //zatrzymanie programu
            if(event.type == SDL_QUIT)
                program = 0;
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.scancode){
                case SDL_SCANCODE_ESCAPE:
                case SDL_SCANCODE_RETURN:
                    if(strcmp(player.name, " ") != 0){
                        programStatus = 5;
                        selectedOption = 1;
                        ranking[RANKING_TOP-1].score = player.score;
                        strcpy(ranking[RANKING_TOP-1].name, player.name);
                        sortRanking();
                        saveRanking();
                        createRankingTextures();
                        for(int i=0; i<7; i++){
                            SDL_DestroyTexture(endTextTextures[i].texture);
                            SDL_DestroyTexture(endTextTextures[i].second_texture);
                        }
                    }
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    if(strlen(player.name) > 1){
                        player.name[strlen(player.name) - 1] = '\0';
                        nameFlag = 1;
                    }else if(strlen(player.name) == 1){
                        strcpy(player.name, " ");
                        nameFlag = 1;
                    }
                    break;
                default:
                    break;
                }

            }else if(event.type == SDL_TEXTINPUT && strlen(player.name)<RANKING_NAME_CHARS-1){
                int forbiddenSymbol = 0;
                for(int i = 0; i<FORBIDDEN_SYMBOLS; i++){
                    if(strcmp(event.text.text, forbidden_symbols[i]) == 0)
                        forbiddenSymbol = 1;
                }
                if(!forbiddenSymbol){
                    strcat(player.name, event.text.text);
                    nameFlag = 1;
                }
            }
        }

        //koniec gry
        endTextTextures[0].rect.x = WINDOW_WIDTH/2-endTextTextures[0].rect.w/2;
        endTextTextures[0].rect.y = 50;
        SDL_RenderCopy(rend, endTextTextures[0].texture, NULL, &endTextTextures[0].rect);

        //punkty
        endTextTextures[1].rect.x = WINDOW_WIDTH/2-endTextTextures[1].rect.w/2;
        endTextTextures[1].rect.y = endTextTextures[0].rect.y+70;
        SDL_RenderCopy(rend, endTextTextures[1].texture, NULL, &endTextTextures[1].rect);

        endTextTextures[2].rect.x = WINDOW_WIDTH/2-endTextTextures[2].rect.w/2;
        endTextTextures[2].rect.y = endTextTextures[1].rect.y+60;
        SDL_RenderCopy(rend, endTextTextures[2].texture, NULL, &endTextTextures[2].rect);

        endTextTextures[3].rect.x = WINDOW_WIDTH/2-endTextTextures[3].rect.w/2;
        endTextTextures[3].rect.y = endTextTextures[2].rect.y+50;
        SDL_RenderCopy(rend, endTextTextures[3].texture, NULL, &endTextTextures[3].rect);

        endTextTextures[4].rect.x = WINDOW_WIDTH/2-endTextTextures[4].rect.w/2;
        endTextTextures[4].rect.y = endTextTextures[3].rect.y+70;
        SDL_RenderCopy(rend, endTextTextures[4].texture, NULL, &endTextTextures[4].rect);

        //gratulacje
        endTextTextures[7].rect.x = WINDOW_WIDTH/2-endTextTextures[7].rect.w/2;
        endTextTextures[7].rect.y = endTextTextures[4].rect.y+150;
        SDL_RenderCopy(rend, endTextTextures[7].texture, NULL, &endTextTextures[7].rect);

        //nazwa gracza
        endTextTextures[8].rect.x = WINDOW_WIDTH/2-endTextTextures[8].rect.w/2;
        endTextTextures[8].rect.y = endTextTextures[7].rect.y+70;
        if(nameFlag){
            SDL_DestroyTexture(endTextTextures[8].texture);
            endTextTextures[8].texture = createTextTexture(font50, player.name, colorGold, 0, font_outline50, colorBlack);
            SDL_QueryTexture(endTextTextures[8].texture, NULL, NULL, &endTextTextures[8].rect.w, &endTextTextures[8].rect.h);
        }
        SDL_RenderCopy(rend, endTextTextures[8].texture, NULL, &endTextTextures[8].rect);

        //potwierdź
        endTextTextures[9].rect.x = WINDOW_WIDTH/2-endTextTextures[9].rect.w/2;
        endTextTextures[9].rect.y = endTextTextures[8].rect.y+90;
        SDL_RenderCopy(rend, endTextTextures[9].texture, NULL, &endTextTextures[9].rect);


        break;
    }
}
