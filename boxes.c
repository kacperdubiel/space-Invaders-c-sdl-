#include "boxes.h"

#include <SDL.h>
#include <stdlib.h>

Box *boxes[MAX_BOXES];
BoxType boxTypes[BOX_TYPES];

void addBox(float x, float y, int type){
    int found = -1;
    for(int i=0;i<MAX_BOXES;i++){
        if(boxes[i]==NULL){
            found = i;
            break;
        }
    }
    if(found>=0){
        boxes[found] = malloc(sizeof(Box));
        boxes[found]->x = x;
        boxes[found]->y = y;
        boxes[found]->boxType = type;
    }
}

void removeBox(int i){
    if(boxes[i]){
        free(boxes[i]);
        boxes[i]=NULL;
    }
}

void boxInit(int type, float dropChance){
    boxTypes[type].dropChance = dropChance;
}

void allBoxesInit(){
    // LISTA SKRZYNEK
    //     TYPE,  DCHANCE
    boxInit(0,    120);
    boxInit(1,    200);
    boxInit(2,    180);
    boxInit(3,    200);
    boxInit(4,    200);
    boxInit(5,    80);
}

void boxCollisionUpdate(){
    for(int i=0;i<MAX_BOXES;i++){
        if(boxes[i]){
            boxes[i]->collisionBoxes[0].x = boxes[i]->x;
            boxes[i]->collisionBoxes[0].y = boxes[i]->y;
            boxes[i]->collisionBoxes[0].w = boxRect.w;
            boxes[i]->collisionBoxes[0].h = boxRect.h;
        }
    }
}
