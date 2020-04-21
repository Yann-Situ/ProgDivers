#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "fichiers.h"
#include "jeu.h"

int jouer(SDL_Surface* screen, int *Continuer1, int Aventure, char NomLevel[],int edition, int LevelDepart);
void deplacerJoueur(int carte[][NB_BLOC_H], SDL_Rect *position, int direction);
void deplacerCaisse(int *PremiereCase, int *SecondeCase);

int jouer(SDL_Surface* screen, int *Continuer1, int Aventure, char NomLevel[],int edition, int LevelDepart)
{
    SDL_Surface *mario[4] = {NULL};
    SDL_Surface *Mur = NULL, *Caisse = NULL, *CaisseOK = NULL, *Objectif = NULL, *Vide = NULL, *MarioActuel = NULL;
    SDL_Surface *TxtBravo = NULL;
    SDL_Rect Position, PositionJoueur;
    SDL_Event Event;
    int Continuer = 1, ObjectifsRestants = 1;// booléens
    int x = 0, y = 0, NumLevel = 0, NBLevel = 0;
    int carte[NB_BLOC_W][NB_BLOC_H] = {0};// WOOOW double crochets ! : comme des coords : il y a [0][0] [0][1] ... [11][11]
    int tempsACT = 0, tempsPRE = 0;
    char charNumLevel = '0';
    NumLevel = LevelDepart;

    FMOD_SYSTEM *systeme;// creation d'un objet systeme,  il ne faut pas oublier de l'allouer
        FMOD_System_Create(&systeme);// allocation de mem pour un objet systeme /!\ ATTENTION ! pointeur sur pointeur d'objet systeme
    FMOD_SOUND *theme_debut = NULL, *theme = NULL;// pour init un son
    FMOD_CHANNELGROUP *channel;// pour un groupe de canals
    FMOD_CHANNEL *canalmusique;
    FMOD_RESULT resultat;

    /*
    Creation des VARIABLES fini
    */

    resultat = FMOD_System_Init(systeme, 32, FMOD_INIT_NORMAL, NULL);
    if (resultat != FMOD_OK)
    {//if error
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }

    FMOD_System_GetChannel(systeme, 1, &canalmusique);

    NBLevel = nombredeNiveaux(Aventure);

    Mur = IMG_Load("mur.jpg");
    Caisse = IMG_Load("caisse.jpg");
    CaisseOK = IMG_Load("caisse_ok.jpg");
    Objectif = IMG_Load("objectif.png");
    Vide = IMG_Load("sol.bmp");

    mario[BAS] = IMG_Load("mario_bas.gif");
    mario[GAUCHE] = IMG_Load("mario_gauche.gif");
    mario[HAUT] = IMG_Load("mario_haut.gif");
    mario[DROITE] = IMG_Load("mario_droite.gif");

    resultat = FMOD_System_CreateSound(systeme, "mario-theme-debut.wav", FMOD_CREATESAMPLE, 0, &theme_debut);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr,"FMOD error! (%d), mario-theme-debut.mp3 non chargé\n", resultat);
        return 0;
    }

    resultat = FMOD_System_CreateSound(systeme, "mario-theme.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &theme);// pour les musiques il faut tou ça
    if (resultat != FMOD_OK)
    {
        fprintf(stderr,"FMOD error! (%d), mario-theme.mp3 non chargé\n", resultat);
        return 0;
    }
    FMOD_Sound_SetLoopCount(theme, -1);
    FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, theme_debut, 0, &canalmusique);
    tempsPRE = SDL_GetTicks();
    while(Continuer)
    {
        tempsACT = SDL_GetTicks();
        if(tempsACT - tempsPRE > 2237)
            Continuer = 0;
        else
            SDL_Delay(2237-(tempsACT-tempsPRE));
    }
    FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, theme, 0, &canalmusique);
    Continuer = 1;
    while(NumLevel < NBLevel && Continuer == 1&& *Continuer1 == 1)
    {

    MarioActuel = mario[BAS];

    // Chargement du niveau
    if (!chargerNiveau(carte,Aventure,NomLevel))
    {// fonction pour charger les cases dans carte
        fprintf(stderr,"Erreur d'initialisation du niveau : %s\n", NomLevel);
        exit(EXIT_FAILURE); // On arrête le jeu si on n'a pas pu charger le niveau
    }
    for(y = 0; y < NB_BLOC_H; y++)
    {// on cherche la psoition de Mario pour la mettre a vide
        for(x = 0; x < NB_BLOC_W; x++)
        {
            if(carte[x][y] == MARIO)// si elle contient le depart de mario
            {
                PositionJoueur.x = x;
                PositionJoueur.y = y;//on met la position du joueur au départ
                carte[x][y] = VIDE;// on met vide
            }
        }
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
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
            }
        }
    }
    SDL_Flip(screen);
    niveauDebut(screen, systeme,NomLevel);
    SDL_EnableKeyRepeat(100, 100);
    ObjectifsRestants = 1;
    /*
    INITIALISATIONS TERMINEES
    */
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
                        fprintf(stderr,"escape");
                        break;
                    case SDLK_UP:
                        MarioActuel = mario[HAUT];
                            deplacerJoueur(carte, &PositionJoueur, HAUT);
                        break;
                    case SDLK_DOWN:
                        MarioActuel = mario[BAS];
                            deplacerJoueur(carte, &PositionJoueur, BAS);
                        break;
                    case SDLK_LEFT:
                        MarioActuel = mario[GAUCHE];
                            deplacerJoueur(carte, &PositionJoueur, GAUCHE);
                             break;
                    case SDLK_RIGHT:
                        MarioActuel = mario[DROITE];
                            deplacerJoueur(carte, &PositionJoueur, DROITE);
                        break;
                }
                break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        ObjectifsRestants = 0; // on initialise les objectifs qui reste
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
                        ObjectifsRestants = 1;// on met a 1 si on trouve un objectif, si ce n'est pas le cas cest que le joueur a gagné
                        break;
                }
            }
        }
         // placement de Mario :
        Position.x = PositionJoueur.x * TAILLE_BLOC;
        Position.y = PositionJoueur.y * TAILLE_BLOC;// Calcul des coord de mario en pixel
        SDL_BlitSurface(MarioActuel, NULL, screen, &Position);

        SDL_Flip(screen);

        if(!ObjectifsRestants)// = if(objectifsrestants != 1)
            Continuer = 0;

    }

    if(ObjectifsRestants == 0)
    {
        if(NumLevel == 9)
            bravo(screen,Continuer1,1);
        else
            bravo(screen,Continuer1,0);
        NumLevel ++;
        Continuer = 1;
    }
    else
        fprintf(stderr,"QUITTER");
    if(!edition)
    {
    switch(NumLevel)
    {
        case 0:
            charNumLevel = '1';
            break;
        case 1:
            charNumLevel = '2';
            break;
        case 2:
            charNumLevel = '3';
            break;
        case 3:
            charNumLevel = '4';
            break;
        case 4:
            charNumLevel = '5';
            break;
        case 5:
            charNumLevel = '6';
            break;
        case 6:
            charNumLevel = '7';
            break;
        case 7:
            charNumLevel = '8';
            break;
        case 8:
            charNumLevel = '9';
            break;
        case 9:
            charNumLevel = 'X';
            break;
    }
    }
    if(Aventure == 1&&ObjectifsRestants == 0&&edition == 0)
        NomLevel[strlen(NomLevel)-1] = charNumLevel;
    else
        Continuer = 0;
    }

    SDL_FreeSurface(Vide);
    SDL_FreeSurface(Mur);
    SDL_FreeSurface(Caisse);
    SDL_FreeSurface(CaisseOK);
    SDL_FreeSurface(Objectif);

    for(x=0; x<4; x++)
    {
        SDL_FreeSurface(mario[x]);
    }
    fprintf(stderr,"\nfin de jouer");
    FMOD_Sound_Release(theme);
    FMOD_Sound_Release(theme_debut);
    FMOD_System_Close(systeme);// _____________________________
    FMOD_System_Release(systeme);//pour liberer l'objet system
    if(!ObjectifsRestants)
        return 1;
    if(Continuer1 == 0)
        fprintf(stderr,"quitter prog");

}// fin de la fonction jouer

