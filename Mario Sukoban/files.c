/*
	-----------------------------
			files.c
	-----------------------------

	By PxCode
	Role : Reading & Writing Level Functions Definitions
	Created on : 09/03/2019
	Last modified on : 13/03/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "files.h"

SDL_bool loadLevel(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT], int levelIndex) {
	FILE *worldFile;
	char *line = malloc(sizeof(char) * NB_BLOCKS_WIDTH * NB_BLOCKS_HEIGHT + 1);
	char *c = malloc(sizeof(char));
	Uint8 levelsCount = 0;
	Uint8 index = 0;

	if (fopen_s(&worldFile, "resources/worlds.level", "r") != 0)
		return SDL_FALSE;

	do {
		*c = fgetc(worldFile);
		if (*c == '\n')
			levelsCount++;
		else if (levelIndex == levelsCount)
			*(line + (index++)) = *c;
	} while (*c != EOF);

	*(line + index) = '\0';

	for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
		for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
			switch (*(line + i * NB_BLOCKS_WIDTH + j)) {
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

	free(c);
	free(line);
	fclose(worldFile);

	return SDL_TRUE;
}

SDL_bool saveLevel(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]) {
	FILE *worldFile;

	if (fopen_s(&worldFile, "resources/worlds.level", "a") != 0)
		return SDL_FALSE;

	char *c = malloc(sizeof(char));
	for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
		for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
			SDL_itoa(map[i][j], c, 10);
			fputc(*c, worldFile);
		}
	}
	fputc('\n', worldFile);

	fclose(worldFile);
	
	return SDL_TRUE;
}

SDL_bool checkLevels(Uint8 levelIndex) {
	FILE *worldFile;

	if (fopen_s(&worldFile, "resources/worlds.level", "r") != 0)
		return SDL_FALSE;
	
	char c;
	Uint8 levelsCount = 0;

	do {
		c = fgetc(worldFile);
		if (c == '\n') levelsCount++;
	} while (c != EOF);

	return levelIndex >= levelsCount-1;
}
