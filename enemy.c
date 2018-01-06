#include "enemy.h"

#include <stdio.h>
#include <stdlib.h>

EnemyType enemyTypes[ENEMY_TYPES];
Enemy *enemies[MAX_ENEMIES];

void enemyInit(int type, int life, int damage, int score, float x_vel, float y_vel, int bulletType, int bulletChance, int cooldownTime, int collisionBoxesCount){
    enemyTypes[type].life   = life;
    enemyTypes[type].damage = damage;
    enemyTypes[type].score  = score;
    enemyTypes[type].x_vel  = x_vel;
    enemyTypes[type].y_vel  = y_vel;
    enemyTypes[type].bulletChance  = bulletChance;
    enemyTypes[type].cooldownTime  = cooldownTime;
    enemyTypes[type].bulletType  = bulletType;
    enemyTypes[type].collisionBoxesCount  = collisionBoxesCount;
}

void allEnemyInit(){
    // LISTA WROGÓW
    //        TYPE, LIFE, DAMAGE, SCORE, X_VEL, Y_VEL, BTYPE, BCHANCE, CDN, COLLBOXCOUNT
    enemyInit(0,    1,    1,      0,     1,     1,     0,     0,       0,   2);   // szary (template)
    enemyInit(1,    1,    1,      15,    0.7,   25,    1,     5,       500, 2); // czerwony
    enemyInit(2,    2,    1,      30,    0.7,   25,    2,     6,       500, 2); // żółty
    enemyInit(3,    3,    1,      45,    0.7,   25,    3,     7,       300, 2); // zielony
    enemyInit(4,    4,    1,      55,    1.7,   0,     4,     20,      50,  2); // niebieski jasny
    enemyInit(5,    5,    1,      70,    0.7,   25,    5,     9,       300, 2); // różowy
    enemyInit(6,    6,    1,      90,    0.7,   25,    6,     10,      300, 2); // niebieski
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
    int enemyMap[LAST_STAGE][4][10] =  {
                                        { //fala 1
                                         {0,0,1,0,1,1,0,1,0,0},
                                         {0,1,0,1,0,0,1,0,1,0},
                                         {0,0,1,1,1,1,1,1,0,0},
                                         {0,0,0,0,1,1,0,0,0,0}
                                        },
                                        { //fala 2
                                         {0,0,1,0,1,1,0,1,0,0},
                                         {0,1,2,1,2,2,1,2,1,0},
                                         {0,0,1,2,1,1,2,1,0,0},
                                         {0,0,0,1,1,1,1,0,0,0}
                                        },
                                        { //fala 3
                                         {0,1,0,3,2,3,0,1,0,0},
                                         {0,2,1,0,1,0,1,2,0,0},
                                         {0,0,2,1,2,1,2,0,0,0},
                                         {0,0,0,2,0,2,0,0,0,0}
                                        },
                                        { //fala 4
                                         {0,0,0,0,4,4,0,0,0,0},
                                         {0,1,3,1,2,2,1,3,1,0},
                                         {0,0,1,2,3,3,2,1,0,0},
                                         {0,0,0,1,2,2,1,0,0,0}
                                        },
                                        { //fala 5
                                         {0,0,4,0,4,0,4,0,0,0},
                                         {0,3,0,3,5,3,0,3,0,0},
                                         {0,3,2,3,2,3,2,3,0,0},
                                         {3,0,0,0,0,0,0,0,3,0}
                                        },
                                        { //fala 6
                                         {0,0,0,0,4,0,0,0,0,0},
                                         {5,1,2,3,0,3,2,1,5,0},
                                         {0,3,0,5,6,5,0,3,0,0},
                                         {0,0,0,0,0,0,0,0,0,0}
                                        },
                                        { //fala 7
                                         {4,0,4,0,4,0,4,0,4,0},
                                         {0,4,0,4,0,4,0,4,0,4},
                                         {1,1,1,1,1,1,1,1,1,1},
                                         {0,0,0,0,0,0,0,0,0,0}
                                        },
                                        { //fala 8
                                         {6,5,0,0,1,0,0,5,6,0},
                                         {0,6,5,0,0,0,5,6,0,0},
                                         {0,0,6,5,0,5,6,0,0,0},
                                         {0,0,0,6,5,6,0,0,0,0}
                                        }
                                       };

    int x = 85, y = -400;
    int type;

    for(int i=0;i<4;i++){
        for(int j=0;j<10;j++){
            type = enemyMap[stage-1][i][j];
            if(type!=0){
                addEnemy(type, x, y, enemyTypes[type].x_vel, enemyTypes[type].y_vel, enemyTypes[type].life);
                x+=enemyTypes[type].enemyRect.w+15;
            }else
                x+=enemyTypes[type].enemyRect.w+15;
        }
        x=85;
        y+=enemyTypes[type].enemyRect.h+25;
    }
}

void enemyCollisionUpdate(){
    for(int i=0; i<MAX_ENEMIES;i++){
        if(enemies[i]){
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
