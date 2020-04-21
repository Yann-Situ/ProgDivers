/*
fonctions.c
___________
    contient diverses fonctions tel les timers ou la gestion des erreurs
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "fonctions.h"
#include "map.h"
#include "evenement.h"

//fonctions classique
void checkerror(SDL_Surface *s, FILE *fichier, int type);
Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y);
int add_timer(C_timer *timer);
int freeOBJF(OBJF *smain);

//fonctions physique
unsigned int colision(SDL_Rect rect1, SDL_Rect rect2);
unsigned int coincage(SDL_Rect *rect1, SDL_Rect rect2, unsigned int xy);

//fonctions animation
int animation(Sanim *anim);
Sanim chargerAnim(Sanim animSelect, int Nbl,int Nbt,int Nul,int Nut,int h,int w,int x,int y,int interv,char *tilesetName);

//fonctions menu
int menu(OBJF *smain, Sanim *animSelect, Sstring *NomChoix, SDL_Surface *fond, char Title[], int NBchoix, int BLnoir, int *Quitter);
int menuNB(OBJF *smain, Sanim *animSelect, SDL_Surface *fond, char Title[], int NBmin, int NBmax, int BLnoir, int *Quitter);

//____________________________________________

//fonctions classique
void checkerror(SDL_Surface *s, FILE *fichier, int type)
{
    if(s == NULL&& type == SDL)
    {
        fprintf(stderr,"\tSDL error:%s!\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }
    else if(s == NULL&& type == IMG)
    {
        fprintf(stderr,"\tIMG error:%s!\n",IMG_GetError());
        exit(EXIT_FAILURE);
    }
    else if(fichier == NULL&& type == FICHIERS)
    {
        fprintf(stderr,"\tFILE error, le fichier n'existe pas ou n'a pas été ouvert!\n");
        exit(EXIT_FAILURE);
    }
}

int add_timer(C_timer *timer)
{
    int tmps = SDL_GetTicks();
    if(tmps < timer->tempsPre + timer->intervalle)
        return 0;
    else
    {
        timer->tempsPre = tmps;
        return 1;
    }
}

Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

int freeOBJF(OBJF *smain)
{
    fprintf(stderr,"*debut freeobjf*\n");
    TTF_CloseFont(smain->police);
    TTF_CloseFont(smain->policeTitle);
    FMOD_System_Close(smain->systeme);// _____________________________
    FMOD_System_Release(smain->systeme);//pour liberer l'objet system
    free(smain);
    fprintf(stderr,"*fin freeobjf*\n");
    return 1;
}

unsigned int colision(SDL_Rect rect1, SDL_Rect rect2)
{
    if(rect1.h == 0 && rect1.w == 0)
    {// si point
        if(rect1.x >= rect2.x && rect1.x <= rect2.w+rect2.x && rect1.y >= rect2.y && rect1.y <= rect2.h+rect2.y)// si appartient
            return 1;
        else
            return 0;
    }
    else
    {// sinon rectangle
        if(rect1.w > rect2.w)
        {
            if(rect1.h > rect2.h)
                return 5;
            else
                return 3;
        }
        else if(rect1.h > rect2.h)
            return 4;

        if(rect1.x >= rect2.x && rect1.x <= rect2.w+rect2.x && rect1.y >= rect2.y && rect1.y <= rect2.h+rect2.y)
        {// si le point haut gauche appartient
            if(rect1.w+rect1.x <= rect2.w+rect2.x && rect1.h+rect1.y <= rect2.h+rect2.y)
                return 1;
            else
                return 2;
        }
        else
            return 0;
    }
}

unsigned int coincage(SDL_Rect *rect1, SDL_Rect rect2, unsigned int xy)
{
    int BLchange = 0;
    /*if(rect1->x < 0)
        rect1->x = 0;
    if(rect1->y < 0)
        rect1->y = 0;*/
    if(rect1->h == 0 && rect1->w == 0)
    {
        if(rect1->x >= rect2.x && rect1->x <= rect2.w+rect2.x && rect1->y >= rect2.y && rect1->y <= rect2.h+rect2.y)
            return 1;
        else
        {
            if(xy != 2)
            {
                if(rect1->x < rect2.x)
                    rect1->x = rect2.x;
                else if(rect1->x > rect2.x+rect2.w)
                    rect1->x = rect2.x+rect2.w;
            }
            if(xy != 1)
            {
                if(rect1->y < rect2.y)
                    rect1->y = rect2.y;
                else if(rect1->y > rect2.y+rect2.h)
                    rect1->y = rect2.y+rect2.h;
            }
            return 2;
        }
    }
    else
    {
        if(rect1->w > rect2.w)
        {
            if(xy == 2)
                return 0;
            else
                xy = 1;
        }
        if(rect1->h > rect2.h)
        {
            if(xy == 1)
                return 0;
            else
                xy = 2;
        }

        if(rect1->x >= rect2.x && rect1->x <= rect2.w+rect2.x &&
           rect1->y >= rect2.y && rect1->y <= rect2.h+rect2.y)
        {// si le point haut gauche appartient
        }
        else
        {// sinon...
            if(xy != 2)
            {
                if(rect1->x < rect2.x)
                    rect1->x = rect2.x;
            }
            if(xy != 1)
            {
                if(rect1->y < rect2.y)
                    rect1->y = rect2.y;
            }
            BLchange = 1;
        }

        if(rect1->w+rect1->x <= rect2.w+rect2.x && rect1->w+rect1->x >= rect2.x &&
           rect1->h+rect1->y <= rect2.h+rect2.y && rect1->h+rect1->y >= rect2.y)// si le point bas droit appartient
        {
        }
        else
        {// sinon...
            if(xy != 2)
            {
                if(rect1->w+rect1->x > rect2.x+rect2.w)
                    rect1->x = rect2.x+rect2.w - rect1->w;
            }
            if(xy != 1)
            {
                if(rect1->y+rect1->h > rect2.y+rect2.h)
                    rect1->y = rect2.y+rect2.h - rect1->h;
            }
            BLchange = 1;
        }
    }
    if(BLchange)
        return 2;
    return 1;
}


