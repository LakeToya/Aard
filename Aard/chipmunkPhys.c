#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <chipmunk.h>
#include "globals.h"
#include "chipmunkPhys.h"



void SetupChipmunk()
{
	cpInitChipmunk();
	globals.space = cpSpaceNew();
	globals.space->gravity = cpv(0,0);
}
void tick()
{  
	cpSpaceStep(globals.space, 1.0f/60.0f);  
} 