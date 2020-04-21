/*
GAME    Yann-Situ Gazull
____
ecrans_menus.c
Contient les fonctions des écrans de menu principaux et secondaires
*/
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "../fonction_0/fonction_0.h"
#include "constante.h"
#include "structure.h"
#include "ecrans_menus.h"

int ecran_titre(OBJF* smain);
int ecran_fichier(OBJF* smain, int* file);
int ecran_map(OBJF* smain, int file);

int switchmonde(int sm, int mondemax, int* mondeact, Srect *posi, Srect *rect);

int ecran_titre(OBJF* smain)
{
    fprintf(stderr,"//DEBUT ECRAN_TITRE\n");
    //CREAT VAR
    SDL_Surface *Background = NULL, *Texte = NULL;
    Sanim *Titre = malloc(sizeof(Sanim));
    SDL_Event event;
    SDL_Rect PositionTitre, PositionTexte;
    SDL_Color couleurBlanche = {255,255,255};
    C_timer frame = {SDL_GetTicks(),FRAME};
    int Continuer = 1, ret = 0, blink = 0;
    //INIT VAR
    Background = IMG_Load(ECRAN_TITRE_BACKGROUND_NAME);
    checkerror(Background,NULL,IMG);
    Texte = TTF_RenderText_Blended(smain->police2,"Appuyer sur une touche",couleurBlanche);
    checkerror(Texte,NULL,SDL);
    Titre = chargerAnim(Titre, -1, ECRAN_TITRE_ANIM_NBT, 0,0, ECRAN_TITRE_ANIM_H, ECRAN_TITRE_ANIM_W, ECRAN_TITRE_ANIM_X, ECRAN_TITRE_ANIM_Y, ECRAN_TITRE_ANIM_INTERVALE, ECRAN_TITRE_ANIM_NAME);
    SDL_SetColorKey(Titre->sprites, SDL_SRCCOLORKEY, SDL_MapRGB(Titre->sprites->format, 255, 100, 100));
    {//RECT
    PositionTitre.x = smain->screen->w/2 - Titre->rectTile.w/2;
    PositionTitre.y = ECRAN_TITRE_TITRE_Y;
    PositionTexte.x = smain->screen->w/2 - Texte->w/2;
    PositionTexte.y = ECRAN_TITRE_TEXTE_Y;
    }
    //EVOLUTION
    CLEAR_EVENT
    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                smain->continuer = 0;
                ret = 0;
                break;
            case(SDL_KEYDOWN):
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    smain->continuer = 0;
                else
                {
                    Continuer = 0;
                    ret = 1;
                }
                break;
            default:
                break;
        }
        QUITMENU
        blink = fondublanc(Background,blink,8);
        animation(Titre);
        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 0,0,0));
        SDL_BlitSurface(Background, NULL, smain->screen, NULL);
        SDL_BlitSurface(Texte, NULL, smain->screen, &PositionTexte);
        SDL_BlitSurface(Titre->sprites, &Titre->rectTile, smain->screen, &PositionTitre);
        SDL_Flip(smain->screen);
        if(!add_timer(&frame))
            SDL_Delay(frame.tempsPre+frame.intervalle-SDL_GetTicks());
    }
    //FREE VAR
    SDL_FreeSurface(Background);
    SDL_FreeSurface(Texte);
    free(Titre);
    fprintf(stderr,"//FIN ECRAN_TITRE\n");
    return ret;
}

