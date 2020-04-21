#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>// pour SDL_ttf
#include <FMOD/fmod.h>
#include "constante.h"
#include "jeu.h"
#include "fichiers.h"
#include "editeur.h"
#include "affichage.h"
/*
Main Rôle : menu du jeu. Permet de choisir entre l'éditeur et le jeu lui-même.
*/
int main(int argc, char *argv[])
{

    SDL_Surface *screen = NULL, *Menu = NULL, *Selection = NULL;
    SDL_Rect PositionMenu, positionSelect;
    SDL_Event Event;
    int i = 0, Continuer = 1, Choix = 0;

    TTF_Init();
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetIcon(IMG_Load("caisse.jpg"), NULL);
    screen = SDL_SetVideoMode(FENETRE_W, FENETRE_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban", NULL);

    Menu = IMG_Load("menu.jpg");
    PositionMenu.x = 0;
    PositionMenu.y = 0;
    Selection = IMG_Load("select.bmp");
    SDL_SetColorKey(Selection,SDL_SRCCOLORKEY, SDL_MapRGB(Selection->format, 255, 0, 0));
    positionSelect.x = 66;
    positionSelect.y = 192;

    SDL_EnableKeyRepeat(200,200);
    while(Continuer)
    {
        SDL_WaitEvent(&Event);
        switch(Event.type)
        {
            case SDL_QUIT:
                Continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(Event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        Continuer = 0;
                        break;
                    case SDLK_1:
                        MenuJouer(screen,Selection, &Continuer);
                        break;
                    case SDLK_2:
                        break;
                    case SDLK_DOWN:
                        if(Choix<1)
                        {
                            positionSelect = curseur(&positionSelect, 30, BAS);
                            Choix += 1;
                        }
                        break;
                    case SDLK_UP:
                        if(Choix>0)
                        {
                            positionSelect = curseur(&positionSelect, 30, HAUT);
                            Choix -= 1;
                        }
                        break;
                    case SDLK_RETURN:
                        if(Choix == 0)
                        {
                            MenuJouer(screen,Selection, &Continuer);
                        }
                        else if(Choix == 1)
                            MenuEditeur(screen,Selection, &Continuer);
                        break;
                }
                break;
        }
        SDL_FillRect(screen,NULL, SDL_MapRGB(screen->format,0,0,0));
        SDL_BlitSurface(Menu,NULL,screen,&PositionMenu);
        SDL_BlitSurface(Selection,NULL,screen,&positionSelect);
        SDL_Flip(screen);
    }
    SDL_FreeSurface(Selection);
    SDL_FreeSurface(Menu);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}


