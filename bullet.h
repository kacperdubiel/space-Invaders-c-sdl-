#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#define MAX_BULLETS 500
#define BULLET_TYPES 7

#include <SDL.h>
#include "textures.h"

#define SINUS_ACCURACY (0.1) //dokładność sinusoidy (im wyższa liczba tym większa częstotliwość drgań)

typedef struct _Bullet{
    float x, y, y_helper;
    float x_start, y_start;
    int bulletType;
    int bullet_amplitude, bullet_damage;
    float bullet_speed;
    int bullet_angle;
    int freeze;
    SDL_Rect collisionBoxes[MAX_COLLISION_BOXES];
} Bullet;

typedef struct _BulletType{
    int bullet_amplitude;
    int bullet_damage;
    int bullet_angle;
    float bullet_speed, spin_speed;
    SDL_Rect rect;
    SDL_Texture* texture;
    int collisionBoxesCount;
} BulletType;

extern Bullet *bullets[MAX_BULLETS];
extern BulletType bulletTypes[BULLET_TYPES];

void addBullet(float x, float y, int type);
void removeBullet(int i);
void bulletInit(int type, int bullet_amplitude, float bullet_speed, int bullet_damage, int bullet_angle, int spin_speed, int collisionBoxesCount);
void allBulletsInit();

void bulletCollisionUpdate();

#endif // BULLET_H_INCLUDED
