#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

int main(int argc, char *argv[])
{
    TTF_Init();
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *screen = NULL, *Texte = NULL;
    SDL_Rect Position;
    SDL_Event event;
    int Continuer = 1;
    SDL_Color couleurBlanche = {255,255,255};
    TTF_Font *police = NULL;
    FMOD_SYSTEM *systeme;// creation d'un objet systeme,  il ne faut pas oublier de l'allouer
        FMOD_System_Create(&systeme);// allocation de mem pour un objet systeme /!\ ATTENTION ! pointeur sur pointeur d'objet systeme
    FMOD_SOUND *musique;
    FMOD_CHANNEL *canalMusique = NULL;
    FMOD_RESULT resultat;

    /*
    Creation des VARIABLES fini
    */

    resultat = FMOD_System_Init(systeme, /*NB canals */, FMOD_INIT_NORMAL, NULL);
    if (resultat != FMOD_OK)                            //init systeme
    {//if error
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }
    resultat = FMOD_System_CreateSound(systeme, "musique", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &musique);// FMOD_System_CreateSound(systeme, "son court", FMOD_CREATESAMPLE, 0, &son); pour les sons courts
    if (resultat != FMOD_OK)                            //init son
    {//if error
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }
    police = TTF_OpenFont("police.ttf", 20/*taille*/);
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

    SDL_WM_SetCaption("nom", NULL);
    Texte = TTF_RenderText_Blended(police,"Hello world!", couleurBlanche);/*    TTF_RenderText_Shaded       TTF_RenderText_Solid    */
    FMOD_System_GetChannel(systeme, 1, &canalMusique);// pour init un canal
    FMOD_Sound_SetLoopCount(musique, -1);// pour repeter la musique : on met le nombre de fois ou elle est repeté, si on met -1 cest l'infini

    /*
    FIN DES INITIALISATION
    */

    FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE/*ou FMOD_CHANNEL_FREE */ , musique, 0, &canalMusique);
    while(Continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
        SDL_Flip(screen);
    }

    /*
    FIN DU PROGRAMME                                            fermeture+liberation de la memoire
    */

    SDL_FreeSurface(Texte);
    TTF_CloseFont(police);
    FMOD_System_Close(systeme);// _____________________________
    FMOD_System_Release(systeme);//pour liberer l'objet system
    FMOD_Sound_Release(musique);
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}

//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\libfmodex.a
//SDL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));