//fonctions animation
int animation(Sanim *anim)
{
    if(anim->NumloopACT >= anim->NBloop && anim->NBloop != -1)
    {

    }
    else if(add_timer(&anim->timer))
    {
        anim->NumTileACT++;
        if(anim->NumTileACT > anim->NBTile-1)
        {
            anim->NumTileACT = 0;
            if(anim->NBloop != -1)
                anim->NumloopACT ++;

        }
    }
    anim->rectTile.x = anim->rectTile.w * anim->NumTileACT;
    return anim->NumTileACT;
}

Sanim chargerAnim(Sanim animSelect, int Nbl,int Nbt,int Nul,int Nut,int h,int w,int x,int y,int interv,char *tilesetName)
{
    animSelect.NBloop = Nbl; animSelect.NBTile = Nbt; animSelect.NumloopACT = Nul;  animSelect.NumTileACT = Nut;
    animSelect.rectTile.h = h;  animSelect.rectTile.w = w;  animSelect.rectTile.x = x;  animSelect.rectTile.y = y;
    animSelect.timer.intervalle = interv; animSelect.timer.tempsPre = 0;
    animSelect.sprites = IMG_Load(tilesetName);
    checkerror(animSelect.sprites,NULL,IMG);
    return animSelect;
}


//fonctions menu
int menu(OBJF *smain, Sanim *animSelect, Sstring *NomChoix, SDL_Surface *fond, char Title[], int NBchoix, int BLnoir, int *Quitter)
{
    fprintf(stderr,"*debut menu*\n");
    int Continuer = 1, DirectionEnfonce = 0, i=0, choixACT = 0, scroll = 0, colr = 0;
    if(BLnoir)
        colr = 0;
    else
        colr = 255;
    SDL_Color couleur = {colr,colr,colr};
    SDL_Event event;
    C_timer timerSCROLL = {0,10};
    SDL_Surface *titre = NULL, **Texte = NULL;
    titre = TTF_RenderText_Blended(smain->policeTitle,Title,couleur);

    if(NBchoix > 0 && NomChoix != NULL)
    {
        Texte = malloc(NBchoix * sizeof(SDL_Surface*));
        for(i=0;i<NBchoix;i++)
        {
            Texte[i] = TTF_RenderText_Blended(smain->police,NomChoix[i].Nom, couleur);
            checkerror(Texte[i],NULL,SDL);
        }
    }

    SDL_Rect PositionTextemid, PositionSelect, rectSCREEN;

    rectSCREEN.x =0; rectSCREEN.y =0; rectSCREEN.h = smain->screen->h; rectSCREEN.w = smain->screen->w;

    if(NBchoix > 0 && NomChoix != NULL)
    {// position textes
        PositionTextemid.x = SPACE_BEFORE_TEXT+POLICE_TAILLE;
        PositionTextemid.y = smain->screen->h/2- Texte[0]->h/2;
    }
    if(animSelect != NULL)
    {// position select
        PositionSelect.y = smain->screen->h/2 - animSelect->sprites->h/2;
        PositionSelect.x = smain->screen->w/2 - animSelect->sprites->w / animSelect->NBTile /2;
    }
    /*

    */
    while(SDL_PollEvent(&smain->event_poubelle));
    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                smain->continuer = 0;
                Continuer = 0;
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        if(Quitter != NULL)
                            *Quitter = 1;
                        choixACT = -1;
                        Continuer = 0;
                    break;
                    case SDLK_DOWN:
                        if(choixACT<NBchoix-1)
                            DirectionEnfonce = 1;
                    break;
                    case SDLK_UP:
                        if(choixACT>0)
                            DirectionEnfonce = -1;
                    break;
                    case SDLK_RETURN:
                        Continuer = 0;
                    break;
                    default:
                    break;
                }
            break;
        }


        SDL_Delay(1);
        if(DirectionEnfonce != 0)
        {// gestion du scroll des textes

            if(add_timer(&timerSCROLL))
            {
                scroll += DirectionEnfonce*(POLICE_TAILLE/5);
                if(scroll > POLICE_TAILLE || scroll < 0- POLICE_TAILLE)
                {
                    choixACT += DirectionEnfonce;
                    DirectionEnfonce = 0;
                    scroll = 0;
                }
            }
        }
        if(animSelect != NULL)
        {
            animation(animSelect);
        }

        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 0,0,0));
        SDL_BlitSurface(fond,&rectSCREEN,smain->screen,NULL);

        if(NBchoix > 0 && NomChoix != NULL)
        {// blit des textes
            for(i=0; i<NBchoix; i++)
            {
                if(i==choixACT)
                {
                    PositionTextemid.y = ( smain->screen->h/2 - Texte[0]->h/2 ) - scroll;
                    PositionTextemid.x = PositionSelect.x + SPACE_BEFORE_TEXT;
                    SDL_BlitSurface(Texte[i],NULL,smain->screen,&PositionTextemid);
                }
                else
                {
                    PositionTextemid.y = ( smain->screen->h/2 - Texte[0]->h/2 ) - POLICE_TAILLE *(choixACT-i) - scroll;
                    PositionTextemid.x = PositionSelect.x + SPACE_BEFORE_TEXT + POLICE_TAILLE;
                    SDL_BlitSurface(Texte[i],NULL,smain->screen,&PositionTextemid);
                }
            }
        }
        if(animSelect != NULL)
        {// blit animation
            SDL_BlitSurface(animSelect->sprites,&animSelect->rectTile,smain->screen,&PositionSelect);
        }

        SDL_BlitSurface(titre,NULL,smain->screen,NULL);
        SDL_Flip(smain->screen);
    }

    if(NBchoix > 0 && NomChoix != NULL)
    {//free strings
        for(i=0; i<NBchoix; i++)
            SDL_FreeSurface(Texte[i]);
    }

    SDL_FreeSurface(titre);
    fprintf(stderr,"*fin menu*\n");

    if(smain->continuer == 0)
    {
        fprintf(stderr,"quit menu!\n");
        return QUITTER;
    }
    return choixACT;
}

