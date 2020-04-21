/*
map.c
___________
    contient diverses fonctions comme les chargements et les sauvegarde de l'éditeur de map
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "map.h"
#include "fonctions.h"
#include "evenement.h"


int mettreNomdsTableau(Sstring *levels, int NbLvL, FILE *fichier);
int chargerMap(Smap *map, OBJF *smain, int niv);
int sauvegarderMap(Smap *map, OBJF *smain, int niv);
int menuNEW(OBJF *smain, Smap *map, FILE *fichier);
int ecrireMap(Smap *map, FILE *fichier);
int lireMap(Smap *map, FILE *fichier);

Stoolbar initPhysique(Stoolbar TB);
int libererMAP(Smap *map);

void printTOOLBAR(OBJF *smain, Smap map, SDL_Rect scroll);
void printINTERFACE(OBJF *smain, Smap map, SDL_Rect scroll, SDL_Surface *vide, SDL_Surface *tilePhysAffichage, int calque);

int mettreNomdsTableau(Sstring *levels, int NbLvL, FILE *fichier)
{
    char caracAct = '0';
    int NumMotAct = 1;
    rewind(fichier);
    while(caracAct != EOF && NumMotAct < NbLvL)
    {
        caracAct = fgetc(fichier);
        if(caracAct == '#')
        {
            if(fgets(levels[NumMotAct].Nom,FILENAME_MAX,fichier) == NULL)
                fprintf(stderr,"error%d fgets numMot : %d\n",errno,NumMotAct);

            levels[NumMotAct].Nom[strlen(levels[NumMotAct].Nom)-1] = '\0';
            NumMotAct++;
        }
    }
    if(caracAct == EOF)
    {
        fprintf(stderr,"\t\tcaracAct == EOF /!| tous les niveaux n'ont pas était trouvé!\n");
        return -1;// -1 si on a pas trouvé tout les niveaux
    }
    return 1;
}

int chargerMap(Smap *map, OBJF *smain, int niv)
{
    // preparation initialisation pour la fonction ------>
    fprintf(stderr,"\t*debut de chargermap*\n\t..........\n");
    FILE *fichier = NULL;
    int quitter = 0, Continuer = 1;
    int NBniv = 0, i = 0, choixACT = 0;
    char caracACT;
    FMOD_RESULT resultat;

    fichier = fopen("niveau.txt","r");
    checkerror(NULL,fichier,FICHIERS);


    rewind(fichier);
    while(caracACT != EOF)
    {
        caracACT = fgetc(fichier);
        if(caracACT == '#')
            NBniv ++;
    }

    while(Continuer)
    {// boucle de chargement de la map avec toutes les options

        if(niv == -1)
        {// si il faut choisir le niveau :
            SDL_Surface *fond = NULL;
            fond = IMG_Load("fond.bmp");
            checkerror(fond,NULL,IMG);
            fprintf(stderr,"nbniv : %d\n",NBniv);
            Sstring *levels = malloc((NBniv+1) * sizeof(Sstring));/* ou malloc(L * sizeof *A); */

            if(!mettreNomdsTableau(levels,NBniv+1,fichier))
                    fprintf(stderr,"error mettreNomdsTableau\n");
            for(i=1; i<NBniv+1;i++)
                fprintf(stderr,"texte[%d] : %s\n",i,levels[i].Nom);
            {// ----> NEW
                levels[0].Nom[0] = 'N';
                levels[0].Nom[1] = 'E';
                levels[0].Nom[2] = 'W';
                levels[0].Nom[3] = '\0';
            }

            Sanim animSelect;
            animSelect = chargerAnim(animSelect, -1,11,0,0,SELECT_TAILLE,SELECT_TAILLE,0,0,35,"selection menu.bmp");
            SDL_SetColorKey(animSelect.sprites, SDL_SRCCOLORKEY, SDL_MapRGB(animSelect.sprites->format, 255, 100, 100));

            choixACT = menu(smain, &animSelect, levels, fond,"Charger Map", NBniv+1, 0, &quitter);

            SDL_FreeSurface(fond);
            SDL_FreeSurface(animSelect.sprites);
            free(levels);
            if(smain->continuer == 0)
            {
                fprintf(stderr,"quit charger map !\n");
                choixACT = -1;
            }
            if(choixACT == -1 || quitter == 1)
                Continuer = 0;
        }
        else// sinon...
            choixACT = niv + 1;// car 0 correspond à NEW

        if(!quitter && choixACT > 0)
        {// mise en place du chargement de la map
            char nomProvi[50] = {0};
            rewind(fichier);
            i=0;
            while(i < choixACT)
            {
                caracACT = fgetc(fichier);
                if(caracACT == '#')
                    {
                        i++;
                    }
            }
            map->NumeroNiv = i-1;
            lireMap(map,fichier);

            resultat = FMOD_System_CreateSound(smain->systeme, map->musicInfo.filename, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &(map->musique));
            if(resultat != FMOD_OK)
            {//if error
                fprintf(stderr,"FMOD error! (%d)\n", resultat);
                return 0;
            }
            Continuer = 0;
            fprintf(stderr,"\t\t//map mis en place\n");
        }

        else if(choixACT == 0)
        {// mise en place d'une nouvelle map
            if(menuNEW(smain, map, fichier) != -1)
            {
                Continuer = 0;
                choixACT = map->NumeroNiv+1;
            }
            else
                choixACT = -1;
            if(smain->continuer == 0)
            {
                fprintf(stderr,"quit charger map !\n");
                Continuer = 0;
                choixACT = -1;
            }
        }
    }

    if(smain->continuer != 0 && choixACT != -1)
    {// remplissage de toolbar   ----->
        map->ToolBar = malloc(sizeof(Stoolbar));

        map->ToolBar->ObjetSelect = 0;
        map->ToolBar->RectSelect.h = TILE_TAILLE;
        map->ToolBar->RectSelect.w = TILE_TAILLE;
        map->ToolBar->RectSelect.x = 0;
        map->ToolBar->RectSelect.y = 0;

        map->ToolBar->TabTilesetPhysique = malloc(sizeof(int*) * TILESET_H);
        for (i = 0 ; i < TILESET_H ; i++)
            map->ToolBar->TabTilesetPhysique[i] = malloc(sizeof(int) * TILESET_W);

        map->ToolBar->Select = IMG_Load("selection.bmp");
        checkerror(map->ToolBar->Select,NULL,IMG);
        SDL_SetColorKey(map->ToolBar->Select, SDL_SRCCOLORKEY, SDL_MapRGB(map->ToolBar->Select->format, 255, 100, 100));
        map->ToolBar->TilesetPhysique = IMG_Load("tileset mapp.bmp");
        checkerror(map->ToolBar->TilesetPhysique,NULL,IMG);

        SDL_LockSurface(map->ToolBar->TilesetPhysique);
        *map->ToolBar = initPhysique(*map->ToolBar);
        SDL_UnlockSurface(map->ToolBar->TilesetPhysique);
    }

    /*
    */

    fclose(fichier);
    fprintf(stderr,"niveau choisi : %d - %s\n\t*fin de chargermap*",choixACT,map->NomNiv);
    if(smain->continuer == 0)
    {
        fprintf(stderr,"\t\t\t\treturn QUITTER\n\t..........\n");
        return QUITTER;
    }
    if(choixACT == -1)
    {
        fprintf(stderr,"\t\t\t\treturn -1\n\t..........\n");
        return -1;
    }
    fprintf(stderr,"\t\t\t\treturn 1\n\t..........\n");
    return 1;
}