int ecran_fichier(OBJF* smain, int* file)
{
    fprintf(stderr,"//\tDEBUT ECRAN_FICHIER\n");
    //CREAT VAR
    SDL_Surface *Background = NULL, **Files = malloc(FILES * sizeof(SDL_Surface*)), *Option = NULL;
    Sanim *Titre = malloc(sizeof(Sanim));
    SDL_Event event;
    SDL_Rect PositionTitre, PositionFile[FILES], PositionOption, RectFile[FILES], RectOption;
    C_timer frame = {SDL_GetTicks(),FRAME};
    int Continuer = 1, ret = 0, i=0, ancien = 1;
    Uint8 keystate[4] = {0};
    //INIT VAR
    {//SURFACE
    Background = IMG_Load(ECRAN_FICHIER_BACKGROUND_NAME);
    checkerror(Background,NULL,IMG);
    Option = IMG_Load(ECRAN_FICHIER_OPTION_NAME);
    checkerror(Option,NULL,IMG);
    SDL_SetColorKey(Option, SDL_SRCCOLORKEY, SDL_MapRGB(Option->format, 255, 100, 100));
    Files[0] = IMG_Load(ECRAN_FICHIER_FILE1_NAME);
    checkerror(Files[0],NULL,IMG);
    SDL_SetColorKey(Files[0], SDL_SRCCOLORKEY, SDL_MapRGB(Files[0]->format, 255, 100, 100));
    Files[1] = IMG_Load(ECRAN_FICHIER_FILE2_NAME);
    checkerror(Files[1],NULL,IMG);
    SDL_SetColorKey(Files[1], SDL_SRCCOLORKEY, SDL_MapRGB(Files[1]->format, 255, 100, 100));
    Files[2] = IMG_Load(ECRAN_FICHIER_FILE3_NAME);
    checkerror(Files[2],NULL,IMG);
    SDL_SetColorKey(Files[2], SDL_SRCCOLORKEY, SDL_MapRGB(Files[2]->format, 255, 100, 100));
    }
    Titre = chargerAnim(Titre, -1, ECRAN_FICHIER_ANIM_NBT, 0,0, ECRAN_FICHIER_ANIM_H, ECRAN_FICHIER_ANIM_W, ECRAN_FICHIER_ANIM_X, ECRAN_FICHIER_ANIM_Y, ECRAN_FICHIER_ANIM_INTERVALE, ECRAN_FICHIER_ANIM_NAME);
    SDL_SetColorKey(Titre->sprites, SDL_SRCCOLORKEY, SDL_MapRGB(Titre->sprites->format, 255, 100, 100));
    {//RECT
    PositionTitre.x = smain->screen->w/2 - Titre->rectTile.w/2;
    PositionTitre.y = ECRAN_FICHIER_TITRE_Y;
    PositionFile[0].y = ECRAN_FICHIER_FILE1_Y;
    PositionFile[0].x = ECRAN_FICHIER_FILE_X;
    PositionFile[1].y = ECRAN_FICHIER_FILE2_Y;
    PositionFile[1].x = ECRAN_FICHIER_FILE_X;
    PositionFile[2].y = ECRAN_FICHIER_FILE3_Y;
    PositionFile[2].x = ECRAN_FICHIER_FILE_X;
    PositionOption.x = smain->screen->w/2 - Option->w/4;
    PositionOption.y = ECRAN_FICHIER_OPTION_Y;
    for(i=0;i<FILES;i++)
    {
        RectFile[i].w = ECRAN_FICHIER_FILE_W;
        RectFile[i].h = ECRAN_FICHIER_FILE_H;
        RectFile[i].x = 0;
        RectFile[i].y = 0;
    }
    RectOption.w = ECRAN_FICHIER_OPTION_W;
    RectOption.h = ECRAN_FICHIER_OPTION_H;
    RectOption.x = 0;
    RectOption.y = 0;
    }
    //EVOLUTION
    CLEAR_EVENT
    ret = 1;
    RectFile[ret-1].x += ECRAN_FICHIER_OPTION_W;
    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                smain->continuer = 0;
                ret = 0;
                break;
            case(SDL_KEYDOWN):
                switch(event.key.keysym.sym)
                {
                    case SDLK_w : case SDLK_UP :
                        if(!keystate[UP])
                        {
                            if(ret > 1)
                            {
                                if(ret == 4)
                                    RectOption.x -= ECRAN_FICHIER_OPTION_W;
                                else
                                    RectFile[ret-1].x -= ECRAN_FICHIER_FILE_W;
                                ret--;
                                RectFile[ret-1].x += ECRAN_FICHIER_FILE_W;
                            }
                            keystate[UP] = 1;
                        }
                        break;
                    case SDLK_s : case SDLK_DOWN :
                        if(!keystate[DOWN])
                        {
                            if(ret < 4 && ret != 0)
                            {
                                RectFile[ret-1].x -= ECRAN_FICHIER_FILE_W;
                                ret++;
                                if(ret == 4)
                                    RectOption.x += ECRAN_FICHIER_OPTION_W;
                                else
                                    RectFile[ret-1].x += ECRAN_FICHIER_FILE_W;
                            }
                            keystate[DOWN] = 1;
                        }
                        break;
                    case SDLK_a : case SDLK_LEFT :
                        if(!keystate[LEFT])
                        {
                            if(ret != 0 && ret < 4)
                            {
                                RectFile[ret-1].x -= ECRAN_FICHIER_FILE_W;
                                ancien = ret;
                                ret = 4;
                                RectOption.x += ECRAN_FICHIER_OPTION_W;
                            }
                            keystate[LEFT] = 1;
                        }
                        break;
                    case SDLK_d : case SDLK_RIGHT :
                        if(!keystate[RIGHT])
                        {
                            if(ret == 4)
                            {
                                RectOption.x -= ECRAN_FICHIER_OPTION_W;
                                ret = ancien;
                                RectFile[ret-1].x += ECRAN_FICHIER_FILE_W;
                            }
                            keystate[RIGHT] = 1;
                        }
                        break;
                    case SDLK_ESCAPE:
                        Continuer = 0;
                        ret = 0;
                        break;
                    case SDLK_RETURN: case SDLK_OK :
                        Continuer = 0;
                        break;
                    default:break;
                }
                break;
            case(SDL_KEYUP):
                switch(event.key.keysym.sym)
                {
                    case SDLK_w : case SDLK_UP :
                        keystate[UP] = 0;
                        break;
                    case SDLK_s : case SDLK_DOWN :
                        keystate[DOWN] = 0;
                        break;
                    case SDLK_a : case SDLK_LEFT :
                        keystate[LEFT] = 0;
                        break;
                    case SDLK_d : case SDLK_RIGHT :
                        keystate[RIGHT] = 0;
                        break;
                    default:break;
                }
            default:break;
        }
        QUITMENU
        animation(Titre);
        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 0,0,0));
        SDL_BlitSurface(Background, NULL, smain->screen, NULL);
        SDL_BlitSurface(Files[0],&RectFile[0],smain->screen,&PositionFile[0]);
        SDL_BlitSurface(Files[1],&RectFile[1],smain->screen,&PositionFile[1]);
        SDL_BlitSurface(Files[2],&RectFile[2],smain->screen,&PositionFile[2]);
        SDL_BlitSurface(Option,&RectOption,smain->screen,&PositionOption);
        SDL_BlitSurface(Titre->sprites, &Titre->rectTile, smain->screen, &PositionTitre);
        SDL_Flip(smain->screen);
        if(!add_timer(&frame))
            SDL_Delay(frame.tempsPre+frame.intervalle-SDL_GetTicks());
    }
    //FREE VAR
    SDL_FreeSurface(Background);
    free(Files);
    SDL_FreeSurface(Option);
    free(Titre);
    fprintf(stderr,"//\tFIN ECRAN_FICHIER\n");
    if(file != NULL)
        *file = ret;
    return ret;
}

