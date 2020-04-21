#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "affichage.h"


void bravo(SDL_Surface* screen, int *Cont1, int fin);
SDL_Rect curseur(SDL_Rect *posi, int Taille, int direction);
void MenuJouer(SDL_Surface* screen, SDL_Surface* Selection, int *Continuer1);
void MenuEditeur(SDL_Surface* screen, SDL_Surface* Selection, int *Continuer1);
void MenuCharger(SDL_Surface* screen, SDL_Surface* Selection, int Aventure, int *Continuer1, int Edition);
void MenuSauvegarder(SDL_Surface* screen, int Aventure, int *Continuer1, int carte[][NB_BLOC_H],FMOD_SYSTEM *systeme);
void niveauDebut(SDL_Surface* screen, FMOD_SYSTEM *systeme,char NomLevel[]);


void bravo(SDL_Surface* screen, int *Cont1, int fin)
{
    SDL_Surface *TxtBravo = NULL;
    SDL_Rect Position;
    SDL_Color Noir = {0,0,0}, Blanc = {255,255,255};
    TTF_Font *Calibris = NULL;
    Calibris = TTF_OpenFont("calibri.ttf", 30);

    if(Calibris == NULL)
        fprintf(stderr,"open calibri.ttf failed %s", SDL_GetError());
    if(!fin)
        TxtBravo = TTF_RenderText_Shaded(Calibris,"Bravo, vous avez finis le niveau!", Blanc, Noir);
    else
        TxtBravo = TTF_RenderText_Shaded(Calibris,"Bravo, vous avez finis l'aventure!", Blanc, Noir);
    Position.x = screen->w/2 - TxtBravo->w/2;
    Position.y = screen->h/2 - TxtBravo->h/2;
    SDL_Delay(200);
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
        SDL_BlitSurface(TxtBravo,NULL,screen,&Position);
        SDL_Flip(screen);
        SDL_Delay(1300);
    SDL_FreeSurface(TxtBravo);
    TTF_CloseFont(Calibris);
}

SDL_Rect curseur(SDL_Rect *posi, int Taille, int direction)
{
    if(direction == HAUT)
    {
        posi->y = posi->y - Taille;
    }
    else if(direction == BAS)
    {
        posi->y = posi->y + Taille;
    }
    else if(direction == GAUCHE)
    {
        posi->x = posi->x - Taille;
    }
    else if(direction == DROITE)
    {
        posi->x = posi->x + Taille;
    }
    return *posi;
}

void MenuJouer(SDL_Surface* screen, SDL_Surface* Selection, int *Continuer1)
{
    int choixAct = 0, NBchoix = 3, TaillePolice = 30;
    /*boléens*/int Continuer = 1;
    SDL_Surface* Texte[3] = {NULL};
    SDL_Event event;
    SDL_Rect PosiCurseur, PosiTexte;
    TTF_Font *police = NULL;
    SDL_Color couleurBlanche = {255,255,255};

    police = TTF_OpenFont("calibri.ttf", TaillePolice);
    if(police == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Texte[0] = TTF_RenderText_Blended(police,"Aventure", couleurBlanche);
    Texte[1] = TTF_RenderText_Blended(police,"Niveau de l'editeur", couleurBlanche);
    Texte[2] = TTF_RenderText_Blended(police,"Retour", couleurBlanche);
    PosiTexte.x = screen->w/2 - Texte[1]->w/2;
    PosiTexte.y = screen->h/2 - Texte[1]->h/2 - 30;
    PosiCurseur.x = PosiTexte.x - 4;
    PosiCurseur.y = PosiTexte.y - 5;

    while(Continuer == 1 && *Continuer1 == 1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                *Continuer1 = 0;
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        Continuer = 0;
                    break;
                    case SDLK_DOWN:
                        if(choixAct<NBchoix-1)
                        {
                            PosiCurseur = curseur(&PosiCurseur,TaillePolice,BAS);
                            choixAct += 1;
                        }
                    break;
                    case SDLK_UP:
                        if(choixAct>0)
                        {
                            PosiCurseur = curseur(&PosiCurseur,TaillePolice,HAUT);
                            choixAct -= 1;
                        }
                    break;
                    case SDLK_RETURN:
                        switch(choixAct)
                        {
                            case 0:
                                MenuCharger(screen,Selection,1,Continuer1,0);
                            break;
                            case 1:
                                MenuCharger(screen,Selection,0,Continuer1,0);
                            break;
                            case 2:
                                Continuer = 0;
                            break;
                        }
                    break;
                }
            break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
        PosiTexte.y = screen->h/2 - Texte[1]->h/2 - 30;
        SDL_BlitSurface(Texte[0],NULL,screen,&PosiTexte);
        PosiTexte.y = screen->h/2 - Texte[1]->h/2;
        SDL_BlitSurface(Texte[1],NULL,screen,&PosiTexte);
        PosiTexte.y = screen->h/2 - Texte[1]->h/2 + 30;
        SDL_BlitSurface(Texte[2],NULL,screen,&PosiTexte);

        SDL_BlitSurface(Selection,NULL,screen,&PosiCurseur);
        SDL_Flip(screen);
    }
    SDL_FreeSurface(Texte[0]);
    SDL_FreeSurface(Texte[1]);
    SDL_FreeSurface(Texte[2]);
}

