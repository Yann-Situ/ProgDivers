/*
map.h
__________
    contient les prototypes des fonctions de map.c
*/


#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

int mettreNomdsTableau(Sstring *levels, int NbLvL, FILE *fichier);
int chargerMap(Smap *map, OBJF *smain, int niv);
int sauvegarderMap(Smap *map, OBJF *smain, int niv);
int menuNEW(OBJF *smain, Smap *map, FILE *fichier);
int ecrireMap(Smap *map, FILE *fichier);
int lireMap(Smap *map, FILE *fichier);

Stoolbar initPhysique(Stoolbar TB);
int libererMAP(Smap *map);

void printTOOLBAR(OBJF *smain, Smap map, SDL_Rect scroll);
void printINTERFACE(OBJF *smain, Smap map, SDL_Rect scroll, SDL_Surface *vide, SDL_Surface *tilePhysAffichage, int calque);


#endif // MAP_H_INCLUDED
