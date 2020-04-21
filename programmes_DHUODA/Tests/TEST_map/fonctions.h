
/*
fonctions.h
__________
    contient les prototypes des fonctions de fonctions.c
*/


#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

//fonctions classique
void checkerror(SDL_Surface *s, FILE *fichier, int type);
Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y);
int add_timer(C_timer *timer);
int freeOBJF(OBJF *smain);
unsigned int colision(SDL_Rect rect1, SDL_Rect rect2);/* return :
    0 : exterieur
    1 : interieur
    2 : une partie interieur
    3 : longueur plus grande que l'autre rect
    4 : hauteur plus grande que l'autre rect
    5 : hauteur et longueur plus grande que l'autre rect
*/
unsigned int coincage(SDL_Rect *rect1, SDL_Rect rect2, unsigned int xy);/* return :
    0 : probleme : rect plus grand que l'autre...
    1 : interieur
    2 : modification faite

    xy :0 -> coinçage classique
        1 -> coinçage seulement horizontal
        2 -> coinçage seulement vertical
*/

//fonctions animation
int animation(Sanim *anim);
Sanim chargerAnim(Sanim animSelect, int Nbl,int Nbt,int Nul,int Nut,int h,int w,int x,int y,int interv,char *tilesetName);

//fonctions menu
int menu(OBJF *smain, Sanim *animSelect, Sstring *NomChoix, SDL_Surface *fond, char Title[], int NBchoix, int BLnoir, int *Quitter);
int menuNB(OBJF *smain, Sanim *animSelect, SDL_Surface *fond, char Title[], int NBmin, int NBmax, int BLnoir, int *Quitter);

#endif // FONCTIONS_H_INCLUDED