void deplacerJoueur(int carte[][NB_BLOC_H], SDL_Rect *pos, int direction)// /!\ quand les double tableau sont appelés par une fonction :
{                                                                             // il faut obligatoirement mettre la 2eme valeur entre crochet
    switch(direction)
    {
        case HAUT:
        /*Gestion des colisions pour HAUT*/
            if(pos->y - 1 < 0)
                break;// si il depasse l'ecran
            if(carte[pos->x][pos->y - 1] == MUR)
                break;// si il y a un mur au dessus
            if(carte[pos->x][pos->y - 1] == CAISSE || carte[pos->x][pos->y - 1] == CAISSE_OK)
            {// si au dessus il y a une caisse ou une caisse ok ...
                if(carte[pos->x][pos->y - 2] == CAISSE || carte[pos->x][pos->y - 2] == CAISSE_OK || carte[pos->x][pos->y - 2] == MUR || pos->y - 2 < 0)
                {// si 2 cases au dessus il y a une caisse, une caisse ok, un mur, ou la bordure de la fenetre alors...
                    break;// break !
                }
            }// les tests sont passés ! on peut bouger le joueur, mais il faut vérifier si il n'y a pas de caisse à bouger...
            if(carte[pos->x][pos->y - 1] == CAISSE || carte[pos->x][pos->y - 1] == CAISSE_OK)
            {
                deplacerCaisse(&carte[pos->x][pos->y - 1], &carte[pos->x][pos->y - 2]);
            }
            pos->y--;// = deplacement vers le haut du joueur
            break;

        case BAS:
        /*Gestion des colisions pour BAS*/
            if(pos->y + 1 >= NB_BLOC_H)
                break;// si il depasse l'ecran
            if(carte[pos->x][pos->y + 1] == MUR)
                break;// si il y a un mur au dessus
            if(carte[pos->x][pos->y + 1] == CAISSE || carte[pos->x][pos->y + 1] == CAISSE_OK)
            {// si au dessus il y a une caisse ou une caisse ok ...
                if(carte[pos->x][pos->y + 2] == CAISSE || carte[pos->x][pos->y + 2] == CAISSE_OK || carte[pos->x][pos->y + 2] == MUR || pos->y + 2 >= NB_BLOC_H)
                {// si 2 cases au dessus il y a une caisse, une caisse ok, un mur, ou la bordure de la fenetre alors...
                    break;// break !
                }
            }// les tests sont passés ! on peut bouger le joueur, mais il faut vérifier si il n'y a pas de caisse à bouger...
            if(carte[pos->x][pos->y + 1] == CAISSE || carte[pos->x][pos->y + 1] == CAISSE_OK)
            {
                deplacerCaisse(&carte[pos->x][pos->y + 1], &carte[pos->x][pos->y + 2]);
            }
            pos->y++;// = deplacement vers le BAS du joueur
            break;


        case GAUCHE:
        /*Gestion des colisions pour GAUCHE*/
            if(pos->x - 1 < 0)
                break;// si il depasse l'ecran
            if(carte[pos->x - 1][pos->y] == MUR)
                break;// si il y a un mur au dessus
            if(carte[pos->x - 1][pos->y] == CAISSE || carte[pos->x - 1][pos->y] == CAISSE_OK)
            {// si au dessus il y a une caisse ou une caisse ok ...
                if(carte[pos->x - 2][pos->y] == CAISSE || carte[pos->x - 2][pos->y] == CAISSE_OK || carte[pos->x - 2][pos->y] == MUR || pos->x - 2 < 0)
                {// si 2 cases au dessus il y a une caisse, une caisse ok, un mur, ou la bordure de la fenetre alors...
                    break;// break !
                }
            }// les tests sont passés ! on peut bouger le joueur, mais il faut vérifier si il n'y a pas de caisse à bouger...
            if(carte[pos->x - 1][pos->y] == CAISSE || carte[pos->x - 1][pos->y] == CAISSE_OK)
            {
                deplacerCaisse(&carte[pos->x - 1][pos->y], &carte[pos->x - 2][pos->y]);
            }
            pos->x--;// = deplacement vers le GAUCHE du joueur
            break;

        case DROITE:
        /*Gestion des colisions pour DROITE*/
            if(pos->x + 1 >= NB_BLOC_W)
                break;// si il depasse l'ecran
            if(carte[pos->x + 1][pos->y] == MUR)
                break;// si il y a un mur au dessus
            if(carte[pos->x + 1][pos->y] == CAISSE || carte[pos->x + 1][pos->y] == CAISSE_OK)
            {// si au dessus il y a une caisse ou une caisse ok ...
                if(carte[pos->x + 2][pos->y] == CAISSE || carte[pos->x + 2][pos->y] == CAISSE_OK || carte[pos->x + 2][pos->y] == MUR || pos->x + 2 >= NB_BLOC_W)
                {// si 2 cases au dessus il y a une caisse, une caisse ok, un mur, ou la bordure de la fenetre alors...
                    break;// break !
                }
            }// les tests sont passés ! on peut bouger le joueur, mais il faut vérifier si il n'y a pas de caisse à bouger...
            if(carte[pos->x + 1][pos->y] == CAISSE || carte[pos->x + 1][pos->y] == CAISSE_OK)
            {
                deplacerCaisse(&carte[pos->x + 1][pos->y], &carte[pos->x + 2][pos->y]);
            }
            pos->x++;// = deplacement vers le DROITE du joueur
            break;


    }

}

void deplacerCaisse(int *PremiereCase, int *SecondeCase)
{
    if(*PremiereCase == CAISSE_OK)
        *PremiereCase = OBJECTIF;
    else
        *PremiereCase = VIDE;

    if(*SecondeCase == OBJECTIF)
        *SecondeCase = CAISSE_OK;
    else
        *SecondeCase = CAISSE;
}


