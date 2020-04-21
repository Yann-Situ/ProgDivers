#ifndef DEF_JEU
#define DEF_JEU
    int jouer(SDL_Surface* screen, int *Continuer1, int Aventure, char NomLevel[],int edition, int LevelDepart);
    void deplacerJoueur(int carte[][NB_BLOC_H], SDL_Rect *position, int direction);
    void deplacerCaisse(int *PremiereCase, int *SecondeCase);
#endif // DEF_JEU


