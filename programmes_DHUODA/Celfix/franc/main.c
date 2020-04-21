#include <stdio.h>
#include <stdlib.h>

double conversion (double francs)
{
    double euros = 0;

    euros = francs / 6.55957;
    return euros;

}


int main()
{
    int euros = 0, appuyez1 = 0;
    double francs = 0;
do
{
    printf("Donner un nombre d'euro a convertir en euro :\n\n");
    scanf("%t", &francs);
    printf("%d francs = %f euro\n\n", francs, conversion(francs));
do
    {

    printf("Si vous voulez faire une nouvelle convertion, appuyez sur 1\n\n");
    scanf("%d", &appuyez1);

    if (appuyez1 != 1)
    {
        printf("erreur vous devez appuyer sur 1\n");
    }
    }while (appuyez1 != 1);
}while (appuyez1 == 1);

    return 0;


}