int ecran_map(OBJF* smain, int file)
{
     fprintf(stderr,"//\t\tDEBUT ECRAN_MAP\n");
    //CREAT VAR
    SDL_Surface *Background = NULL, *Texte = NULL, *ImageMonde = NULL;
    Sanim *Titre = malloc(sizeof(Sanim));
    SDL_Event event;
    Srect PositionTitre, PositionTexte, PositionMonde, RectMonde;
    SDL_Rect SDLPositionTitre, SDLPositionTexte, SDLPositionMonde, SDLRectMonde;
    SDL_Color couleurBlanche = {255,255,255};
    C_timer frame = {SDL_GetTicks(),FRAME};
    int Continuer = 1, ret = 0, blinkbg = 0, blinkmonde = 0, SwitchMonde = 0, Monde = 1, MondeSave = Monde, MondeMax = NBMONDES /*= getMondeMax(file)*/;
    char NumMonde[10] = "MONDE 1";
    Uint8 keystate[4] = {0};
    //INIT VAR
    {//SURFACE
    Background = IMG_Load(ECRAN_MAP_BACKGROUND_NAME);
    checkerror(Background,NULL,IMG);
    Texte = TTF_RenderText_Blended(smain->police1,NumMonde,couleurBlanche);
    checkerror(Texte,NULL,SDL);
    ImageMonde = IMG_Load(ECRAN_MAP_MONDE_NAME);
    checkerror(ImageMonde,NULL,IMG);
    SDL_SetColorKey(ImageMonde, SDL_SRCCOLORKEY, SDL_MapRGB(ImageMonde->format, 255, 100, 100));
    }
    Titre = chargerAnim(Titre, -1, ECRAN_MAP_ANIM_NBT, 0,0, ECRAN_MAP_ANIM_H, ECRAN_MAP_ANIM_W, ECRAN_MAP_ANIM_X, ECRAN_MAP_ANIM_Y, ECRAN_MAP_ANIM_INTERVALE, ECRAN_MAP_ANIM_NAME);
    SDL_SetColorKey(Titre->sprites, SDL_SRCCOLORKEY, SDL_MapRGB(Titre->sprites->format, 255, 100, 100));
    {//RECT
    PositionTitre.x = smain->screen->w/2 - Titre->rectTile.w/2;
    PositionTitre.y = ECRAN_MAP_TITRE_Y;
    PositionTexte.x = smain->screen->w/2 - Texte->w/2;
    PositionTexte.y = ECRAN_MAP_TEXTE_Y;
    RectMonde.h = ECRAN_MAP_MONDE_H;
    RectMonde.w = ECRAN_MAP_MONDE_W;
    RectMonde.x = Monde * RectMonde.w;
    RectMonde.y = 0;
    PositionMonde.x = smain->screen->w/2 - RectMonde.w/2;
    PositionMonde.y = ECRAN_MAP_MONDE_Y;
    }
    //EVOLUTION
    CLEAR_EVENT
    while(Continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                Continuer = 0;
                smain->continuer = 0;
                ret = 0;
                break;
            case(SDL_KEYDOWN):
                switch(event.key.keysym.sym)
                {
                    case SDLK_w : case SDLK_UP :
                        if(!keystate[UP])
                        {
                            if(Monde<MondeMax)
                            {
                                SwitchMonde = 2;
                            }
                            keystate[UP] = 1;
                        }
                        break;
                    case SDLK_s : case SDLK_DOWN :
                        if(!keystate[DOWN])
                        {
                            if(Monde>0)
                            {
                                SwitchMonde = -2;
                            }
                            keystate[DOWN] = 1;
                        }
                        break;
                    case SDLK_a : case SDLK_LEFT :
                        if(!keystate[LEFT])
                        {
                            if(Monde>0)
                            {
                                SwitchMonde = -1;
                            }
                            keystate[LEFT] = 1;
                        }
                        break;
                    case SDLK_d : case SDLK_RIGHT :
                        if(!keystate[RIGHT])
                        {
                            if(Monde<MondeMax)
                            {
                                SwitchMonde = 1;
                            }
                            keystate[RIGHT] = 1;
                        }
                        break;
                    case SDLK_ESCAPE:
                        Continuer = 0;
                        ret = 0;
                        break;
                    case SDLK_RETURN: case SDLK_OK :
                        Continuer = 0;
                        break;
                    default:break;
                }
                break;
            case(SDL_KEYUP):
                switch(event.key.keysym.sym)
                {
                    case SDLK_w : case SDLK_UP :
                        keystate[UP] = 0;
                        break;
                    case SDLK_s : case SDLK_DOWN :
                        keystate[DOWN] = 0;
                        break;
                    case SDLK_a : case SDLK_LEFT :
                        keystate[LEFT] = 0;
                        break;
                    case SDLK_d : case SDLK_RIGHT :
                        keystate[RIGHT] = 0;
                        break;
                    default:break;
                }
            default:
                break;
        }
        QUITMENU
        blinkbg = fondublanc(Background,blinkbg,10);
        blinkmonde = fondublanc(ImageMonde,blinkmonde,10);

        animation(Titre);
        if(SwitchMonde != 0)
        {
            MondeSave = Monde;
            SwitchMonde = switchmonde(SwitchMonde, MondeMax, &Monde, &PositionMonde, &RectMonde);
            if(Monde != MondeSave)
            {
                if(Monde == 0)
                    sprintf(NumMonde,"PRACTICE");
                else
                    sprintf(NumMonde,"MONDE %d",Monde);
                Texte = TTF_RenderText_Blended(smain->police1,NumMonde,couleurBlanche);
            }
        }

        SDLPositionTitre = recttoSDL(PositionTitre) ; SDLPositionTexte = recttoSDL(PositionTexte) ; SDLPositionMonde = recttoSDL(PositionMonde) ; SDLRectMonde = recttoSDL(RectMonde);
        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 0,0,0));
        SDL_BlitSurface(Background, NULL, smain->screen, NULL);
        SDL_BlitSurface(Texte, NULL, smain->screen, &SDLPositionTexte);
        SDL_BlitSurface(Titre->sprites, &Titre->rectTile, smain->screen, &SDLPositionTitre);
        SDL_BlitSurface(ImageMonde, &SDLRectMonde, smain->screen, &SDLPositionMonde);
        SDL_Flip(smain->screen);
        if(!add_timer(&frame))
            SDL_Delay(frame.tempsPre+frame.intervalle-SDL_GetTicks());
    }
    //FREE VAR
    SDL_FreeSurface(Background);
    SDL_FreeSurface(Texte);
    free(Titre);
    fprintf(stderr,"//\t\tFIN ECRAN_MAP\n");
    return ret;
}

