#include <stdio.h>
#include <stdlib.h>

double conversion (double euros)
{
    double francs = 0;

    francs = 6.55957 * euros;
    return francs;

}


int main()
{
    int euro = 0, appuyez1 = 0;
do
{
    printf("Donner un nombre d'euro a convertir en francs :\n\n");
    scanf("%d", &euro);
    printf("%d euro = %f francs\n\n", euro, conversion(euro));
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
