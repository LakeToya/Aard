#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "globals.h"
#include "camera.h"

void SetCamera()
{
	SDL_Rect c;
	globals.hotBox.x = globals.playerEnt->position.x - 200;
	globals.hotBox.y = globals.playerEnt->position.y - 100;
	globals.hotBox.w = .25 * SCREEN_WIDTH;
	globals.hotBox.h = .25 * SCREEN_HEIGHT;
	c.x = 0;c.y = 0;
	c.w = SCREEN_WIDTH;
	c.h = SCREEN_HEIGHT;
	globals.camera = c;
}
void UpdateCamera()
{
	if(globals.playerEnt->position.x + globals.playerEnt->sprite->width< globals.hotBox.x )
		globals.hotBox.x = globals.playerEnt->position.x + globals.playerEnt->sprite->width;
	if(globals.playerEnt->position.y < globals.hotBox.y)
		globals.hotBox.y = globals.playerEnt->position.y;
	if(globals.playerEnt->position.x > globals.hotBox.x + globals.hotBox.w)
		globals.hotBox.x = globals.playerEnt->position.x - globals.hotBox.w;
	if(globals.playerEnt->position.y +globals. playerEnt->sprite->height >globals.hotBox.y+ globals.hotBox.h)
		globals.hotBox.y = globals.playerEnt->position.y+ globals.playerEnt->sprite->height - globals.hotBox.h;

	globals.camera.x =(globals.hotBox.x + globals.hotBox.w/2) - SCREEN_WIDTH/2;
	globals.camera.y =(globals.hotBox.y + globals.hotBox.h/2) - SCREEN_HEIGHT/2;
	/*
	if(globals.playerEnt->position.y > SCREEN_HEIGHT/2)
	{
		globals.camera.y +=5;
		globals.playerEnt->position.y -= globals.playerEnt->speed;
	}
	*/

}
