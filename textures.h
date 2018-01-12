#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

#define MAX_COLLISION_BOXES 10
#define HUD_HEIGHT 45
#define MAX_ANIMATIONS 100
#define ANIMATION_TYPES 5
#define BG_SCROLLING_SPEED_INCREASE 4/LAST_STAGE
extern float bg_scrolling_speed;

#include <SDL.h>
#include <SDL_ttf.h>
#include "ranking.h"
#include "boxes.h"

typedef struct _Texture{
    SDL_Rect rect;
    SDL_Texture* texture;
    SDL_Texture* selected_texture;
} Texture;

typedef struct _Animation{
    float x, y;
    int angle;
    SDL_Rect frameRect;
    int animationType;
} Animation;

typedef struct _AnimationType{
    int verticalCount, horizontalCount;
    int textureW, textureH;
    SDL_Rect rect;
    SDL_Texture* texture;
} AnimationType;

extern Animation *animations[MAX_ANIMATIONS];
extern AnimationType animationTypes[ANIMATION_TYPES];
extern Texture menuTextTextures[4];
extern Texture pauseTextTextures[3];
extern Texture rankingTextTextures[4+RANKING_TOP];
extern Texture cooldownsTextTextures[BOX_TYPES];
extern Texture endTextTextures[7];
extern Texture hotkeysTextTextures[3];
extern Texture iconTextures[3];
extern Texture spellsCdTextures[3];

extern int heartX;

//czcionka
TTF_Font* font72, *font_outline72;
TTF_Font* font60, *font_outline60;
TTF_Font* font50, *font_outline50;
TTF_Font* font45, *font_outline45;
TTF_Font* font30, *font_outline30;
TTF_Font* font25, *font_outline25;
TTF_Font* font20, *font_outline20;
TTF_Font* font15, *font_outline15;
TTF_Font* font, *font_outline;

// kolor czcionki
extern SDL_Color colorBlack;
extern SDL_Color colorWhite;
extern SDL_Color colorGold;

extern SDL_Texture* bgTexture, *hudTexture, *heartTexture, *tempTexture, *scoreTextTexture;
extern SDL_Texture* playerTexture, *playerBulletTexture;
extern SDL_Texture* animationTexture;
extern SDL_Texture* textTexture;
extern SDL_Texture* arrowsTexture, *qTexture, *wTexture, *eTexture;

extern SDL_Rect bgRect, hudRect1, hudRect2, heartRect, scoreTextRect;
extern SDL_Rect playerRect, playerBulletRect, animationRect, frame;
extern SDL_Rect boxRect, textRect;
extern SDL_Rect arrowsRect, keyRect;

void surfaceError();
SDL_Texture* createTexture(char *path);
SDL_Texture* createTextTexture(TTF_Font* font, char *text, SDL_Color color, int outline, TTF_Font* outlineFont, SDL_Color outlineColor);

void createTexturesAndRects();
void createRankingTextures();
void createEndTextures();

void collisionUpdate();
int collisionCheck(int n, SDL_Rect collBoxes[n], int m, SDL_Rect collBoxes2[m]);

void createAnimationTextures();
void addAnimation(float x, float y, int angle, int type);
void removeAnimation(int i);
void animationInit(int type, int verticalCount, int horizontalCount);
void allAnimationInit();

void cleanMap();

#endif // TEXTURES_H_INCLUDED
