/*
GAME    Yann-Situ Gazull
____
ecrans_menus.h
Contient les prototypes des fonctions de ecrans_menus.h
*/
#ifndef ECRANS_MENUS_H_INCLUDED
#define ECRANS_MENUS_H_INCLUDED

int ecran_titre(OBJF* smain);
int ecran_fichier(OBJF* smain, int* file);
int ecran_map(OBJF* smain, int file);

int switchmonde(int sm, int mondemax, int* mondeact, Srect *posi, Srect *rect);

#endif // ECRANS_MENUS_H_INCLUDED