int sauvegarderMap(Smap *map, OBJF *smain, int niv)
{
    // preparation initialisation pour la fonction ------>
    fprintf(stderr,"\t*debut de sauvergarderMap*\n\t..........\n");
    FILE *fichier = NULL;
    int NBniv = 0, i = 0, choixACT = 0;
    char caracACT;

    fichier = fopen("niveau.txt","r+");
    checkerror(NULL,fichier,FICHIERS);

    rewind(fichier);
    caracACT = 'A';
    while(caracACT != EOF)
    {
        caracACT = fgetc(fichier);
        if(caracACT == '#')
        {
            NBniv ++;
        }
    }
    fprintf(stderr,"nbniv : %d ........ niv : %d\n", NBniv, niv);

    if(niv != -1)
    {// Si enregistrer normal
        if(niv+1>NBniv)
        {// si nouveau niveau
            fprintf(stderr,"niv new\n");
            rewind(fichier);
            caracACT = 'A';
            while(caracACT != '!')
            {
                caracACT = fgetc(fichier);
                if(caracACT == EOF)
                {
                    fprintf(stderr,"end of file !\n");
                    return 0;
                }
            }
            fseek(fichier, -1, SEEK_CUR);
            fputc('#',fichier);
            ecrireMap(map, fichier);
            fputs("\n!\nfin",fichier);
        }
        else
        {// sinon niveau existant
            fprintf(stderr,"niv exist\n");
            rewind(fichier);
            caracACT = 'A';
            while(i < niv+1)
            {// placement #
                caracACT = fgetc(fichier);
                if(caracACT == '#')
                    i ++;
                if(caracACT == EOF)
                {
                    fprintf(stderr,"end of file !\n");
                    return 0;
                }
            }
            fseek(fichier, -1, SEEK_CUR);
            fputc('#',fichier);
            ecrireMap(map, fichier);
        }
    }
    else
    {// sinon enregistrer sous

    }

    fclose(fichier);
    fprintf(stderr,"niveau choisi : %d - %s\n\t*fin de sauvegarderMap*\n\t..........\n",niv+1,map->NomNiv);
    return 1;
}

