#include "bullet.h"

#include <stdlib.h>
#include <stdio.h>

Bullet *allBullets[MAX_BULLETS];
BulletType bulletTypes[BULLET_TYPES];
Explosion *explosions[MAX_EXPLOSIONS];

void addBullet(float x, float y, int type){
    int found;
    for(int i=0;i<MAX_BULLETS;i++){
        if(allBullets[i]==NULL){
            found = i;
            break;
        }
    }
    if(found>=0){
        allBullets[found] = malloc(sizeof(Bullet));
        allBullets[found]->x = x;
        allBullets[found]->y = y;
        allBullets[found]->y_helper = 0;
        allBullets[found]->x_start = x;
        allBullets[found]->y_start = y;
        allBullets[found]->bulletType = type;
        allBullets[found]->bullet_angle = bulletTypes[type].bullet_angle;
    }
}

void removeBullet(int i){
    if(allBullets[i]){
        free(allBullets[i]);
        allBullets[i]=NULL;
    }
}

void bulletInit(int type, float bullet_amplitude, int bullet_speed, int bullet_damage, int bullet_angle, int spin_speed, int collisionBoxesCount){
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
    bulletInit(1,    5,    1.1,   1,   0,     0,        1);
    bulletInit(2,    2,    1,     1,   0,     0,        1);
    bulletInit(3,    10,   1,     1,   0,     0,        1);
    bulletInit(4,    2,    2,     1,   150,   10,       1);
    bulletInit(5,    5,    1,     1,   60,    -15,      1);
    bulletInit(6,    5,    1,     1,   220,   8,        1);
}

void addExplosion(float x, float y, int frame, int lastFrame){
    int found;
    for(int i=0;i<MAX_EXPLOSIONS;i++){
        if(explosions[i]==NULL){
            found = i;
            break;
        }
    }
    if(found>=0){
        explosions[found] = malloc(sizeof(Explosion));
        explosions[found]->x = x;
        explosions[found]->y = y;
        explosions[found]->frame = frame;
        explosions[found]->lastFrame = lastFrame;
    }
}

void removeExplosion(int i){
    if(explosions[i]){
        free(explosions[i]);
        explosions[i]=NULL;
    }
}

void bulletCollisionUpdate(){
    for(int i=0;i<MAX_BULLETS;i++){
        if(allBullets[i]){
            if(allBullets[i]->bulletType!=1){
                allBullets[i]->collisionBoxes[0].x = allBullets[i]->x+3;
                allBullets[i]->collisionBoxes[0].y = allBullets[i]->y+4;
                allBullets[i]->collisionBoxes[0].w = 19;
                allBullets[i]->collisionBoxes[0].h = 10;
            }
            if(allBullets[i]->bulletType==1){
                allBullets[i]->collisionBoxes[0].x = allBullets[i]->x+3;
                allBullets[i]->collisionBoxes[0].y = allBullets[i]->y;
                allBullets[i]->collisionBoxes[0].w = 9;
                allBullets[i]->collisionBoxes[0].h = 31;
            }

        }
    }
}
