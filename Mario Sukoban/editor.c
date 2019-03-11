/*
	-----------------------------
			editor.c
	-----------------------------

	By PxCode
	Role : Editor Functions Definitions
	Created on : 09/03/2019
	Last modified on : 10/03/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "files.h"
#include "editor.h"

int editor(SDL_Renderer *renderer, RESOURCES *resources) {
	int currentBlock,
		currentTime = 0,
		previousTime = 0;
	SDL_bool
		quit_requested = SDL_FALSE,
		mouse_button_held = SDL_FALSE;
	SDL_Rect mouse_pos, block_pos;
	SDL_Texture *cursors[6] = { NULL };
	enum BLOCK new_map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT] = { EMPTY };
	enum BLOCK blocks[] = {EMPTY, MARIO, WALL, BOX, BOX_OK, CHECKPOINT};

	block_pos.w = block_pos.h = mouse_pos.w = mouse_pos.h = BLOCK_SIZE;
	currentBlock = 1;

	cursors[0] = resources->cursor_delete;
	cursors[1] = resources->cursor_spawn;
	cursors[2] = resources->cursor_wall;
	cursors[3] = resources->cursor_box;
	cursors[4] = resources->cursor_box_ok;
	cursors[5] = resources->cursor_checkpoint;
	
	SDL_ShowCursor(SDL_DISABLE);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Usage", "Press 'S' to save the level\nPress 'Q' to quit", NULL);

	while (!quit_requested) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit_requested = SDL_TRUE;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						// cursor_mario spawn
						case SDLK_1:
							currentBlock = 1;
							break;
						// cursor_wall
						case SDLK_2:
							currentBlock = 2;
							break;
						// cursor_box
						case SDLK_3:
							currentBlock = 3;
							break;
						// cursor_box_ok
						case SDLK_4:
							currentBlock = 4;
							break;
						// cursor_checkpoint
						case SDLK_5:
							currentBlock = 5;
							break;
						// cursor_delete
						case SDLK_DELETE:
							currentBlock = 0;
							break;
						// save level
						case SDLK_s:
							if (saveLevel(new_map))
								SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Level Editor", "Level has been saved!", NULL);
							break;
						// quit level editor
						case SDLK_q:
							quit_requested = SDL_TRUE;
							break;
						}
					break;
				case SDL_MOUSEWHEEL:
					if (event.wheel.y > 0){
						currentBlock++;
						if (currentBlock >= 6)
							currentBlock = 0;
					}
					else {
						currentBlock--;
						if (currentBlock < 0)
							currentBlock = 5;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (blocks[currentBlock] == MARIO && spawnExists(new_map))
						break;
					new_map[mouse_pos.x / BLOCK_SIZE][mouse_pos.y / BLOCK_SIZE] = blocks[currentBlock];
					if (blocks[currentBlock] != MARIO)
						mouse_button_held = SDL_TRUE;
					
					break;
				case SDL_MOUSEBUTTONUP:
					mouse_button_held = SDL_FALSE;
					break;
				case SDL_MOUSEMOTION:
					mouse_pos.x = event.motion.x;
					mouse_pos.y = event.motion.y;
					if (mouse_button_held)
						new_map[mouse_pos.x / BLOCK_SIZE][mouse_pos.y / BLOCK_SIZE] = blocks[currentBlock];
					break;
			}
		}

		currentTime = SDL_GetTicks();
		if (currentTime - previousTime > 30) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, resources->editorBG, NULL, NULL);
			// Draw new map
			for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
				for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
					block_pos.x = i * BLOCK_SIZE;
					block_pos.y = j * BLOCK_SIZE;
					switch (new_map[i][j]) {
					case MARIO:
						SDL_RenderCopy(renderer, resources->marioDown, NULL, &block_pos);
						break;
					case WALL:
						SDL_RenderCopy(renderer, resources->wall, NULL, &block_pos);
						break;
					case BOX:
						SDL_RenderCopy(renderer, resources->box, NULL, &block_pos);
						break;
					case BOX_OK:
						SDL_RenderCopy(renderer, resources->box_ok, NULL, &block_pos);
						break;
					case CHECKPOINT:
						SDL_RenderCopy(renderer, resources->checkpoint, NULL, &block_pos);
						break;
					}
				}
			}
			// Draw grid
			SDL_RenderCopy(renderer, resources->grid, NULL, NULL);
			// Draw Mouse
			SDL_RenderCopy(renderer, cursors[currentBlock], NULL, &mouse_pos);
			SDL_RenderPresent(renderer);
		}
		else {
			SDL_Delay(30 - (currentTime - previousTime));
		}
	}

	return SDL_TRUE;
}

SDL_bool spawnExists(enum BLOCK map[NB_BLOCKS_WIDTH][NB_BLOCKS_HEIGHT]) {

	for (int i = 0; i < NB_BLOCKS_WIDTH; i++) {
		for (int j = 0; j < NB_BLOCKS_HEIGHT; j++) {
			if (map[i][j] == MARIO)
				return SDL_TRUE;
		}
	}

	return SDL_FALSE;
}
