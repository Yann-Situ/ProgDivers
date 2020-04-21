/*
action.c
________
contient les fonction des animations, des movements et des actions

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "consante.h"
#include "action.h"
#include "fonctions.h"


int INITactions(action action[][4],int L ,int l);
int animation(SDL_Rect *RectTile, action *act,action actions[][4], int NumAnim);
SDL_Rect bougerperso(SDL_Surface *screen, SDL_Rect position, action act);
action boutonENFONCE(action actionACT, action actions[][4],Uint8 Keystate[]);
void miseDIRECTION(action* actionACT, Uint8 Keystate[]);

int INITactions(action action[][4],int L ,int l)
{
    int i = 0, j = 0;
    for(i=0; i<L;i++)
    {
        for(j=0; j<l;j++)
        {
            action[i][j].NumTYPE = i*l+j;
            switch(i)
            {//endTYPE
                case MOOVE:         case MOOVEd:
                case MOOVE_ATTACK:  case MOOVE_ATTACKd:
                case CHARGE_ATTACK:
                    action[i][j].endTYPE = BOUTON_LACHE;
                    break;
                case DODGE:
                case ATTACK:
                case TIR:
                    action[i][j].endTYPE = FIN_DE_LANIM;
                    break;
            }
            switch(i)
            {//NBtiles
                case MOOVE:         case MOOVEd:
                    action[i][j].NBtiles = 10;
                    break;
                case ATTACK:
                    action[i][j].NBtiles = 6;
                    break;
                case TIR:
                    action[i][j].NBtiles = 9;
                    break;
                case DODGE:
                    action[i][j].NBtiles = 5;
                    break;
                case MOOVE_ATTACK:  case MOOVE_ATTACKd:
                    action[i][j].NBtiles = 10;
                    break;
                default:
                    action[i][j].NBtiles = 0;
                    break;
            }
            switch(i)
            {//effets vitesseVar
                case DODGE:
                    action[i][j].effets.vitesseVar = 3;
                    break;
                case MOOVEd:
                    action[i][j].effets.vitesseVar = 0.9;
                    break;
                case MOOVE_ATTACK:  case MOOVE_ATTACKd:
                    action[i][j].effets.vitesseVar = 0.6;
                    break;
                default:
                    action[i][j].effets.vitesseVar = 1;
                    break;
            }
            switch(i)
            {// diago
                case MOOVEd: case MOOVE_ATTACKd:
                    action[i][j].diago = 1;
                    break;
                default:
                    action[i][j].diago = 0;
                    break;
            }
            switch(i)
            {//effets DirectionX
            case CHARGE_ATTACK: case STOP: case ATTACK: case MOOVE_ATTACK: case TIR:
                action[i][j].effets.directionX = 0;
                break;
            case DODGE:
                switch(j)
                    {
                        case RIGHT:
                            action[i][j].effets.directionX = -1;
                            break;
                        case LEFT:
                            action[i][j].effets.directionX = 1;
                            break;
                        default:
                            action[i][j].effets.directionX = 0;
                            break;
                    }
                    break;
                break;
            default:
            switch(action[i][j].diago)
            {
                case 1:
                    switch(j)
                    {
                        case DOWN_RIGHT: case UP_RIGHT:
                            action[i][j].effets.directionX = 1;
                            break;
                        case DOWN_LEFT: case UP_LEFT:
                            action[i][j].effets.directionX = -1;
                            break;
                        default:
                            action[i][j].effets.directionX = 0;
                            break;
                    }
                    break;

                case 0:
                    switch(j)
                    {
                        case RIGHT:
                            action[i][j].effets.directionX = 1;
                            break;
                        case LEFT:
                            action[i][j].effets.directionX = -1;
                            break;
                        default:
                            action[i][j].effets.directionX = 0;
                            break;
                    }
                    break;
            }
            break;
            }
            switch(i)
            {//effets DirectionY
            case CHARGE_ATTACK: case STOP: case ATTACK: case MOOVE_ATTACK: case TIR:
                action[i][j].effets.directionY = 0;
                break;
            case DODGE:
                switch(j)
                    {
                        case DOWN:
                            action[i][j].effets.directionY = -1;
                            break;
                        case UP:
                            action[i][j].effets.directionY = 1;
                            break;
                        default:
                            action[i][j].effets.directionY = 0;
                            break;
                    }
                    break;
                break;
            default:
                switch(action[i][j].diago)
            {
                case 1:
                    switch(j)
                    {
                        case DOWN_RIGHT: case DOWN_LEFT:
                            action[i][j].effets.directionY = 1;
                            break;
                        case UP_RIGHT: case UP_LEFT:
                            action[i][j].effets.directionY = -1;
                            break;
                        default:
                            action[i][j].effets.directionY = 0;
                            break;
                    }
                    break;

                case 0:
                    switch(j)
                    {
                        case DOWN:
                            action[i][j].effets.directionY = 1;
                            break;
                        case UP:
                            action[i][j].effets.directionY = -1;
                            break;
                        default:
                            action[i][j].effets.directionY = 0;
                            break;
                    }
                    break;
            }
            break;
            }
            switch(i)
            {//effets hitBox
                case DODGE:
                    action[i][j].effets.hitBox.w = 0;
                    action[i][j].effets.hitBox.h = 0;
                    break;
                default:
                    action[i][j].effets.hitBox.w = W_HITBOX;
                    action[i][j].effets.hitBox.h = H_HITBOX;
                    break;
            }
            switch(i)
            {//effets hitBoxDamage
                case MOOVE_ATTACK:  case MOOVE_ATTACKd:
                    action[i][j].effets.hitBoxDamage.w = l_HITBOXDAMAGE;
                    action[i][j].effets.hitBoxDamage.h = l_HITBOXDAMAGE;
                    break;
                case ATTACK:
                    switch(j)
                    {
                        case DOWN:
                            action[i][j].effets.hitBoxDamage.w = L_HITBOXDAMAGE;
                            action[i][j].effets.hitBoxDamage.h = l_HITBOXDAMAGE;
                            break;
                        case RIGHT:
                            action[i][j].effets.hitBoxDamage.w = l_HITBOXDAMAGE;
                            action[i][j].effets.hitBoxDamage.h = L_HITBOXDAMAGE;
                            break;
                        case LEFT:
                            action[i][j].effets.hitBoxDamage.w = l_HITBOXDAMAGE;
                            action[i][j].effets.hitBoxDamage.h = L_HITBOXDAMAGE;
                            break;
                        case UP:
                            action[i][j].effets.hitBoxDamage.w = L_HITBOXDAMAGE;
                            action[i][j].effets.hitBoxDamage.h = l_HITBOXDAMAGE;
                            break;
                    }
                    break;
            }
            switch(i)
            {//autreAction
                case MOOVE:     case MOOVEd:     case CHARGE_ATTACK:
                    action[i][j].autreAction = STOP;
                    break;
                case TIR:
                case DODGE:
                case MOOVE_ATTACK:  case MOOVE_ATTACKd:
                case ATTACK:
                    action[i][j].autreAction = MOOVE;
                    break;
                case STOP:
                    action[i][j].autreAction = -1;
                    break;
            }
            switch(i)
            {//ligneTileset
                case STOP: case MOOVE: case MOOVEd:
                    action[i][j].ligneTileset = STOP_LT*4+j;
                    break;
                case DODGE:
                    action[i][j].ligneTileset = DODGE_LT*4+j;
                    break;
                case ATTACK:
                    action[i][j].ligneTileset = ATTACK_LT*4+j;
                    break;
                case CHARGE_ATTACK: case MOOVE_ATTACK: case MOOVE_ATTACKd:
                    action[i][j].ligneTileset = CHARGE_ATTACK_LT*4+j;
                    break;
                case TIR:
                    action[i][j].ligneTileset = TIR_LT*4+j;
                    break;
                default:
                    action[i][j].ligneTileset = 0;
                    break;
            }
        }
    }
    return 1;
}

int animation(SDL_Rect *RectTile, action *act,action actions[][4], int NumAnim)
{
    int i=0;
    if(act->NBtiles!=0)
    {
        if(NumAnim > act->NBtiles-1)
        {
            if(act->endTYPE==FIN_DE_LANIM)
            {
                NumAnim = 0;
                *act = actions[act->autreAction][act->NumTYPE%4];
            }
            else
            {
                NumAnim = 1;
            }
        }
        else
        {
            NumAnim++;
        }
    }
    else
    {
        NumAnim = 0;
    }
    RectTile->x = NumAnim * L_TILE_PERSO;
    return NumAnim;
}

SDL_Rect bougerperso(SDL_Surface *screen, SDL_Rect position, action act)
{
    if(act.effets.directionX == 1)
        position.x+= 2;
    else if(act.effets.directionX == -1)
        position.x-= 2;

    if(act.effets.directionY == 1)
        position.y+= 2;
    else if(act.effets.directionY == -1)
        position.y-= 2;

    if(position.x+L_TILE_PERSO/2 > screen->w)
        position.x = L_TILE_PERSO/2;
    else if(position.x < 0)
        position.x = screen->w-L_TILE_PERSO/2;

    if(position.y+L_TILE_PERSO/2 > screen->h)
        position.y = L_TILE_PERSO/2;
    else if(position.y < 0)
        position.y = screen->h-L_TILE_PERSO/2;
    return position;
}

action boutonENFONCE(action actionACT, action actions[8][4],Uint8 Keystate[])
{
    int i = (actionACT.NumTYPE-actionACT.NumTYPE%4)/4, j = actionACT.ligneTileset%4;
    switch(i)
    {
            case DODGE: case ATTACK: case TIR:
            break;
            case CHARGE_ATTACK: case MOOVE_ATTACK:
                if(i == CHARGE_ATTACK)
                    actionACT = actions[MOOVE_ATTACK][actionACT.NumTYPE%4];
                miseDIRECTION(&actionACT,Keystate);
                if(actionACT.effets.directionY == 0 && actionACT.effets.directionX == 0)
                    actionACT = actions[CHARGE_ATTACK][actionACT.NumTYPE%4];
            break;
            default:
                miseDIRECTION(&actionACT,Keystate);
                switch(actionACT.effets.directionY)
                {
                    case 1:
                        switch(actionACT.effets.directionX)
                        {
                            case 1:
                                actionACT = actions[MOOVEd][DOWN_RIGHT];
                                actionACT.ligneTileset = j;
                            break;
                            case 0:
                                actionACT = actions[MOOVE][DOWN];
                            break;
                            case -1:
                                actionACT = actions[MOOVEd][DOWN_LEFT];
                                actionACT.ligneTileset = j;
                            break;
                        }
                    break;
                    case 0:
                        switch(actionACT.effets.directionX)
                        {
                            case 1:
                                actionACT = actions[MOOVE][RIGHT];
                            break;
                            case 0:
                                actionACT = actions[STOP][j];
                            break;
                            case -1:
                                actionACT = actions[MOOVE][LEFT];
                            break;
                        }
                    break;
                    case -1:
                        switch(actionACT.effets.directionX)
                        {
                            case 1:
                                actionACT = actions[MOOVEd][UP_RIGHT];
                                actionACT.ligneTileset = j;
                            break;
                            case 0:
                                actionACT = actions[MOOVE][UP];
                            break;
                            case -1:
                                actionACT = actions[MOOVEd][UP_LEFT];
                                actionACT.ligneTileset = j;
                            break;
                        }
                    break;

                }
            break;
    }
    return actionACT;
}

void miseDIRECTION(action* actionACT, Uint8 Keystate[])
{
    if(Keystate[SDLK_DOWN])
        actionACT->effets.directionY = 1;
    else if(Keystate[SDLK_UP])
        actionACT->effets.directionY = -1;
    else
        actionACT->effets.directionY = 0;

    if(Keystate[SDLK_RIGHT])
        actionACT->effets.directionX = 1;
    else if(Keystate[SDLK_LEFT])
        actionACT->effets.directionX = -1;
    else
        actionACT->effets.directionX = 0;
}

