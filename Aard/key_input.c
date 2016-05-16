#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glib.h>
#include <string>
#include "globals.h"
#include "key_input.h"

void InitKeyData()
{
	globals.keyData = (KeyData*)malloc(sizeof(KeyData));
	memset(globals.keyData,0,sizeof(KeyData));
	atexit(CloseKeyData);
}
void CloseKeyData()
{
	free(globals.keyData);
	globals.keyData = NULL;
}
/**
*@brief handles input from keyboard depending on the gameState
*@brief handles player velocity
*@param Takes in the game state
*/
void handleInput(int (*gameState)())
{
	if(gameState == &StateGame)
	{
		//printf("This is the Game\n");
		//if(playerEnt != NULL){
		
		if(globals.mainEvent->type == SDL_KEYDOWN)	
		{
			switch(globals.mainEvent->key.keysym.sym)
			{
				
			case SDLK_LEFT:
				globals.keyData->ArrowKeyLeft = 1;
				break;
			case SDLK_RIGHT:

				globals.keyData->ArrowKeyRight = 1;
				break;
			case SDLK_UP:
				globals.keyData->ArrowKeyUp = 1;
				break;
			case SDLK_DOWN:
				globals.keyData->ArrowKeyDown = 1;
				break;
			case SDLK_q:
				printf("q pressed");globals.keyData->Q = 1;
				break;
			case SDLK_w:
				printf("w pressed");globals.keyData->W = 1;
				break;
			case SDLK_e:
				printf("e pressed");globals.keyData->E = 1;
				break;
			case SDLK_r:
				printf("r pressed");globals.keyData->R = 1;
				break;
			case SDLK_SPACE:
				printf("Spacebar pressed\n");globals.keyData->Spacebar = 1;
				break;
			case SDLK_RETURN:
				/*
				ItemRef* ref;
				printf("Enter pressed\n");
				keyData->Enter = 1;
				SetGameState(StateInventory);
				Mix_PlayChannel(-1,(Mix_Chunk*)g_hash_table_lookup(playerData->inventory->sounds,"Open"),0);
				playerData->inventory->cursor->ref = 
					g_list_nth(playerData->inventory->inventory,0);
					*/
				break;
			default:
				break;
			}
		}

		if (globals.mainEvent->type == SDL_KEYUP)
		{
			switch(globals.mainEvent->key.keysym.sym)
			{
			case SDLK_LEFT:
				globals.keyData->ArrowKeyLeft = 0;break;
			case SDLK_RIGHT:
				globals.keyData->ArrowKeyRight = 0;break;
			case SDLK_UP:
				globals.keyData->ArrowKeyUp = 0;break;
			case SDLK_DOWN:
				globals.keyData->ArrowKeyDown = 0;break;
			case SDLK_q:
				globals.keyData->Q = 0;
				break;
			case SDLK_w:
				globals.keyData->W = 0;
				break;
			case SDLK_e:
				globals.keyData->E = 0;
				break;
			case SDLK_r:
				globals.keyData->R = 0;
				break;
			case SDLK_SPACE:
				globals.keyData->Spacebar = 0;
				//LoadMapBinary();
				break;
			default:
				break;
			}
		}
	//}
}
	}