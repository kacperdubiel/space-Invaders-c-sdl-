#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#define MAX_BULLETS 500
#define MAX_EXPLOSIONS 100
#define BULLET_TYPES 7

#include <SDL.h>
#include "textures.h"

#define SINUS_ACCURACY (0.1) //dokładność sinusoidy (im wyższa liczba tym większa częstotliwość drgań)

typedef struct _Bullet{
    float x, y, y_helper;
    float x_start, y_start;
    int bulletType;
    int bullet_angle;
    SDL_Rect collisionBoxes[MAX_COLLISION_BOXES];
} Bullet;

typedef struct _BulletType{
    float bullet_amplitude;
    int bullet_speed, bullet_damage;
    int bullet_angle;
    int spin_speed;
    SDL_Rect bulletRect;
    SDL_Texture* texture;
    int collisionBoxesCount;
} BulletType;

typedef struct _Explosion{
    float x, y;
    int frame;
    int lastFrame;
} Explosion;

extern Bullet *allBullets[MAX_BULLETS];
extern BulletType bulletTypes[BULLET_TYPES];
extern Explosion *explosions[MAX_EXPLOSIONS];

void addBullet(float x, float y, int type);
void removeBullet(int i);
void bulletInit(int type, float bullet_amplitude, int bullet_speed, int bullet_damage, int bullet_angle, int spin_speed, int collisionBoxesCount);
void allBulletsInit();

void addExplosion(float x, float y, int frame, int lastFrame);
void removeExplosion(int i);

void bulletCollisionUpdate();

#endif // BULLET_H_INCLUDED
