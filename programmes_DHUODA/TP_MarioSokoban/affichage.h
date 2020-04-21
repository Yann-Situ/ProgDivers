#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED
void bravo(SDL_Surface* screen, int *Cont1,int fin);
SDL_Rect curseur(SDL_Rect *posi, int Taille, int direction);

void MenuJouer(SDL_Surface* screen, SDL_Surface* Selection, int *Continuer1);
void MenuEditeur(SDL_Surface* screen, SDL_Surface* Selection, int *Continuer1);
void MenuCharger(SDL_Surface* screen, SDL_Surface* Selection, int Aventure, int *Continuer1, int Edition);
void MenuSauvegarder(SDL_Surface* screen, int Aventure, int *Continuer1, int carte[][NB_BLOC_H],FMOD_SYSTEM *systeme);
void niveauDebut(SDL_Surface* screen, FMOD_SYSTEM *systeme,char NomLevel[]);

#endif // AFFICHAGE_H_INCLUDED
