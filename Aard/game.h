#ifndef _GAME_H_
#define _GAME_H


typedef struct GameData{
	int (*gameState) ();
	SDL_Texture* textureBuffer;
	TTF_Font *font;
	SDL_Rect *camera;
}Game;

void Init();
void InitGame();
void Loop();
#endif