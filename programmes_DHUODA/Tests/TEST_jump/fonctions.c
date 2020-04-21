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

//fonctions classique
void checkerror(SDL_Surface *s, FILE *fichier, int type);
Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y);
int add_timer(C_timer *timer);
int freeOBJF(OBJF *smain);
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