int switchmonde(int sm, int mondemax, int* mondeact, Srect *posi, Srect *rect)
{
    int mix = SCREEN_W/2 - rect->w/2, side;
    float facteur = ECRAN_MAP_SWITCHMONDE_INTERVALE*(1+(abs(posi->x - mix))/(SCREEN_W/4));
    if(posi->x < mix)
        side = LEFT;
    else if(posi->x > mix)
        side = RIGHT;
    else
        side = DOWN;
    switch(sm)
    {
        case 2 :case 1:
            posi->x -= facteur;
            if(posi->x < 0-rect->w)
            {
                if(sm == 2)
                {
                    rect->x = mondemax * rect->w;
                    *mondeact = mondemax;
                }
                else
                {
                    rect->x += rect->w;
                    *mondeact = *mondeact + 1;
                }
                posi->x = SCREEN_W;
            }
            else if(posi->x <= mix && side == RIGHT)
            {
                posi->x = mix;
                sm = 0;
            }
            break;
        case 0:
            break;
        case -1: case -2:
            posi->x += facteur;
            if(posi->x > SCREEN_W)
            {
                if(sm == -2)
                {
                    rect->x = 0;
                    *mondeact = 0;
                }
                else
                {
                    rect->x -= rect->w;
                    *mondeact = *mondeact - 1;
                }
                posi->x = 0-rect->w;
            }
            else if(posi->x >= mix && side == LEFT)
            {
                posi->x = mix;
                sm = 0;
            }
            break;
    }
    return sm;
}