void MenuEditeur(SDL_Surface* screen, SDL_Surface* Selection, int *Continuer1)
{
    int choixAct = 0, NBchoix = 3, TaillePolice = 30;
    /*boléens*/int Continuer = 1;
    SDL_Surface* Texte[3] = {NULL};
    SDL_Event event;
    SDL_Rect PosiCurseur, PosiTexte;
    TTF_Font *police = NULL;
    SDL_Color couleurBlanche = {255,255,255};

    police = TTF_OpenFont("calibri.ttf", TaillePolice);
    if(police == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Texte[0] = TTF_RenderText_Blended(police,"Modifier niveaux de l'aventure", couleurBlanche);
    Texte[1] = TTF_RenderText_Blended(police,"Modifier niveaux de l'editeur", couleurBlanche);
    Texte[2] = TTF_RenderText_Blended(police,"Retour", couleurBlanche);
    PosiTexte.x = screen->w/2 - Texte[1]->w/2;
    PosiTexte.y = screen->h/2 - Texte[1]->h/2 - 30;
    PosiCurseur.x = PosiTexte.x - 4;
    PosiCurseur.y = PosiTexte.y - 5;

    while(Continuer == 1 && *Continuer1 == 1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                *Continuer1 = 0;
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        Continuer = 0;
                    break;
                    case SDLK_DOWN:
                        if(choixAct<NBchoix-1)
                        {
                            PosiCurseur = curseur(&PosiCurseur,TaillePolice,BAS);
                            choixAct += 1;
                        }
                    break;
                    case SDLK_UP:
                        if(choixAct>0)
                        {
                            PosiCurseur = curseur(&PosiCurseur,TaillePolice,HAUT);
                            choixAct -= 1;
                        }
                    break;
                    case SDLK_RETURN:
                        switch(choixAct)
                        {
                            case 0:
                                MenuCharger(screen,Selection,1,Continuer1,1);
                            break;
                            case 1:
                                MenuCharger(screen,Selection,0,Continuer1,1);
                            break;
                            case 2:
                                Continuer = 0;
                            break;
                        }
                    break;
                }
            break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
        PosiTexte.y = screen->h/2 - Texte[1]->h/2 - 30;
        SDL_BlitSurface(Texte[0],NULL,screen,&PosiTexte);
        PosiTexte.y = screen->h/2 - Texte[1]->h/2;
        SDL_BlitSurface(Texte[1],NULL,screen,&PosiTexte);
        PosiTexte.y = screen->h/2 - Texte[1]->h/2 + 30;
        SDL_BlitSurface(Texte[2],NULL,screen,&PosiTexte);

        SDL_BlitSurface(Selection,NULL,screen,&PosiCurseur);
        SDL_Flip(screen);
    }
    SDL_FreeSurface(Texte[0]);
    SDL_FreeSurface(Texte[1]);
    SDL_FreeSurface(Texte[2]);
}

