#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SDL.h>
#include <tmx.h>
#include <chipmunk.h>
#include "entity.h"
#include "key_input.h"
static const int SCREEN_WIDTH =960;
static const int SCREEN_HEIGHT =736;
static const int MAX_SPRITES = 300;
static const int MAX_ENTITIES = 300;
typedef struct{
	cpSpace *space;
	SDL_Event* mainEvent;
	KeyData * keyData;
	SDL_Rect camera;
	SDL_Rect hotBox;
	Entity* playerEnt;
	//map stuff
	tmx_map *map;
	SDL_Texture *mapTexture;
	SDL_Rect mapRect;
	tmx_layer* solidLayer;
	int tileHeight;
	int tileWidth;
}Globals;

extern Globals globals;
#endif
