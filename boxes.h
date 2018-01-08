#ifndef BOXES_H_INCLUDED
#define BOXES_H_INCLUDED

#include <SDL.h>
#include "textures.h"

#define MAX_BOXES 50
#define BOX_TYPES 5

typedef struct _Box{
    float x, y;
    int boxType;
    SDL_Rect collisionBoxes[MAX_COLLISION_BOXES];
} Box;

typedef struct _BoxType{
    int life;
    int amp;
    int attSpeed;
    int dmg;
    //int shield;
    //int bomb;
    float dropChance;
    SDL_Texture* texture;
    SDL_Texture* offTexture;
} BoxType;

extern Box *boxes[MAX_BOXES];
extern BoxType boxTypes[BOX_TYPES];

void addBox(float x, float y, int type);
void removeBox(int i);
void boxInit(int type, float dropChance);
void allBoxesInit();

void boxCollisionUpdate();

#endif // BOXES_H_INCLUDED
