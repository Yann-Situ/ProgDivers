/*

"constante.h"
par SITU
    définnie les define, les enum et les struct des autres fichiers
*/

#ifndef CONSANTE_H_INCLUDED
#define CONSANTE_H_INCLUDED

#define NB_ACTIONS 9
#define W_HITBOX 16
#define H_HITBOX 16
#define L_HITBOXDAMAGE 48
#define l_HITBOXDAMAGE 16
#define L_TILE_PERSO 48

/* Type d'action*/
enum{STOP,
MOOVE,
MOOVEd,
DODGE,
ATTACK,
CHARGE_ATTACK,
MOOVE_ATTACK,
MOOVE_ATTACKd,
TIR
};
enum{STOP_LT = 0,
MOOVE_LT = 0,
MOOVEd_LT = 0,
DODGE_LT = 1,
ATTACK_LT = 2,
CHARGE_ATTACK_LT = 3,
MOOVE_ATTACK_LT = 3,
MOOVE_ATTACKd_LT = 3,
TIR_LT = 4
};
enum{DOWN,RIGHT,LEFT,UP};
enum{DOWN_RIGHT,DOWN_LEFT,UP_RIGHT,UP_LEFT};

enum{BOUTON_LACHE,FIN_DE_LANIM,AUTRE_BOUTON};

/* Structure effet*/
typedef struct effet effet;
struct effet
{
    float vitesseVar;
    int directionX;
    int directionY;
    SDL_Rect hitBox;
    SDL_Rect hitBoxDamage;
};

/* Structure actions*/
typedef struct action action;
struct action
{
    int NumTYPE;
    int endTYPE;
    int NBtiles;
    int diago;
    effet effets;
    int autreAction;
    int ligneTileset;
};

/* Structure timer*/
typedef struct C_timer C_timer;
struct C_timer
{
    float tempsPre;
    float intervalle;
};

#endif // CONSANTE_H_INCLUDED
