#ifndef FONCTION_0_H_INCLUDED
#define FONCTION_0_H_INCLUDED

// ENUM
//__________________________
    enum {SDL,FICHIERS,IMG};
// STRUCT
//__________________________
    typedef struct Sstring Sstring;
    struct Sstring
    {
        char Nom[FILENAME_MAX];
    };

    typedef struct objetFonction OBJF;
    struct objetFonction
    {
        FMOD_SYSTEM *systeme;
        SDL_Surface *screen;
        SDL_Event event_poubelle;
        int continuer;
        TTF_Font *policeTitle, *police1, *police2, *police3;
    };

    typedef struct C_timer C_timer;
    struct C_timer
    {
        float tempsPre;
        float intervalle;
    };

    typedef struct Sanim Sanim;
    struct Sanim
    {
        C_timer timer;
        SDL_Surface *sprites;
        SDL_Rect rectTile;
        int NBTile;
        int NumTileACT;
        int NBloop;
        int NumloopACT;
    };

    typedef struct Srect Srect;
    struct Srect
    {
        int h,w,x,y;
    };

//fonctions classique
void checkerror(SDL_Surface *s, FILE *fichier, int type); //gère les error des structures
Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y);//récupère le pixel sur une surface à une position
void SetAlpha(SDL_Surface *surface, Uint8 alpha);//change la valeur de la transparence alpha
int add_timer(C_timer *timer);// retourne 1 si le temps du timer est dépassé
int freeOBJF(OBJF *smain);// libère une structure OBJF de la mémoire

//fonctions animation
int animation(Sanim *anim);// gère l'animation des tile en fonction du timer etc
int clignot(SDL_Surface *s, int blink, int ecart);// retourne la valeur de la transpa alpha envoyer dans blink apres les modif avec l'écart et change cette valeur dans la surface
int fondublanc(SDL_Surface *s, int blink, int ecart);//fondu jusqu'au blanc
int fondunoir(SDL_Surface *s, int blink, int ecart);//fondu jusqu'au noir
Sanim* chargerAnim(Sanim *animSelect, int Nbl,int Nbt,int Nul,int Nut,int h,int w,int x,int y,int interv,char *tilesetName);// aloue de la mémoire pour une structure animation

//fonctions rect
SDL_Rect recttoSDL(Srect r);
Srect SDLtorect(SDL_Rect r);

#endif // FONCTION_0_H_INCLUDED
