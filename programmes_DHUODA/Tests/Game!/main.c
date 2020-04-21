/*
GAME    Yann-Situ Gazull
____
main.c
fichier principal du projet
*/
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "../fonction_0/fonction_0.h"
#include "constante.h"
#include "structure.h"
#include "ecrans_menus.h"
#include "jouer.h"

int main(int argc, char *argv[])
{
    TTF_Init();
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //CREAT VAR
    OBJF* smain = NULL;
    int fichier = 0;
    //INIT VAR
    {// SMAIN
        FMOD_RESULT resultat;
        smain = (OBJF*)malloc(sizeof(OBJF));
        smain->continuer = 1;
        FMOD_System_Create(&smain->systeme);// allocation de mem pour un objet systeme /!\ ATTENTION ! pointeur sur pointeur d'objet systeme
        resultat = FMOD_System_Init(smain->systeme, MAX_CHANNEL, FMOD_INIT_NORMAL, NULL);
        if (resultat != FMOD_OK)                            //init systeme
        {//if error
            fprintf(stderr,"FMOD error! (%d)\n", resultat);
            return 0;
        }

        smain->screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        checkerror(smain->screen, NULL,SDL);

        smain->policeTitle = TTF_OpenFont("moonlight.ttf",POLICE_TITLE_SIZE);
        if(smain->policeTitle == NULL)                                  //init police
        {//if error
            fprintf(stderr, "Erreur d'initialisation de la policetitle de caractere : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        smain->police1 = TTF_OpenFont("fairydust.ttf",POLICE1_SIZE);
        if(smain->police1 == NULL)                                  //init police
        {//if error
            fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        smain->police2 = TTF_OpenFont("dungeon.ttf",POLICE2_SIZE);
        if(smain->police2 == NULL)                                  //init police
        {//if error
            fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        smain->police3 = TTF_OpenFont("calibri.ttf",POLICE3_SIZE);
        if(smain->police3 == NULL)                                  //init police
        {//if error
            fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    //EVOLUTION

    while( smain->continuer != 0 )
    {
        while(ecran_titre(smain) != 0)
        {
            while(ecran_fichier(smain, &fichier) != 0)
            {
                if(fichier < 4)
                {
                    while(ecran_map(smain, fichier) != 0)
                    {
                        /*while(ecran-jeu != 0)
                        {

                        }*/
                    }
                }
                /*else
                {
                    while(ecran-option != 0)
                    {

                    }
                }*/
            }
        }
    }

    //FREE VAR
    freeOBJF(smain);

    //FIN DU PROGRAMME
    SDL_Quit();
    TTF_Quit();
    fprintf(stderr,"FIN DU PROGRAMME");
    return EXIT_SUCCESS;
}

//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\libfmodex.a
//SDL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));
//int SDL_SetAlpha(SDL_Surface *surface, SDL_SRCALPHA, Uint8 alpha);

