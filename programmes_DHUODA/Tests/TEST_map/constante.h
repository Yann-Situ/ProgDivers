#ifndef CONSTANTE_H_INCLUDED
#define CONSTANTE_H_INCLUDED

#define SCREEN_W 1200
#define SCREEN_H 646

#define TILE_TAILLE 32

#define TILESET_W 20
#define TILESET_H 79

#define TOOLBAR_EDGE 64
#define UP_EDGE 64

#define TOOLBAR_W (TILESET_W * TILE_TAILLE /2)
#define TOOLBAR_H (SCREEN_H - UP_EDGE)
#define TOOLBAR_NBBLOCS_W (TOOLBAR_W / TILE_TAILLE)
#define TOOLBAR_NBBLOCS_H (TOOLBAR_H / TILE_TAILLE)

#define INTERFACE_W (SCREEN_W - (TOOLBAR_EDGE + TOOLBAR_W))
#define INTERFACE_H (SCREEN_H - UP_EDGE)
#define INTERFACE_X (TOOLBAR_W + TOOLBAR_EDGE)
#define INTERFACE_Y UP_EDGE

#define SELECT_TAILLE 32
#define SPACE_BEFORE_TEXT (6 + SELECT_TAILLE)
#define MARGE_CLIC_MOLLETTE 50
#define DIVISEUR_CLIC_MOLLETTE 10
#define POLICE_TITLE_TAILLE 40
#define POLICE_TAILLE 32

#define MAX_CHANNEL 12

#define MRG_SCRL 100// marge scroll


#define QUITTER -65

enum {DOWN,RIGHT,UP,LEFT};
enum {SDL,FICHIERS,IMG};
enum {BACK,MID,FRONT,PHYSIQUE};
enum {VIDE,PLEIN,BAS,HAUT,DROITE,GAUCHE,BAS_DROITE,BAS_GAUCHE,HAUT_DROITE,HAUT_GAUCHE};
enum {___VOID____,TILESETS, MUSIQUE};
enum {CHANEL_MUSIC,CHANEL_SON};
enum{CLIC_DROIT, CLIC_GAUCHE, CLIC_MOLLETTE};


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
