#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#define FREEZE_COOLDOWN   30 //w sekundach
#define FREEZE_DURATION   5  //w sekundach
#define BOMBS_COOLDOWN    10 //w klatkach

#define BONUS_DURATION    10 //w sekundach
#define MAX_LIFE          10

#include <SDL.h>
#include "textures.h"
#include "boxes.h"
#include "ranking.h"

typedef struct _Player{
    char name[RANKING_NAME_CHARS];
    float x, y, x_vel, y_vel;
    int life;
    int right, left, up, down, fire, score, stage;
    int attack_speed, attCooldown, speed, bullet_amplitude, damage;
    float bullet_speed;
    int freezeCooldown, freezeTime;
    int bombs, bombsCooldown;
    int timers[BOX_TYPES];
    int collisionBoxesCount;
    SDL_Rect collisionBoxes[MAX_COLLISION_BOXES];
} Player;

extern Player player;

void addPlayerBasicStats();
void playerCollisionUpdate();

#endif // PLAYER_H_INCLUDED
