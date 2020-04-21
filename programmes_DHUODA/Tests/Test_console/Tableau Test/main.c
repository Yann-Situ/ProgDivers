#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <string.h>

int main()
{
    printf("\tHello man!\n");
    int tableau_a[3] = {0};
    int NbFois = 0;
    int yo = 0;
    printf("Combien de fois pour le test du tableau ?\n");
    scanf("%d", &NbFois);

    while (yo < NbFois)
    {
        // fonction :
        CreatValeurTableau(tableau_a,3);

        // fonction :
        affichageTable(tableau_a,3);

        // fonction :
        SommeTableau(tableau_a,3);

        printf("\nInitialisation du tableau...\n");

        // fonction :
        Initialisation(tableau_a,3);

        yo++;
    }
    printf("entrer votre nom :\n");
    char Prenom[30];
    scanf("%s", Prenom);
    printf("Vous etes donc %s\n", Prenom);
    printf("\n\tLe Programme est termine");
    return 0;
}
