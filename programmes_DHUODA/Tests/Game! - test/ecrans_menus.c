/*
GAME    Yann-Situ Gazull
____
ecrans_menus.c
Contient les fonctions des écrans de menu principaux et secondaires
*/
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "../fonction_0/fonction_0.h"
#include "constante.h"
#include "structure.h"
#include "ecrans_menus.h"

int ecran_titre(OBJF* smain);
int ecran_fichier(OBJF* smain);

int ecran_titre(OBJF* smain)
{
    //CREAT VAR
    SDL_Surface *Background = NULL, *Texte = NULL;
    Sanim *Titre = malloc(sizeof(Sanim));
    SDL_Event event;
    SDL_Rect PositionTitre, PositionTexte;
    SDL_Color couleurBlanche = {255,255,255};
    C_timer frame = {SDL_GetTicks(),FRAME};
    int Continuer = 1, ret = 0, blink = 0;
    //INIT VAR
    Background = IMG_Load(ECRAN_TITRE_BACKGROUND_NAME);
    checkerror(Background,NULL,IMG);
    Texte = TTF_RenderText_Blended(smain->police,"Appuyer sur une touche",couleurBlanche);
    checkerror(Texte,NULL,SDL);
    Titre = chargerAnim(Titre, -1, ECRAN_TITRE_ANIM_NBT, 0,0, ECRAN_TITRE_ANIM_H, ECRAN_TITRE_ANIM_W, ECRAN_TITRE_ANIM_X, ECRAN_TITRE_ANIM_Y, ECRAN_TITRE_ANIM_INTERVALE, ECRAN_TITRE_ANIM_NAME);
    PositionTitre.x = smain->screen->w/2 - Titre->rectTile.w/2;
    PositionTitre.y = ECRAN_TITRE_Y;
    PositionTexte.x = smain->screen->w/2 - Titre->rectTile.w/2;
    PositionTexte.y = ECRAN_TITRE_TEXTE_Y;
    //EVOLUTION
    while(SDL_PollEvent(&smain->event_poubelle));
    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                smain->continuer = 0;
                ret = 0;
                break;
            case(SDL_KEYDOWN):
                Continuer = 0;
                ret = 1;
                break;
            default:
                break;
        }
        blink = clignot(Texte,blink,16);
        animation(Titre);
        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 0,0,0));
        SDL_BlitSurface(Background, NULL, smain->screen, NULL);
        SDL_BlitSurface(Titre->sprites, &Titre->rectTile, smain->screen, &PositionTitre);
        SDL_BlitSurface(Texte, NULL, smain->screen, &PositionTexte);
        SDL_Flip(smain->screen);
        if(!add_timer(&frame))
            SDL_Delay(frame.tempsPre+frame.intervalle-SDL_GetTicks());
    }
    //FREE VAR
    SDL_FreeSurface(Background);
    SDL_FreeSurface(Texte);
    free(Titre);
    return ret;
}

int ecran_fichier(OBJF* smain)
{
    //CREAT VAR
    SDL_Surface *Background = NULL;
    Sanim *Titre = malloc(sizeof(Sanim));
    SDL_Event event;
    SDL_Rect PositionTitre;
    SDL_Color couleurBlanche = {255,255,255};
    C_timer frame = {SDL_GetTicks(),FRAME};
    int Continuer = 1, ret = 0;
}