int ecrireMap(Smap *map, FILE *fichier)
{
    char chaine[50];
    int y = 0, x = 0;

    fputs(map->NomNiv,fichier);
    fputc('\n',fichier);
    sprintf(chaine,"%d",map->w_blocs);
    fputs(chaine,fichier);
    fputc('\n',fichier);
    sprintf(chaine,"%d",map->h_blocs);
    fputs(chaine,fichier);
    fputc('\n',fichier);
    fputs(map->tilesetname,fichier);
    fputc('\n',fichier);
    fputs(map->musicInfo.filename,fichier);
    fputc('\n',fichier);
    sprintf(chaine,"%d",map->musicInfo.Loopstart);
    fputs(chaine,fichier);
    fputc('\n',fichier);
    sprintf(chaine,"%d",map->musicInfo.Loopend);
    fputs(chaine,fichier);
    for(y = 0; y < map->h_blocs; y++)
    {// mapBACK
        fputc('\n',fichier);
        for(x = 0; x < map->w_blocs; x++)
        {
            sprintf(chaine,"%d",map->Cases[y][x].back);
            fputs(chaine,fichier);
            fputc(' ',fichier);
        }
    }
    fprintf(fichier,"E");
    for(y = 0; y < map->h_blocs; y++)
    {// mapMID
        fputc('\n',fichier);
        for(x = 0; x < map->w_blocs; x++)
        {
            sprintf(chaine,"%d",map->Cases[y][x].mid);
            fputs(chaine,fichier);
            fputc(' ',fichier);
        }
    }
    fprintf(fichier,"E");
    for(y = 0; y < map->h_blocs; y++)
    {// mapFRONT
        fputc('\n',fichier);
        for(x = 0; x < map->w_blocs; x++)
        {
            sprintf(chaine,"%d",map->Cases[y][x].front);
            fputs(chaine,fichier);
            fputc(' ',fichier);
        }
    }
    fprintf(fichier,"E\n");
    for(y = 0; y < map->h_blocs; y++)
    {// map Phys
        for(x = 0; x < map->w_blocs; x++)
        {
            sprintf(chaine,"%d",map->Cases[y][x].phys);
            fputs(chaine,fichier);
        }
    }
    fputc('\n',fichier);

    return 1;
}

