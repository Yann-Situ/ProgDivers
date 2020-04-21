#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL\SDL_ttf.h>// pour SDL_ttf

int main(int argc, char *argv[])
{
    TTF_Init();// pour ouvrir la sdl_ttf

    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *screen = NULL, *Texte = NULL, *Compteur = NULL;
    int Continuer = 1;
    int temps_pre = 0, temps_act = 0, centiSecondes = 0;
    char chnTemps[30] = "";
    SDL_Event event;
    SDL_Rect posi, posiTemps;
    SDL_Color couleurBlanche = {255,255,255};// /!\ ceci est pour créer une couleur a utiliser avec la sdl_ttf, ne pas l'utiliser avec la SDL
    TTF_Font *PoliceAriBlk = NULL; // pour creer une variable de type police

    PoliceAriBlk = TTF_OpenFont("arialblack.ttf",50);
//TTF_OpenFont("police.ttf",taille) pour charger un police

    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("SDL_ttf test", NULL);

    /* Fonction pour utiliser du texte
            *si vous avez un texte qui change souvent, comme un compte à rebours, utilisez Solid ;
            *si votre texte ne change pas très souvent et que vous voulez blitter votre texte sur un fond uni, utilisez Shaded ;
            *si votre texte ne change pas très souvent mais que vous voulez blitter sur un fond non uni (comme une image), utilisez Blended.*/
    /*  Fonction en Latin1 pour les 3 fonction si dessus:
                *TTF_RenderText_Solid ;
                *TTF_RenderText_Shaded ;
                *TTF_RenderText_Blended
            Ces fonctions prennent en parametre (*police, "texte", couleur en SDL_color,   +couleur en fond(pour shaded seulement))*/

    TTF_SetFontStyle(PoliceAriBlk,TTF_STYLE_UNDERLINE);
    /*  TTF_SetFontStyle(*police, flags) pour le style          flags :
                                                            TTF_STYLE_NORMAL : normal ;
                                                            TTF_STYLE_BOLD : gras ;
                                                            TTF_STYLE_ITALIC : italique ;
                                                            TTF_STYLE_UNDERLINE : souligné.*/
        //a faire avant avoir créer un txt

    Texte = TTF_RenderText_Blended(PoliceAriBlk,"Hello world!", couleurBlanche);

    // Fonction sprintf pour mettre dans un char une variable : sprintf(*char,"texte %d", variable)
    sprintf(chnTemps,"Temps ecoule : %d", centiSecondes);
    Compteur = TTF_RenderText_Blended(PoliceAriBlk, chnTemps, couleurBlanche);

    posiTemps.x = 0;
    posiTemps.y = 0;

    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                break;
        }
        temps_act = SDL_GetTicks();
        if(temps_act-temps_pre > 8)
        {
            centiSecondes += 1;
            temps_pre = temps_act;
        }
        else
        {
            SDL_Delay(8 - (temps_act-temps_pre));
        }
        sprintf(chnTemps,"Temps ecoule : %d", centiSecondes);
        Compteur = TTF_RenderText_Blended(PoliceAriBlk, chnTemps, couleurBlanche);


        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0,0));
        posi.x = screen->w/2 - Texte->w/2;
        posi.y = screen->h/2 - Texte->h/2;
        SDL_BlitSurface(Compteur,NULL,screen,&posiTemps);
        SDL_BlitSurface(Texte,NULL,screen,&posi);
        SDL_Flip(screen);
    }

    TTF_CloseFont(PoliceAriBlk);
    SDL_FreeSurface(Texte);
    SDL_FreeSurface(Compteur);
    SDL_Quit();
    TTF_Quit();// pour fermer la sdl_ttf
    return EXIT_SUCCESS;
}

//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//DL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));
