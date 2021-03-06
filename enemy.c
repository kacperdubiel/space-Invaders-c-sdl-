#include "enemy.h"

#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "init.h"

Enemy *enemies[MAX_ENEMIES];
EnemyType enemyTypes[ENEMY_TYPES];

void enemyInit(int type, int life, int damage, int score, float x_vel, float y_vel, int bulletType, int bulletChance, int cooldownTime, int collisionBoxesCount, float dropChanceMultiplier){
    enemyTypes[type].life   = life;
    enemyTypes[type].damage = damage;
    enemyTypes[type].score  = score;
    enemyTypes[type].x_vel  = x_vel;
    enemyTypes[type].y_vel  = y_vel;
    enemyTypes[type].bulletChance  = bulletChance;
    enemyTypes[type].cooldownTime  = cooldownTime;
    enemyTypes[type].bulletType  = bulletType;
    enemyTypes[type].collisionBoxesCount  = collisionBoxesCount;
    enemyTypes[type].dropChanceMultiplier  = dropChanceMultiplier;
}

void allEnemyInit(){
    // LISTA WROGÓW
    //        TYPE, LIFE, DAMAGE,   SCORE, X_VEL, Y_VEL, BTYPE, BCHANCE, CDN,  COLLBOXES, DROPMULT
    enemyInit(0,    1,    1,        0,     1,     1,     0,     0,       0,    2,         1);   // szary (template)
    enemyInit(1,    1,    1,        15,    0.7,   25,    1,     5,       500,  2,         0.8); // czerwony
    enemyInit(2,    2,    1,        30,    0.7,   25,    2,     6,       500,  2,         1.1); // żółty
    enemyInit(3,    3,    1,        45,    0.7,   25,    3,     7,       300,  2,         1.6); // zielony
    enemyInit(4,    4,    1,        55,    1.7,   0,     4,     20,      50,   2,         1.7); // niebieski jasny
    enemyInit(5,    5,    1,        70,    0.7,   25,    5,     9,       300,  2,         2.0); // różowy
    enemyInit(6,    6,    1,        90,    0.7,   25,    6,     10,      300,  2,         2.3); // niebieski
    enemyInit(7,    90,   MAX_LIFE, 1000,  0.7,   0,     8,     1000,    70,   5,         0);   // boss
    enemyInit(8,    10,   2,        100,   0,     0,     10,    1000,    200,  3,         1000);// pomocnik bossa
}

void addEnemy(int enemyType, float x, float y, float x_vel, float y_vel, int life){
    int found = -1;
    for(int i=0;i<MAX_ENEMIES;i++){
        if(enemies[i]==NULL){
            found = i;
            break;
        }
    }
    if(found>=0){
        int i = found;
        enemies[i] = malloc(sizeof(Enemy));
        enemies[i]->enemyType = enemyType;
        enemies[i]->x = x;
        enemies[i]->y = y;
        enemies[i]->x_vel = x_vel;
        enemies[i]->y_vel = y_vel;
        enemies[i]->life = life;
        enemies[i]->cooldown = STARTING_COOLDOWN;
        enemies[i]->freeze = 0;
        printf("Created Enemy(%d)[%d;%d]\n",i,(int)enemies[i]->x,(int)enemies[i]->y);
    }
}

void removeEnemy(int i){
    if(enemies[i]){
        printf("Destroyed Enemy(%d)[%d;%d]\n",i,(int)enemies[i]->x,(int)enemies[i]->y);
        free(enemies[i]);
        enemies[i]=NULL;
    }
}

