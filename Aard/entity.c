#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <chipmunk.h>
#include "map.h"
#include "globals.h"
#include "entity.h"

Entity* EntityList = NULL;

void InitEntityList()
{
	EntityList = (Entity*)malloc(sizeof(Entity) * MAX_ENTITIES);
	memset(EntityList,0,sizeof(Entity) * MAX_ENTITIES);	
	atexit(CloseEntityList);
}
void CloseEntityList()
{
	int i;
	if(!EntityList)
	{
		printf("Entity List not initialized");
		return;
	}
	for(i = 0;i<MAX_ENTITIES;i++)
	{
		if(EntityList[i].sprite != NULL)
		{
		if(EntityList[i].free != NULL)
			(*EntityList[i].free)(&EntityList[i]);
		}
	}
	free(EntityList);
	EntityList = NULL;
}
void UpdateEntities()
{
	int i;
	for(i =0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].inuse <= 0)
		{
			continue;
		}
		if(EntityList[i].update != NULL)
			(*EntityList[i].update)(&EntityList[i]);
	}
}
void ThinkEntities()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].inuse <= 0)
		{
			continue;
		}
		if(EntityList[i].think != NULL)
			(*EntityList[i].think)(&EntityList[i]);
	}
}
void DrawEntities()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].inuse <= 0 || EntityList[i].sprite == NULL)
		{
			continue;
		}
			if(EntityList[i].draw != NULL)
				(*EntityList[i].draw)(&EntityList[i]);
	}
}
Entity* CreateEntity()
{
	int i;
	for(i = 0;i < MAX_ENTITIES;i++)
	{
		if(EntityList[i].inuse)
		{
			continue;
		}
		memset(&EntityList[i],0,sizeof(Entity));
		EntityList[i].inuse = 1;
		EntityList[i].id = i;
		EntityList[i].body = NULL;
		EntityList[i].shape = NULL;
		return &EntityList[i];
		}
	printf("Reached Max number of Entities.");
	return 0;
}
void FreeEntity(Entity* ent)
{
	int i;
	if(ent != NULL){
		ent->inuse--;
		if(ent->sprite != NULL)
		{
			FreeSprite(ent->sprite);
		}
		ent->id =  NULL;
		ent->update = NULL;
		ent = NULL;
	}
}

void SolidLayerCollision(Entity* ent)
{
	Vec2D dir;
	Vec2D mid = CreateVec2D(ent->position.x + ent->sprite->width/2,ent->position.y + ent->sprite->height/2);
	dir.x = dir.y = 0;
	int x,y,
		x2 = ent->position.x+ent->sprite->width-1,
		y2 = ent->position.y+ent->sprite->height-1;
	for(x = ent->position.x; x < x2;x+=globals.tileWidth)
	{
		for( y = ent->position.y; y < y2;y+=globals.tileHeight)
		{
			if(CheckTileSolid(x/globals.tileWidth,y/globals.tileHeight)){
				if(CheckTileSolid(x/globals.tileWidth,y2/globals.tileHeight)){
					
				}
				//dir.y = ent->speed;
				printf("TileCollision");
			}
		}
		if(CheckTileSolid(x/globals.tileWidth,y2/globals.tileHeight)){
			//dir.y = -ent->speed;
			printf("TileCollision");
		}
	}
	for(y = ent->position.y;y < y2;y+=globals.tileHeight)
	{
		if(CheckTileSolid(x2/globals.tileWidth,y/globals.tileHeight))
		{
			//dir.x = -ent->speed;
			//dir.y = ent->speed;
			printf("TileCollision");
		}
	}
		if(CheckTileSolid(x2/globals.tileWidth,y2/globals.tileHeight))
		{
			//dir.x = -ent->speed;
			//dir.y = -ent->speed;
			printf("TileCollision");
		}


}

int GetID(Entity *ent)
{
	return ent->id;
}

Entity* GetEntityByID(int id)
{
	int i;
	for (i = 0;i < MAX_ENTITIES;i++)
	{
		if(!EntityList[i].id == id)
			continue;
	}
	return &EntityList[i];
}
void SwitchAnimation(Entity* ent, AnimationType type)
{
	if(ent->animNum != type){
	ent->animations[ent->animNum].currentFrame = ent->animations[ent->animNum].startFrame;
	ent->animNum = type;
	}
}