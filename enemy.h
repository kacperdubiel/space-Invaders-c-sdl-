#ifndef ENEMIES_H_INCLUDED
#define ENEMIES_H_INCLUDED

#include <SDL.h>

#define MAX_ENEMIES 100
#define ENEMY_TYPES 6+1 //1 - template, nie używany
#define LAST_STAGE  8
#define STARTING_COOLDOWN 30

#include "textures.h"

typedef struct _Enemy{
    float x, y, x_vel, y_vel;
    int life;
    int enemyType;
    int cooldown;

    SDL_Rect collisionBoxes[MAX_COLLISION_BOXES];
} Enemy;

typedef struct _EnemyType{
    int life;
    int score, damage;
    float x_vel, y_vel;
    int bulletType, bulletChance, cooldownTime;
    SDL_Rect enemyRect;
    SDL_Texture* texture;

    int collisionBoxesCount;
} EnemyType;

extern EnemyType enemyTypes[ENEMY_TYPES];
extern Enemy *enemies[MAX_ENEMIES];

void enemyInit(int type, int life, int damage, int score, float x_vel, float y_vel, int bulletType, int bulletChance, int coolDownTime, int collisionBoxesCount);
void allEnemyInit();
void removeEnemy(int i);
void addEnemy(int enemyType, float x, float y, float x_vel, float y_vel, int life);
void addEnemies(int stage);
void enemyCollisionUpdate();

#endif // ENEMIES_H_INCLUDED
