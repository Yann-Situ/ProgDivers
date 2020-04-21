#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Parfait(long NbMin, long NbMax)
{
    long Reste = 0, Divise = 1, Nombre = 1, Total=0;
    Nombre = NbMin;
    do
    {
        Total = 0;
        Divise = 1;
        do
        {
            Reste = Nombre % Divise;
            if (Reste == 0)
            {
                Total += Divise;
            }
            Divise ++;
        }while (Divise < Nombre);
        if (Total == Nombre)
        {
            printf("\tNombre parfait : %ld\n", Nombre);
        }
        Nombre ++;
    }while (Nombre < NbMax);
}
int main()
{
    printf("Hello guys!\n");
    printf("Entrer un nombre maximum :\n");
    long Maximum = 0, Minimum = 0;
    scanf("%ld", &Maximum);
    printf("Entrer un nombre minimum :\n");
    scanf("%ld", &Minimum);
    Parfait(Minimum, Maximum);
    printf("La recherche est termine\n\n");
    return 0;
}
