#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *screen = NULL, *Link = NULL;
    SDL_Rect position;
    bool Continuer = true, Droite = true, Haut = true;
    SDL_Event event;
    int Temps_pre = 0, Temps_act = 0, Pause = 0;

    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("gestion du temps", NULL);

    SDL_FillRect(screen,NULL, SDL_MapRGB(screen->format, 255,255,255));

    Link = IMG_Load("link.bmp");
    position.x = screen->w/2 - Link->w/2;
    position.y = screen->h/2 - Link->h/2;
    SDL_SetColorKey(Link, SDL_SRCCOLORKEY, SDL_MapRGB(Link->format, 0, 0, 255));
    SDL_BlitSurface(Link, NULL, screen, &position);

    SDL_EnableKeyRepeat(500,500);
    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                Continuer = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_p)
                {
                    if(Pause == 0)
                        Pause = 1;
                    else
                        Pause = 0;
                }
                break;
        }
        if(Pause == 0)
        {
            Temps_act = SDL_GetTicks();// SDL_GetTicks retourne le temps qu'il s'est écoulé depuis le début du programme en ms
            if(Temps_act - Temps_pre > 10)
            {
                if(Droite)
                    position.x++;
                else
                    position.x--;
                if(Haut)
                    position.y--;
                else
                    position.y++;

                if(position.x > screen->w - Link->w)
                    Droite = false;
                if(position.y > screen->h - Link->h)
                    Haut = true;
                if(position.x < 0)
                    Droite = true;
                if(position.y < 0)
                    Haut = false;
                Temps_pre = Temps_act;
            }
            else
            {
                SDL_Delay(10 -  (Temps_act-Temps_pre));// SDL_Delay(ms) pour mettre en pause le programme un certain nombre de ms
            }// on fait ce else pour utiliser mons de processeur
        }
        SDL_FillRect(screen,NULL, SDL_MapRGB(screen->format, 255,255,255));
        SDL_BlitSurface(Link, NULL, screen, &position);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(Link);
    SDL_Quit();
    return EXIT_SUCCESS;
}
