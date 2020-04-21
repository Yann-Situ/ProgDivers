#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include "constante.h"

void animation(SDL_Rect *RectTile, int *NumAnim, int MaxTiles);
int toucheDejaEnfonce(int touche[]);

int main(int argc, char *argv[])
{
    TTF_Init();
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *screen = NULL, *Texte = NULL, *perso = NULL;
    perso = IMG_Load("babydoll_tileset.bmp"/*"Joubert test.bmp"*/);
    if(perso == NULL)
    {
        fprintf(stderr,"image non charge : %s",SDL_GetError());
        return 0;
    }
    SDL_SetColorKey(perso,SDL_SRCCOLORKEY, SDL_MapRGB(perso->format, 255,100,100));
    SDL_Rect Position, PositionHello;
    SDL_Rect partieTilset;
    SDL_Event event;
    int Continuer = 1, toucheEnf[8] = {0}, reapatAnim = 1;
    int NumTile = 0, Temps_pre = 0, Temps_pre_Dodge = 0, Temps_act = 0, DureePas = 200, Direct = BAS, i=0, MaxTiles = 10;
    float vitesseINIT = 1, vitesse = vitesseINIT;
    SDL_Color couleurNoir = {0,0,0};
    TTF_Font *police = NULL;

    partieTilset.w = 18;
    partieTilset.h = 26;
    partieTilset.y = 0;
    partieTilset.x = 0;

    police = TTF_OpenFont("angelina.ttf", 30);
    if(police == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Texte = TTF_RenderText_Blended(police,"Hello world!", couleurNoir);
//          TTF_RenderText_Shaded
//          TTF_RenderText_Solid
    screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(screen == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation du mode video : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("animation", NULL);


PositionHello.x = screen->w/2 - Texte->w/2;
PositionHello.y = 0;
Position.x = screen->w/2 - perso->w/2;
Position.y = screen->h/2 - perso->h/2;

    while(Continuer)
    {
        partieTilset.y = Direct*26;
        Temps_act = SDL_GetTicks();
        if(Temps_act - Temps_pre > (2*DureePas)/(10)&&toucheDejaEnfonce(toucheEnf))
        {
            animation(&partieTilset, &NumTile, MaxTiles);
            if(reapatAnim == 0&&NumTile == 0)
            {
                switch(Direct)
                        {
                            case DBAS:
                                Direct = BAS;
                                toucheEnf[DBAS] = 0;
                            break;
                            case DDROITE:
                                Direct = DROITE;
                                toucheEnf[DDROITE] = 0;
                            break;
                            case DGAUCHE:
                                Direct = GAUCHE;
                                toucheEnf[DGAUCHE] = 0;
                            break;
                            case DHAUT:
                                Direct = HAUT;
                                toucheEnf[DHAUT] = 0;
                            break;
                        }
                    MaxTiles = 10;
                    reapatAnim = 1;
                    vitesse = vitesseINIT;
            }
            Temps_pre = Temps_act;
        }
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_DOWN:
                        if(!toucheDejaEnfonce(toucheEnf))
                            Direct = BAS;
                        toucheEnf[BAS] = 1;

                    break;
                    case SDLK_RIGHT:
                        if(!toucheDejaEnfonce(toucheEnf))
                            Direct = DROITE;
                        toucheEnf[DROITE] = 1;
                    break;
                    case SDLK_LEFT:
                        if(!toucheDejaEnfonce(toucheEnf))
                            Direct = GAUCHE;
                        toucheEnf[GAUCHE] = 1;
                    break;
                    case SDLK_UP:
                        if(!toucheDejaEnfonce(toucheEnf))
                            Direct = HAUT;
                        toucheEnf[HAUT] = 1;
                    break;
                    case SDLK_SPACE:
                        if(vitesse <= vitesseINIT)
                            vitesse = vitesse * 2;
                    break;
                    case SDLK_RSHIFT:
                        if(Temps_act - Temps_pre_Dodge > 1000)
                        {
                            NumTile = 0;
                            MaxTiles = 5;
                            reapatAnim = 0;
                            switch(Direct)
                            {
                                case BAS:
                                    Direct = DBAS;
                                    toucheEnf[DBAS] = 1;
                                    toucheEnf[BAS] = 0;
                                break;
                                case DROITE:
                                    Direct = DDROITE;
                                    toucheEnf[DDROITE] = 1;
                                    toucheEnf[DROITE] = 0;
                                break;
                                case GAUCHE:
                                    Direct = DGAUCHE;
                                    toucheEnf[DGAUCHE] = 1;
                                    toucheEnf[GAUCHE] = 0;
                                break;
                                case HAUT:
                                    Direct = DHAUT;
                                    toucheEnf[DHAUT] = 1;
                                    toucheEnf[HAUT] = 0;
                                break;
                            }
                            vitesse = 4.5;
                            Temps_pre_Dodge = Temps_act;
                        }

                    break;
                }
            break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_DOWN:
                        toucheEnf[BAS] = 0;
                    break;
                    case SDLK_RIGHT:
                        toucheEnf[DROITE] = 0;
                    break;
                    case SDLK_LEFT:
                        toucheEnf[GAUCHE] = 0;
                    break;
                    case SDLK_UP:
                        toucheEnf[HAUT] = 0;
                    break;
                    case SDLK_SPACE:
                        if(vitesse > vitesseINIT)
                            vitesse= vitesseINIT;
                    break;
                }
                break;
        }

        SDL_Delay(DureePas/(16*vitesse));// SDL_Delay(ms) pour mettre en pause le programme un certain nombre de ms

        if(toucheEnf[BAS] == 1)
            Position.y++;
        if(toucheEnf[DROITE] == 1)
            Position.x++;
        if(toucheEnf[GAUCHE] == 1)
            Position.x--;
        if(toucheEnf[HAUT] == 1)
            Position.y--;
        if(toucheEnf[DBAS] == 1)
            Position.y--;
        if(toucheEnf[DDROITE] == 1)
            Position.x--;
        if(toucheEnf[DGAUCHE] == 1)
            Position.x++;
        if(toucheEnf[DHAUT] == 1)
            Position.y++;
        else if(!toucheDejaEnfonce(toucheEnf))
        {
            partieTilset.x = 0;
            NumTile = 0;
        }
        if(Position.y > screen->h)
            Position.y = Texte->h;
        if(Position.x > screen->w)
            Position.x = 0;
        if(Position.y < Texte->h)
            Position.y = screen->h;
        if(Position.x < 0)
            Position.x = screen->w;

        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,190,210,150));
        SDL_BlitSurface(Texte,NULL,screen,&PositionHello);
        SDL_BlitSurface(perso,&partieTilset,screen,&Position);
        SDL_Flip(screen);
    }
    SDL_FreeSurface(Texte);
    SDL_FreeSurface(perso);
    TTF_CloseFont(police);
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}

void animation(SDL_Rect *RectTile, int *NumAnim, int MaxTiles)
{
    RectTile->x = (*NumAnim+1)*18;
    *NumAnim = *NumAnim + 1;
    if(*NumAnim > MaxTiles-1)
        *NumAnim = 0;
}

int toucheDejaEnfonce(int touche[])
{
    int i = 0, OK = 0;
    while(i<8)
    {
        if(touche[i] == 1)
            OK = 1;
        i++;
    }
    return OK;
}
//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//DL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));
