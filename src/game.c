//
// Created by Papoun on 27/12/2017.
//
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "utils.h"

void makeGrid(Grid *grid, int *nbObjectifs, SDL_Rect *positionMario) {
    int i = 0;
    int j = 0;
    char level[NB_BLOCS_LARGEUR*NB_BLOCS_HAUTEUR+1]= ""; //+1 for \0

    getLevel(0, level);
    for (i=0; i<NB_BLOCS_HAUTEUR; i++) {
        for (j=0; j<NB_BLOCS_LARGEUR; j++) {
            grid->tab[i][j] = (int)(level[j+NB_BLOCS_LARGEUR*(i)])-48;
            if (grid->tab[i][j] == MARIO){
                positionMario->x = j;
                positionMario->y = i;
            }
            if (grid->tab[i][j] == OBJECTIF){
                *nbObjectifs += 1;
            }
        }
    }
}

void getLevel(int level, char *readedLevel) {
    FILE *levelFile = NULL;

    levelFile = fopen("./levels", "r");
    exitOnFailure(levelFile, "Fichiers des niveaux introuvable");

    fseek(levelFile, (level-1)*(NB_BLOCS_LARGEUR*NB_BLOCS_HAUTEUR)+(level-1), SEEK_SET);
    fgets(readedLevel, NB_BLOCS_LARGEUR*NB_BLOCS_LARGEUR+1, levelFile); //+1 for\0

    fclose(levelFile);
}

void updateSurface(SDL_Window *fenetre, SDL_Surface *fenetreSurface, Grid grid, SDL_Surface *mur, SDL_Surface *caisse, SDL_Surface *caisse_ok, SDL_Surface *objectif, SDL_Surface *mario[], int marioIsOnObjectif, int direction) {
    SDL_Rect position;
    SDL_FillRect(fenetreSurface, NULL, SDL_MapRGB(fenetreSurface->format, 255, 255, 255));
    for (int i=0; i<NB_BLOCS_HAUTEUR; i++) {
        for (int j=0; j<NB_BLOCS_LARGEUR; j++) {
            position.x = j * TAILLE_BLOC;
            position.y = i * TAILLE_BLOC;
            switch (grid.tab[i][j]){
                case MUR:
                    SDL_BlitSurface(mur, NULL, fenetreSurface, &position);
                    break;
                case CAISSE:
                    SDL_BlitSurface(caisse, NULL, fenetreSurface, &position);
                    break;
                case CAISSE_OK:
                    SDL_BlitSurface(caisse_ok, NULL, fenetreSurface, &position);
                    break;
                case OBJECTIF:
                    SDL_BlitSurface(objectif, NULL, fenetreSurface, &position);
                    break;
                case MARIO:
                    if (marioIsOnObjectif) {
                        SDL_BlitSurface(objectif, NULL, fenetreSurface, &position);
                    }
                    SDL_BlitSurface(mario[direction], NULL, fenetreSurface, &position);
                    break;
                default:
                    break;
            }

        }
    }
    SDL_UpdateWindowSurface(fenetre);
}

void move(int direction, Grid *grid, SDL_Rect *positionMario, int *marioIsOnObjectif, int *nbOnObjectifs) {
    int x = positionMario->x;
    int y = positionMario->y;

    int nextX = 0;
    int nextY = 0;
    int isValidNext = 0;
    int nextItem = -1;

    int next2X = 0;
    int next2Y = 0;
    int isValidNext2 = 0;
    int next2Item = -1;

    switch (direction) {
        case BAS:
            nextX = positionMario->x;
            next2X = positionMario->x;

            nextY = positionMario->y+1;
            next2Y = positionMario->y+2;

            break;
        case HAUT:
            nextX = positionMario->x;
            next2X = positionMario->x;

            nextY = positionMario->y-1;
            next2Y = positionMario->y-2;
            break;
        case GAUCHE:
            nextX = positionMario->x-1;
            next2X = positionMario->x-2;

            nextY = positionMario->y;
            next2Y = positionMario->y;
            break;
        case DROITE:
            nextX = positionMario->x+1;
            next2X = positionMario->x+2;

            nextY = positionMario->y;
            next2Y = positionMario->y;
            break;
        default:
            break;
    }

    isValidNext = (nextX >= 0 && nextY >=0 && nextX < NB_BLOCS_LARGEUR && nextY < NB_BLOCS_HAUTEUR) ? 1 : 0;
    isValidNext2 = (next2X >= 0 && next2Y >=0 && next2X < NB_BLOCS_LARGEUR && next2Y < NB_BLOCS_HAUTEUR) ? 1 : 0;

    if (isValidNext) {
        nextItem = grid->tab[nextY][nextX];
    }

    if (isValidNext2) {
        next2Item = grid->tab[next2Y][next2X];
    }

    switch (nextItem) {
        case VIDE:
        case OBJECTIF:
            moveMario(x, y, nextX, nextY, nextItem, positionMario, marioIsOnObjectif, grid);
            break;
        case CAISSE:
        case CAISSE_OK:
            if (isValidNext2 && (next2Item == VIDE || next2Item == OBJECTIF)){
                if (nextItem == CAISSE_OK) {
                    *nbOnObjectifs -= 1;
                }
                moveCaisse(next2X, next2Y, next2Item, grid, nbOnObjectifs);
                moveMario(x, y, nextX, nextY, nextItem, positionMario, marioIsOnObjectif, grid);
                break;
            }
        default:;
    }
}