void MenuSauvegarder(SDL_Surface* screen, int Aventure, int *Continuer1, int carte[][NB_BLOC_H],FMOD_SYSTEM *systeme)
{
    int choixAct = 0, i = 0, NBchoixNiveau = nombredeNiveaux(Aventure), TaillePolice = 30, OK = 0;
    /*boléens*/int Continuer = 1;
    SDL_Surface *Selection = NULL;
    InfoLevel *Niveaux = NULL;
    FMOD_SOUND *son = NULL;
    FMOD_CHANNEL *canal = NULL;
    if (FMOD_System_CreateSound(systeme, "Super Mario pipe.wav", FMOD_CREATESAMPLE, 0, &son) != FMOD_OK)
        fprintf(stderr,"FMOD error! super mario pipe.wav non chargé\n");
    FMOD_System_GetChannel(systeme, 9, &canal);

    fprintf(stderr,"\nAffichage Lvel\nChoix : %d\n",NBchoixNiveau);

    Selection = IMG_Load("select.bmp");
    SDL_SetColorKey(Selection,SDL_SRCCOLORKEY, SDL_MapRGB(Selection->format, 255, 0, 0));
    Niveaux = malloc(NBchoixNiveau * sizeof(InfoLevel));
    OK = mettreNomdsTableau(Niveaux,Aventure,NBchoixNiveau);
    if(OK == 0)
        exit(EXIT_FAILURE);
    else if(OK == -1)
        fprintf(stderr,"Tous les fichiers n'ont pas été trouvé : il devait y en avoir %d",NBchoixNiveau);

    fprintf(stderr,"OK : %d\n",OK);
    for(i=0; i<NBchoixNiveau; i++)
    {
        fprintf(stderr,"chaine Niveau[%d] : %s\n",i,Niveaux[i].NomNiv);
    }
    SDL_Surface* *Texte = NULL;
    Texte = malloc(NBchoixNiveau * sizeof(SDL_Surface*));

    SDL_Event event;
    SDL_Rect PosiCurseur, PosiTexte;
    TTF_Font *police = NULL;
    SDL_Color couleurBlanche = {255,255,255};

    police = TTF_OpenFont("calibri.ttf", TaillePolice);
    if(police == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i=0; i<NBchoixNiveau; i++)
    {
        Texte[i] = TTF_RenderText_Blended(police,Niveaux[i].NomNiv, couleurBlanche);
    }

    PosiTexte.x = screen->w/2 - Texte[0]->w/2;
    PosiTexte.y = screen->h/2 - Texte[0]->h/2 - 30;
    PosiCurseur.x = PosiTexte.x - 4;
    PosiCurseur.y = PosiTexte.y - 5;

    int Scroll = 1;

    while(Continuer == 1 && *Continuer1 == 1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                *Continuer1 = 0;
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        Continuer = 0;
                    break;
                    case SDLK_DOWN:
                        if(choixAct<NBchoixNiveau-1)
                        {
                            Scroll ++;
                            choixAct += 1;
                        }
                    break;
                    case SDLK_UP:
                        if(choixAct>0)
                        {
                            Scroll --;
                            choixAct -= 1;
                        }
                    break;
                    case SDLK_RETURN:
                        sauvegarderNiveau(carte,Aventure,Niveaux[choixAct].NomNiv);
                        FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, son, 0, &canal);
                        Continuer = 0;
                        SDL_Delay(1000);
                    break;
                }
            break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
        for(i=0; i<NBchoixNiveau; i++)
        {
            PosiTexte.y = screen->h/2 - Texte[i]->h/2 - ((Scroll-i)*30);
            SDL_BlitSurface(Texte[i],NULL,screen,&PosiTexte);
        }
        SDL_BlitSurface(Selection,NULL,screen,&PosiCurseur);
        SDL_Flip(screen);
    }
    for(i=0; i<NBchoixNiveau; i++)
    {
        SDL_FreeSurface(Texte[i]);
    }
    SDL_FreeSurface(Selection);
    free(Niveaux);
    free(Texte);
    if(FMOD_Sound_Release(son)!=FMOD_OK)
        fprintf(stderr,"\nFMOD error! son release failed ()");
    else
        fprintf(stderr,"\nson release");
}