int lireMap(Smap *map, FILE *fichier)
{
    char chaine[50] = {0}, caracACT = '0';
    int i = 0;

    fgets(map->NomNiv, 20, fichier);
    map->NomNiv[strlen(map->NomNiv)-1] = '\0';
    fgets(chaine,50,fichier);
    map->w_blocs = atoi(chaine);
    fgets(chaine,50,fichier);
    map->h_blocs = atoi(chaine);

    fgets(map->tilesetname, FILENAME_MAX, fichier);
    map->tilesetname[strlen(map->tilesetname)-1] = '\0';
    map->Tileset = IMG_Load(map->tilesetname);
    checkerror(map->Tileset,NULL,IMG);
    SDL_SetColorKey(map->Tileset, SDL_SRCCOLORKEY, SDL_MapRGB(map->Tileset->format, 255, 100, 100));

    fgets(map->musicInfo.filename, FILENAME_MAX, fichier);
    map->musicInfo.filename[strlen(map->musicInfo.filename)-1] = '\0';

    fgets(chaine,50,fichier);
    map->musicInfo.Loopstart = atoi(chaine);
    fgets(chaine,50,fichier);
    map->musicInfo.Loopend = atoi(chaine);

    {// remplissage des tableaux ----->
    map->Cases = malloc(map->h_blocs * sizeof(Scase*));
    for (i = 0 ; i < map->h_blocs ; i++)
        map->Cases[i] = malloc(sizeof(Scase) * map->w_blocs);
    char chn5[10] = "";
    int compteurChar = 0, x = 0, y = 0;

    while(caracACT != 'E')
    {// remplissage mapBACK
        caracACT = fgetc(fichier);
        if(caracACT == ' ')
        {
            chn5[compteurChar] = '\0';
            map->Cases[y][x].back = atoi(chn5);
            compteurChar = 0;
            x++;
        }
        else if(caracACT == '\n')
        {
            chn5[compteurChar] = '\0';
            map->Cases[y][x].back = atoi(chn5);
            compteurChar = 0;
            x = 0;
            y++;
        }
        else
        {
            chn5[compteurChar] = caracACT;
            compteurChar++;
        }
    }
    fprintf(stderr,"E\n");
    compteurChar = 0;
    chn5[0] = '\0';
    x = 0;
    y = 0;
    caracACT = fgetc(fichier);
    caracACT = '0';
    while(caracACT != 'E')
    {// remplissage mapMID
        caracACT = fgetc(fichier);
        if(caracACT == ' ')
        {
            chn5[compteurChar] = '\0';
            map->Cases[y][x].mid = atoi(chn5);
            compteurChar = 0;
            x++;
        }
        else if(caracACT == '\n')
        {
            chn5[compteurChar] = '\0';
            map->Cases[y][x].mid = atoi(chn5);
            compteurChar = 0;
            x = 0;
            y++;
        }
        else
        {
            chn5[compteurChar] = caracACT;
            compteurChar++;
        }
    }
    fprintf(stderr,"E\n");
    compteurChar = 0;
    chn5[0] = '\0';
    x = 0;
    y = 0;
    caracACT = fgetc(fichier);
    caracACT = '0';
    while(caracACT != 'E')
    {// remplissage mapFORE
        caracACT = fgetc(fichier);
        if(caracACT == ' ')
        {
            chn5[compteurChar] = '\0';
            map->Cases[y][x].front = atoi(chn5);
            compteurChar = 0;
            x++;
        }
        else if(caracACT == '\n')
        {
            chn5[compteurChar] = '\0';
            map->Cases[y][x].front = atoi(chn5);
            compteurChar = 0;
            x = 0;
            y++;
        }
        else
        {
            chn5[compteurChar] = caracACT;
            compteurChar++;
        }
    }
    fprintf(stderr,"E\n");
    compteurChar = 0;
    chn5[0] = '\0';
    x = 0;
    y = 0;
    caracACT = fgetc(fichier);
    caracACT = '0';
    while(caracACT != '\n')
    {// remplissage mapPHYSIQUE
        caracACT = fgetc(fichier);
        if(caracACT == '\n')
            break;
        map->Cases[y][x].phys = caracACT - '0';
        fprintf(stderr,"%d.",map->Cases[y][x].phys);
        x++;
        if(x >= map->w_blocs)
        {
            x = 0;
            y++;
            fprintf(stderr,"\n");
        }
    }
    }//fin des tableaux
    fprintf(stderr,"\t\t*fin lireMAP()*\n");
    return 1;
}