void moveCaisse(int next2X, int next2Y, int next2Item, Grid *grid, int *nbOnObjectifs) {
    if (next2Item == OBJECTIF) {
        grid->tab[next2Y][next2X] = CAISSE_OK;
        *nbOnObjectifs += 1;
    } else {
        grid->tab[next2Y][next2X] = CAISSE;
    }
}

void moveMario(int x, int y, int nextX, int nextY, int nextItem, SDL_Rect *positionMario, int *marioIsOnObjectif, Grid *grid) {
    grid->tab[nextY][nextX] = MARIO;
    if (*marioIsOnObjectif){
        grid->tab[y][x] = OBJECTIF;
    } else {
        grid->tab[y][x] = VIDE;
    }
    positionMario->x = nextX;
    positionMario->y = nextY;
    *marioIsOnObjectif = (nextItem == OBJECTIF || nextItem == CAISSE_OK);
}

int launchGame(SDL_Window *fenetre, SDL_Surface *fenetreSurface, TTF_Font *font) {
    SDL_Surface *mario[4] = {NULL};
    SDL_Surface *mur = NULL;
    SDL_Surface *caisse = NULL;
    SDL_Surface *caisse_ok = NULL;
    SDL_Surface *objectif = NULL;

    /* WIN TEXT */
    SDL_Color color={0,0,0};
    SDL_Surface *win_text_surface;
    SDL_Rect win_text_position;

    /* Objectifs managment */
    int nbObjectifs = 0;
    int nbOnObjectifs = 0;
    int marioIsOnObjectif = 0;

    /* Keyboard direction*/
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    int direction = -1; /* -1: no move */

    /* Event managment */
    int again = 1;
    int toMenu = 1;
    SDL_Event event;
    int lastTimeMove = 0;
    int actualTime = 0;

    /* Grid and position mario */
    Grid grid;
    SDL_Rect positionMario;

    /******************* INIT SURFACES *********************/
    /* marios */
    mario[BAS] = IMG_Load("./sprites/mario_bas.gif");
    exitOnFailure(mario[BAS], SDL_GetError());
    mario[HAUT] = IMG_Load("./sprites/mario_haut.gif");
    exitOnFailure(mario[HAUT], SDL_GetError());
    mario[GAUCHE] = IMG_Load("./sprites/mario_gauche.gif");
    exitOnFailure(mario[GAUCHE], SDL_GetError());
    mario[DROITE] = IMG_Load("./sprites/mario_droite.gif");
    exitOnFailure(mario[DROITE], SDL_GetError());

    /* init mur */
    mur = IMG_Load("./sprites/mur.jpg");
    exitOnFailure(mur, SDL_GetError());

    /* init caisses */
    caisse = IMG_Load("./sprites/caisse.jpg");
    exitOnFailure(caisse, SDL_GetError());
    caisse_ok = IMG_Load("./sprites/caisse_ok.jpg");
    exitOnFailure(caisse_ok, SDL_GetError());

    /* init objectif */
    objectif = IMG_Load("./sprites/objectif.png");
    exitOnFailure(objectif, SDL_GetError());

    /* init win text */
    win_text_surface = TTF_RenderText_Blended(font, WIN_TEXT, color);
    exitOnFailure(win_text_surface, TTF_GetError());
    win_text_position.x = fenetreSurface->w/2 - win_text_surface->w/2;
    win_text_position.y = fenetreSurface->h/2 - win_text_surface->h/2;

    /******************* Creation Grille *********************/
    makeGrid(&grid, &nbObjectifs, &positionMario);
    updateSurface(fenetre, fenetreSurface, grid, mur, caisse, caisse_ok, objectif, mario, marioIsOnObjectif, BAS);

    while(again) {
        SDL_PumpEvents();
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            again = 0;
            toMenu = 0;
        }

        if (keyboardState[SDL_SCANCODE_UP]) {
            direction = HAUT;
        } else if (keyboardState[SDL_SCANCODE_DOWN]) {
            direction = BAS;
        }  else if (keyboardState[SDL_SCANCODE_RIGHT]) {
            direction = DROITE;
        }  else if (keyboardState[SDL_SCANCODE_LEFT]) {
            direction = GAUCHE;
        } else {
            direction = -1;
        }

        if (direction != -1) {
            actualTime = SDL_GetTicks();
            if (lastTimeMove == 0 || actualTime - lastTimeMove > KEYBOARD_REPEAT) {
                lastTimeMove = actualTime;
                move(direction, &grid, &positionMario, &marioIsOnObjectif, &nbOnObjectifs);
                updateSurface(fenetre, fenetreSurface, grid, mur, caisse, caisse_ok, objectif, mario, marioIsOnObjectif, direction);
                if (nbOnObjectifs == nbObjectifs) {
                    SDL_BlitSurface(win_text_surface, NULL, fenetreSurface, &win_text_position);
                    SDL_UpdateWindowSurface(fenetre);
                }
            }
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            again = 0;
            toMenu = 1;
        }
        if (keyboardState[SDL_SCANCODE_R]) {
            makeGrid(&grid, &nbObjectifs, &positionMario);
            updateSurface(fenetre, fenetreSurface, grid, mur, caisse, caisse_ok, objectif, mario, marioIsOnObjectif, BAS);
        }
        SDL_Delay(50);
    }

    SDL_FreeSurface(mario[BAS]);
    SDL_FreeSurface(mario[HAUT]);
    SDL_FreeSurface(mario[GAUCHE]);
    SDL_FreeSurface(mario[DROITE]);
    SDL_FreeSurface(mur);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(caisse_ok);
    SDL_FreeSurface(objectif);

    return toMenu;
}