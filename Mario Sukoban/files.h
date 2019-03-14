/*
	-----------------------------
			files.h
	-----------------------------

	By PxCode
	Role : Reading & Writing Level Functions Prototypes
	Created on : 08/03/2019
	Last modified on : 13/03/2019
*/

#ifndef FILES_H
#define FILES_H

#include "constants.h"

SDL_bool loadLevel(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT], int levelIndex);
SDL_bool saveLevel(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]);
SDL_bool checkLevels(Uint8 levelIndex);

#endif