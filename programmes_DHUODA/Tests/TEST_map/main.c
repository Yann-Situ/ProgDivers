#ifdef __cplusplus
#error "Vous compilez en C++!"
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "fonctions.h"
#include "map.h"

int main(int argc, char *argv[])
{
    TTF_Init();
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    char Title[FILENAME_MAX+2] = "(", strparenthese[] = ")", calqueChar[30] = "", objetChar[30] = "";
    int Calque = BACK, x =0, y = 0, timerSELECTION_int = 0,
            BLclic_gauche = 0, BLclic_droite = 0, BLclic_mollette = 0, BLsave = 0;
    OBJF *smain = NULL;
    SDL_Surface *Texte = NULL, *CalqueTexte_1 = NULL, *CalqueTexte_2 = NULL, *ObjetTexte_1 = NULL, *ObjetTexte_2 = NULL,
            *vide = NULL, *tilePhysAffichage = NULL, *Edge_v = NULL, *Edge_h = NULL, *Fond = NULL;
    SDL_Rect PositionTitle,  PositionEDGE_v, PositionEDGE_h, PositionCalqueTexte, PositionObjetTexte, PositionFond, PositionMouse,
            scrollInterface, scrollToolbar, scrollMouse;

    SDL_Rect Souris,SourisMollette, Toolbar, Interface, TilesetRect, MapRect;

    SDL_Event event;
    Uint8 *Keystate = SDL_GetKeyState(NULL);
    Smap map;
    SDL_Color couleurNOIRE = {0,0,0}, couleurBLANCHE = {255,255,255};
    C_timer timerSCROLL = {0,25}, timerSELECTION_TILE = {0,120}, frames = {0, 16};
    C_timer timerTOUCHE_1 = {0,120}, timerCLIC_1 = {0,200};

    FMOD_CHANNEL **canal = NULL;
    FMOD_RESULT resultat;
    FMOD_BOOL fmodBOOL;

    fprintf(stderr,"\n/*\nCreation des VARIABLES fini\n*/\n");
    /*
    Creation des VARIABLES fini
    */

    {// SMAIN
    smain = malloc(sizeof(OBJF));
    smain->continuer = 1;
    FMOD_System_Create(&smain->systeme);// allocation de mem pour un objet systeme /!\ ATTENTION ! pointeur sur pointeur d'objet systeme
    resultat = FMOD_System_Init(smain->systeme, MAX_CHANNEL, FMOD_INIT_NORMAL, NULL);
    if (resultat != FMOD_OK)                            //init systeme
    {//if error
        fprintf(stderr,"FMOD error! (%d)\n", resultat);
        return 0;
    }

    smain->screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    checkerror(smain->screen, NULL,SDL);

    smain->policeTitle = TTF_OpenFont("babalu.ttf",POLICE_TITLE_TAILLE /*taille*/);
    if(smain->policeTitle == NULL)                                  //init police
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la policetitle de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    TTF_SetFontStyle(smain->policeTitle, TTF_STYLE_UNDERLINE);

    smain->police = TTF_OpenFont("impact.ttf",POLICE_TAILLE /*taille*/);
    if(smain->police == NULL)                                  //init police
    {//if error
        fprintf(stderr, "Erreur d'initialisation de la police de caractere : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    }

    Fond = IMG_Load("fond1.jpg");
    checkerror(Fond,NULL,IMG);
    SDL_SetColorKey(Fond, SDL_SRCCOLORKEY, SDL_MapRGB(Fond->format, 255, 100, 100));

    SDL_WM_SetCaption("TEST_map", NULL);
    map.NumeroNiv = -1;
    while(map.NumeroNiv < 0)
    {// écran de chargement

        if(menu(smain, NULL, NULL, Fond, "EDITEUR DE MAP", 0, 0, NULL) < 0)
        {
            freeOBJF(smain);
            SDL_FreeSurface(Fond);
            fprintf(stderr,"\n\nEXIT_SUCCESS");
            return EXIT_SUCCESS;
        }

        map.NumeroNiv = -1;
        if(chargerMap(&map,smain,-1) == QUITTER)
        {
            freeOBJF(smain);
            SDL_FreeSurface(Fond);
            fprintf(stderr,"\n\nEXIT_SUCCESS");
            return EXIT_SUCCESS;
        }
    }
    fprintf(stderr,"\t\t//niveau choisi!\n");

    {// IMG_LOAD
    Edge_h = IMG_Load("up_edge.bmp");
    checkerror(Edge_h,NULL,IMG);
    SDL_SetColorKey(Edge_h, SDL_SRCCOLORKEY, SDL_MapRGB(Edge_h->format, 255, 100, 100));

    Edge_v = IMG_Load("tool_edge.bmp");
    checkerror(Edge_v,NULL,IMG);
    SDL_SetColorKey(Edge_v, SDL_SRCCOLORKEY, SDL_MapRGB(Edge_v->format, 255, 100, 100));

    vide = IMG_Load("void.bmp");
    checkerror(vide,NULL,IMG);
    SDL_SetColorKey(vide, SDL_SRCCOLORKEY, SDL_MapRGB(vide->format, 255, 100, 100));

    tilePhysAffichage = IMG_Load("tilePhysAffichage.bmp");
    checkerror(tilePhysAffichage,NULL,IMG);
    SDL_SetColorKey(tilePhysAffichage, SDL_SRCCOLORKEY, SDL_MapRGB(tilePhysAffichage->format, 255, 100, 100));
    SDL_SetAlpha(tilePhysAffichage, SDL_SRCALPHA, 128);
    }
    fprintf(stderr,"IMG LOAD ok!\t");
    {// FMOD
    canal = malloc(MAX_CHANNEL * sizeof(FMOD_CHANNEL*));
    FMOD_System_GetChannel(smain->systeme, CHANEL_MUSIC+1, &canal[CHANEL_MUSIC]);// pour init un canal
    FMOD_Sound_SetLoopPoints(map.musique, map.musicInfo.Loopstart, FMOD_TIMEUNIT_MS, map.musicInfo.Loopend, FMOD_TIMEUNIT_MS);
    FMOD_Sound_SetLoopCount(map.musique, -1);// pour repeter la musique : on met le nombre de fois ou elle est repeté, si on met -1 cest l'infini
    FMOD_System_PlaySound(smain->systeme, FMOD_CHANNEL_REUSE/*ou FMOD_CHANNEL_FREE */ , map.musique, 0, &canal[CHANEL_MUSIC]);
    }
    fprintf(stderr,"FMOD ok!\t");
    {// RENDER
    strcat(Title, map.NomNiv);
    strcat(Title, strparenthese);
    Texte = TTF_RenderText_Shaded(smain->policeTitle,Title, couleurBLANCHE, couleurNOIRE);/*    TTF_RenderText_Shaded       TTF_RenderText_Solid    */

    sprintf(calqueChar,"%d",Calque);
    CalqueTexte_2 = TTF_RenderText_Shaded(smain->police, calqueChar, couleurBLANCHE, couleurNOIRE);
    CalqueTexte_1 = TTF_RenderText_Shaded(smain->police, "[calque :    ]", couleurBLANCHE, couleurNOIRE);

    sprintf(objetChar,"%d",map.ToolBar->ObjetSelect);
    ObjetTexte_2 = TTF_RenderText_Shaded(smain->police, objetChar, couleurBLANCHE, couleurNOIRE);
    ObjetTexte_1 = TTF_RenderText_Shaded(smain->police, "[objet  :            ]", couleurBLANCHE, couleurNOIRE);
    }
    fprintf(stderr,"RENDER ok!\t");
    {// RECT
    PositionTitle.x = SPACE_BEFORE_TEXT;
    PositionTitle.y = Edge_h->h/2 - Texte->h/2;
    PositionCalqueTexte.x = PositionTitle.x + Texte->w + SPACE_BEFORE_TEXT;             PositionCalqueTexte.y = PositionTitle.y;
    PositionObjetTexte.x = PositionCalqueTexte.x + CalqueTexte_1->w +SPACE_BEFORE_TEXT; PositionObjetTexte.y = PositionTitle.y;
    PositionEDGE_h.x = 0;                                                               PositionEDGE_h.y = 0;
    PositionEDGE_v.x = TOOLBAR_W;                                                       PositionEDGE_v.y = UP_EDGE;
    PositionFond.x = 0;                                                                 PositionFond.y = UP_EDGE;
    PositionMouse.x = PositionObjetTexte.x + ObjetTexte_1->w + SPACE_BEFORE_TEXT;       PositionMouse.y = PositionTitle.y;

    scrollInterface.x = 0;                                                              scrollInterface.y = 0;
    scrollInterface.w = INTERFACE_W;                                                    scrollInterface.h = INTERFACE_H;
    scrollToolbar.x = 0;                                                                scrollToolbar.y = 0;
    scrollToolbar.w = TOOLBAR_W;                                                        scrollToolbar.h = TOOLBAR_H;
    scrollMouse.x = 0;                                                                  scrollMouse.y = 0;
    scrollMouse.w = TILE_TAILLE;                                                        scrollMouse.h = TILE_TAILLE;

    Souris.h = 0; Souris.w = 0; Souris.x = event.button.x; Souris.y = event.button.y;
    Toolbar.h = TOOLBAR_H; Toolbar.w = TOOLBAR_W; Toolbar.x = 0; Toolbar.y = UP_EDGE;
    Interface.h = INTERFACE_H; Interface.w = INTERFACE_W; Interface.x = INTERFACE_X; Interface.y = INTERFACE_Y;
    SourisMollette.h = 0; SourisMollette.w = 0; SourisMollette.x = 0; SourisMollette.y = 0;

    TilesetRect.h = TILESET_H*TILE_TAILLE; TilesetRect.w = TILESET_W*TILE_TAILLE; TilesetRect.x = 0; TilesetRect.y = 0;
    MapRect.h = map.h_blocs*TILE_TAILLE; MapRect.w = map.w_blocs*TILE_TAILLE; MapRect.x = 0; MapRect.y = 0;
    }
    fprintf(stderr,"RECT ok!\t");
    fprintf(stderr,"/*\nFIN DES INITIALISATION\n*/\n");
    /*
    FIN DES INITIALISATION
    */
    while(SDL_PollEvent(&smain->event_poubelle));
    event.type = SDL_ADDEVENT;
    if(event.type == SDL_QUIT)
        fprintf(stderr,"/!|quit/!|\n");

    while(smain->continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case(SDL_QUIT):
                fprintf(stderr,"\t\t\t\tQUIT\n");
                smain->continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    default:break;
                }
            break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_s: case SDLK_w: case SDLK_d: case SDLK_a:
                        if(!Keystate[SDLK_s] && !Keystate[SDLK_w] &&!Keystate[SDLK_d] &&!Keystate[SDLK_a])
                        {
                            timerSELECTION_TILE.intervalle = 1;
                            timerSELECTION_int = 0;
                        }
                    break;
                    case SDLK_LCTRL:
                        BLsave = 0;
                    break;
                    default:break;
                }
            break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        BLclic_gauche = 1;
                        if(add_timer(&timerCLIC_1))
                        {
                            if(colision(Souris,Interface))
                            {
                                if(Calque == PHYSIQUE)
                                {
                                    if(Keystate[SDLK_LCTRL])
                                        mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 1);
                                    else if(add_timer(&timerCLIC_1))
                                        mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 0);
                                }
                                else
                                {
                                    if(Keystate[SDLK_LCTRL])
                                        mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 1);
                                    else
                                        mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 0);
                                }
                            }
                            else if(colision(Souris,Toolbar))
                            {
                                if(Keystate[SDLK_LCTRL])
                                    mouseClic(&map, event, scrollToolbar, CLIC_GAUCHE, Calque,1, 1);
                                else
                                    mouseClic(&map, event, scrollToolbar, CLIC_GAUCHE, Calque,1, 0);
                                x = map.ToolBar->ObjetSelect % TILESET_W;
                                y = map.ToolBar->ObjetSelect / TILESET_W;

                                sprintf(objetChar,"%d",map.ToolBar->ObjetSelect);
                                ObjetTexte_2 = TTF_RenderText_Shaded(smain->police,objetChar, couleurBLANCHE, couleurNOIRE);
                            }
                        }
                    break;
                    case SDL_BUTTON_RIGHT:
                        BLclic_droite = 1;
                        if(add_timer(&timerCLIC_1))
                        {
                            if(colision(Souris,Interface))
                            {
                                if(Keystate[SDLK_LCTRL])
                                    mouseClic(&map, event, scrollInterface, CLIC_DROIT, Calque,0, 1);
                                else
                                    mouseClic(&map, event, scrollInterface, CLIC_DROIT, Calque,0, 0);
                            }
                            else if(colision(Souris,Toolbar))
                            {
                                if(Keystate[SDLK_LCTRL])
                                    mouseClic(&map, event, scrollToolbar, CLIC_DROIT, Calque,1, 1);
                                else
                                    mouseClic(&map, event, scrollToolbar, CLIC_DROIT, Calque,1, 0);
                            }
                        }
                    break;

                    case SDL_BUTTON_WHEELDOWN:
                        Calque--;
                        if(Calque < 0)
                            Calque = 0;
                        sprintf(calqueChar,"%d",Calque);
                        CalqueTexte_2 = TTF_RenderText_Shaded(smain->police,calqueChar, couleurBLANCHE, couleurNOIRE);
                        checkerror(CalqueTexte_2,NULL,SDL);
                    break;
                    case SDL_BUTTON_WHEELUP:
                        Calque++;
                        if(Calque > 3)
                            Calque = 3;
                        sprintf(calqueChar,"%d",Calque);
                        CalqueTexte_2 = TTF_RenderText_Shaded(smain->police,calqueChar, couleurBLANCHE, couleurNOIRE);
                        checkerror(CalqueTexte_2,NULL,SDL);
                    break;
                    case SDL_BUTTON_MIDDLE:
                        BLclic_mollette = 1;
                        SourisMollette.x = event.button.x; SourisMollette.y = event.button.y;
                    break;
                    default:break;
                }
            break;
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button)
                {
                    case SDL_BUTTON_RIGHT:
                        BLclic_droite = 0;
                    break;
                    case SDL_BUTTON_LEFT:
                        BLclic_gauche = 0;
                    break;
                    case SDL_BUTTON_MIDDLE:
                        BLclic_mollette = 0;
                        coincage(&scrollInterface, MapRect,0);
                        coincage(&scrollToolbar, TilesetRect,0);
                    break;
                    default:break;
                }
            break;
            case SDL_MOUSEMOTION:
                Souris.x = event.button.x; Souris.y = event.button.y;
                if(BLclic_gauche)
                {
                    if(colision(Souris,Interface))
                    {
                        if(Calque == PHYSIQUE)
                        {
                            if(Keystate[SDLK_LCTRL])
                                mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 1);
                            else if(add_timer(&timerCLIC_1))
                                mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 0);
                        }
                        else
                        {
                            if(Keystate[SDLK_LCTRL])
                                mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 1);
                            else
                                mouseClic(&map, event, scrollInterface, CLIC_GAUCHE, Calque,0, 0);
                        }
                    }
                    else if(colision(Souris,Toolbar))
                    {
                        if(Keystate[SDLK_LCTRL])
                                mouseClic(&map, event, scrollToolbar, CLIC_GAUCHE, Calque,1, 1);
                            else
                                mouseClic(&map, event, scrollToolbar, CLIC_GAUCHE, Calque,1, 0);
                            x = map.ToolBar->ObjetSelect % TILESET_W;
                            y = map.ToolBar->ObjetSelect / TILESET_W;

                            sprintf(objetChar,"%d",map.ToolBar->ObjetSelect);
                            ObjetTexte_2 = TTF_RenderText_Shaded(smain->police,objetChar, couleurBLANCHE, couleurNOIRE);
                    }
                }
                if(BLclic_droite)
                {
                    if(colision(Souris,Interface))
                    {
                        if(Keystate[SDLK_LCTRL])
                            mouseClic(&map, event, scrollInterface, CLIC_DROIT, Calque,0, 1);
                        else
                            mouseClic(&map, event, scrollInterface, CLIC_DROIT, Calque,0, 0);
                    }
                    else if(colision(Souris,Toolbar))
                    {
                        if(Keystate[SDLK_LCTRL])
                            mouseClic(&map, event, scrollToolbar, CLIC_DROIT, Calque,1, 1);
                        else
                            mouseClic(&map, event, scrollToolbar, CLIC_DROIT, Calque,1, 0);
                    }
                }
                if(BLclic_mollette)
                {
                    if(colision(SourisMollette,Interface))
                    {
                        switch(colision(MapRect,scrollInterface))
                        {
                            case 5:
                                mouseClicMollette(&map, SourisMollette, Souris, &scrollInterface);
                                coincage(&MapRect, scrollInterface,0);
                                //fprintf(stderr,"5\n");
                            break;
                            case 4:
                                mouseClicMollette(&map, SourisMollette, Souris, &scrollInterface);
                                coincage(&scrollInterface, MapRect,1);
                                //fprintf(stderr,"4\n");
                            break;
                            case 3:
                                mouseClicMollette(&map, SourisMollette, Souris, &scrollInterface);
                                coincage(&scrollInterface, MapRect,2);
                                //fprintf(stderr,"3\n");
                            break;
                            case 1:
                            break;
                            default:
                                mouseClicMollette(&map, SourisMollette, Souris, &scrollInterface);
                                coincage(&scrollInterface, MapRect,0);
                                //fprintf(stderr,"0;1;2\n");
                            break;
                        }
                        fprintf(stderr,"%d;%d\t%d;%d\n", scrollInterface.x, scrollInterface.y, MapRect.x, MapRect.y);
                    }
                    else if(colision(SourisMollette,Toolbar))
                    {
                        mouseClicMollette(&map, SourisMollette, Souris, &scrollToolbar);
                        coincage(&scrollToolbar, TilesetRect,0);
                    }
                }
            break;
            default:break;
        }

        if(Keystate[SDLK_s] && Keystate[SDLK_LCTRL])
        {
            if(!BLsave)
            {
                sauvegarderMap(&map, smain, map.NumeroNiv);
                if(smain->continuer == 0)
                    break;
                BLsave = 1;
            }
        }
        else if(Keystate[SDLK_s] || Keystate[SDLK_w] ||Keystate[SDLK_d] ||Keystate[SDLK_a] )
        {
            if(add_timer(&timerSELECTION_TILE))
            {// Selection mise a jour avec les touches
                if(timerSELECTION_int == 1)
                    timerSELECTION_TILE.intervalle = 45;
                if(timerSELECTION_int == 0)
                {
                    timerSELECTION_int = 1;
                    timerSELECTION_TILE.intervalle = 120;
                }

                if(Keystate[SDLK_s])
                    selectMiseAJour(&map,DOWN);
                if(Keystate[SDLK_w])
                    selectMiseAJour(&map,UP);
                if(Keystate[SDLK_d])
                    selectMiseAJour(&map,RIGHT);
                if(Keystate[SDLK_a])
                    selectMiseAJour(&map,LEFT);
                x = map.ToolBar->ObjetSelect % TILESET_W;
                y = map.ToolBar->ObjetSelect / TILESET_W;

                sprintf(objetChar,"%d",map.ToolBar->ObjetSelect);
                ObjetTexte_2 = TTF_RenderText_Shaded(smain->police,objetChar, couleurBLANCHE, couleurNOIRE);

                if(scrollToolbar.x > x*TILE_TAILLE)
                    scrollToolbar.x = x*TILE_TAILLE;

                if(scrollToolbar.x + scrollToolbar.w < (x+1)*TILE_TAILLE)
                    scrollToolbar.x = (x+1)*TILE_TAILLE - scrollToolbar.w;

                if(scrollToolbar.y > y*TILE_TAILLE)
                    scrollToolbar.y = y*TILE_TAILLE;

                if(scrollToolbar.y + scrollToolbar.h < (y+1)*TILE_TAILLE)
                    scrollToolbar.y = (y+1)*TILE_TAILLE - scrollToolbar.h;
            }
        }
        else if(Keystate[SDLK_F6])
        {
            if(add_timer(&timerTOUCHE_1))
            {
                FMOD_Channel_GetPaused( canal[CHANEL_MUSIC] , &fmodBOOL );
                if(fmodBOOL == 1)
                    FMOD_Channel_SetPaused( canal[CHANEL_MUSIC] , 0 );
                else
                    FMOD_Channel_SetPaused( canal[CHANEL_MUSIC] , 1 );
            }
        }
        else if(Keystate[SDLK_RSHIFT])
        {
            if(chargerMap(&map,smain,-1) > 0)
            {
                {//mise en place du Title
                Title[0] = '(';
                Title[1] = '\0';
                strparenthese[0] = ')';
                strparenthese[1] = '\0';
                strcat(Title, map.NomNiv);
                strcat(Title, strparenthese);
                Texte = TTF_RenderText_Shaded(smain->policeTitle,Title, couleurBLANCHE, couleurNOIRE);/*    TTF_RenderText_Shaded       TTF_RenderText_Solid    */
                }

                PositionCalqueTexte.x = PositionTitle.x + Texte->w + SPACE_BEFORE_TEXT;
                PositionObjetTexte.x = PositionCalqueTexte.x + CalqueTexte_1->w +SPACE_BEFORE_TEXT;
                scrollInterface.x = 0;                                                              scrollInterface.y = 0;
                scrollInterface.w = INTERFACE_W;                                                    scrollInterface.h = INTERFACE_H;
                TilesetRect.h = TILESET_H*TILE_TAILLE; TilesetRect.w = TILESET_W*TILE_TAILLE; TilesetRect.x = 0; TilesetRect.y = 0;
                MapRect.h = map.h_blocs*TILE_TAILLE; MapRect.w = map.w_blocs*TILE_TAILLE; MapRect.x = 0; MapRect.y = 0;
                FMOD_System_PlaySound(smain->systeme, FMOD_CHANNEL_REUSE , map.musique, 0, &canal[CHANEL_MUSIC]);
            }
            if(smain->continuer == 0)
                break;
            while(SDL_PollEvent(&smain->event_poubelle));
        }

        SDL_FillRect(smain->screen, NULL, SDL_MapRGB(smain->screen->format, 0, 0, 0));
        SDL_BlitSurface(Fond,NULL,smain->screen,&PositionFond);

        printINTERFACE(smain, map, scrollInterface, vide, tilePhysAffichage, Calque);
        printTOOLBAR(smain, map, scrollToolbar);

        while(PositionEDGE_h.x < SCREEN_W)
        {//print edge horizontal
            SDL_BlitSurface(Edge_h,NULL,smain->screen, &PositionEDGE_h);
            PositionEDGE_h.x += Edge_h->w;
        }
        PositionEDGE_h.x = 0;
        while(PositionEDGE_v.y < SCREEN_H)
        {// print edge vertical
            SDL_BlitSurface(Edge_v,NULL,smain->screen, &PositionEDGE_v);
            PositionEDGE_v.y += Edge_v->h;
        }
        PositionEDGE_v.y = UP_EDGE;

        SDL_BlitSurface(CalqueTexte_1,NULL,smain->screen,&PositionCalqueTexte);
        PositionCalqueTexte.x += 118;
        SDL_BlitSurface(CalqueTexte_2,NULL,smain->screen,&PositionCalqueTexte);
        PositionCalqueTexte.x = PositionTitle.x + Texte->w + SPACE_BEFORE_TEXT;

        SDL_BlitSurface(ObjetTexte_1,NULL,smain->screen,&PositionObjetTexte);
        PositionObjetTexte.x += 106;
        SDL_BlitSurface(ObjetTexte_2,NULL,smain->screen,&PositionObjetTexte);
        PositionObjetTexte.x = PositionCalqueTexte.x + CalqueTexte_1->w + SPACE_BEFORE_TEXT;

        SDL_BlitSurface(Texte,NULL,smain->screen,&PositionTitle);

        scrollMouse.x = (map.ToolBar->ObjetSelect%TILESET_W) *TILE_TAILLE;
        scrollMouse.y = (map.ToolBar->ObjetSelect/TILESET_W) *TILE_TAILLE;

        SDL_BlitSurface(map.Tileset, &scrollMouse, smain->screen, &PositionMouse);

        SDL_Flip(smain->screen);

        if(!add_timer(&frames))
            SDL_Delay(frames.intervalle - (SDL_GetTicks() + frames.tempsPre));
    }

    /*
    FIN DU PROGRAMME                              fermeture+liberation de la memoire
    */

    libererMAP(&map);

    SDL_FreeSurface(Texte);
    SDL_FreeSurface(CalqueTexte_1);
    SDL_FreeSurface(CalqueTexte_2);
    SDL_FreeSurface(ObjetTexte_1);
    SDL_FreeSurface(ObjetTexte_2);
    SDL_FreeSurface(vide);
    SDL_FreeSurface(tilePhysAffichage);
    SDL_FreeSurface(Edge_v);
    SDL_FreeSurface(Edge_h);
    SDL_FreeSurface(Fond);

    freeOBJF(smain);
    SDL_Quit();
    TTF_Quit();
    fprintf(stderr,"FIN DU PROGRAMME");
    return EXIT_SUCCESS;
}

//C:\Program Files (x86)\CodeBlocks\SDL-1.2.15\lib\SDL_image.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\SDL_ttf.lib
//C:\Program Files (x86)\CodeBlocks\MinGW\mingw32\lib\libfmodex.a
//SDL_SetColorKey(SURFACE, SDL_SRCCOLORKEY, SDL_MapRGB(SURFACE->format, R, V, B));