int menuNEW(OBJF *smain, Smap *map, FILE *fichier)
{
    fprintf(stderr,"*debut menuNEW*\n");
    FILE *file_menus;
    file_menus = fopen("menus.txt","r");
    checkerror(NULL,file_menus,FICHIERS);
    char caracACT = '\0', chainePRO[50];
    int i = 0, j =0, NBtilesets = 0, NumTileset = 0, NBmusiques = 0, NumMusique = 0, Continuer = 1, quitter = 0, n = 0, numniv = 0, w_blocs = 0, h_blocs = 0;
    int QUIT = 0;
    int defCHOIX[4] = {0}, defCURSEUR[4] = {0};
    Sstring *stringTileset, *stringMusique;
    Sanim animSelect = chargerAnim(animSelect, -1,11,0,0,SELECT_TAILLE,SELECT_TAILLE,0,0,35,"selection menu.bmp");
    SDL_SetColorKey(animSelect.sprites, SDL_SRCCOLORKEY, SDL_MapRGB(animSelect.sprites->format, 255, 100, 100));
    SDL_Surface *fond = IMG_Load("fond.bmp");
    checkerror(fond,NULL,IMG);

    while(Continuer)
    {

        rewind(fichier);
        i=0;
        caracACT = '0';
        while(caracACT != EOF)
        {
            caracACT = fgetc(fichier);
            if(caracACT == '#')
                {
                    numniv++;
                }
        }
        n=0;

        if(!defCHOIX[n])
        {//w_blocs
            w_blocs = menuNB(smain, &animSelect, fond, "Blocs de longueur", 10, 200, 0,&quitter);
            if(smain->continuer == 0)
                break;
            if(w_blocs == -1)
            {
                defCHOIX[n] = 0;
                QUIT = 1;
                Continuer = 0;
            }
            else
                defCHOIX[n] = 1;
        }
        n++;
        while(SDL_PollEvent(&smain->event_poubelle));
        if(!defCHOIX[n])
        {//h_blocs
            h_blocs = menuNB(smain, &animSelect, fond, "Blocs de hauteur", 10, 200, 0,&quitter);
            if(smain->continuer == 0)
                break;
            if(h_blocs == -1)
            {
                defCHOIX[n-1] = 0;
                continue;
            }
            else
                defCHOIX[n] = 1;
        }

        n++;
        if(!defCURSEUR[n])
        {// METTRE TILESET DANS TABLEAU STRING
            i=0;
            while(i < TILESETS)
            {// positionnement du curseur
                caracACT = fgetc(file_menus);
                if(caracACT == '#')
                    i++;
            }
            fgets(chainePRO,50,file_menus);
            NBtilesets = atoi(chainePRO);
            stringTileset = malloc(NBtilesets * sizeof(Sstring));
            for(i=0; i<NBtilesets;i++)
            {
                fgets(stringTileset[i].Nom,FILENAME_MAX,file_menus);
                stringTileset[i].Nom[strlen(stringTileset[i].Nom)-1] = '\0';
            }
            defCURSEUR[n] = 1;
        }

        if(!defCHOIX[n])
        {// tileset
            NumTileset = menu(smain, &animSelect, stringTileset, fond,"Choix Tileset", NBtilesets, 0, &quitter);
            if(smain->continuer == 0)
                break;
            if(NumTileset == -1)
            {
                defCHOIX[n-1] = 0;
                continue;
            }
            else
                defCHOIX[n] = 1;
        }

        n++;
        if(!defCURSEUR[n])
        {// METTRE MUSIQUE DANS TABLEAU STRING
            rewind(file_menus);
            i=0;
            while(i < MUSIQUE)
            {// positionnement du curseur
                caracACT = fgetc(file_menus);
                if(caracACT == '#')
                    i++;
            }
            fgets(chainePRO,50,file_menus);
            NBmusiques = atoi(chainePRO);
            stringMusique = malloc(NBmusiques * sizeof(Sstring));
            for(i=0; i<NBmusiques;i++)
            {
                fgets(stringMusique[i].Nom,FILENAME_MAX,file_menus);
                stringMusique[i].Nom[strlen(stringMusique[i].Nom)-1] = '\0';
            }
            defCURSEUR[n] = 1;
        }

        if(!defCHOIX[n])
        {// musique
            NumMusique = menu(smain, &animSelect, stringMusique, fond,"Choix Musiques", NBmusiques, 0, &quitter);
            if(smain->continuer == 0)
                break;
            if(NumMusique == -1)
            {
                defCHOIX[n-1] = 0;
                continue;
            }
            else
                defCHOIX[n] = 1;
        }

        if(defCHOIX[0]&&defCHOIX[1]&&defCHOIX[2]&&defCHOIX[3])
            Continuer = 0;
    }


    if(smain->continuer != 0 && QUIT != 1)
    {// MISE EN PLACE DE LA MAP
        map->NumeroNiv = numniv;
        map->w_blocs = w_blocs;
        map->h_blocs = h_blocs;

        sprintf(map->NomNiv,"level%d",numniv);

        fprintf(stderr,"map->h:%d \n",map->h_blocs);
        fprintf(stderr,"map->w:%d \n",map->w_blocs);

        sprintf(map->tilesetname,"tileset map%d.bmp",NumTileset);
        fprintf(stderr,"map->tilesetname:%s \n",map->tilesetname);
        map->Tileset = IMG_Load(map->tilesetname);
        checkerror(map->Tileset,NULL,IMG);
        SDL_SetColorKey(map->Tileset, SDL_SRCCOLORKEY, SDL_MapRGB(map->Tileset->format, 255, 100, 100));

        sprintf(map->musicInfo.filename,"theme%d.mp3",NumMusique);
        fprintf(stderr,"map->musicInfo.filename:%s \n",map->musicInfo.filename);
        if(FMOD_System_CreateSound(smain->systeme, map->musicInfo.filename, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &(map->musique)) != FMOD_OK)
        {// if error
            fprintf(stderr,"error fmod! EXIT");
            exit(EXIT_FAILURE);
        }

        {// Mise en place des loop de la musique
                i=0;
                while(i < NumMusique+1)
                {// positionnement du curseur
                    caracACT = fgetc(file_menus);
                    if(caracACT == ';')
                        i++;
                }
                i=0;
                while(caracACT != ':')
                {
                    caracACT = fgetc(file_menus);
                    chainePRO[i] = caracACT;
                    i++;
                }
                chainePRO[i] = '\0';
                map->musicInfo.Loopstart = atoi(chainePRO);
                fgets(chainePRO,50,file_menus);
                chainePRO[strlen(chainePRO)-1] = '\0';
                map->musicInfo.Loopend = atoi(chainePRO);
            }

        fprintf(stderr,"map->musicInfo.Loopstart:%d \n",map->musicInfo.Loopstart);
        fprintf(stderr,"map->musicInfo.Loopend:%d \n",map->musicInfo.Loopend);

        map->Cases = malloc(map->h_blocs * sizeof(Scase*));
            for (i = 0 ; i < map->h_blocs ; i++)
                map->Cases[i] = malloc(sizeof(Scase) * map->w_blocs);
        for(i = 0; i<map->h_blocs; i++)
        {
            for(j = 0; j<map->w_blocs; j++)
            {
                map->Cases[i][j].back = 0;
                map->Cases[i][j].mid = 0;
                map->Cases[i][j].front = 0;
                map->Cases[i][j].phys = 0;
            }
        }

    }

    free(stringTileset);
    free(stringMusique);
    SDL_FreeSurface(fond);
    SDL_FreeSurface(animSelect.sprites);
    fprintf(stderr,"*fin menuNEW*\n");
    if(smain->continuer == 0)
    {
        fprintf(stderr,"quit menu new map !\n");
        return QUITTER;
    }
    if(QUIT)
        return -1;
    return 1;
}

