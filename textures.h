#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>

#define HUD_HEIGHT 45

#define MAX_ANIMATIONS 100
#define ANIMATION_TYPES 5

#define MAX_COLLISION_BOXES 10
#define BG_SCROLLING_SPEED_INCREASE 4/LAST_STAGE
extern float bg_scrolling_speed;

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

extern int heartX;

//czcionka
TTF_Font* font72, *font_outline72;
TTF_Font* font60, *font_outline60;
TTF_Font* font45, *font_outline45;
TTF_Font* font30, *font_outline30;
TTF_Font* font25, *font_outline25;
TTF_Font* font20, *font_outline20;
TTF_Font* font, *font_outline;

// kolor czcionki
extern SDL_Color colorBlack;
extern SDL_Color colorWhite;
extern SDL_Color colorGold;

extern SDL_Texture* bgTexture, *hudTexture, *heartTexture, *tempTexture, *scoreTextTexture;
extern SDL_Texture* playerTexture, *playerBulletTexture;
extern SDL_Texture* animationTexture;
extern SDL_Texture* startTextTexture, *selectedStartTextTexture, *helpTextTexture, *selectedHelpTextTexture, *endTextTexture, *selectedEndTextTexture;
extern SDL_Texture* pauseTextTexture, *yesTextTexture, *selectedYesTextTexture, *noTextTexture, *selectedNoTextTexture;
extern SDL_Texture* iconFreezeTexture, *iconFreezeCdTexture, *freezeCdTexture, *freezeHtkTexture;
extern SDL_Texture* textTexture;
extern SDL_Texture* arrowsTexture, *qTexture, *wTexture, *eTexture;

extern SDL_Rect bgRect, hudRect1, hudRect2, heartRect, scoreTextRect;
extern SDL_Rect playerRect, playerBulletRect, animationRect, frame;
extern SDL_Rect startTextRect, helpTextRect, endTextRect;
extern SDL_Rect pauseTextRect, yesTextRect, noTextRect;
extern SDL_Rect iconFreezeRect, freezeCdRect, freezeHtkRect;
extern SDL_Rect boxRect, textRect;
extern SDL_Rect arrowsRect, keyRect;

void surfaceError();
SDL_Texture* createTexture(char *path);
SDL_Texture* createTextTexture(TTF_Font* font, char *text, SDL_Color color, int outline, TTF_Font* outlineFont, SDL_Color outlineColor);

void createTexturesAndRects();

void collisionUpdate();
int collisionCheck(int n, SDL_Rect collBoxes[n], int m, SDL_Rect collBoxes2[m]);

void addAnimation(float x, float y, int angle, int type);
void removeAnimation(int i);
void animationInit(int type, int verticalCount, int horizontalCount);
void allAnimationInit();

void cleanMap();

#endif // TEXTURES_H_INCLUDED
