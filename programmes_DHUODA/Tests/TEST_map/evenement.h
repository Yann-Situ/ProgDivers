/*
evenement.h
__________
    contient les prototypes des fonctions de evenement.c
*/


#ifndef EVENEMENT_H_INCLUDED
#define EVENEMENT_H_INCLUDED

int mouseClic(Smap *map, SDL_Event event, SDL_Rect scroll, int clic, int calque, int BLtoolbar, int BLctrl);
int mouseClicMollette(Smap *map, SDL_Rect SourisDep, SDL_Rect Souris, SDL_Rect *scroll);
void selectMiseAJour(Smap *map, int direction);


#endif // EVENEMENT_H_INCLUDED