Stoolbar initPhysique(Stoolbar TB)
{
    fprintf(stderr,"*debut de initphysique*\n");
    int y=0, x=0;
    Uint8 r=0,g=0,b=0;

    for(y=0; y < TILESET_H; y++)
    {
        for(x=0; x < TILESET_W; x++)
        {
            SDL_GetRGB(SDL_GetPixel32(TB.TilesetPhysique,x*TILE_TAILLE,y*TILE_TAILLE),TB.TilesetPhysique->format,&r,&g,&b);
            if(r==255 && g==255 && b==255)
                TB.TabTilesetPhysique[y][x] = VIDE;
            else if(r==0 && g==0 && b==0)
                TB.TabTilesetPhysique[y][x] = PLEIN;
            else if(r==255 && g==0 && b==0)
                TB.TabTilesetPhysique[y][x] = BAS;
            else if(r==0 && g==0 && b==255)
                TB.TabTilesetPhysique[y][x] = HAUT;
            else if(r==0 && g==255 && b==0)
                TB.TabTilesetPhysique[y][x] = DROITE;
            else if(r==128 && g==128 && b==128)
                TB.TabTilesetPhysique[y][x] = GAUCHE;
            else if(r==255 && g==255 && b==0)
                TB.TabTilesetPhysique[y][x] = BAS_DROITE;
            else if(r==255 && g==128 && b==128)
                TB.TabTilesetPhysique[y][x] = BAS_GAUCHE;
            else if(r==0 && g==255 && b==255)
                TB.TabTilesetPhysique[y][x] = HAUT_DROITE;
            else if(r==128 && g==128 && b==255)
                TB.TabTilesetPhysique[y][x] = HAUT_GAUCHE;
        }
        fprintf(stderr,".");
    }
    fprintf(stderr,"\n");
    fprintf(stderr,"*fin de initphysique*\n");
    return TB;
}

