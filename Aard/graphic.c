#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glib.h>
#include <string>
#include <stdlib.h>
#include <math.h>
#include "graphic.h"

static SDL_Window* window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  * bufferTexture = NULL;
static SDL_Surface  * surface = NULL;
static SDL_Surface  * temp_buffer = NULL;


SDL_Surface *buffer; 

static Uint32 Delay = 45;
static Uint32 NOW = 0;
static Uint32 THEN = 0;
static Uint8 PrintFps = false;
static Uint32 FPS = 0; 

void Init_Graphics(	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight)
{
	bool quit = false;
	

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Unable to initilaize SDL sys: %s",SDL_GetError());
		graphics_close();
		return;
	}else{printf("SDL Init Success\n");}
	
	window = SDL_CreateWindow(windowName,SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								renderWidth, renderHeight,
								SDL_WINDOW_SHOWN);
	if(!window)
	{ 
		fprintf(stdout,"Window creation failed: %s",SDL_GetError());
		graphics_close();
		return;
	}
	buffer = SDL_GetWindowSurface(window);

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED
											| SDL_RENDERER_TARGETTEXTURE
											| SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
    {
        printf("failed to create renderer: %s",SDL_GetError());
		graphics_close();
        return;
    }else{
		printf("SDL Renderer Success\n");
	}

	bufferTexture = SDL_CreateTextureFromSurface(renderer,buffer);


}

SDL_Renderer* GetRenderer()
{
	return renderer;
}
SDL_Window* GetWindow()
{
	return window;
}

void FrameDelay()
{
    Uint32 diff;
    THEN = NOW;
    NOW = SDL_GetTicks();
    diff = (NOW - THEN);
    if (diff < Delay)
    {
        SDL_Delay(Delay - diff);
	}
    FPS = 1000.0/MAX(SDL_GetTicks() - THEN,0.001);
}

void NextFrame()
{
	SDL_RenderPresent(renderer);
    FrameDelay();
}
void graphics_close()
{
    if (bufferTexture)
    {
        SDL_DestroyTexture(bufferTexture);
    }
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    if (surface)
    {
        SDL_FreeSurface(surface);
    }
    if (temp_buffer)
    {
        SDL_FreeSurface(temp_buffer);
    }
    surface = NULL;
    window = NULL;
    renderer = NULL;
    bufferTexture = NULL;
    temp_buffer = NULL;
}
SDL_Texture* GetTextureBuffer()
{
	return bufferTexture;
}