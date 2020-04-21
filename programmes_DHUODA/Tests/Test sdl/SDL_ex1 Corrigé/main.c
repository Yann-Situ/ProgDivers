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
    SDL_Surface *ecran = NULL, *rectangle = NULL;
    SDL_Rect position;
    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
// Allocation de la surface
    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 220, 180, 32, 0,0, 0, 0);
    SDL_WM_SetCaption("Ma super fenetre SDL !", NULL);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 206,112));
    position.x = 0; // Les coordonnées de la surface seront (0, 0)
    position.y = 0;
// Remplissage de la surface avec du blanc
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 255,255, 255));
    SDL_BlitSurface(rectangle, NULL, ecran, &position); // Collage de la surface sur l'écran
    SDL_Flip(ecran); // Mise à jour de l'écran
    pause();
    SDL_FreeSurface(rectangle); // Libération de la surface
    SDL_Quit();
    return EXIT_SUCCESS;
}
