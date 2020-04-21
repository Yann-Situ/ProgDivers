#include <stdio.h>
#include <stdlib.h>

double aireDeMonRectangle (double largeur, double hauteur);


int main()
{
    int appuyez1 = 0;
    printf("On a un rectangle de largeur 10 et de hauteur 5 sont aire est de...................\n\n\n");
    do
    {
        printf("appuyez sur 1 pour continuer...\n");
        scanf("%d", &appuyez1);
        if (appuyez1 != 1)
        {
            printf("appuyez sur 1 pour conaitre la reponce !");
        }
     else
     {
         printf("%f!!!\n\n", aireDeMonRectangle(5, 10));
     }
    }while (appuyez1 != 1);

return 0;

}
double aireDeMonRectangle(double largeur, double hauteur)
{
    return largeur * hauteur;
}
