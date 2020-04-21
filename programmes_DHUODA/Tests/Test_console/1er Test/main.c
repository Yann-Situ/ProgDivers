#include <stdio.h>
#include <stdlib.h>

void MultDeux(int *Pointeur, int *Pointeurb);// " * " devant pointeur = variable
int main()
{
    int Nombre = 0, Chiffre = 0;
    printf("Hello guys!\n");
    printf("entrer deux nombres :\n");
    scanf("%d\n", &Nombre);
    scanf("%d", &Chiffre);
    MultDeux(&Nombre, &Chiffre);
    printf("resultat multiplication : %d\nresultat + deuxieme nombre : %d\n\t", Nombre, Chiffre);
    printf("\t=== Cheveux ===\n");
    printf("1 = Brune\n");
    printf("2 = Blonde\n");
    printf("3 = Rousse\n");
    int ChoixUn = 0;
    do
    {
        scanf("%d", &ChoixUn);// Pas mettre de " \n "
        switch (ChoixUn)
        {
        case 1 : printf("\n\tTres bon Choix !!");
        break;
        case 2 : printf("\n\tOuais ca va...");
        break;
        case 3 : printf("\n\tGros seins cest bien");
        break;
        default : printf("\n\tJ'avais dit un nombre entre 1 et 3 !\n");
        break;// Pas oubler le " ; "
        }
    }while (ChoixUn > 3);// Pas oublier le " ; "
    MultDeux(&ChoixUn, &Nombre);
    printf("\nVotre choix + le resultat multiplication = %d", ChoixUn);
    return 0;
}
void MultDeux(int *Pointeur, int *Pointeurb)
{
    *Pointeur *= *Pointeurb;
    *Pointeurb = *Pointeur+*Pointeurb;
}
