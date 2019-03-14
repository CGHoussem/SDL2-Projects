/*
	-----------------------------
			game.h
	-----------------------------

	By PxCode
	Role : Game Functions Prototypes
	Created on : 08/03/2019
	Last modified on : 13/03/2019
*/

#include "constants.h"

#ifndef GAME_H
#define GAME_H

SDL_bool play(SDL_Renderer *renderer, RESOURCES *resources);
SDL_bool movePlayer(int map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT], SDL_Rect *playerPos, enum DIRECTION dir);
SDL_bool moveBox(int map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT], SDL_Rect *playerPos, enum DIRECTION dir);
SDL_bool checkCheckpoints(int map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]);
void initialiserResources(RESOURCES *resources);
void loadResources(SDL_Renderer *renderer, RESOURCES *resources);
void freeResources(RESOURCES *resources);

#endif