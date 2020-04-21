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

    SDL_Surface *screen = NULL, *Texte = NULL, *Poing = NULL;
    SDL_Rect Position, PosiPoing;
    SDL_Event event;
    int Continuer = 1, v = 255;
    SDL_Color couleurBlanche = {255,255,255};
    TTF_Font *police = NULL;
    float Volume = 1.0;

    FMOD_SYSTEM *systeme;// creation d'un objet systeme,  il ne faut pas oublier de l'allouer

    FMOD_SOUND *son = NULL, *musique;// pour init un son

    FMOD_CHANNELGROUP *channel;// pour un groupe de canals
    FMOD_CHANNEL *canalmusique, *canalPoing;

    FMOD_RESULT resultat;
    /*
    Creation des VARIABLES fini
    */
    FMOD_System_Create(&systeme);// allocation de mem pour un objet systeme /!\ ATTENTION ! pointeur sur pointeur d'objet systeme
    resultat = FMOD_System_Init(systeme, 2, FMOD_INIT_NORMAL, NULL);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }

    police = TTF_OpenFont("angelina.ttf", 30/*taille*/);
    if(police == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if(screen == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation du mode video : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    resultat = FMOD_System_CreateSound(systeme, "punch.wav", FMOD_CREATESAMPLE, 0, &son);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }

    resultat = FMOD_System_CreateSound(systeme, "Final Fantasy 13-2 OST - Paradigm Shift.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &musique);// pour les musiques il faut tou ça
    if (resultat != FMOD_OK)
    {
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }

    SDL_WM_SetCaption("gestion du son en SDL", NULL);

    Poing = IMG_Load("poing.bmp");
    SDL_SetColorKey(Poing, SDL_SRCCOLORKEY, SDL_MapRGB(Poing->format, 255, 100, 100));
    PosiPoing.x = 100;
    PosiPoing.y = 100;

    Texte = TTF_RenderText_Blended(police,"Hello world!", couleurBlanche);//          TTF_RenderText_Shaded     -    TTF_RenderText_Solid
    Position.x = screen->w/2 - Texte->w/2;
    Position.y = screen->h/2 - Texte->h/2;

    FMOD_System_GetChannel(systeme, 1, &canalmusique);
    FMOD_System_GetChannel(systeme, 2, &canalPoing);

    FMOD_Sound_SetLoopCount(musique, -1);// pour repeter la musique : on met le nombre de fois ou elle est repeté, si on met -1 cest l'infini
    SDL_EnableKeyRepeat(500,200);
    SDL_ShowCursor(SDL_DISABLE);
    /*
    FIN DES INITIALISATIONS
    */
    FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, musique, 0, &canalmusique);
    while(Continuer == 1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                fprintf(stderr, "QUIT");
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_DOWN:
                        if(Volume>0)
                            Volume -= 0.1;
                        FMOD_System_GetMasterChannelGroup(systeme,&channel);
                        FMOD_ChannelGroup_SetVolume(channel,Volume);
                        v = 255 * Volume;
                    break;
                    case SDLK_UP:
                        if(Volume<1)
                            Volume += 0.1;
                        FMOD_System_GetMasterChannelGroup(systeme,&channel);
                        FMOD_ChannelGroup_SetVolume(channel,Volume);
                        v = 255 * Volume;
                    break;
                    case SDLK_p:
                        FMOD_BOOL etat;
                        FMOD_Channel_GetPaused(canalmusique, &etat);// ceci met ds etat l'etat de la chanson
                        if (etat == 1) // Si la chanson est en pause
                            FMOD_Channel_SetPaused(canalmusique, 0); // On enlève la pause
                        else // Sinon, elle est en cours de lecture
                            FMOD_Channel_SetPaused(canalmusique, 1); // On met en pause
                    break;/*FMOD_ChannelGroup_GetPaused et FMOD_ChannelGroup_SetPaused, à la seule différence qu'il faut faire passer
                            comme paramètre un FMOD_CHANNELGROUP au lieu d'un FMOD_CHANNEL.*/
                }
            break;
            case SDL_MOUSEBUTTONDOWN:
                FMOD_System_GetChannel(systeme, 2, &canalPoing);
                FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, son, 0, &canalPoing);
                break;
            case SDL_MOUSEMOTION:
                PosiPoing.x = event.motion.x - Poing->w/2;
                PosiPoing.y = event.motion.y - Poing->h/2;
                break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, v,v,v));
        SDL_BlitSurface(Texte,NULL,screen,&Position);
        SDL_BlitSurface(Poing,NULL,screen,&PosiPoing);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(Texte);
    SDL_FreeSurface(Poing);
    TTF_CloseFont(police);
    FMOD_System_Close(systeme);// _____________________________
    FMOD_System_Release(systeme);//pour liberer l'objet system
    FMOD_Sound_Release(son);
    FMOD_Sound_Release(musique);
    SDL_Quit();
    TTF_Quit();
    fprintf(stderr, "FIN");
    return EXIT_SUCCESS;
}//Il suffit d'appeler FMOD_Channel_Stop pour stopper une musique sur un canal, ou bien FMOD_ChannelGroup_Stop pour stopper la musique

//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//DL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));
