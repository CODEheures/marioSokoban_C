/*
 * gcc src/*.c -o out/main.exe -I./include -L./libs  -lmingw32 -lSDL2_image -lSDL2_TTF -lSDL2main -lSDL2 -mwindows
 *
 *
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "variables.h"
#include "utils.h"
#include "game.h"

int main(int argc, char **argv) {

    int init = -1;
    int again = 1;
    SDL_Event event;
    SDL_Window *fenetre = NULL;
    SDL_Surface *fenetreSurface = NULL;
    SDL_Surface *menu = NULL;
    SDL_Rect position;
    TTF_Font *font;

    init = SDL_Init(SDL_INIT_VIDEO);
    if(init != 0)
        exitOnFailure(fenetre, SDL_GetError());

    init = TTF_Init();
    if(init != 0)
        exitOnFailure(fenetre, SDL_GetError());

    fenetre = SDL_CreateWindow(TITRE_FENETRE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGEUR_FENETRE, HAUTEUR_FENETRE, 0);
    exitOnFailure(fenetre, SDL_GetError());

    fenetreSurface = SDL_GetWindowSurface(fenetre);
    exitOnFailure(fenetre, SDL_GetError());

    menu = IMG_Load("./sprites/menu.jpg");
    exitOnFailure(menu, SDL_GetError());

    position.x = 0;
    position.y = 0;

    font = TTF_OpenFont("streetcred.ttf", 96);
    exitOnFailure(font, TTF_GetError());

    while(again) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                again = 0;
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        fprintf(stdout, "Touche Quitter presse");
                        again = 0;
                        break;
                    case SDLK_1:
                    case SDLK_KP_1:
                        fprintf(stdout, "Touche JOUER presse");
                        again = launchGame(fenetre, fenetreSurface, font);
                        break;
                    case SDLK_2:
                    case SDLK_KP_2:
                        fprintf(stdout, "Touche EDITER presse");
                        break;
                    default:
                        again = 1;
                }
                break;
            default:
                again = 1;
        }

        SDL_FillRect(fenetreSurface, NULL, SDL_MapRGB(fenetreSurface->format, 255, 255, 255));
        SDL_BlitSurface(menu, NULL, fenetreSurface, &position);
        SDL_UpdateWindowSurface(fenetre);
    }


    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
    SDL_FreeSurface(fenetreSurface);
    SDL_DestroyWindow(fenetre);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}