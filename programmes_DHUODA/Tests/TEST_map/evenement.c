/*
evenement.c
___________
    contient les fonctions des évenements de l'éditeur de map
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "evenement.h"
#include "fonctions.h"

int mouseClic(Smap *map, SDL_Event event, SDL_Rect scroll, int clic, int calque, int BLtoolbar, int BLctrl);
int mouseClicMollette(Smap *map, SDL_Rect SourisDep, SDL_Rect Souris, SDL_Rect *scroll);
void selectMiseAJour(Smap *map, int direction);

int mouseClic(Smap *map, SDL_Event event, SDL_Rect scroll, int clic, int calque, int BLtoolbar, int BLctrl)
{
    int x = 0, y = 0, casetile = 0, casemap = 0, different = 1;

    if(!BLtoolbar)
    {
        x = ((event.button.x - INTERFACE_X + scroll.x)/TILE_TAILLE)% map->w_blocs;
        y = ((event.button.y - INTERFACE_Y + scroll.y)/TILE_TAILLE);

        if(event.button.x - INTERFACE_X + scroll.x +1 > map->w_blocs*TILE_TAILLE)
            return 0;
        if(event.button.y - INTERFACE_Y + scroll.y +1 > map->h_blocs*TILE_TAILLE)
            return 0;

        if(clic == CLIC_GAUCHE)
        {
            if(BLctrl)
            {
                switch(calque)
                {
                    case BACK:
                        if(map->Cases[y][x].back != 0)
                            map->ToolBar->ObjetSelect = map->Cases[y][x].back;
                    break;
                    case MID:
                        if(map->Cases[y][x].mid != 0)
                            map->ToolBar->ObjetSelect = map->Cases[y][x].mid;
                    break;
                    case FRONT:
                        if(map->Cases[y][x].front != 0)
                            map->ToolBar->ObjetSelect = map->Cases[y][x].front;
                    break;
                    default:
                        map->Cases[y][x].phys = 1;
                    break;
                }
            }
            else
            {
                switch(calque)
                {// test si on est sur la meme tile qu'avant
                    case BACK:
                        if(map->Cases[y][x].back == map->ToolBar->ObjetSelect)
                            different = 0;
                    break;
                    case MID:
                        if(map->Cases[y][x].mid == map->ToolBar->ObjetSelect)
                            different = 0;
                    break;
                    case FRONT:
                        if(map->Cases[y][x].front == map->ToolBar->ObjetSelect)
                            different = 0;
                    break;
                    default:
                    break;
                }
                if(different)
                {
                    switch(calque)
                    {
                        case BACK:
                            map->Cases[y][x].back = map->ToolBar->ObjetSelect;
                        break;
                        case MID:
                            map->Cases[y][x].mid = map->ToolBar->ObjetSelect;
                        break;
                        case FRONT:
                            map->Cases[y][x].front = map->ToolBar->ObjetSelect;
                        break;
                        default:
                            if(map->Cases[y][x].phys < 9)
                                map->Cases[y][x].phys += 1;
                            else
                                map->Cases[y][x].phys = 1;
                        break;
                    }
                    casemap = map->Cases[y][x].phys;
                    casetile = map->ToolBar->TabTilesetPhysique[map->ToolBar->ObjetSelect/TILESET_W][map->ToolBar->ObjetSelect%TILESET_W];
                    if(calque != PHYSIQUE)
                    {
                        switch(casetile)
                        {// VIDE, PLEIN, BAS, HAUT, DROITE, GAUCHE, BAS_DROITE, BAS_GAUCHE, HAUT_DROITE, HAUT_GAUCHE; pour PHYSIQUE
                            case VIDE:
                            break;
                            case PLEIN:
                                map->Cases[y][x].phys = casetile;
                            break;
                            case BAS :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case HAUT : case HAUT_DROITE : case HAUT_GAUCHE :
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                    case DROITE :
                                        map->Cases[y][x].phys = BAS_DROITE;
                                    break;
                                    case GAUCHE :
                                        map->Cases[y][x].phys = BAS_GAUCHE;
                                    break;
                                    default:
                                    break;

                                }
                            break;
                            case HAUT :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case BAS : case BAS_DROITE : case BAS_GAUCHE :
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                    case DROITE :
                                        map->Cases[y][x].phys = HAUT_DROITE;
                                    break;
                                    case GAUCHE :
                                        map->Cases[y][x].phys = HAUT_GAUCHE;
                                    break;
                                    default:
                                    break;

                                }
                            break;
                            case DROITE :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case GAUCHE : case HAUT_GAUCHE : case BAS_GAUCHE :
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                    case HAUT :
                                        map->Cases[y][x].phys = HAUT_DROITE;
                                    break;
                                    case BAS :
                                        map->Cases[y][x].phys = BAS_DROITE;
                                    break;
                                    default:
                                    break;

                                }
                            break;
                            case GAUCHE :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case DROITE : case HAUT_DROITE : case BAS_DROITE :
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                    case HAUT :
                                        map->Cases[y][x].phys = HAUT_GAUCHE;
                                    break;
                                    case BAS :
                                        map->Cases[y][x].phys = BAS_GAUCHE;
                                    break;
                                    default:
                                    break;

                                }
                            break;
                            case BAS_DROITE :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case BAS : case DROITE : case BAS_DROITE :
                                    break;
                                    default:
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                }
                            break;
                            case BAS_GAUCHE :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case BAS : case GAUCHE : case BAS_GAUCHE :
                                    break;
                                    default:
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                }
                            break;
                            case HAUT_DROITE :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case HAUT : case DROITE : case HAUT_DROITE :
                                    break;
                                    default:
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                }
                            break;
                            case HAUT_GAUCHE :
                                switch(casemap)
                                {
                                    case VIDE :
                                        map->Cases[y][x].phys = casetile;
                                    break;
                                    case HAUT : case GAUCHE : case HAUT_GAUCHE :
                                    break;
                                    default:
                                        map->Cases[y][x].phys = PLEIN;
                                    break;
                                }
                            break;
                            default:
                            break;
                        }
                    }
                }
            }
        }
        else if(clic == CLIC_DROIT)
        {
            if(BLctrl)
            {
                map->Cases[y][x].back = 0;
                map->Cases[y][x].mid = 0;
                map->Cases[y][x].front = 0;
                map->Cases[y][x].phys = 0;
            }
            else
            {
                switch(calque)
                {
                    case BACK:
                        map->Cases[y][x].back = 0;
                    break;
                    case MID:
                        map->Cases[y][x].mid = 0;
                    break;
                    case FRONT:
                        map->Cases[y][x].front = 0;
                    break;
                    default:
                        map->Cases[y][x].phys = 0;
                    break;
                }
            }
        }

    }
    else if(BLtoolbar)
    {
        x = ((event.button.x + scroll.x)/TILE_TAILLE)% TILESET_W;
        y = ((event.button.y - UP_EDGE + scroll.y)/TILE_TAILLE);
        if(clic == CLIC_GAUCHE)
        {
            if(BLctrl)
            {

            }
            else
            {
                map->ToolBar->ObjetSelect = y*TILESET_W + x;
            }
        }
        else if(clic == CLIC_DROIT)
        {
            if(BLctrl)
            {

            }
            else
            {

            }
        }
    }

    return 1;
}

int mouseClicMollette(Smap *map, SDL_Rect SourisDep, SDL_Rect Souris, SDL_Rect *scroll)
{
    int w = Souris.x - SourisDep.x, h = Souris.y - SourisDep.y;
    SDL_Rect Case;
    Case.h = MARGE_CLIC_MOLLETTE*2;
    Case.w = MARGE_CLIC_MOLLETTE*2;
    Case.x = SourisDep.x - MARGE_CLIC_MOLLETTE;
    Case.y = SourisDep.y - MARGE_CLIC_MOLLETTE;
    if(w>0)
        w = w/DIVISEUR_CLIC_MOLLETTE;
    else
        w = w/DIVISEUR_CLIC_MOLLETTE;
    if(h>0)
        h = h/DIVISEUR_CLIC_MOLLETTE;
    else
        h = h/DIVISEUR_CLIC_MOLLETTE;
    if(!colision(Souris,Case))
    {
        scroll->x += w;
        scroll->y += h;
    }
    return 1;
}

void selectMiseAJour(Smap *map, int direction)
{
    switch(direction)
    {

        case DOWN:
                if(map->ToolBar->ObjetSelect < TILESET_H*TILESET_W - TILESET_W +1)
                    map->ToolBar->ObjetSelect += TILESET_W;
        break;
        case UP:
                if(map->ToolBar->ObjetSelect > 0 + TILESET_W -1)
                    map->ToolBar->ObjetSelect -= TILESET_W;
        break;
        case RIGHT:
                if(map->ToolBar->ObjetSelect < TILESET_H*TILESET_W && map->ToolBar->ObjetSelect%TILESET_W < TILESET_W-1)
                    map->ToolBar->ObjetSelect ++;
        break;
        case LEFT:
                if(map->ToolBar->ObjetSelect > 0 && map->ToolBar->ObjetSelect%TILESET_W > 0)
                    map->ToolBar->ObjetSelect --;
        break;
    }
}


