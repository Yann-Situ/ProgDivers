#ifndef CONSTANTE_H_INCLUDED
#define CONSTANTE_H_INCLUDED

#define SCREEN_W 1200
#define SCREEN_H 646

#define POLICE_TITLE_TAILLE 32
#define POLICE_TAILLE 24

#define MAX_CHANNEL 1

#define ADHERENCE 0.6
#define H_JUMP 19
#define VIT_MAX 16
#define GRAVITY 0.6
#define ACCELERATION 0.8

#define QUITTER -65

enum {SDL,FICHIERS,IMG};
enum {TERRE,AIR};

typedef struct Smusic Smusic;
struct Smusic
{
    char filename[FILENAME_MAX];
    int Loopstart;
    int Loopend;
};

typedef struct Stoolbar Stoolbar;
struct Stoolbar
{

    SDL_Surface *TilesetPhysique;
    SDL_Surface *Select;

    SDL_Rect RectSelect;

    int **TabTilesetPhysique;
    int ObjetSelect;
};

typedef struct Scase Scase;
struct Scase
{
    int back, mid, front, phys;
};

typedef struct Smap Smap;
struct Smap
{
    int NumeroNiv;
    char NomNiv[FILENAME_MAX];
    int w_blocs,h_blocs;

    char tilesetname[FILENAME_MAX];
    SDL_Surface *Tileset;

    Scase **Cases;

    Stoolbar *ToolBar;

    FMOD_SOUND *musique;
    Smusic musicInfo;
};

typedef struct Sstring Sstring;
struct Sstring
{
    char Nom[FILENAME_MAX];
};

typedef struct objetFonction OBJF;
struct objetFonction
{
    TTF_Font *police;
    FMOD_SYSTEM *systeme;
    SDL_Surface *screen;
    SDL_Event event_poubelle;
    int continuer;
    TTF_Font *policeTitle;
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



#endif // CONSTANTE_H_INCLUDED
