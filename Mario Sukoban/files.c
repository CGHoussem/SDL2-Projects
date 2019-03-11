/*
	-----------------------------
			files.c
	-----------------------------

	By PxCode
	Role : Reading & Writing Level Functions Definitions
	Created on : 09/03/2019
	Last modified on : 10/03/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "files.h"

SDL_bool loadLevel(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]) {
	FILE *worldFile;
	char worldLine[NB_BLOCKS_WIDTH * NB_BLOCKS_HEIGHT + 1];

	if (fopen_s(&worldFile, "resources/worlds.level", "r") != 0)
		return SDL_FALSE;

	fgets(worldLine, NB_BLOCKS_WIDTH * NB_BLOCKS_HEIGHT + 1, worldFile);
	fclose(worldFile);

	for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
		for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
			switch (worldLine[(i * NB_BLOCKS_WIDTH) + j]) {
			case '0':
				map[i][j] = EMPTY;
				break;
			case '1':
				map[i][j] = WALL;
				break;
			case '2':
				map[i][j] = BOX;
				break;
			case '3':
				map[i][j] = BOX_OK;
				break;
			case '4':
				map[i][j] = CHECKPOINT;
				break;
			case '5':
				map[i][j] = MARIO;
				break;
			}
		}
	}

	return SDL_TRUE;
}

SDL_bool saveLevel(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]) {
	FILE *worldFile;

	if (fopen_s(&worldFile, "resources/worlds.level", "w") != 0)
		return SDL_FALSE;

	char *c = malloc(1);
	for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
		for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
			SDL_itoa(map[i][j], c, 10);
			fputc(*c, worldFile);
		}
	}

	fclose(worldFile);
	
	return SDL_TRUE;
}