int libererMAP(Smap *map)
{
    fprintf(stderr,"*DEBUT libererMAP*\n");
    free(map->Cases);
    free(map->ToolBar->TabTilesetPhysique);

    SDL_FreeSurface(map->Tileset);
    SDL_FreeSurface(map->ToolBar->TilesetPhysique);
    SDL_FreeSurface(map->ToolBar->Select);

    free(map->ToolBar);

    FMOD_Sound_Release(map->musique);
    fprintf(stderr,"*FIN libererMAP*\n");
    return 1;
}

void printTOOLBAR(OBJF *smain, Smap map, SDL_Rect scroll)
{
    int x = 0, y = 0;
    SDL_Rect position, RectTileset;
    RectTileset.h = TOOLBAR_H;
    RectTileset.w = TOOLBAR_W;
    RectTileset.x = 0 + scroll.x;
    RectTileset.y = 0 + scroll.y;

    position.x = 0;
    position.y = UP_EDGE;

    SDL_BlitSurface(map.Tileset, &RectTileset, smain->screen, &position);

    RectTileset.h = TILE_TAILLE/2;
    RectTileset.w = TILE_TAILLE/2;
    RectTileset.x = 0;
    RectTileset.y = 0;

    x = map.ToolBar->ObjetSelect % TILESET_W;
    y = map.ToolBar->ObjetSelect / TILESET_W;

    if(scroll.x-1 < x*TILE_TAILLE && scroll.w+scroll.x > x*TILE_TAILLE && scroll.y-1 < y*TILE_TAILLE && scroll.h+scroll.y > y*TILE_TAILLE)
    {
        position.x = x*TILE_TAILLE - scroll.x;
        position.y = y*TILE_TAILLE + UP_EDGE - scroll.y;
        SDL_BlitSurface(map.ToolBar->Select, &RectTileset, smain->screen, &position);

        RectTileset.x += TILE_TAILLE/2;
        position.x += map.ToolBar->RectSelect.w - TILE_TAILLE/2;
        SDL_BlitSurface(map.ToolBar->Select, &RectTileset, smain->screen, &position);

        RectTileset.y += TILE_TAILLE/2;
        position.y += map.ToolBar->RectSelect.h - TILE_TAILLE/2;
        SDL_BlitSurface(map.ToolBar->Select, &RectTileset, smain->screen, &position);

        RectTileset.x -= TILE_TAILLE/2;
        position.x -= map.ToolBar->RectSelect.w - TILE_TAILLE/2;
        SDL_BlitSurface(map.ToolBar->Select, &RectTileset, smain->screen, &position);
    }
}

