#include <stdio.h>
#include <stdlib.h>
#include <time.h>//pour les rand()
#include <string.h>
#include <ctype.h>
#include "dico.h"

int piocherMot(char *MotPioche)// fonction qui prend l'adresse où sera ecrite le mot dans main.c
{
    FILE *dico = NULL;// Le pointeur de fichier qui va contenir notre fichier
    int NbMots = 0, NumMotChoisi = 0;
    int CaracLu = 0;

    dico = fopen("dico.txt", "r");// "r" est "lecture seul"

    if (dico == NULL)
    {// Verification d'ouverture de fichier
        printf("Impossible d'ouvrir le dictionnaire de mots");
        return 0;
    }       // Si on n'a PAS réussi à ouvrir le fichier

    do
    {//Décompte du nombre de mots ( nombre de "entrer" )
        CaracLu = fgetc(dico);// fgetc(pointeur_fichier) renvoi un caractere lu, du coup, il avance aussi le curseur
        if(CaracLu == '\n')
            NbMots ++;// on augmente le nombre de mots
    }while(CaracLu != EOF);
    printf("mots %d\t",NbMots);
    NumMotChoisi = nbRand(NbMots);// nbRand(max) = Nb aleatoire entre 0 et Max
    printf("Numero %d\n",NumMotChoisi);

    rewind(dico);//remet le curseur au début

    while(NumMotChoisi > 0)
    {//Positionnement du curseur à la ligne
        CaracLu = fgetc(dico);
        if(CaracLu == '\n')
            NumMotChoisi --;    //on se rapproche du mot quand on descend d'une ligne
    }                           // on arrive au mot choisi

    fgets(MotPioche,50,dico);//fgets(chaine_receveur,nb_carac,pointeur_fichier) lit une chaine de carac (lit en fait une ligne avec le \n )
    MotPioche[ strlen(MotPioche) - 1 ] = '\0';// strlen(chaine) pour la longueur dune chaine / '\0' pour la fin d'une chaine ou tableau

    fclose(dico);// on ferme le fichier
    return 1;// tout cest bien passé
}

int nbRand(int Max)// fonction qui tire un Nb au pif entre 0 et Max
{
    srand(time(NULL));// instruction a lire UNE SEULE FOIS PAR PROGRAMME pour utiliser autant de rand() qu'on veut
    return(rand() % Max);// (rand() % maximum) choisi un nombre aleatoire entre 0 et le maximum
}

int lireMot2Joueurs(char *Chaine, int Longueur)//renvoie 1 ou 0 et met la chaine ds le dico et le pointeur
{
     int i = 0;
     char *positionEntree = NULL;
     FILE *dico = NULL;// Le pointeur de fichier qui va contenir notre fichier

     dico = fopen("dico.txt", "a");// "a" pour ajout a la fin

     if (dico == NULL)
     {// Verification d'ouverture de fichier
        printf("Impossible d'ouvrir le dictionnaire de mots");
        return 0;
     }       // Si on n'a PAS réussi à ouvrir le fichier

     fgets(Chaine,Longueur,stdin);
     while(Chaine[i] != '\n')
     {
         Chaine[i] = toupper(Chaine[i]);
         i++;
     }
     fputs(Chaine,dico);

     positionEntree = strchr(Chaine, '\n');// strchr(Point_chaine, carac) renvoie un pointeur sur lemplacement du caractere dans la chaine
                                                // ou NULL si ça na pas fonctionné
    if (positionEntree != NULL)
    {
        *positionEntree = '\0';
    }
    else
    {
        viderBuffer();
    }

     fclose(dico);// on ferme le fichier
     return 1;
}
