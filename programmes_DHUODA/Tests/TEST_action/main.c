#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "consante.h"
#include "action.h"

int main(int argc, char *argv[])
{
    TTF_Init();
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *screen = NULL, *Texte = NULL, *Tileset = NULL;
    SDL_Rect PositionTexte, PositionPerso, RECTtile;
    SDL_Event event;
    int Continuer = 1, j_enfonce = 0;
    int i=0, j=0, NumTile = 0;
    Uint8 *Keystate = SDL_GetKeyState(NULL);
    float vitesseInitiale = 1, vitessePerso = vitesseInitiale, vitesseCOURIR = vitesseInitiale;
    SDL_Color couleurBlanche = {255,255,255};
    TTF_Font *police = NULL;
    C_timer timerFRAME = {0,19}, timerANIM = {0,39}, timerDODGE = {0,600}, timerATTACK = {0,2*timerFRAME.intervalle*8}, timerTIR = {0,700};
    C_timer timerTest = {0,500};

    action actions[NB_ACTIONS][4];

    /*
    Creation des VARIABLES fini
    */

    police = TTF_OpenFont("angelina.ttf", 20/*taille*/);
    if(police == NULL)                                  //init police
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(screen == NULL)                                  //init ecran
    {//if error
        fprintf(stderr, "Erreur d'initialisation du mode video : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Tileset = IMG_Load("babydoll_tileset.bmp");
    if(Tileset == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de ''babydoll_tileset1'' : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_SetColorKey(Tileset, SDL_SRCCOLORKEY, SDL_MapRGB(Tileset->format, 255, 100, 100));
    SDL_WM_SetCaption("TEST_action", NULL);
    Texte = TTF_RenderText_Blended(police,"Hello world!", couleurBlanche);/*    TTF_RenderText_Shaded       TTF_RenderText_Solid    */

    if(!INITactions(actions,NB_ACTIONS,4))
    {//if error
        fprintf(stderr,"INITactions n'a pas fonctionné");
        exit(EXIT_FAILURE);
    }
    for(i=0;i<NB_ACTIONS;i++)
    {// ecriture des actions ds stderr
        for(j=0;j<4;j++)
        {
            fprintf(stderr, "actions[%d][%d]  x:%d   y:%d\n",i,j,actions[i][j].effets.directionX,actions[i][j].effets.directionY);
        }
        fprintf(stderr, "\n");
    }

    action actionACT = actions[STOP][DOWN];
    PositionTexte.x = screen->w/2-Texte->w/2;
    PositionTexte.y = 0;

    RECTtile.w = L_TILE_PERSO;
    RECTtile.h = L_TILE_PERSO;
    RECTtile.x = 0;
    RECTtile.y = 0;
    PositionPerso.x = screen->w/2-RECTtile.w/2;
    PositionPerso.y = screen->h/2-RECTtile.h/2;
    /*
    FIN DES INITIALISATION
    */

    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {

                    case SDLK_s: case SDLK_DOWN:
                        break;
                    case SDLK_d: case SDLK_RIGHT:
                        break;
                    case SDLK_a: case SDLK_LEFT:
                        break;
                    case SDLK_w: case SDLK_UP:
                        break;

                    case SDLK_j:
                        if(add_timer(&timerATTACK))
                        {
                            if(j_enfonce == 0)
                            {
                                j_enfonce = 1;
                                NumTile = 0;
                                actionACT = actions[ATTACK][actionACT.ligneTileset%4];
                            }
                            else
                            {
                                NumTile = 0;
                                actionACT = actions[CHARGE_ATTACK][actionACT.ligneTileset%4];
                            }
                        }
                        break;

                    case SDLK_i:
                        if(add_timer(&timerTIR))
                        {
                                NumTile = 0;
                                actionACT = actions[TIR][actionACT.ligneTileset%4];
                        }
                        break;

                    case SDLK_LSHIFT:
                        if(add_timer(&timerDODGE))
                        {
                            NumTile = 0;
                            actionACT = actions[DODGE][actionACT.ligneTileset%4];
                        }
                        break;

                    case SDLK_SPACE:
                        if(vitessePerso<=1)
                            vitesseCOURIR = 1.7;
                        break;
                    default:
                        break;
                }

            break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_s: case SDLK_DOWN:
                        break;
                    case SDLK_d: case SDLK_RIGHT:
                        break;
                    case SDLK_a: case SDLK_LEFT:
                        break;
                    case SDLK_w: case SDLK_UP:
                        break;

                    case SDLK_j:
                        j_enfonce = 0;
                        if((actionACT.NumTYPE-actionACT.NumTYPE%4)/4 == CHARGE_ATTACK || (actionACT.NumTYPE-actionACT.NumTYPE%4)/4 == MOOVE_ATTACK)
                        {
                            actionACT = actions[STOP][actionACT.NumTYPE%4];
                        }
                        break;

                    case SDLK_SPACE:
                        vitesseCOURIR = vitesseInitiale;
                        break;
                    default:
                        break;
                }
            break;

        }

        SDL_PumpEvents();

        actionACT = boutonENFONCE(actionACT,actions,Keystate);

        RECTtile.y = actionACT.ligneTileset * L_TILE_PERSO;
        vitessePerso = vitesseInitiale*actionACT.effets.vitesseVar*vitesseCOURIR;
        timerFRAME.intervalle = 20/vitessePerso;
        if(add_timer(&timerFRAME))
        {
            if(add_timer(&timerANIM))
                NumTile = animation(&RECTtile,&actionACT,actions,NumTile);
            PositionPerso = bougerperso(screen,PositionPerso,actionACT);
        }
        else
        {
            SDL_Delay(1);
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100,100,100));
        SDL_BlitSurface(Texte,NULL,screen,&PositionTexte);
        SDL_BlitSurface(Tileset,&RECTtile,screen,&PositionPerso);
        SDL_Flip(screen);
    }

    /*
    FIN DU PROGRAMME                                            fermeture+liberation de la memoire
    */
    SDL_FreeSurface(Texte);
    SDL_FreeSurface(Tileset);
    TTF_CloseFont(police);
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}

//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\libfmodex.a
//SDL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));
