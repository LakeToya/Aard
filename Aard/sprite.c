#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glib.h>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "graphic.h"
#include "sprite.h"

static Sprite *SpriteList = NULL;
int numSprites= 0;
/**
*@brief Deallocates memory used for sprites
*/
void CloseSpriteSystem()
{
	int i,j;
	if(!SpriteList)
	{
		return;
	}
	for (i = 0;i < MAX_SPRITES;i++)
	{
		if(SpriteList[i].image != 0)
		{
			FreeSprite(&SpriteList[i]);
		}
		
	}
	free(SpriteList);
	SpriteList = NULL;
}

/**
*@brief Initializes Sprite List
*/
void InitSpriteList()
{
	int x,j;
	SpriteList = (Sprite*)malloc(sizeof(Sprite) * MAX_SPRITES);
	memset(SpriteList,0,sizeof(Sprite) * MAX_SPRITES);
	for(x = 0;x < MAX_SPRITES;x++)
	{
	SpriteList[x].image = NULL;
	}
	atexit(CloseSpriteSystem);
}
void FreeSprite(Sprite *sprite)
{
	int i;
	if(sprite == NULL)
	{
		return;
	}
  if(sprite->refCount > 0)
  {
	sprite->refCount--;
  }
if(sprite->refCount <= 0)
  {
  strcpy(sprite->filename,"\0");
  if(sprite->image != NULL)
	  SDL_DestroyTexture(sprite->image);
  sprite->image = NULL;
  }
  sprite = NULL;
}

/**
*@brief Load in data for a sprite
*/
Sprite *LoadSprite(char *filename,int sizex, int sizey,int fpl)
{
	
	int i;
	SDL_Surface *temp;
	SDL_Texture* temp2;
	for(i = 0; i< MAX_SPRITES;i++)
	{
		if(strncmp(filename,SpriteList[i].filename,40) == 0 && SpriteList[i].width == sizex && SpriteList[i].height == sizey)
		{
			SpriteList[i].refCount++;
			return &SpriteList[i];
		}
	}
	if(numSprites + 1 >= MAX_SPRITES)
	{
		fprintf(stderr,"Max Sprites Reached.\n");
		exit(1);
	}
	numSprites++;
  for(i = 0;i <= MAX_SPRITES;i++)
  {
    if(!SpriteList[i].refCount)break;
  }
   temp = IMG_Load(filename);
  if(temp == NULL)
  {
    fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
    exit(0);
  }
  temp2 = SDL_CreateTextureFromSurface(GetRenderer(),temp);
  if(temp2 == NULL)
  {
	  fprintf(stderr,"CREATE TEXTURE FROM SURFACE HAS FAILED: %s\n",SDL_GetError());
	  exit(0);
  }
  SpriteList[i].image = temp2;
// SDL_SetColorKey(SpriteList[i].image, SDL_TRUE , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
  strncpy(SpriteList[i].filename,filename,30);
  SpriteList[i].fpl= fpl;
  SpriteList[i].width = sizex;
  SpriteList[i].height = sizey;
  SpriteList[i].refCount++;
  return &SpriteList[i];
}
/**
*@brief Draws specified sprite
*/
void DrawSprite(Sprite *sprite,int sx,int sy, int frame,SDL_Renderer *renderer,SDL_RendererFlip flip)
{
	SDL_Rect dest,src;
	src.x = frame%sprite->fpl * sprite->width;
    src.y = frame/sprite->fpl * sprite->height;
    src.w =sprite->width;
    src.h =sprite->height;
	dest.x = sx - globals.camera.x;
    dest.y = sy - globals.camera.y;
    dest.w = sprite->width;
    dest.h = sprite->height;
	SDL_RenderCopyEx(renderer, sprite->image,&src,&dest,0,0,flip);
}
 /**
*@brief Increases or decrease the current frame of the animation
*/
void Animate(Animation* animation,int startFrame) {
    if(animation->oldTime + animation->frameRate > SDL_GetTicks()) {
        return;
    }
 
    animation->oldTime = SDL_GetTicks();
	if(animation->holdFrame == 1)
	{
		animation->currentFrame = animation->startFrame + animation->heldFrame - 1;
		return;
	}
    animation->currentFrame += animation->frameInc;
 
    if(animation->oscillate) {
        if(animation->frameInc > 0) {
			if(animation->currentFrame >= animation->maxFrames-1 + startFrame) 
			{
                animation->frameInc = -animation->frameInc;
            }
        }else{
            if(animation->currentFrame <= startFrame) 
			{
                animation->frameInc = -animation->frameInc;
            }
        }
    }else{
        if(animation->currentFrame >= animation->maxFrames + startFrame) {
            animation->currentFrame = startFrame;
        }
    }
}
/**
*@brief Set framerate of specified animation
*/
void SetFrameRate(Animation* animation,int Rate) {
    animation->frameRate = Rate;
}
/**
*@brief Set current frame of animation
*/
void SetCurrentFrame(Animation* animation,int Frame) {
    if(Frame < 0 || Frame >= animation->maxFrames) return;
 
    animation->currentFrame = Frame;
}
/**
*@brief Returns current frame of specified animation
*/
int GetCurrentFrame(Animation* animation) {
    return animation->currentFrame;
}


