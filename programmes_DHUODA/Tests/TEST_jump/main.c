#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "fonctions.h"

int main(int argc, char *argv[])
{
    fprintf(stderr,"DEBUT JUMP AROUND\n*****************\n");
    TTF_Init();
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    OBJF *smain = NULL;
    SDL_Surface *Texte = NULL, *rectangle = NULL, *sol = NULL;
    SDL_Rect PositionSOL, PositionRECT;
    SDL_Color couleurBlanche = {255,255,255}, couleurNOIRE = {0,0,0};
    SDL_Event event;
    Uint8 *Keystate = SDL_GetKeyState(NULL);
    FMOD_SOUND *son_jump;
    FMOD_RESULT resultat;

    double v_x = 0, v_y = 0, gravity = GRAVITY, adherence = ADHERENCE, h_jump = H_JUMP, frame = 10, acceleration = ACCELERATION, vit_max = VIT_MAX;
    int etat = AIR, memPositionRECT_x = 0, memPositionRECT_y = 0;

    /*
    Creation des VARIABLES fini
    */

    // SMAIN
    smain = (OBJF*)malloc(sizeof(OBJF));
    {
    smain->continuer = 1;
    FMOD_System_Create(&smain->systeme);// allocation de mem pour un objet systeme /!\ ATTENTION ! pointeur sur pointeur d'objet systeme
    resultat = FMOD_System_Init(smain->systeme, 1, FMOD_INIT_NORMAL, NULL);
    if (resultat != FMOD_OK)                            //init systeme
    {//if error
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }

    smain->screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    checkerror(smain->screen, NULL,SDL);

    smain->policeTitle = TTF_OpenFont("angelina.ttf",POLICE_TITLE_TAILLE /*taille*/);
    if(smain->policeTitle == NULL)                                  //init police
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la policetitle de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    TTF_SetFontStyle(smain->policeTitle, TTF_STYLE_UNDERLINE);

    smain->police = TTF_OpenFont("angelina.ttf",POLICE_TAILLE /*taille*/);
    if(smain->police == NULL)                                  //init police
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    }

    SDL_WM_SetCaption("JUMP AROUND !", NULL);
    //Texte = TTF_RenderText_Blended(police,"Hello world!", couleurBlanche);/*    TTF_RenderText_Shaded       TTF_RenderText_Solid    */

    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 60, 32, 0, 0, 0, 0);
    sol = SDL_CreateRGBSurface(SDL_HWSURFACE, smain->screen->w, 100, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 0,255,0));
    SDL_FillRect(sol, NULL, SDL_MapRGB(sol->format, 0,0,0));

    PositionRECT.x = 0;
    PositionRECT.y = 0;

    PositionSOL.x = 0;
    PositionSOL.y = smain->screen->h - sol->h;

    memPositionRECT_x = smain->screen->w/2 - rectangle->w/2;
    memPositionRECT_y = smain->screen->h/2 - rectangle->h/2;
    /*
    FIN DES INITIALISATION
    */

    while(smain->continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                smain->continuer = 0;
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        if(v_y > 0)
                           v_y = 0;
                    break;
                    default:
                    break;
                }
        }
        // INPUT ******************
        if(Keystate[SDLK_SPACE])
        {
            if(etat != AIR)
            {
                v_y = h_jump;
            }
        }
        if(Keystate[SDLK_RIGHT])
        {
            v_x += acceleration;
            if(v_x > vit_max)
                v_x = vit_max;
        }
        else if(Keystate[SDLK_LEFT])
        {
            v_x -= acceleration;
            if(v_x < -vit_max)
                v_x = -vit_max;
        }
        // EVOLUTION **************
        if(etat == AIR)
        {
            v_y -= gravity;
        }
        if(v_x > 0)
        {
            v_x -= adherence;
            if(v_x < 0)
                v_x = 0;
        }
        if(v_x < 0)
        {
            v_x += adherence;
            if(v_x > 0)
                v_x = 0;
        }

        memPositionRECT_x += v_x;
        memPositionRECT_y -= v_y;

        if(memPositionRECT_y + rectangle->h > smain->screen->h - sol->h)
        {
            memPositionRECT_y = smain->screen->h - sol->h - rectangle->h;
            etat = TERRE;
        }
        else
        {
            etat = AIR;
        }

        if(memPositionRECT_x > smain->screen->w)
        {
            memPositionRECT_x = 0 - rectangle->w;
        }

        if(memPositionRECT_x + rectangle->w < 0)
        {
            memPositionRECT_x = smain->screen->w;
        }
        PositionRECT.x = memPositionRECT_x;
        PositionRECT.y = memPositionRECT_y;

        // BLITS ******************
        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 255,255,255));

        SDL_BlitSurface(sol,NULL,smain->screen,&PositionSOL);
        SDL_BlitSurface(rectangle,NULL,smain->screen,&PositionRECT);

        SDL_Flip(smain->screen);

        SDL_Delay(frame);
    }

    /*
    FIN DU PROGRAMME                                            fermeture+liberation de la memoire
    */

    SDL_FreeSurface(Texte);
    SDL_FreeSurface(rectangle);
    freeOBJF(smain);
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}

//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\libfmodex.a
//SDL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));


