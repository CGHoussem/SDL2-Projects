/*
	-----------------------------
			constants.h
	-----------------------------

	By PxCode
	Role : define constants for the game
	Created on : 08/03/2019
	Last modified on : 10/03/2019
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL.h>

#define	BLOCK_SIZE			34
#define	NB_BLOCKS_WIDTH		12
#define	NB_BLOCKS_HEIGHT	12
#define	WINDOW_WIDTH		BLOCK_SIZE * NB_BLOCKS_WIDTH
#define WINDOW_HEIGHT		BLOCK_SIZE * NB_BLOCKS_HEIGHT

enum DIRECTION {UP, DOWN, LEFT, RIGHT};
enum BLOCK {EMPTY, WALL, BOX, BOX_OK, CHECKPOINT, MARIO};
typedef struct {
	SDL_Texture
		*menu,
		*levelsBG,
		*editorBG,
		*grid,
		*marioLeft,
		*marioRight,
		*marioUp,
		*marioDown,
		*wall,
		*checkpoint,
		*box,
		*box_ok,
		*cursor_wall,
		*cursor_checkpoint,
		*cursor_spawn,
		*cursor_box,
		*cursor_box_ok,
		*cursor_delete;
} RESOURCES;

#endif