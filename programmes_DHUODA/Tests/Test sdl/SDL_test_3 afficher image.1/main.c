#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp

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

    SDL_Surface *screen = NULL, *ImageFond = NULL, *Zozor = NULL, *Sapin = NULL;
    SDL_Rect PositionFond, PositionZozor, PositionSapin;
    PositionFond.x = 0;
    PositionFond.y = 0;
    PositionZozor.x = 500;
    PositionZozor.y = 260;
    PositionSapin.x = 600;
    PositionSapin.y = 260;

    SDL_WM_SetIcon(IMG_Load("sdl_icone.bmp"), NULL);/*
    SDL_WM_SetIcon(Surface_qui_contient_l'image, info_sur_la_transparence) pour changer l'icone de la fenetre
    /!\ le faire avant SDL_SetVideoMode !   */
    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Affichage d'images en SDL", NULL);


    ImageFond = IMG_Load("lac_en_montagne.bmp");// fonction IMG_Load("image.bmp") Alloue la mem pour la surface et la remplie de l'image.
    SDL_BlitSurface(ImageFond, NULL, screen, &PositionFond);


    Zozor = IMG_Load("zozor.bmp");
    SDL_SetColorKey(Zozor, SDL_SRCCOLORKEY, SDL_MapRGB(Zozor->format, 0, 0, 255));/*            Certains format gere la transpa comme .png et .gif
    SDL_SetColorKey(Surface, SDL_SRCCOLORKEY, Couleur) pour rendre une couleur sur l'image transparente à faire avant blitter l'image*/
    SDL_SetAlpha(Zozor, SDL_SRCALPHA, 128);/*
    SDL_SetAlpha(Surface, SDL_SRCALPHA, transpaA entre 0(invisible) et 255) pour la transparence Alpha | 128 est plus rapide à calculer*/
    SDL_BlitSurface(Zozor, NULL, screen, &PositionZozor);


    Sapin = IMG_Load("sapin.png");
    SDL_BlitSurface(Sapin, NULL, screen, &PositionSapin);// pas besoin de gérer la transpa car .png | mais il faut cependant installer la SDL_image :
            /* Dabord placer les DLL ds le fichier du projet | ensuite project\build option\linker sttings\add | ecrire :
            C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib | pour finir rajouter #include <SDL_image.h> au début du programme */

    SDL_Flip(screen);
    pause();
    SDL_FreeSurface(ImageFond);
    SDL_FreeSurface(Sapin);
    SDL_Quit();
    return 0;
}
