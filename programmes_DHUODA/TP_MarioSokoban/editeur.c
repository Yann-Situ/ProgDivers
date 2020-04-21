#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "fichiers.h"
#include "editeur.h"

void editer(SDL_Surface* screen, int *Continuer1, int Aventure, char NomLevel[]);

void editer(SDL_Surface* screen, int *Continuer1, int Aventure, char NomLevel[])
{
    SDL_Surface *Mur = NULL, *Caisse = NULL, *CaisseOK = NULL, *Objectif = NULL, *Vide = NULL, *Mario = NULL;
    SDL_Surface *ObjetTenu = NULL;
    SDL_Rect Position, PosiCurseur;
    SDL_Event Event;
    int Continuer = 1, ClicGauche = 0, ClicDroit = 0, tmpsPRE = 0, tmpsACT = 0;// booléens
    int x = 0, y = 0;
    int carte[NB_BLOC_W][NB_BLOC_H] = {0};// WOOOW double crochets ! : comme des coords : il y a [0][0] [0][1] ... [11][11]
    int ObjetActuel = 1;

    FMOD_SYSTEM *systeme;// creation d'un objet systeme,  il ne faut pas oublier de l'allouer
        FMOD_System_Create(&systeme);// allocation de mem pour un objet systeme /!\ ATTENTION ! pointeur sur pointeur d'objet systeme
    FMOD_SOUND *bruit[5], *SonActuel;// pour init un son
    FMOD_CHANNEL *canal[5], *CanalActuel;
    FMOD_RESULT resultat;

    /*
    Creation des VARIABLES fini
    */

    resultat = FMOD_System_Init(systeme, 10, FMOD_INIT_NORMAL, NULL);
    if (resultat != FMOD_OK)
    {//if error
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }
    for(x=0;x<5;x++)
    {
        FMOD_System_GetChannel(systeme, x+1, &canal[x]);
    }

    if (FMOD_System_CreateSound(systeme, "Super Mario kill jump.wav", FMOD_CREATESAMPLE, 0, &bruit[VIDE]) != FMOD_OK)
        fprintf(stderr,"FMOD error! (0), mario-theme-.wav non chargé\n");
    if (FMOD_System_CreateSound(systeme, "Super Mario pops.wav", FMOD_CREATESAMPLE, 0, &bruit[1]) != FMOD_OK)
        fprintf(stderr,"FMOD error! (1), mario-theme-.wav non chargé\n");
    if (FMOD_System_CreateSound(systeme, "Super Mario coin.wav", FMOD_CREATESAMPLE, 0, &bruit[2]) != FMOD_OK)
        fprintf(stderr,"FMOD error! (3), mario-theme-.wav non chargé\n");
    if (FMOD_System_CreateSound(systeme, "Super Mario 1up.wav", FMOD_CREATESAMPLE, 0, &bruit[3]) != FMOD_OK)
        fprintf(stderr,"FMOD error! (4), mario-theme-.wav non chargé\n");
    if (FMOD_System_CreateSound(systeme, "Super Mario it's me.wav", FMOD_CREATESAMPLE, 0, &bruit[4]) != FMOD_OK)
        fprintf(stderr,"FMOD error! (5), mario-theme-.wav non chargé\n");

    SonActuel = bruit[1];
    CanalActuel = canal[1];

    Mur = IMG_Load("mur.jpg");
    Caisse = IMG_Load("caisse.jpg");
    CaisseOK = IMG_Load("caisse_ok.jpg");
    Objectif = IMG_Load("objectif.png");
    Vide = IMG_Load("sol.bmp");
    Mario = IMG_Load("mario_bas.gif");

    ObjetTenu = IMG_Load("mur.jpg");

    if (!chargerNiveau(carte,Aventure,NomLevel))
    {// fonction pour charger les cases dans carte
        fprintf(stderr,"Erreur d'initialisation du niveau : %s\n", SDL_GetError());
        exit(EXIT_FAILURE); // On arrête le jeu si on n'a pas pu charger le niveau
    }
    Position.x = 0;
    Position.y = 0;
    SDL_ShowCursor(SDL_DISABLE);

    while(Continuer)
    {
        SDL_WaitEvent(&Event);
        switch(Event.type)
        {
            case SDL_QUIT:
                Continuer = 0;
                *Continuer1 = 0;
                break;
            case SDL_KEYDOWN:
                switch(Event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        Continuer = 0;
                        break;
                    case SDLK_s:
                        MenuSauvegarder(screen, Aventure, Continuer1, carte,systeme);
                        break;
                    case SDLK_c:
                        Continuer = 0;
                        break;
                    case SDLK_t:
                        jouer(screen,Continuer1, Aventure, NomLevel, 1,0);
                    case SDLK_1:
                        ObjetActuel = 1;

                        break;
                    case SDLK_2:
                        ObjetActuel = 2;

                        break;
                    case SDLK_3:
                        ObjetActuel = 3;

                        break;
                    case SDLK_4:
                        ObjetActuel = 4;

                        break;
                    case SDLK_5:
                        ObjetActuel = 5;

                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(Event.button.button == SDL_BUTTON_LEFT)
                {
                    Position.x = Event.button.x;
                    Position.y = Event.button.y;
                    if(ObjetActuel == MARIO)
                    {
                        for(y = 0; y < NB_BLOC_H; y++)
                        {
                            for(x = 0; x < NB_BLOC_W; x++)
                            {
                                if(carte[x][y] == MARIO)
                                    carte[x][y] = VIDE;
                            }
                        }

                    }
                    if(carte[Position.x/TAILLE_BLOC][Position.y/TAILLE_BLOC] != MARIO)
                    {
                        carte[Position.x/TAILLE_BLOC][Position.y/TAILLE_BLOC] = ObjetActuel;
                        FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, SonActuel, 0, &CanalActuel);
                    }
                    ClicGauche = 1;

                }
                else if(Event.button.button == SDL_BUTTON_RIGHT)
                {
                    Position.x = Event.button.x;
                    Position.y = Event.button.y;
                    if(carte[Position.x/TAILLE_BLOC][Position.y/TAILLE_BLOC] != MARIO)
                    {
                        carte[Position.x/TAILLE_BLOC][Position.y/TAILLE_BLOC] = VIDE;
                        FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, bruit[0], 0, &canal[0]);
                    }

                    ClicDroit = 1;
                }
                switch(Event.button.button)
                {
                    case SDL_BUTTON_WHEELDOWN:
                        ObjetActuel++;
                        if(ObjetActuel>5)
                            ObjetActuel = 5;
                        break;
                    case SDL_BUTTON_WHEELUP:
                        ObjetActuel--;
                        if(ObjetActuel<1)
                            ObjetActuel = 1;
                        break;
                }


                break;
            case SDL_MOUSEBUTTONUP:
                if(Event.button.button == SDL_BUTTON_LEFT)
                    ClicGauche = 0;
                else if(Event.button.button == SDL_BUTTON_RIGHT)
                    ClicDroit = 0;
                break;



            case SDL_MOUSEMOTION:
                if(ClicGauche == 1 && ObjetActuel != MARIO && carte[Event.motion.x / TAILLE_BLOC][Event.motion.y / TAILLE_BLOC] != MARIO)
                {
                    carte[Event.motion.x / TAILLE_BLOC][Event.motion.y / TAILLE_BLOC] = ObjetActuel;
                    tmpsACT = SDL_GetTicks();
                    if(tmpsACT-tmpsPRE>170)
                    {
                        FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, SonActuel, 0, &CanalActuel);
                        tmpsPRE = tmpsACT;
                    }

                }
                else if(ClicDroit == 1 && carte[Event.motion.x / TAILLE_BLOC][Event.motion.y / TAILLE_BLOC] != MARIO)
                {
                    carte[Event.motion.x / TAILLE_BLOC][Event.motion.y / TAILLE_BLOC] = VIDE;
                    tmpsACT = SDL_GetTicks();
                    if(tmpsACT-tmpsPRE>170)
                    {
                        FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, bruit[0], 0, &canal[0]);
                        tmpsPRE = tmpsACT;
                    }
                }
                PosiCurseur.x = Event.motion.x - TAILLE_BLOC/2;
                PosiCurseur.y = Event.motion.y - TAILLE_BLOC/2;
                break;
        }
        switch(ObjetActuel)
            {
                case MUR:
                    ObjetTenu = IMG_Load("mur.jpg");
                        SonActuel = bruit[1];
                        CanalActuel = canal[1];
                    break;
                case CAISSE:
                    ObjetTenu = IMG_Load("caisse.jpg");
                        SonActuel = bruit[2];
                        CanalActuel = canal[2];
                    break;
                case CAISSE_OK:
                    ObjetTenu = IMG_Load("caisse_ok.jpg");
                        SonActuel = bruit[2];
                        CanalActuel = canal[2];
                    break;
                case OBJECTIF:
                    ObjetTenu = IMG_Load("objectif.png");
                        SonActuel = bruit[3];
                        CanalActuel = canal[3];
                    break;
                case MARIO:
                    ObjetTenu = IMG_Load("mario_bas.gif");
                        SonActuel = bruit[4];
                        CanalActuel = canal[4];
                    break;
            }
        SDL_FillRect(screen,NULL, SDL_MapRGB(screen->format,0,0,0));

        for(y = 0; y < NB_BLOC_H; y++)
        {// placement des blocs sur la carte :
            for(x = 0; x < NB_BLOC_W; x++)
            {
                Position.x = x*TAILLE_BLOC;
                Position.y = y*TAILLE_BLOC;// on met les positions réel en pixel et non en case

                switch(carte[x][y])
                {// on blitte les images au bons endroits
                    case VIDE:
                        SDL_BlitSurface(Vide,NULL,screen,&Position);
                        break;
                    case MUR:
                        SDL_BlitSurface(Mur,NULL,screen,&Position);
                        break;
                    case CAISSE:
                        SDL_BlitSurface(Caisse,NULL,screen,&Position);
                        break;
                    case CAISSE_OK:
                        SDL_BlitSurface(CaisseOK,NULL,screen,&Position);
                        break;
                    case OBJECTIF:
                        SDL_BlitSurface(Objectif,NULL,screen,&Position);
                        break;
                    case MARIO:
                        SDL_BlitSurface(Vide,NULL,screen,&Position);
                        SDL_BlitSurface(Mario,NULL,screen,&Position);
                        break;
                }
            }
        }
        SDL_BlitSurface(ObjetTenu,NULL,screen,&PosiCurseur);
        SDL_Flip(screen);
    }
    SDL_FreeSurface(Vide);
    SDL_FreeSurface(Mur);
    SDL_FreeSurface(Caisse);
    SDL_FreeSurface(CaisseOK);
    SDL_FreeSurface(Objectif);
    SDL_FreeSurface(Mario);
    SDL_FreeSurface(ObjetTenu);

    resultat = FMOD_Sound_Release(SonActuel);
    if(resultat!=FMOD_OK)
        fprintf(stderr,"\nFMOD error! SonActuel release faileed (%d)",resultat);

    fprintf(stderr,"\n\n\t\tswagg");
    for(x=0;x<5;x++)
    {
            resultat = FMOD_Channel_Stop(CanalActuel);
            if(resultat!=FMOD_OK)
                fprintf(stderr,"\nFMOD error! canal[%d] stop failed (%d)",x,resultat);
            else
                fprintf(stderr,"\ncanal[%d] stop",x);

        resultat = FMOD_Sound_Release(bruit[x]);
        if(resultat!=FMOD_OK)
            fprintf(stderr,"\nFMOD error! bruit[%d] release failed (%d)",x,resultat);
        else
            fprintf(stderr,"\nbruit[%d] release", x);
    }
    resultat = FMOD_System_Close(systeme);// _____________________________
    if(resultat!=FMOD_OK)
        fprintf(stderr,"\nFMOD error! systeme close failed (%d)",resultat);
    resultat = FMOD_System_Release(systeme);//pour liberer l'objet system
    if(resultat!=FMOD_OK)
        fprintf(stderr,"\nFMOD error! systeme release failed (%d)",resultat);
}
