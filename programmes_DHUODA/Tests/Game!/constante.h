/*
GAME    Yann-Situ Gazull
____
constante.h
Contient les constantes et #define du projet
*/
#ifndef CONSTANTE_H_INCLUDED
#define CONSTANTE_H_INCLUDED

#define MAX_CHANNEL 12
#define POLICE_TITLE_SIZE 54
#define POLICE1_SIZE 48
#define POLICE2_SIZE 36
#define POLICE3_SIZE 12
#define SCREEN_W 800
#define SCREEN_H 600

#define CLEAR_EVENT while(SDL_PollEvent(&smain->event_poubelle));event.type = SDL_NOEVENT;
#define FILES 3
#define NBMONDES 4
#define SDLK_OK SDLK_i
#define QUITMENU if(smain->continuer == 0){Continuer = 0;ret = 0;}

#define ECRAN_TITRE_ANIM_X 0
#define ECRAN_TITRE_ANIM_Y 0
#define ECRAN_TITRE_ANIM_W 200
#define ECRAN_TITRE_ANIM_H 80
#define ECRAN_TITRE_ANIM_NBT 6
#define ECRAN_TITRE_ANIM_INTERVALE 32
#define ECRAN_TITRE_ANIM_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_titre/ecran-titre-title.bmp"
#define ECRAN_TITRE_TITRE_Y 10
#define ECRAN_TITRE_TEXTE_Y 250
#define ECRAN_TITRE_BACKGROUND_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_titre/ecran-titre-background.bmp"

#define  ECRAN_FICHIER_ANIM_X 0
#define  ECRAN_FICHIER_ANIM_Y 0
#define  ECRAN_FICHIER_ANIM_W 200
#define  ECRAN_FICHIER_ANIM_H 80
#define  ECRAN_FICHIER_ANIM_NBT 6
#define  ECRAN_FICHIER_ANIM_INTERVALE 32
#define  ECRAN_FICHIER_ANIM_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_fichier/ecran-fichier-title.bmp"
#define  ECRAN_FICHIER_TITRE_Y 10
#define  ECRAN_FICHIER_FILE_W 200
#define  ECRAN_FICHIER_FILE_H 90
#define  ECRAN_FICHIER_FILE_X 550
#define  ECRAN_FICHIER_FILE1_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_fichier/ecran-fichier-file1.bmp"
#define  ECRAN_FICHIER_FILE1_Y 100
#define  ECRAN_FICHIER_FILE2_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_fichier/ecran-fichier-file2.bmp"
#define  ECRAN_FICHIER_FILE2_Y 200
#define  ECRAN_FICHIER_FILE3_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_fichier/ecran-fichier-file3.bmp"
#define  ECRAN_FICHIER_FILE3_Y 300
#define  ECRAN_FICHIER_OPTION_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_fichier/ecran-fichier-option.bmp"
#define  ECRAN_FICHIER_OPTION_Y 500
#define  ECRAN_FICHIER_OPTION_W 200
#define  ECRAN_FICHIER_OPTION_H 90
#define  ECRAN_FICHIER_BACKGROUND_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_fichier/ecran-fichier-background.bmp"

#define ECRAN_MAP_ANIM_X 0
#define ECRAN_MAP_ANIM_Y 0
#define ECRAN_MAP_ANIM_W 200
#define ECRAN_MAP_ANIM_H 80
#define ECRAN_MAP_ANIM_NBT 6
#define ECRAN_MAP_ANIM_INTERVALE 32
#define ECRAN_MAP_ANIM_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_map/ecran-map-title.bmp"
#define ECRAN_MAP_TITRE_Y 10
#define ECRAN_MAP_TEXTE_Y 250
#define ECRAN_MAP_BACKGROUND_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_map/ecran-map-background.bmp"
#define ECRAN_MAP_MONDE_Y 300
#define ECRAN_MAP_MONDE_W 200
#define ECRAN_MAP_MONDE_H 250
#define ECRAN_MAP_MONDE_NAME "D:/Utilisateurs/Yann'S/Documents/Programation/programmes/Tests/Game!/ressources/ecran_map/ecran-map-monde.bmp"
#define ECRAN_MAP_SWITCHMONDE_INTERVALE 25

#define FRAME 1000/60

enum {DOWN,RIGHT,UP,LEFT};

#endif // CONSTANTE_H_INCLUDED
