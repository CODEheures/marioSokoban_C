//
// Created by Papoun on 27/12/2017.
//

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include "variables.h"

#ifndef SOKOBAN_GAME_H
#define SOKOBAN_GAME_H

void makeGrid(Grid *grid, int *nbObjectifs, SDL_Rect *positionMario);
void getLevel(int level, char *readedLevel);
int launchGame(SDL_Window *fenetre, SDL_Surface *fenetreSurface, TTF_Font *font);
void updateSurface(SDL_Window *fenetre, SDL_Surface *fenetreSurface, Grid grid, SDL_Surface *mur, SDL_Surface *caisse, SDL_Surface *caisse_ok, SDL_Surface *objectif, SDL_Surface *mario[], int marioIsOnObjectif, int direction);
void move(int direction, Grid *grid, SDL_Rect *positionMario, int *marioIsOnObjectif, int *nbOnObjectifs);
void moveCaisse(int nextX, int nextY, int nextItem, Grid *grid, int *nbOnObjectifs);
void moveMario(int x, int y, int nextX, int nextY, int nextItem, SDL_Rect *positionMario, int *marioIsOnObjectif, Grid *grid);

#endif //SOKOBAN_GAME_H
