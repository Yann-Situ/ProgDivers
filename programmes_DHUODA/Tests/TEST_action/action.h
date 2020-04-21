/*
action.h
________
contient les prototypes des fonctions de action.c

*/

#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

int INITactions(action action[][4],int L ,int l);
int animation(SDL_Rect *RectTile, action *act,action actions[][4], int NumAnim);
SDL_Rect bougerperso(SDL_Surface *screen, SDL_Rect position, action act);
action boutonENFONCE(action actionACT, action actions[][4],Uint8 Keystate[]);
void miseDIRECTION(action* actionACT, Uint8 Keystate[]);
#endif // ACTION_H_INCLUDED
