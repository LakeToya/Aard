#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <chipmunk.h>
#include <jansson.h>
#include "vector.h"
#include "globals.h"
#include "entity.h"
#include "graphic.h"
#include "player.h"

Player* player = NULL; 
void LoadPlayerAnimations(Entity* ent)
{
	
	json_t *root;
	json_error_t err;
	root = json_load_file("AnimationData.json",0,&err);
	if(!root)
	{
		fprintf(stderr,"error: on line %d : %s\n", err.line,err.text);
		exit(1);
	}

		json_t *data , *Entity, *Animations;
		char *ENT ,*filepath; 
		int frameW, frameH,fpl;
		data = json_object_get(root,"AnimationData");
		Entity = json_array_get(data,0);
		json_unpack(Entity,"{s:s,s:s,s:i,s:i,s:i}","Entity",&ENT,"filepath",&filepath,"frame-width",&frameW,"frame-height",&frameH,"framesperline",&fpl);
		//load sprite
		ent->sprite = LoadSprite(filepath,frameW,frameH,fpl);
		data = json_object_get(Entity,"Animations");
			if(!json_is_array(data))
			{
				fprintf(stderr, "error: commit %d: Not an array\n", 0);
				json_decref(root);
				exit(1);
			}
		//load animations
		for(int i = 0;i < json_array_size(data);++i)
		{
			Animations = json_array_get(data,i);	
			if(!json_is_object(Animations))
			{
				fprintf(stderr, "error: commit %d: Not an object\n", 0);
				json_decref(root);
				exit(1);
			}
			ent->animations[i].startFrame = json_number_value(json_object_get(Animations,"start-frame"));
			ent->animations[i].currentFrame = ent->animations[i].startFrame;
			ent->animations[i].maxFrames = json_number_value(json_object_get(Animations,"max-frames"));
			ent->animations[i].oscillate = json_number_value(json_object_get(Animations,"oscillate"));
			ent->animations[i].holdFrame = 0;
			ent->animations[i].frameInc = 1;
			ent->animations[i].frameRate = 100;
			/*
			g_hash_table_insert(ent->sounds,"WhipAttack",Mix_LoadWAV("sound/whip_Attack.wav"));
			g_hash_table_insert(ent->sounds,"PlayerDeath",Mix_LoadWAV("sound/player_Death.wav"));
			g_hash_table_insert(ent->sounds,"PlayerHurt",Mix_LoadWAV("sound/Hurt.wav"));
			g_hash_table_insert(ent->sounds,"PlayerPickUp",Mix_LoadWAV("sound/PickUp.wav"));
			g_hash_table_insert(ent->sounds,"PlayerThrow",Mix_LoadWAV("sound/Throw.wav"));
			*/
			if(json_object_get(Animations,"held-frame"))
				ent->animations[i].heldFrame = json_number_value(json_object_get(Animations,"held-frame"));
			else
				ent->animations[i].heldFrame = -1;
		}
		ent->animNum = Walk_Horizontal;
		json_decref(root);
}
Entity *CreatePlayer()
{
	tmx_layer *layers = globals.map->ly_head;
	tmx_object* obj;
	player = (Player*)malloc(sizeof(Player));
	memset(player,0,sizeof(Player));

	Entity* self = CreateEntity();
	LoadPlayerAnimations(self);
	//parse through map objects
	while (layers)
	{
		if(strcmp(layers->name,"GameObjects") == 0)
		{
			break;
		}
		layers = layers->next;
	}
	obj = layers->content.objgr->head;
	while(obj)
	{
		if(strcmp(obj->name,"PlayerStart") == 0)
		{
			self->position.x = obj->x;
			self->position.y = obj->y;
			break;
		}
		obj = obj->next;
	}
	self->speed = 3;
	self->animNum = Idle;
	self->body = cpSpaceAddBody(globals.space,cpBodyNew(1,cpMomentForBox(1,16,21)));
	cpBodySetPos(self->body, cpv(0, 0));
	self->shape = cpSpaceAddShape(globals.space, cpBoxShapeNew(self->body,21,16));
	cpShapeSetFriction(self->shape, 0.7);


	self->draw = &DrawPlayer;
	self->update = &UpdatePlayer;
	self->think = &ThinkPlayer;
	
	return self;
}
void UpdatePlayer(Entity *ent)
{

	Movement(ent);
	Vec2DAdd(ent->position,ent->position,ent->velocity);
	//Handle Tile Static Collision
	Vec2DAdd(ent->position,ent->position,SolidLayerCollision(ent));
}
void ThinkPlayer(Entity* ent)
{

	//Handle Animations
	if(globals.keyData->ArrowKeyLeft != 0 || globals.keyData->ArrowKeyRight != 0)
	{
		SwitchAnimation(ent, Walk_Horizontal);
	}
	else if (globals.keyData->ArrowKeyUp != 0)
	{
		SwitchAnimation(ent,Walk_Up);
	}
	else if (globals.keyData->ArrowKeyDown != 0)
	{
		SwitchAnimation(ent,Walk_Down);
	}

	ent->animations[ent->animNum].holdFrame = ent
		->velocity.x == 0 && ent->velocity.y == 0 ? 1 : 0;
	ent->frame = ent->animations[ent->animNum].currentFrame;
}
void DrawPlayer(Entity* ent)
{
	Animate(&ent->animations[ent->animNum],ent->animations[ent->animNum].startFrame);
	DrawSprite(ent->sprite,ent->position.x,ent->position.y,
		ent->animations[ent->animNum].currentFrame,GetRenderer(),ent->flipped);
}
void FreePlayer(Entity* ent)
{
	FreeEntity(ent);
	free(ent);
}
void Movement(Entity* ent)
{
	if(globals.keyData->ArrowKeyLeft)
	{
		ent->velocity.x = -ent->speed;
		ent->facing.x = -1;
		ent->facing.y = 0;
		ent->flipped = SDL_FLIP_HORIZONTAL;
	}else
	{
		ent->velocity.x = 0;
	}
	if(globals.keyData->ArrowKeyRight)
	{
		ent->facing.x = 1;
		ent->facing.y = 0;
		ent->velocity.x = ent->speed;
		ent->flipped = SDL_FLIP_NONE;
	}
	if(globals.keyData->ArrowKeyUp)
	{
		ent->velocity.y = -ent->speed;
		ent->facing.x = 0;
		ent->facing.y = 1;
		globals.keyData->ArrowKeyUp = 1;
	}
	else
	{
		ent->velocity.y = 0;
	}
	if(globals.keyData->ArrowKeyDown)
	{
		ent->velocity.y = ent->speed;
		ent->facing.x = 0;
		ent->facing.y = -1;
		globals.keyData->ArrowKeyDown = 1;
	}
}