void printINTERFACE(OBJF *smain, Smap map, SDL_Rect scroll, SDL_Surface *vide, SDL_Surface *tilePhysAffichage, int calque)
{

    int maxi = 0, maxj = 0;
    unsigned int i=0, j=0, rectx=0, recty=0;
    SDL_Rect position, RectTileset;
    RectTileset.h = TILE_TAILLE;
    RectTileset.w = TILE_TAILLE;

    if(INTERFACE_H/TILE_TAILLE > map.h_blocs)
        maxi = map.h_blocs;
    else
        maxi = INTERFACE_H/TILE_TAILLE + 1;

    if(INTERFACE_W/TILE_TAILLE > map.w_blocs)
        maxj = map.w_blocs;
    else
        maxj = INTERFACE_W/TILE_TAILLE + 1;


    for(i = scroll.y/TILE_TAILLE ; i < maxi; i++)
    {
        for(j = scroll.x/TILE_TAILLE ; j < maxj; j++)
        {
            position.x = j*TILE_TAILLE + INTERFACE_X - scroll.x;
            position.y = i*TILE_TAILLE + INTERFACE_Y - scroll.y;

            if((calque == BACK || calque == MID || calque == FRONT || calque == PHYSIQUE) && map.Cases[i][j].back != 0)
            {
                rectx = map.Cases[i][j].back%TILESET_W;
                recty = map.Cases[i][j].back / TILESET_W;
                RectTileset.x = rectx*TILE_TAILLE;
                RectTileset.y = recty*TILE_TAILLE;
                SDL_BlitSurface(map.Tileset, &RectTileset, smain->screen, &position);
            }
            if((calque == MID || calque == FRONT || calque == PHYSIQUE) && map.Cases[i][j].mid != 0)
            {
                rectx = map.Cases[i][j].mid%TILESET_W;
                recty = map.Cases[i][j].mid / TILESET_W;
                RectTileset.x = rectx*TILE_TAILLE;
                RectTileset.y = recty*TILE_TAILLE;
                SDL_BlitSurface(map.Tileset, &RectTileset, smain->screen, &position);
            }

            if((calque == FRONT || calque == PHYSIQUE) && map.Cases[i][j].front != 0)
            {
                rectx = map.Cases[i][j].front%TILESET_W;
                recty = map.Cases[i][j].front / TILESET_W;
                RectTileset.x = rectx*TILE_TAILLE;
                RectTileset.y = recty*TILE_TAILLE;
                SDL_BlitSurface(map.Tileset, &RectTileset, smain->screen, &position);
            }

            if(!map.Cases[i][j].back && !map.Cases[i][j].mid && !map.Cases[i][j].front)
            {
                SDL_BlitSurface(vide, NULL, smain->screen, &position);
            }
            else if(calque == PHYSIQUE && map.Cases[i][j].phys != 0)
            {
                rectx = map.Cases[i][j].phys;
                RectTileset.x = rectx*TILE_TAILLE;
                RectTileset.y = 0;
                SDL_BlitSurface(tilePhysAffichage, &RectTileset, smain->screen, &position);
            }
        }
    }
}



