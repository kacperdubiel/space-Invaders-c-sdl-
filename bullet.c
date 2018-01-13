#include "bullet.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "player.h"

Bullet *bullets[MAX_BULLETS];
BulletType bulletTypes[BULLET_TYPES];

void addBullet(float x, float y, int type){
    int found = -1;
    for(int i=0;i<MAX_BULLETS;i++){
        if(bullets[i]==NULL){
            found = i;
            break;
        }
    }
    if(found>=0){
        bullets[found] = malloc(sizeof(Bullet));
        bullets[found]->x = x;
        bullets[found]->y = y;
        if(type == 10){
            float x_len = player.x+playerRect.w/2 - x-15;
            float y_len = player.y+playerRect.h/2 - y-15;

            float hyp = sqrt(x_len*x_len + y_len*y_len);

            bullets[found]->x_vel = (x_len/hyp)*bulletTypes[type].bullet_speed;
            bullets[found]->y_vel = (y_len/hyp)*bulletTypes[type].bullet_speed;
        }else{
            bullets[found]->x_vel = 0;
            bullets[found]->y_vel = 0;
        }
        bullets[found]->x_start = x;
        bullets[found]->y_start = y;
        bullets[found]->bulletType = type;
        if(type==0){ //pocisk gracza
            bullets[found]->bullet_amplitude = player.bullet_amplitude;
            bullets[found]->bullet_speed = player.bullet_speed;
            bullets[found]->bullet_damage = player.damage;
        }else{       //wszystkie inne pociski
            bullets[found]->bullet_amplitude = bulletTypes[type].bullet_amplitude;
            bullets[found]->bullet_speed = bulletTypes[type].bullet_speed;
            bullets[found]->bullet_damage = bulletTypes[type].bullet_damage;
        }
        bullets[found]->bullet_angle = bulletTypes[type].bullet_angle;
        bullets[found]->freeze = 0;
    }
}

void removeBullet(int i){
    if(bullets[i]){
        free(bullets[i]);
        bullets[i]=NULL;
    }
}

void bulletInit(int type, int bullet_amplitude, float bullet_speed, int bullet_damage, int bullet_angle, int spin_speed, int collisionBoxesCount){
    bulletTypes[type].bullet_amplitude = bullet_amplitude;
    bulletTypes[type].bullet_speed = bullet_speed;
    bulletTypes[type].bullet_damage = bullet_damage;
    bulletTypes[type].bullet_angle = bullet_angle;
    bulletTypes[type].spin_speed = spin_speed;
    bulletTypes[type].collisionBoxesCount = collisionBoxesCount;
}

void allBulletsInit(){
    // LISTA POCISKÓW
    //        TYPE,  AMP,  SPD,  DMG, ANGLE, SPIN_SPD, COLLBOXCOUNT
    bulletInit(0,    35,   1,     1,   0,     0,        1);
    bulletInit(1,    5,    1,     1,   0,     0,        1);
    bulletInit(2,    2,    1,     1,   0,     0,        1);
    bulletInit(3,    10,   1,     1,   0,     0,        1);
    bulletInit(4,    2,    2,     1,   150,   10,       1);
    bulletInit(5,    5,    1.3,   1,   60,    -15,      1);
    bulletInit(6,    5,    1.4,   1,   220,   8,        1);
    bulletInit(7,    0,    -3.5,  3,   0,     0,        3);
    bulletInit(8,   15,    5,     1,   0,     0,        1);
    bulletInit(9,    5,    3,     1,   0,     0,        1);
    bulletInit(10,   0,    2,     1,   0,     5,        1);
}

void bulletCollisionUpdate(){
    for(int i=0;i<MAX_BULLETS;i++){
        if(bullets[i]){
            int bType = bullets[i]->bulletType;

            if(bType==0){
                bullets[i]->collisionBoxes[0].x = bullets[i]->x+3;
                bullets[i]->collisionBoxes[0].y = bullets[i]->y;
                bullets[i]->collisionBoxes[0].w = 9;
                bullets[i]->collisionBoxes[0].h = 31;
            }else if(bType==7){
                bullets[i]->collisionBoxes[0].x = bullets[i]->x+18;
                bullets[i]->collisionBoxes[0].y = bullets[i]->y+30;
                bullets[i]->collisionBoxes[0].w = 16;
                bullets[i]->collisionBoxes[0].h = 35;

                bullets[i]->collisionBoxes[1].x = bullets[i]->x+11;
                bullets[i]->collisionBoxes[1].y = bullets[i]->y+42;
                bullets[i]->collisionBoxes[1].w = 30;
                bullets[i]->collisionBoxes[1].h = 67;

                bullets[i]->collisionBoxes[1].x = bullets[i]->x;
                bullets[i]->collisionBoxes[1].y = bullets[i]->y+87;
                bullets[i]->collisionBoxes[1].w = 50;
                bullets[i]->collisionBoxes[1].h = 31;
            }else if(bType==8){
                bullets[i]->collisionBoxes[0].x = bullets[i]->x+3;
                bullets[i]->collisionBoxes[0].y = bullets[i]->y+2;
                bullets[i]->collisionBoxes[0].w = 12;
                bullets[i]->collisionBoxes[0].h = 44;
            }else if(bType==9){
                bullets[i]->collisionBoxes[0].x = bullets[i]->x+4;
                bullets[i]->collisionBoxes[0].y = bullets[i]->y+7;
                bullets[i]->collisionBoxes[0].w = 29;
                bullets[i]->collisionBoxes[0].h = 40;
            }else{
                bullets[i]->collisionBoxes[0].x = bullets[i]->x+3;
                bullets[i]->collisionBoxes[0].y = bullets[i]->y+4;
                bullets[i]->collisionBoxes[0].w = 19;
                bullets[i]->collisionBoxes[0].h = 10;
            }
        }
    }
}