void addEnemies(int stage){
    if(stage>0 && stage<=LAST_STAGE){
        int enemyMap[LAST_STAGE][4][10] =  {
                                            { //fala 1
                                             {0,0,1,0,1,1,0,1,0,0},
                                             {0,1,0,2,0,0,2,0,1,0},
                                             {0,0,1,1,0,0,1,1,0,0},
                                             {0,0,0,0,1,1,0,0,0,0}
                                            },
                                            { //fala 2
                                             {0,0,1,0,3,3,0,1,0,0},
                                             {0,1,2,1,2,2,1,2,1,0},
                                             {0,0,1,2,1,1,2,1,0,0},
                                             {0,0,0,1,0,0,1,0,0,0}
                                            },
                                            { //fala 3
                                             {0,0,0,0,4,4,0,0,0,0},
                                             {0,1,3,1,2,2,1,3,1,0},
                                             {0,0,1,2,3,3,2,1,0,0},
                                             {0,0,0,0,2,2,0,0,0,0}
                                            },
                                            { //fala 4
                                             {0,0,4,0,4,0,4,0,0,0},
                                             {0,3,0,3,5,3,0,3,0,0},
                                             {0,3,2,3,2,3,2,3,0,0},
                                             {3,0,0,0,0,0,0,0,3,0}
                                            },
                                            { //fala 5
                                             {0,0,0,0,4,0,0,0,0,0},
                                             {5,1,2,3,0,3,2,1,5,0},
                                             {0,3,0,5,6,5,0,3,0,0},
                                             {0,0,0,0,0,0,0,0,0,0}
                                            },
                                            { //fala 6
                                             {4,0,4,0,4,0,4,0,4,0},
                                             {0,4,0,4,0,4,0,4,0,4},
                                             {1,1,1,1,1,1,1,1,1,1},
                                             {0,0,0,0,0,0,0,0,0,0}
                                            },
                                            { //fala 7
                                             {6,5,0,1,1,1,0,5,6,0},
                                             {0,6,5,0,0,0,5,6,0,0},
                                             {0,0,6,5,0,5,6,0,0,0},
                                             {0,0,0,6,5,6,0,0,0,0}
                                            },
                                            { //fala 8
                                             {0,0,0,0,7,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0},
                                             {0,8,0,0,0,0,0,0,0,8},
                                             {0,0,0,0,0,0,0,0,0,0}
                                            }
                                           };

        int x = 85, y = -400;
        int type;

        for(int i=0;i<4;i++){
            for(int j=0;j<10;j++){
                type = enemyMap[stage-1][i][j];
                if(type!=0){
                    addEnemy(type, x, y, enemyTypes[type].x_vel, enemyTypes[type].y_vel, enemyTypes[type].life);
                    x+=enemyTypes[type].rect.w+15;
                }else
                    x+=enemyTypes[type].rect.w+15;
            }
            x=85;
            y+=enemyTypes[type].rect.h+25;
        }
    }
}

void enemyCollisionUpdate(){
    for(int i=0; i<MAX_ENEMIES;i++){
        if(enemies[i]){
            int type = enemies[i]->enemyType;

            if(type == 7){
                enemies[i]->collisionBoxes[0].x = enemies[i]->x+2;
                enemies[i]->collisionBoxes[0].y = enemies[i]->y;
                enemies[i]->collisionBoxes[0].w = 163;
                enemies[i]->collisionBoxes[0].h = 26;

                enemies[i]->collisionBoxes[1].x = enemies[i]->x+12;
                enemies[i]->collisionBoxes[1].y = enemies[i]->y+28;
                enemies[i]->collisionBoxes[1].w = 145;
                enemies[i]->collisionBoxes[1].h = 19;

                enemies[i]->collisionBoxes[2].x = enemies[i]->x+20;
                enemies[i]->collisionBoxes[2].y = enemies[i]->y+42;
                enemies[i]->collisionBoxes[2].w = 129;
                enemies[i]->collisionBoxes[2].h = 30;

                enemies[i]->collisionBoxes[3].x = enemies[i]->x+69;
                enemies[i]->collisionBoxes[3].y = enemies[i]->y+71;
                enemies[i]->collisionBoxes[3].w = 32;
                enemies[i]->collisionBoxes[3].h = 38;

                enemies[i]->collisionBoxes[4].x = enemies[i]->x+76;
                enemies[i]->collisionBoxes[4].y = enemies[i]->y+105;
                enemies[i]->collisionBoxes[4].w = 16;
                enemies[i]->collisionBoxes[4].h = 21;
            }else if(type == 8){
                enemies[i]->collisionBoxes[0].x = enemies[i]->x+1;
                enemies[i]->collisionBoxes[0].y = enemies[i]->y+3;
                enemies[i]->collisionBoxes[0].w = 66;
                enemies[i]->collisionBoxes[0].h = 25;

                enemies[i]->collisionBoxes[1].x = enemies[i]->x+19;
                enemies[i]->collisionBoxes[1].y = enemies[i]->y+27;
                enemies[i]->collisionBoxes[1].w = 31;
                enemies[i]->collisionBoxes[1].h = 26;

                enemies[i]->collisionBoxes[2].x = enemies[i]->x+7;
                enemies[i]->collisionBoxes[2].y = enemies[i]->y+34;
                enemies[i]->collisionBoxes[2].w = 55;
                enemies[i]->collisionBoxes[2].h = 9;
            }else{
                enemies[i]->collisionBoxes[0].x = enemies[i]->x;
                enemies[i]->collisionBoxes[0].y = enemies[i]->y;
                enemies[i]->collisionBoxes[0].w = 60;
                enemies[i]->collisionBoxes[0].h = 39;

                enemies[i]->collisionBoxes[1].x = enemies[i]->x+21;
                enemies[i]->collisionBoxes[1].y = enemies[i]->y+34;
                enemies[i]->collisionBoxes[1].w = 19;
                enemies[i]->collisionBoxes[1].h = 30;
            }
        }
    }
}
