#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct
{
	
}Player;
Entity *CreatePlayer();
void LoadPlayerAnimations(Entity* ent);
void UpdatePlayer(Entity *ent);
void ThinkPlayer(Entity* ent);
void DrawPlayer(Entity* ent);
void FreePlayer(Entity* player);
void Movement(Entity *ent);
#endif