#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "sprite.h"
#include "vector.h"
enum EntityType
{
	Aard,
	Enemy
};
typedef struct EntityData
{
	Sprite* sprite;
	cpBody * body;
	cpShape *shape;

	Animation animations[10];
	AnimationType animNum;
	int speed;
	Vec2D velocity;
	Vec2D facing;
	Vec2D position;
	EntityType entType;
	int inuse;
	int id;
	SDL_RendererFlip flipped;
	int frame;
	SDL_Rect hitBox;

	void (*update)(struct EntityData *ent);
	void (*think)(struct EntityData *ent);
	void (*touch)(struct EntityData *ent, struct EntityData *other);
	void (*free)(struct EntityData *ent);
	void(*draw)(struct EntityData *ent);
}Entity;

void DrawEntity(Entity *ent,int animationNum, int x, int y);	
Entity* CreateEntity();
void FreeEntity(Entity* ent);
void UpdateEntities();
void ThinkEntities();
void DrawEntities(); 
void InitEntityList();
void CloseEntityList();
void SwitchAnimation(Entity* ent, AnimationType type);
int GetID(Entity *ent);
Entity* GetEntityByID(int id);

void SolidLayerCollision(Entity* ent);
#endif