#include "player.h"

#include "init.h"
#include "textures.h"

Player player;

void addPlayerBasicStats(){
    player.right = 0;
    player.left = 0;
    player.up = 0;
    player.down = 0;
    player.fire = 0;

    //początkowa pozycja gracza
    player.x = (WINDOW_WIDTH - playerRect.w) / 2;
    player.y = WINDOW_HEIGHT;

    player.life = 5;
    player.score = 0;
    player.stage = 1;

    player.damage = 1;
    player.speed = 6;           //w px/s
    player.bullet_speed = 5;    //w px/s
    player.bullet_amplitude = 35; //w px
    player.attack_speed = 25; //co ile klatek można strzelać
    player.attCooldown = player.attack_speed; //pierwszy pocisk możliwy w pierwszej klatce
}

void playerCollisionUpdate(){
    player.collisionBoxesCount = 4;

    player.collisionBoxes[0].x = player.x+31;
    player.collisionBoxes[0].y = player.y+3;
    player.collisionBoxes[0].w = 9;
    player.collisionBoxes[0].h = 19;

    player.collisionBoxes[1].x = player.x+25;
    player.collisionBoxes[1].y = player.y+17;
    player.collisionBoxes[1].w = 20;
    player.collisionBoxes[1].h = 30;

    player.collisionBoxes[2].x = player.x+11;
    player.collisionBoxes[2].y = player.y+32;
    player.collisionBoxes[2].w = 48;
    player.collisionBoxes[2].h = 30;

    player.collisionBoxes[3].x = player.x;
    player.collisionBoxes[3].y = player.y+43;
    player.collisionBoxes[3].w = 70;
    player.collisionBoxes[3].h = 15;
}
