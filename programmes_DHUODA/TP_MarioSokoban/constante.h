#ifndef CONSTANTE_H_INCLUDED
#define CONSTANTE_H_INCLUDED

#define TAILLE_BLOC 34
#define NB_BLOC_W 12
#define NB_BLOC_H 12
#define FENETRE_W TAILLE_BLOC * NB_BLOC_W
#define FENETRE_H TAILLE_BLOC * NB_BLOC_H

enum {HAUT,BAS,GAUCHE,DROITE};
enum {VIDE, MUR, CAISSE, OBJECTIF, MARIO, CAISSE_OK};

typedef struct InfoLevel InfoLevel;
struct InfoLevel
{
    int NumeroNiv;
    char NomNiv[20];
    int Aventure;
};

#endif // CONSTANTE_H_INCLUDED


