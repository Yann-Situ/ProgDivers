#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

void pause()
{
    int continuer = 1;
    SDL_Event event;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
            continuer = 0;
        }
    }
}


int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur stderr fichier auto pour les erreurs
        exit(EXIT_FAILURE); // On quitte le programme                                // SDL_GetError() renvoie derniere erreur de la SDL
    }//                     EXIT_FAILURE est un Define pour tout OS representant valeur à renvoyer en cas d'échec du programme

    SDL_Surface *Lignes[256] = {NULL}, *screen = NULL;
    SDL_Rect Position;
    int i = 0, r=255, j=0;

    screen = SDL_SetVideoMode(640, 256, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Degrade", NULL);// DSL_WM_SetCaption( nom de la fenetre , NULL ) pour changer le nom de la fenetre

    for(i=0; i<=255; i++)
    {
        Lignes[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 1, 32, 0,0,0,0);
    }// alloue la memoire pour toutes les lignes

    for(i=0; i<=255; i++)
    {
        Position.x = 0;
        Position.y = i;
        SDL_FillRect(Lignes[i], NULL, SDL_MapRGB(screen->format, j, j, r));
        SDL_BlitSurface(Lignes[i],NULL,screen,&Position);
        r--;
    }
    SDL_Flip(screen);/* SDL_Flip(ecran) pour Mise à jour de l'écran */
    pause();
    for (i = 0 ; i <= 255 ; i++) // libérer les 256 surfaces
    {
        SDL_FreeSurface(Lignes[i]);
    }
    return 0;
}
