#ifndef _SPRITE_H_
#define _SPRITE_H_

enum AnimationType
{
	Walk_Down,
	Walk_Up,
	Walk_Horizontal,
	Attack,
	Idle
};
typedef struct AnimationData{
	int startFrame;
	int currentFrame;
	int frameInc;
	int frameRate;
	int heldFrame;
	long oldTime;
	int maxFrames;
	int oscillate;
	int holdFrame;
}Animation;
	void Animate(Animation* animation,int startFrame);
	void SetCurrentFrame(Animation* animation,int Frame);
	void SetFrameRate(Animation* animation);
	int GetCurrentFrame(Animation* animation);

typedef struct SpriteData{
  SDL_Texture *image;		/*pointer to the actual image in memory*/
  char filename[30];			/*the name of the file, keep them short*/
  int width ,height;					/*the width and height of the frames of the sprites, not the file itself*/
  int fpl;			/*default is 16*/
  int refCount;					/*used by the maintanenc functions to keep track of how many times a single sprite is being used*/
}Sprite;


void InitSpriteList();
void CloseSpriteSystem();
void FreeSprite(Sprite *img);
Sprite *LoadSprite(char *filename,int sizex, int sizey,int fpl);
void DrawSprite(Sprite *sprite,int sx,int sy, int frame,SDL_Renderer *renderer,SDL_RendererFlip flip);
#endif