void MenuCharger(SDL_Surface* screen, SDL_Surface* Selection, int Aventure, int *Continuer1, int Edition)
{
    int choixAct = 0, i = 0, NBchoixNiveau = nombredeNiveaux(Aventure), TaillePolice = 30, OK = 0;
    /*boléens*/int Continuer = 1;

    fprintf(stderr,"\nChoix : %d\n",NBchoixNiveau);

    InfoLevel *Niveaux = NULL;

    Niveaux = malloc(NBchoixNiveau * sizeof(InfoLevel));
    OK = mettreNomdsTableau(Niveaux,Aventure,NBchoixNiveau);
    if(OK == 0)
        exit(EXIT_FAILURE);
    else if(OK == -1)
        fprintf(stderr,"Tous les fichiers n'ont pas été trouvé : il devait y en avoir %d",NBchoixNiveau);

    fprintf(stderr,"OK : %d\n",OK);
    for(i=0; i<NBchoixNiveau; i++)
    {
        fprintf(stderr,"chaine Niveau[%d] : %s\n",i,Niveaux[i].NomNiv);
    }
    SDL_Surface* *Texte = NULL;
    Texte = malloc(NBchoixNiveau * sizeof(SDL_Surface*));

    SDL_Event event;
    SDL_Rect PosiCurseur, PosiTexte;
    TTF_Font *police = NULL;
    SDL_Color couleurBlanche = {255,255,255};

    police = TTF_OpenFont("calibri.ttf", TaillePolice);
    if(police == NULL)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i=0; i<NBchoixNiveau; i++)
    {
        Texte[i] = TTF_RenderText_Blended(police,Niveaux[i].NomNiv, couleurBlanche);
    }

    PosiTexte.x = screen->w/2 - Texte[0]->w/2;
    PosiTexte.y = screen->h/2 - Texte[0]->h/2 - 30;
    PosiCurseur.x = PosiTexte.x - 4;
    PosiCurseur.y = PosiTexte.y - 5;

    int Scroll = 1;

    while(Continuer == 1 && *Continuer1 == 1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                *Continuer1 = 0;
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        Continuer = 0;
                    break;
                    case SDLK_DOWN:
                        if(choixAct<NBchoixNiveau-1)
                        {
                            Scroll ++;
                            choixAct += 1;
                        }
                    break;
                    case SDLK_UP:
                        if(choixAct>0)
                        {
                            Scroll --;
                            choixAct -= 1;
                        }
                    break;
                    case SDLK_RETURN:
                        if(!Edition)
                            jouer(screen, Continuer1,Aventure,Niveaux[choixAct].NomNiv,0,choixAct);
                            if(Continuer1 == 0)
                                fprintf(stderr,"quitter prog");
                        else if(Edition)
                            editer(screen,Continuer1,Aventure,Niveaux[choixAct].NomNiv);
                    break;
                }
            break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
        for(i=0; i<NBchoixNiveau; i++)
        {
            PosiTexte.y = screen->h/2 - Texte[i]->h/2 - ((Scroll-i)*30);
            SDL_BlitSurface(Texte[i],NULL,screen,&PosiTexte);
        }
        SDL_BlitSurface(Selection,NULL,screen,&PosiCurseur);
        SDL_Flip(screen);
    }
    for(i=0; i<NBchoixNiveau; i++)
    {
        SDL_FreeSurface(Texte[i]);
    }
    free(Niveaux);
    free(Texte);
}

void niveauDebut(SDL_Surface* screen, FMOD_SYSTEM *systeme,char NomLevel[])
{
    FMOD_SOUND *lvlUP = NULL;
    FMOD_CHANNEL *canal;
    FMOD_System_GetChannel(systeme, 2, &canal);
    if (FMOD_System_CreateSound(systeme, "mario-red-mushroom.wav", FMOD_CREATESAMPLE, 0, &lvlUP) != FMOD_OK)
        fprintf(stderr,"FMOD error!,mario-red-mushroom.wav non chargé\n");

    int TempsPre = 0, TempsAct = 0, c = 1;
    SDL_Surface *Txt = NULL;
    SDL_Rect Position;
    SDL_Color Noir = {0,0,0}, Blanc = {255,255,255};
    TTF_Font *Calibris = NULL;
    Calibris = TTF_OpenFont("calibri.ttf", 40);
    if(Calibris == NULL)
        fprintf(stderr,"open calibri.ttf failed %s", SDL_GetError());
    Txt = TTF_RenderText_Shaded(Calibris,NomLevel, Blanc, Noir);
    Position.x = screen->w/2 - Txt->w/2;
    Position.y = screen->h/2 - Txt->h/2;
    FMOD_System_PlaySound(systeme, FMOD_CHANNEL_REUSE, lvlUP, 0, &canal);
    SDL_BlitSurface(Txt, NULL, screen, &Position);
    SDL_Flip(screen);
    TempsPre = SDL_GetTicks();
    while(c)
    {
        TempsAct = SDL_GetTicks();
        if(TempsAct - TempsPre > 2000)
            c = 0;
        else
            SDL_Delay(2000-(TempsAct-TempsPre));
    }
    FMOD_Sound_Release(lvlUP);
    SDL_FreeSurface(Txt);
}