int menuNB(OBJF *smain, Sanim *animSelect, SDL_Surface *fond, char Title[], int NBmin, int NBmax, int BLnoir, int *Quitter)
{
    fprintf(stderr,"*debut menuNB*\n");
    int Continuer = 1, choixACT = NBmin, choixPRE = choixACT, colr = 0;
    char charNum[10] = "0";
    if(BLnoir)
        colr = 0;
    else
        colr = 255;
    SDL_Color couleur = {colr,colr,colr};
    SDL_Event event;
    SDL_Surface *Texte = NULL, *titre = NULL;
    C_timer KeyRepeat1 = {0,90}, KeyRepeat10 = {0,120};

    sprintf(charNum,"%d",choixACT);
    titre = TTF_RenderText_Blended(smain->policeTitle,Title,couleur);
    Texte = TTF_RenderText_Solid(smain->police,charNum,couleur);
    SDL_Rect PositionTextemid, PositionSelect, rectSCREEN;

    rectSCREEN.x =0; rectSCREEN.y =0; rectSCREEN.h = smain->screen->h; rectSCREEN.w = smain->screen->w;

    PositionSelect.y = smain->screen->h/2 - animSelect->sprites->h/2;
    PositionSelect.x = smain->screen->w/2 - animSelect->sprites->w / animSelect->NBTile /2;

    PositionTextemid.x = PositionSelect.x + SPACE_BEFORE_TEXT;
    PositionTextemid.y = smain->screen->h/2- Texte->h/2;
    /*

    */
    while(SDL_PollEvent(&smain->event_poubelle));
    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                smain->continuer = 0;
                Continuer = 0;
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        *Quitter = 1;
                        Continuer = 0;
                        choixACT = -1;
                    break;
                    case SDLK_UP:
                        if(choixACT<NBmax && add_timer(&KeyRepeat1))
                        {
                            choixACT++;
                        }

                    break;
                    case SDLK_DOWN:
                        if(choixACT>NBmin && add_timer(&KeyRepeat1))
                        {
                            choixACT--;
                        }
                    break;
                    case SDLK_RIGHT:
                        if(choixACT<=NBmax-10 && add_timer(&KeyRepeat10))
                        {
                            choixACT+= 10;
                        }
                        else if(choixACT>NBmax-10 && add_timer(&KeyRepeat10))
                            choixACT = NBmax;
                    break;
                    case SDLK_LEFT:
                        if(choixACT>10+NBmin && add_timer(&KeyRepeat10))
                        {
                            choixACT-= 10;
                        }
                        else if(choixACT<=NBmin+10 && add_timer(&KeyRepeat10))
                            choixACT = NBmin;
                    break;
                    case SDLK_RETURN:
                        Continuer = 0;
                    break;
                    default:
                    break;
                }
            break;
        }
        SDL_Delay(1);

        if(choixPRE != choixACT)
        {
            sprintf(charNum,"%d",choixACT);
            Texte = TTF_RenderText_Solid(smain->police,charNum,couleur);
            choixPRE = choixACT;
        }

        animation(animSelect);

        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 0,0,0));
        SDL_BlitSurface(fond,&rectSCREEN,smain->screen,NULL);

        SDL_BlitSurface(Texte,NULL,smain->screen,&PositionTextemid);

        SDL_BlitSurface(titre,NULL,smain->screen,NULL);

        SDL_BlitSurface(animSelect->sprites,&animSelect->rectTile,smain->screen,&PositionSelect);
        SDL_Flip(smain->screen);
    }
    SDL_FreeSurface(Texte);
    SDL_FreeSurface(titre);
    fprintf(stderr,"*fin de menuNB*\n");
    if(smain->continuer == 0)
    {
        fprintf(stderr,"quit menuNB !\n");
        return QUITTER;
    }
    return choixACT;
}


