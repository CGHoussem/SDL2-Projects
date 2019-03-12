/*
	-----------------------------
			    main.c
	-----------------------------

	By PxCode
	Role : Main Game Program
	Created on : 08/03/2019
	Last modified on : 12/03/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "constants.h"
#include "game.h"
#include "editor.h"

int main(int argc, char** argv) {

	SDL_Window *window;
	SDL_Renderer *renderer;
	RESOURCES resources;

	SDL_bool quit_requested = SDL_FALSE;
	Uint32 window_flags = SDL_WINDOW_OPENGL;
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	Uint32 image_flags = IMG_INIT_JPG | IMG_INIT_PNG;

	int previousTime = 0, currentTime = 0, menu_item_selected = 0;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	window = SDL_CreateWindow(
		"Mario Sokoban",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		window_flags
	);

	if (window == NULL) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		render_flags
	);

	if (renderer == NULL) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	if (!IMG_Init(image_flags)) {
		fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	if (TTF_Init() != 0) {
		fprintf(stderr, "Error initializing TTF: %s\n", TTF_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	initialiserResources(&resources);
	loadResources(renderer, &resources);

	while (!quit_requested){
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit_requested = SDL_TRUE;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_RETURN:
					switch (menu_item_selected) {
					case 0:	// PLAY
						if (!play(renderer, &resources)) {
							printf("Error occured in the game!\n");
						}
						break;
					case 1:	// EDITOR
						if (!editor(renderer, &resources)) {
							printf("Error occured in the editor!\n");
						}
						break;
					case 2:	// QUIT
						quit_requested = SDL_TRUE;
						break;
					}
					break;
				case SDLK_DOWN:
					menu_item_selected++;
					if (menu_item_selected >= MENU_ITEMS_COUNT)
						menu_item_selected = 0;
					break;
				case SDLK_UP:
					menu_item_selected--;
					if (menu_item_selected < 0)
						menu_item_selected = MENU_ITEMS_COUNT - 1;
					break;
				}
			
				break;
			}
		}

		currentTime = SDL_GetTicks();
		if (currentTime - previousTime > 30) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, resources.menu_bg, NULL, NULL);
			for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
				if (menu_item_selected == i)
					SDL_SetTextureAlphaMod(resources.menu_items[i], 255);
				else
					SDL_SetTextureAlphaMod(resources.menu_items[i], 178);
				SDL_RenderCopy(renderer, resources.menu_items[i], NULL, NULL);
			}

			SDL_RenderCopy(renderer, resources.menu_items_outline[menu_item_selected], NULL, NULL);			
			SDL_RenderPresent(renderer);

			previousTime = currentTime;
		}
		else {
			SDL_Delay(30 - (currentTime - previousTime));
		}
	}

	// Freeing
	freeResources(&resources);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
