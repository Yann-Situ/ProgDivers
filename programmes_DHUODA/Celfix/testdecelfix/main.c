#include <stdio.h>
#include <stdlib.h>

int main()
{
    int appuyez1 = 0, age = 0;
    printf("Bonjour ! \n bienvenu sur le test incontournable ! \n\n");
    printf("appuyer sur 1 pour continuer...");
    do
    {
        scanf("%d", &appuyez1);
        if(appuyez1 != 1)
        {
            printf("appuis sur 1 bordel de merde !");
        }
        else
        {
        printf("Bien on peu continuer..\n\n");
        }
    }while(appuyez1 != 1);

do
{
    printf("Quel age tu as ? \n");
    scanf("%d", &age);
    if(age != 15)
    {
        printf("c'est faut ! Entre ton vrais age piti chenapan !\n\n");
    }
    else
    {
    printf("Bien tu n'a pas mentis\n\n");
    }
/*
}while(age != 15);

printf("comment tu t'appelle ?\n\n");

char prenom[100];
char Chaine_Yann = "Yannsitu";
do
{
scanf("%s", prenom);
    if (prenom != Chaine_Yann)
    {
        printf("tu n'a pas entree ton vrais nom !\n\n");
    }
    else
    {
        printf("Bon c'est bien");
    }
}while (prenom != Chaine_Yann);
*/

    return 0;
}
