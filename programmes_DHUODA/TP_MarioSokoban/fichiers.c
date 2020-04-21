#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constante.h"
#include "fichiers.h"

    int chargerNiveau(int carte[][NB_BLOC_H],int Aventure, char *Nomlevel);
    int sauvegarderNiveau(int carte[][NB_BLOC_H],int Aventure, char *Nomlevel);
    int comparerChaine(char *chn1, char *chn2);
    int nombredeNiveaux(int Aventure);
    int mettreNomdsTableau(InfoLevel level[],int Aventure, int NbLvL);

int chargerNiveau(int carte[][NB_BLOC_H],int Aventure, char *Nomlevel)
{
    FILE *fichier = NULL;
    char tabChiffre[NB_BLOC_W * NB_BLOC_H + 1] = {0}; // le + 1 pour le '\0'
    char caracAct = '0', Nomprovisoir[20] = "";
    int x = 0, y = 0, i = 0;
    int BonNom = 0;

    if(Aventure == 1)
        fichier = fopen("niveau.txt","r");
    else
        fichier = fopen("niveau_editeur.txt","r");

    if(fichier == NULL)
    {
        fprintf(stderr,"ouverture du fichier impossible : %s", SDL_GetError());
        return 0;
    }

    rewind(fichier);
    while(BonNom != 1)
    {
        caracAct = fgetc(fichier);
        if(caracAct == '#')
        {
            fgets(Nomprovisoir, NB_BLOC_H*NB_BLOC_W+1, fichier);
            fseek(fichier,-1,SEEK_CUR);
            Nomprovisoir[strlen(Nomprovisoir)-1] = '\0';
            i++;
        }
        else if(caracAct == EOF)
        {
            fprintf(stderr,"niveaux trouve : %d\nlevel non trouvé\t",i);
            return 0;
        }

        if(comparerChaine(Nomprovisoir,Nomlevel))
            BonNom = 1;
    }
    fseek(fichier,1,SEEK_CUR);
    fgets(tabChiffre,NB_BLOC_W * NB_BLOC_H + 1,fichier);// on inscrit la ligne du fichier dans tabChiffre
    for(y = 0; y < NB_BLOC_H; y++)
    {
        for(x = 0; x < NB_BLOC_W; x++)
        {
            switch(tabChiffre[(y * NB_BLOC_W)+x])// y*NB_BLOC_W pour calculer a partir de la bonne ligne
            {
                case '0':
                    carte[x][y] = 0;
                    break;
                case '1':
                    carte[x][y] = 1;
                    break;
                case '2':
                    carte[x][y] = 2;
                    break;
                case '3':
                    carte[x][y] = 3;
                    break;
                case '4':
                    carte[x][y] = 4;
                    break;
                case '5':
                    carte[x][y] = 5;
                    break;
            }
        }
    }
    fclose(fichier);
    return 1;
}

int sauvegarderNiveau(int carte[][NB_BLOC_H],int Aventure, char *Nomlevel)
{
    FILE *fichier = NULL;
    int x = 0, y = 0, i = 0;
    int BonNom = 0;


    char caracAct = '0', Nomprovisoir[NB_BLOC_H*NB_BLOC_W+1] = "";

    if(Aventure == 1)
        fichier = fopen("niveau.txt","r+");
    else
        fichier = fopen("niveau_editeur.txt","r+");

    if(fichier == NULL)
    {
        fprintf(stderr,"ouverture du fichier impossible : %s", SDL_GetError());
        return 0;
    }

    while(BonNom != 1)
    {
        caracAct = fgetc(fichier);
        if(caracAct == '#')
        {
            fgets(Nomprovisoir, NB_BLOC_H*NB_BLOC_W+1, fichier);
            fseek(fichier,-1,SEEK_CUR);
            Nomprovisoir[strlen(Nomprovisoir)-1] = '\0';
            i++;
        }
        else if(caracAct == EOF)
        {
            fprintf(stderr,"niveaux trouve : %d\nlevel non trouvé\t",i);
            return 0;
        }

        if(comparerChaine(Nomprovisoir,Nomlevel))
            BonNom = 1;
    }
    fseek(fichier,1,SEEK_CUR);

    for(y = 0; y < NB_BLOC_H; y++)
    {
        for(x = 0; x < NB_BLOC_W; x++)
        {
            fprintf(fichier,"%d",carte[x][y]);
        }
    }

    fprintf(fichier,"\n");
    fclose(fichier);

    return 1;
}

int comparerChaine(char *chn1, char *chn2)
{
    int i = 0;

    do
    {
        if(chn1[i] != chn2[i])
            return 0;
        i++;
    }while(chn1[i-1] != '\0');
    return 1;
}

int nombredeNiveaux(int Aventure)
{
    FILE *fichier = NULL;
    char caracAct = '0';
    int i = 0;

    if(Aventure == 1)
        fichier = fopen("niveau.txt","r");
    else
        fichier = fopen("niveau_editeur.txt","r");

    if(fichier == NULL)
    {
        fprintf(stderr,"ouverture du fichier impossible : %s", SDL_GetError());
        return 0;
    }

    while(caracAct != EOF)
    {
        caracAct = fgetc(fichier);
        if(caracAct == '#')
            i++;
    }
    if(i == 0)
        fprintf(stderr,"Aucun Niveau trouve : pas de # ds le fichier\n");
    return i;
    fclose(fichier);
}

int mettreNomdsTableau(InfoLevel level[],int Aventure, int NbLvL)
{
    FILE *fichier = NULL;
    char caracAct = '0';
    int NumMotAct = 0;

    if(Aventure == 1)
        fichier = fopen("niveau.txt","r");
    else
        fichier = fopen("niveau_editeur.txt","r");
    if(fichier == NULL)
    {
        fprintf(stderr,"ouverture du fichier impossible : %s", SDL_GetError());
        return 0;
    }

    while(caracAct != EOF && NumMotAct != NbLvL)
    {
        caracAct = fgetc(fichier);
        if(caracAct == '#')
        {
            NumMotAct++;
            fprintf(stderr,"# trouve, NumMotAct : %d\t",NumMotAct);
            fgets(level[NumMotAct-1].NomNiv,20,fichier);
            level[NumMotAct-1].NomNiv[strlen(level[NumMotAct-1].NomNiv)-1] = '\0';
            level[NumMotAct-1].NumeroNiv = NumMotAct;
            level[NumMotAct-1].Aventure = Aventure;
        }

    }
    fclose(fichier);
    if(caracAct == EOF)
        return -1;// -1 si on a pas trouvé tout les niveaux
    return 1;
}

