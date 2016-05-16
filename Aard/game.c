#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <chipmunk.h>
#include <glib.h>
#include <tmx.h>
#include "camera.h"
#include "graphic.h"
#include "sprite.h"
#include "globals.h"
#include "key_input.h"
#include "entity.h"
#include "player.h"
#include "map.h"
#include "chipmunkPhys.h"
#include "game.h"

Sprite* sp;
Game* game;
Globals globals;
void* sdl_img_loader(const char *path);
int main(int argc,char *argv[])
{
	Init();
	Loop();


  return 0;
}
void Init()
{
	Init_Graphics("Aard:GTS",
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_WIDTH,
    SCREEN_HEIGHT);
	InitSpriteList();
	InitKeyData();
	InitEntityList();
	SetupChipmunk();

	game = (Game*)malloc(sizeof(Game));
	memset(game,0,sizeof(Game));
	globals.mainEvent = new SDL_Event();
	LoadMap("Maps/example.tmx","images/Resources1.png");
	globals.playerEnt = CreatePlayer();
	SetCamera();

	game->textureBuffer = GetTextureBuffer();
	game->gameState = StateGame;
}

void Loop()
{
	int quit = 0;
	do
	{
		SDL_PollEvent(globals.mainEvent);
		handleInput(game->gameState);	
		quit = game->gameState();
			

		}
	while(!quit && globals.mainEvent->type != SDL_QUIT);
	graphics_close();
	exit(0);
	delete globals.mainEvent;
	return;

}

int StateGame()
{
		SDL_RenderClear(GetRenderer());
		ThinkEntities();
		UpdateEntities();
		UpdateCamera();
		render_map(globals.map);
		DrawEntities();
		NextFrame();
	return 0;

}
void* sdl_img_loader(const char *path) {
	return IMG_LoadTexture(GetRenderer(), path);
}
