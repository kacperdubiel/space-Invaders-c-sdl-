#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SDL.h>
#include "textures.h"

typedef struct _Player{
    float x, y, x_vel, y_vel;
    int life;
    int right, left, up, down, fire, score, stage;
    int attack_speed, attCooldown, speed, bullet_speed, bullet_amplitude, damage;
    int collisionBoxesCount;
    SDL_Rect collisionBoxes[MAX_COLLISION_BOXES];
} Player;

extern Player player;

void addPlayerBasicStats();
void playerCollisionUpdate();

#endif // PLAYER_H_INCLUDED
