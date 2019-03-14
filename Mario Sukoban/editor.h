/*
	-----------------------------
			editor.h
	-----------------------------

	By PxCode
	Role : Editor Functions Prototypes
	Created on : 08/03/2019
	Last modified on : 13/03/2019
*/

#ifndef EDITOR_H
#define EDITOR_H

#include "constants.h"

SDL_bool editor(SDL_Renderer *renderer, RESOURCES *resources);
SDL_bool spawnExists(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]);

#endif