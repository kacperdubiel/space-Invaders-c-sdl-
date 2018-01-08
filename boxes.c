#include "boxes.h"

#include <SDL.h>
#include <stdlib.h>

Box *boxes[MAX_BOXES];
BoxType boxTypes[BOX_TYPES];

void addBox(float x, float y, int type){
    int found;
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
    boxInit(0,    60);
    boxInit(1,    120);
    boxInit(2,    90);
    boxInit(3,    120);
    boxInit(4,    100);
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
