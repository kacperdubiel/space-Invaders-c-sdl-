#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SDL.h>
#include "textures.h"
#include "boxes.h"

#define FREEZE_COOLDOWN   30
#define FREEZE_DURATION   5
#define BONUS_DURATION    10
#define MAX_LIFE          10

typedef struct _Player{
    float x, y, x_vel, y_vel;
    int life;
    int right, left, up, down, fire, score, stage;
    int attack_speed, attCooldown, speed, bullet_amplitude, damage;
    float bullet_speed;
    int freezeCooldown, freezeTime;
    int timers[BOX_TYPES];
    int collisionBoxesCount;
    SDL_Rect collisionBoxes[MAX_COLLISION_BOXES];
} Player;

extern Player player;

void addPlayerBasicStats();
void playerCollisionUpdate();

#endif // PLAYER_H_INCLUDED
