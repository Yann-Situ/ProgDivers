#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>// pour les toupper() et tout
#include <string.h>//  pour les strlen() et tout
#include <time.h>//pour les rand()
#include "Header1.h"
#include "dico.h"// pour les fonction pioche et rand
#include "Ecrit_secu.h"

int main()
{
    printf("QUE PUISSE COMMENCER LE PROGRAMME!\n    LE PENDU\n\n");

    long Recommencer = 0;
    do
    {

    char MotRecherche[50] = {0};
    int *MotTrouveBol = NULL;// pointeur où on mettra le tableau de boléen pour les lettres trouvées
    int NbVie = 0,i = 0,Diffi = 0;
    long NbJoueur = 0;
    int TailleMot = 0;

    printf("Combien voulez vous de joueur ? 1 ou 2\n--------------------------------------------------------------------------------\n");
    NbJoueur = lireLong();

    if(NbJoueur < 1 && NbJoueur > 2)
    {
        NbJoueur = 1;
    }

    if(NbJoueur == 2)
    {
        printf("Joueur 1 ecrivez le mot secret : ");
        lireMot2Joueurs(MotRecherche,50);
        printf("________________________________________________________________________________\n________________________________________________________________________________\n________________________________________________________________________________\n________________________________________________________________________________\n________________________________________________________________________________\n");
    }

    printf("Quel difficulte ?\n0 = NOOB\t1 = FACILE\t2 = NORMAL\t3 = DIFFICILE\t4 = LEGENDAIRE\n--------------------------------------------------------------------------------\n");
    Diffi = lireLong();

    switch (Diffi)
        {
        case 0 : printf("Noob, si tu n'y arrive pas, suicide toi\n");
            NbVie = 26;
        break;
        case 1 : printf("Facile\n");
            NbVie = 14;
        break;
        case 3 : printf("Difficile\n");
            NbVie = 8;
        break;
        case 4 : printf("Legendaire, si tu es un dieux, tu peux gagner\n");
            NbVie = 1;
        break;
        default : printf("Normal\n");
            NbVie = 11;
        break;// Pas oubler le " ; "
        }
        printf("________________________________________________________________________________\n________________________________________________________________________________\n________________________________________________________________________________\n");

    if(NbJoueur == 1)
    {
        if(!piocherMot(MotRecherche))// "si on na pas recu vrai dans la fonction piocherMot on arrete tout" / piocherMot fait 2 chose : il met le mot
            exit(0);//                                                           ds MotRecherche et il renvoie un boléen pour voir si ça a fonctionné
    }
    TailleMot = strlen(MotRecherche);// strlen renvoi la taille de la chaine

    MotTrouveBol = malloc(TailleMot * sizeof(int)); /* On alloue dynam le tableau MotRecherche (dont on ne connaissait pas la taille au départ)*/
    if (MotTrouveBol == NULL)// malloc(octet a liberer) renvoie un pointeur
        exit(0);// si ça marche pas on arrete

    for (i = 0 ; i < TailleMot ; i++)
        MotTrouveBol[i] = 0;

    printf("Le mot est pres!\n");
    if(NbJoueur == 2)
    {
        printf("Joueur 1, cest votre tour, essayez de trouver le mot !\n");
    }

    while( NbVie > 0 && !gagner(MotTrouveBol,TailleMot))// /!\ && = "ou"  "!"  devant une condition inverse la fonction : "si ça ne renvoie pas vrai"
    {

        printf("\t\tLe mot est : ");
        for(i = 0; i < TailleMot; i++)
        {//ecriture du mot en cours
            if(MotTrouveBol[i] == 1)
                printf("%c",MotRecherche[i]);
            else
                printf("*");
        }
        printf("\n\n");
        printf("Il reste %d vies\nProposez une lettre\n", NbVie);
        char MaLettre = 0;
        int CptLettre = 0;
        i = 0;
        MaLettre = EcrireCaractere();//ecriture de la lettre

        for(i = 0; i < TailleMot; i++)
        {//recherche de lettre
            if(MaLettre == MotRecherche[i])
            {
                MotTrouveBol[i] = 1;
                CptLettre ++;
            }
        }

        if(CptLettre < 2)
            printf("\n%d lettre a ete trouve\n", CptLettre);
        else
            printf("\n%d lettre ont ete trouve\n", CptLettre);

        if(CptLettre == 0)
        {
            printf("Vous perdez une vie\n");
            NbVie --;
        }
    }

    if(gagner(MotTrouveBol,TailleMot))
    {//condition si perdu ou gagné
        if(NbVie == 1)
            printf("\nBravo jeune padawan, tu as gagne alors qu il te restait qu'une vie.\nLe mot etait bien %s\n",MotRecherche);
        else
            printf("\nBravo jeune padawan, tu as gagne et il te reste %d vies.\nLe mot etait bien %s\n", NbVie, MotRecherche);
    }
    else
    {
        printf("Tu as perdu, tu n es pas digne d etre un jedi, le mot secret etait : %s.", MotRecherche);
    }
    free(MotTrouveBol);
    printf("\nLa partie est termine.\n");
    printf("Veut tu recommencer une nouvelle partie ?\n0 = non\t\t1 = oui\n");
    Recommencer = lireLong();

    }while(Recommencer > 0);
    printf("\nLe programme est termine.\n");
    return 0;
}
