/*
fonctions.c
____________
contient les fonctions du prjet TEST_action
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "consante.h"
#include "fonctions.h"
#include "action.h"

int add_timer(C_timer *timer);

/*
*/
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
