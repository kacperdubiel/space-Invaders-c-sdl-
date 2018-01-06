#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>

#define HUD_HEIGHT 45

#define MAX_COLLISION_BOXES 10
#define BG_SCROLLING_SPEED_INCREASE 4/LAST_STAGE
extern float bg_scrolling_speed;

extern int heartX;
// czcionka
extern TTF_Font* font;
// kolor czcionki
extern SDL_Color colorBlack;
extern SDL_Color colorWhite;
extern SDL_Color colorGold;

extern SDL_Texture* bgTexture, *hudTexture, *heartTexture, *tempTexture, *scoreTextTexture;
extern SDL_Texture* playerTexture, *playerBulletTexture;
extern SDL_Texture* explosionTexture;
extern SDL_Texture* startTextTexture, *selectedStartTextTexture, *helpTextTexture, *selectedHelpTextTexture, *endTextTexture, *selectedEndTextTexture;
extern SDL_Texture* pauseTextTexture, *yesTextTexture, *selectedYesTextTexture, *noTextTexture, *selectedNoTextTexture;

extern SDL_Rect bgRect, hudRect1, hudRect2, heartRect, scoreTextRect;
extern SDL_Rect playerRect, playerBulletRect, explosionRect, frame;
extern SDL_Rect startTextRect, helpTextRect, endTextRect;
extern SDL_Rect pauseTextRect, yesTextRect, noTextRect;

void surfaceError();
SDL_Texture* createTexture(char *path);
SDL_Texture* createTextTexture(TTF_Font* font, char *text, SDL_Color color);
void createTexturesAndRects();

void collisionUpdate();
int collisionCheck(int n, SDL_Rect collBoxes[n], int m, SDL_Rect collBoxes2[m]);

void cleanMap();

#endif // TEXTURES_H_INCLUDED
