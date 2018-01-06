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

void onExit();
void renderGame();
void renderMenu();
void renderPause();

//TO DO
void renderWin();
void renderLose();

int main( int argc, char* args[] ) {
    //inicjalizacja SDL, TTF, stworzenie okna i renderu
    if(init()!=0) return 1;

    //stworzenie potrzebnych tekstur
    createTexturesAndRects();
    //zainicjowanie statystyk wrogów
    allEnemyInit();
    //zainicjowanie wartości pocisków
    allBulletsInit();
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
            renderPause();
            break;
        case 4:
//            renderWin();
            break;
        case 5:
//            renderLose();
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
    SDL_DestroyTexture(scoreTextTexture);
    SDL_DestroyTexture(startTextTexture);
    SDL_DestroyTexture(helpTextTexture);
    SDL_DestroyTexture(endTextTexture);
    SDL_DestroyTexture(pauseTextTexture);
    SDL_DestroyTexture(yesTextTexture);
    SDL_DestroyTexture(noTextTexture);
    SDL_DestroyTexture(explosionTexture);
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
            case SDL_SCANCODE_A:
                selectedOption--;
                if(selectedOption<1) selectedOption=3;
                break;
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D:
                selectedOption++;
                if(selectedOption>3) selectedOption=1;
                break;
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W:
                selectedOption--;
                if(selectedOption<1) selectedOption=3;
                break;
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S:
                selectedOption++;
                if(selectedOption>3) selectedOption=1;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_C:
                switch(selectedOption){
                case 1:
                    programStatus = 2;
                    deadTimer = DEAD_TIMER;
                    gameEnd = 0;
                    global_timer = 0;
                    //załadowanie podstawowych wartości gracza
                    addPlayerBasicStats();
                    bg_scrolling_speed = 2;
                    cleanMap();
                    selectedOption=1;
                    break;
                case 2:
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
                programStatus  = 3;
                selectedOption = 1;
                break;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                player.up = 1;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                player.down = 1;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                player.right = 1;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                player.left = 1;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_C:
                player.fire = 1;
                break;
            default:
                break;
            }
        }
        if(event.type == SDL_KEYUP){
            switch(event.key.keysym.scancode){
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                player.up = 0;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                player.down = 0;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                player.right = 0;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                player.left = 0;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_C:
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
    for(int i=0;i<MAX_ENEMIES;i++){
        if(enemies[i]){
            if(enemies[i]->cooldown<=0)
                enemies[i]->cooldown = 0;
            else
                enemies[i]->cooldown--;
        }
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

    //gracz
    if(player.life>0)
        SDL_RenderCopy(rend, playerTexture, NULL, &playerRect);

    //ruch pocisków gracza
    for(int i=0;i<MAX_BULLETS;i++){
        if(allBullets[i] && allBullets[i]->bulletType == 0){
            int bType = allBullets[i]->bulletType;

            bulletTypes[bType].bulletRect.x = allBullets[i]->x;
            bulletTypes[bType].bulletRect.y = allBullets[i]->y;
            SDL_RenderCopy(rend, bulletTypes[bType].texture, NULL, &bulletTypes[bType].bulletRect);

            allBullets[i]->x = allBullets[i]->x_start + sin(allBullets[i]->y_helper)*player.bullet_amplitude;
            allBullets[i]->y_helper += SINUS_ACCURACY;
            allBullets[i]->y -= player.bullet_speed;

            if(allBullets[i]->y<=0){
                removeBullet(i);
                continue;
            }

            for(int j=0;j<MAX_ENEMIES;j++){
                if(enemies[j]){
                    int type = enemies[j]->enemyType;

                    collisionUpdate();
                    int n = bulletTypes[bType].collisionBoxesCount;
                    int m = enemyTypes[type].collisionBoxesCount;

                    if(collisionCheck(n, allBullets[i]->collisionBoxes, m, enemies[j]->collisionBoxes)){
                        enemies[j]->life-=player.damage;
                        addExplosion(allBullets[i]->x+bulletTypes[bType].bulletRect.w/2-128, allBullets[i]->y+bulletTypes[bType].bulletRect.h/2-128, 0, 29);
                        removeBullet(i);
                        if(enemies[j]->life<=0){
                            player.score += enemyTypes[type].score;
                            addExplosion(enemies[j]->x+enemyTypes[type].enemyRect.w/2-128, enemies[j]->y+enemyTypes[type].enemyRect.h/2-128, 0, 29);
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
    if(stageCleared){
        if(player.stage>LAST_STAGE){
            program = 0;
            return;
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
            enemyTypes[type].enemyRect.x = enemies[i]->x;
            enemyTypes[type].enemyRect.y = enemies[i]->y;

            collisionUpdate();
            int n = enemyTypes[type].collisionBoxesCount;
            int m = player.collisionBoxesCount;

            if(collisionCheck(n, enemies[i]->collisionBoxes, m, player.collisionBoxes)){
                player.life-=enemyTypes[type].damage;
                addExplosion(enemies[i]->x+enemyTypes[type].enemyRect.w/2-128, enemies[i]->y+enemyTypes[type].enemyRect.h/2-128, 0, 29);
                addExplosion(player.x+playerRect.w/2-128, player.y+playerRect.h/2-128, 0, 29);
                addExplosion(heartRect.x+heartRect.w/2-128, heartRect.y+heartRect.h/2-128, 0, 29);
                removeEnemy(i);
                continue;
            }else
                SDL_RenderCopy(rend, enemyTypes[type].texture, NULL, &enemyTypes[type].enemyRect);

            if(enemies[0] && enemies[0]->y<60){
                for(int j=0;j<MAX_ENEMIES;j++){
                    if(enemies[j] ) enemies[j]->y+=0.3;
                }
            }else{
                if(enemies[i]->cooldown<=0){
                    int roll;
                    for(int l=0;l<5;l++)
                        roll = rand();
                    roll=roll%2000+1;
                    if(roll<=enemyTypes[type].bulletChance){
                        addBullet(enemies[i]->x+enemyTypes[type].enemyRect.w/2, enemies[i]->y+enemyTypes[type].enemyRect.h-5, enemyTypes[type].bulletType);
                        enemies[i]->cooldown = enemyTypes[type].cooldownTime;
                    }
                }

                enemies[i]->x += enemies[i]->x_vel;
                if(enemies[i]->x+enemyTypes[type].enemyRect.w >= WINDOW_WIDTH || enemies[i]->x <= 0){
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
        if(allBullets[i] && allBullets[i]->bulletType > 0){
            int bType = allBullets[i]->bulletType;

            bulletTypes[bType].bulletRect.x = allBullets[i]->x;
            bulletTypes[bType].bulletRect.y = allBullets[i]->y;
            SDL_RenderCopyEx(rend,bulletTypes[bType].texture,NULL,&bulletTypes[bType].bulletRect,allBullets[i]->bullet_angle,NULL,SDL_FLIP_NONE);
            allBullets[i]->bullet_angle += bulletTypes[bType].spin_speed;
            if(allBullets[i]->bullet_angle>=360){
                allBullets[i]->bullet_angle-=360;
            }

            allBullets[i]->x = allBullets[i]->x_start - sin(allBullets[i]->y_helper)*bulletTypes[bType].bullet_amplitude;
            allBullets[i]->y_helper += SINUS_ACCURACY;
            allBullets[i]->y += bulletTypes[bType].bullet_speed;

            if(allBullets[i]->y>=WINDOW_HEIGHT){
                removeBullet(i);
                continue;
            }

            collisionUpdate();
            int n = bulletTypes[bType].collisionBoxesCount;
            int m = player.collisionBoxesCount;

            if(collisionCheck(n, allBullets[i]->collisionBoxes, m, player.collisionBoxes)){
                player.life-=bulletTypes[bType].bullet_damage;
                addExplosion(allBullets[i]->x+bulletTypes[bType].bulletRect.w/2-128, allBullets[i]->y+bulletTypes[bType].bulletRect.h/2-128, 0, 29);
                addExplosion(heartRect.x+heartRect.w/2-128, heartRect.y+heartRect.h/2-128, 0, 29);
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

    //zmiana klatek eksplozji
    for(int i=0;i<MAX_EXPLOSIONS;i++){
        if(explosions[i]){
            explosionRect.x = explosions[i]->x;
            explosionRect.y = explosions[i]->y;
            frame.x = 0;
            frame.y = 0;
            for(int j=0;j<explosions[i]->frame%5;j++)
                frame.x+=frame.w;
            for(int j=0;j<explosions[i]->frame/5;j++)
                frame.y+=frame.h;
            SDL_RenderCopy(rend, explosionTexture, &frame, &explosionRect);
            explosions[i]->frame++;
            if(explosions[i]->frame==explosions[i]->lastFrame)
                removeExplosion(i);
        }
    }

    SDL_RenderCopy(rend, hudTexture, NULL, &hudRect2);

    //napis PUNKTY
    font = TTF_OpenFont("font.ttf", 30);
    sprintf(scoreBuffer, "Punkty: %d", player.score);
    scoreTextTexture = createTextTexture(font, scoreBuffer, colorGold);
    SDL_QueryTexture(scoreTextTexture, NULL, NULL, &scoreTextRect.w, &scoreTextRect.h);
    scoreTextRect.x = WINDOW_WIDTH-scoreTextRect.w-10;
    scoreTextRect.y = 2;
    SDL_RenderCopy(rend, scoreTextTexture, NULL, &scoreTextRect);
    SDL_DestroyTexture(scoreTextTexture);
    TTF_CloseFont(font);

    //licznik klatek
    global_timer++;

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
            case SDL_SCANCODE_A:
                selectedOption--;
                if(selectedOption<1) selectedOption=2;
                break;
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D:
                selectedOption++;
                if(selectedOption>2) selectedOption=1;
                break;
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W:
                selectedOption--;
                if(selectedOption<1) selectedOption=2;
                break;
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S:
                selectedOption++;
                if(selectedOption>2) selectedOption=1;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_C:
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
