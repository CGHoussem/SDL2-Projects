/*
	-----------------------------
				game.c
	-----------------------------

	By PxCode
	Role : Main Game Functions Definitions
	Created on : 09/03/2019
	Last modified on : 12/03/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "files.h"
#include "game.h"

int play(SDL_Renderer *renderer, RESOURCES *resources) {
	int previousTime = 0, currentTime = 0;
	enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT] = { EMPTY };
	SDL_Rect pos, marioPos;
	SDL_bool 
		quit_requested = SDL_FALSE,
		won = SDL_FALSE;
	SDL_Color white_color = { 255, 255, 255 };
	SDL_Texture
		*mario[4] = { NULL },
		*actualMario = NULL;

	mario[UP] = resources->marioUp;
	mario[RIGHT] = resources->marioRight;
	mario[DOWN] = resources->marioDown;
	mario[LEFT] = resources->marioLeft;
	
	actualMario = mario[DOWN];
	pos.w = pos.h = marioPos.w = marioPos.h = BLOCK_SIZE;

	if (!loadLevel(map)) {
		fprintf(stderr, "Error loading the level!\n");
		return SDL_FALSE;
	}

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "How To Play", "Goal: Move the box(es) to checkpoint(s)!", NULL);

	for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
		for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
			if (map[i][j] == MARIO) {
				marioPos.x = BLOCK_SIZE * i;
				marioPos.y = BLOCK_SIZE * j;
				map[i][j] = EMPTY;
			}
		}
	}

	while (!quit_requested && !won) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit_requested = SDL_TRUE;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							quit_requested = SDL_TRUE;
							break;
						case SDLK_RIGHT:  
							actualMario = mario[RIGHT];
							if (movePlayer(map, &marioPos, RIGHT) && checkCheckpoints(map))
								won = SDL_TRUE;
							break;
						case SDLK_LEFT:
							actualMario = mario[LEFT];
							if (movePlayer(map, &marioPos, LEFT) && checkCheckpoints(map))
								won = SDL_TRUE;
							break;
						case SDLK_UP:
							actualMario = mario[UP];
							if (movePlayer(map, &marioPos, UP) && checkCheckpoints(map))
								won = SDL_TRUE;
							break;
						case SDLK_DOWN:
							actualMario = mario[DOWN];
							if (movePlayer(map, &marioPos, DOWN) && checkCheckpoints(map))
								won = SDL_TRUE;
							break;
					}
					break;
			}
		}

		currentTime = SDL_GetTicks();
		if (currentTime - previousTime > 30) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, resources->levelsBG, NULL, NULL);
			for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
				for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
					pos.x = BLOCK_SIZE * i;
					pos.y = BLOCK_SIZE * j;
					switch (map[i][j]) {
						case WALL:
							SDL_RenderCopy(renderer, resources->wall, NULL, &pos);
							break;
						case BOX:
							SDL_RenderCopy(renderer, resources->box, NULL, &pos);
							break;
						case BOX_OK:
							SDL_RenderCopy(renderer, resources->box_ok, NULL, &pos);
							break;
						case CHECKPOINT:
							SDL_RenderCopy(renderer, resources->checkpoint, NULL, &pos);
							break;
						}
				}
			}
			SDL_RenderCopy(renderer, actualMario, NULL, &marioPos);
			SDL_RenderPresent(renderer);

			previousTime = currentTime;
		}
		else {
			SDL_Delay(30 - (currentTime - previousTime));
		}

		if (won)
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_INFORMATION,
				"Congratulations",
				"Well done!\nYou have won the level!",
				NULL);
	}

	return SDL_TRUE;
}

SDL_bool movePlayer(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT], SDL_Rect *playerPos, enum DIRECTION dir) {
	int i, j;
	switch (dir) {
		case RIGHT:
			i = (playerPos->x + BLOCK_SIZE) / BLOCK_SIZE;
			j = playerPos->y / BLOCK_SIZE;
			if (map[i][j] == EMPTY || map[i][j] == CHECKPOINT)
				playerPos->x += BLOCK_SIZE;
			else if (map[i][j] == BOX)
				return moveBox(map, playerPos, dir);
			else
				printf("Mouvement Blocked!\n");
			break;
		case LEFT:
			i = (playerPos->x - BLOCK_SIZE) / BLOCK_SIZE;
			j = playerPos->y / BLOCK_SIZE;
			if (map[i][j] == EMPTY || map[i][j] == CHECKPOINT)
				playerPos->x -= BLOCK_SIZE;
			else if (map[i][j] == BOX)
				return moveBox(map, playerPos, dir);
			else
				printf("Mouvement Blocked!\n");
			break;
		case UP:
			i = playerPos->x / BLOCK_SIZE;
			j = (playerPos->y - BLOCK_SIZE) / BLOCK_SIZE;
			if (map[i][j] == EMPTY || map[i][j] == CHECKPOINT)
				playerPos->y -= BLOCK_SIZE;
			else if (map[i][j] == BOX)
				return moveBox(map, playerPos, dir);
			else
				printf("Mouvement Blocked!\n");
			break;
		case DOWN:
			i = playerPos->x / BLOCK_SIZE;
			j = (playerPos->y + BLOCK_SIZE) / BLOCK_SIZE;
			if (map[i][j] == EMPTY || map[i][j] == CHECKPOINT)
				playerPos->y += BLOCK_SIZE;
			else if (map[i][j] == BOX)
				return moveBox(map, playerPos, dir);
			else
				printf("Mouvement Blocked!\n");
			break;
	}

	return SDL_FALSE;
}

SDL_bool moveBox(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT], SDL_Rect *playerPos, enum DIRECTION dir) {
	int i, j;
	switch (dir) {
		case RIGHT:
			i = (playerPos->x + BLOCK_SIZE) / BLOCK_SIZE;
			j = playerPos->y / BLOCK_SIZE;
			
			if (map[i + 1][j] == EMPTY)
			{
				playerPos->x += BLOCK_SIZE;
				map[i + 1][j] = map[i][j];
				map[i][j] = EMPTY;
			}
			else if (map[i + 1][j] == CHECKPOINT) {
				playerPos->x += BLOCK_SIZE;
				map[i + 1][j] = BOX_OK;
				map[i][j] = EMPTY;
				return SDL_TRUE;
			}
			else
				printf("Box Blocked!\n");
			break;
		case LEFT:		
			i = (playerPos->x - BLOCK_SIZE) / BLOCK_SIZE;
			j = playerPos->y / BLOCK_SIZE;
			if (map[i - 1][j] == EMPTY)
			{
				playerPos->x -= BLOCK_SIZE;
				map[i - 1][j] = map[i][j];
				map[i][j] = EMPTY;
			}
			else if (map[i - 1][j] == CHECKPOINT) {
				playerPos->x -= BLOCK_SIZE;
				map[i - 1][j] = BOX_OK;
				map[i][j] = EMPTY;
				return SDL_TRUE;
			}
			else
				printf("Box Blocked!\n");
			break;
		case UP:
			i = playerPos->x / BLOCK_SIZE;
			j = (playerPos->y - BLOCK_SIZE) / BLOCK_SIZE;
			if (map[i][j-1] == EMPTY)
			{
				playerPos->y -= BLOCK_SIZE;
				map[i][j-1] = map[i][j];
				map[i][j] = EMPTY;
			}
			else if (map[i][j - 1] == CHECKPOINT) {
				playerPos->y -= BLOCK_SIZE;
				map[i][j - 1] = BOX_OK;
				map[i][j] = EMPTY;
				return SDL_TRUE;
			}
			else
				printf("Box Blocked!\n");
			break;
		case DOWN:
			i = playerPos->x / BLOCK_SIZE;
			j = (playerPos->y + BLOCK_SIZE) / BLOCK_SIZE;
			if (map[i][j + 1] == EMPTY)
			{
				playerPos->y += BLOCK_SIZE;
				map[i][j + 1] = map[i][j];
				map[i][j] = EMPTY;
			}
			else if (map[i][j + 1] == CHECKPOINT) {
				playerPos->y += BLOCK_SIZE;
				map[i][j + 1] = BOX_OK;
				map[i][j] = EMPTY;
				return SDL_TRUE;
			}
			else
				printf("Box Blocked!\n");
			break;
	}

	return SDL_FALSE;
}

SDL_bool checkCheckpoints(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]) {
	int nb_checkpoints = 0;
	for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
		for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
			if (map[i][j] == CHECKPOINT)
				nb_checkpoints++;
		}
	}

	return (nb_checkpoints == 0);
}

void initialiserResources(RESOURCES *resources) {
	resources->menu_bg = NULL;
	for (int i = 0; i < MENU_ITEMS_COUNT; i++){
		resources->menu_items[i] = NULL;
		resources->menu_items_outline[i] = NULL;
	}
	resources->editorBG = NULL;
	resources->levelsBG = NULL;
	resources->grid = NULL;
	resources->wall = NULL;
	resources->marioDown = NULL;
	resources->marioUp = NULL;
	resources->marioRight = NULL;
	resources->marioLeft = NULL;
	resources->checkpoint = NULL;
	resources->box = NULL;
	resources->box_ok = NULL;
	resources->cursor_wall = NULL;
	resources->cursor_spawn = NULL;
	resources->cursor_checkpoint = NULL;
	resources->cursor_box = NULL;
	resources->cursor_box_ok = NULL;
	resources->cursor_delete = NULL;
}

void loadResources(SDL_Renderer *renderer, RESOURCES *resources) {
	resources->menu_bg = IMG_LoadTexture(renderer, "resources/images/menu_bg.png");
	resources->menu_items[0] = IMG_LoadTexture(renderer, "resources/images/menu_item1.png");
	resources->menu_items[1] = IMG_LoadTexture(renderer, "resources/images/menu_item2.png");
	resources->menu_items[2] = IMG_LoadTexture(renderer, "resources/images/menu_item3.png");
	resources->menu_items_outline[0] = IMG_LoadTexture(renderer, "resources/images/menu_item1_outline.png");
	resources->menu_items_outline[1] = IMG_LoadTexture(renderer, "resources/images/menu_item2_outline.png");
	resources->menu_items_outline[2] = IMG_LoadTexture(renderer, "resources/images/menu_item3_outline.png");
	resources->editorBG = IMG_LoadTexture(renderer, "resources/images/editorBG.jpg");
	resources->levelsBG = IMG_LoadTexture(renderer, "resources/images/editorBG.jpg");
	resources->grid = IMG_LoadTexture(renderer, "resources/images/grid.png");
	resources->wall = IMG_LoadTexture(renderer, "resources/images/wall.jpg");
	resources->marioDown = IMG_LoadTexture(renderer, "resources/images/mario_front.gif");
	resources->marioUp = IMG_LoadTexture(renderer, "resources/images/mario_back.gif");
	resources->marioRight = IMG_LoadTexture(renderer, "resources/images/mario_right.gif");
	resources->marioLeft = IMG_LoadTexture(renderer, "resources/images/mario_left.gif");
	resources->checkpoint = IMG_LoadTexture(renderer, "resources/images/checkpoint.png");
	resources->box = IMG_LoadTexture(renderer, "resources/images/box.jpg");
	resources->box_ok = IMG_LoadTexture(renderer, "resources/images/box_ok.jpg");
	resources->cursor_wall= IMG_LoadTexture(renderer, "resources/images/cursor_wall.png");
	resources->cursor_spawn = IMG_LoadTexture(renderer, "resources/images/cursor_mario.png");
	resources->cursor_checkpoint = IMG_LoadTexture(renderer, "resources/images/cursor_checkpoint.png");
	resources->cursor_box = IMG_LoadTexture(renderer, "resources/images/cursor_box.png");
	resources->cursor_box_ok = IMG_LoadTexture(renderer, "resources/images/cursor_box_ok.png");
	resources->cursor_delete = IMG_LoadTexture(renderer, "resources/images/cursor_delete.png");

	resources->mainFont = TTF_OpenFont("resources/fonts/Players.ttf", MAIN_FONT_SIZE);
	
	printf("All resources has been loaded!\n");

}

void freeResources(RESOURCES *resources) {
	SDL_DestroyTexture(resources->menu_bg);
	for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
		SDL_DestroyTexture(resources->menu_items[i]);
		SDL_DestroyTexture(resources->menu_items_outline[i]);
	}
	SDL_DestroyTexture(resources->editorBG);
	SDL_DestroyTexture(resources->levelsBG);
	SDL_DestroyTexture(resources->grid);
	SDL_DestroyTexture(resources->wall);
	SDL_DestroyTexture(resources->marioDown);
	SDL_DestroyTexture(resources->marioUp);
	SDL_DestroyTexture(resources->marioRight);
	SDL_DestroyTexture(resources->marioLeft);
	SDL_DestroyTexture(resources->checkpoint);
	SDL_DestroyTexture(resources->box);
	SDL_DestroyTexture(resources->box_ok);
	SDL_DestroyTexture(resources->cursor_wall);
	SDL_DestroyTexture(resources->cursor_spawn);
	SDL_DestroyTexture(resources->cursor_delete);
	SDL_DestroyTexture(resources->cursor_checkpoint);
	SDL_DestroyTexture(resources->cursor_box);
	SDL_DestroyTexture(resources->cursor_box_ok);
	TTF_CloseFont(resources->mainFont);
	resources->mainFont = NULL;
}
