#ifndef HEADER1_H_INCLUDED
#define HEADER1_H_INCLUDED

#endif // HEADER1_H_INCLUDED
#include <ctype.h>
#include <string.h>

char EcrireCaractere()
{

   char carac = 0;

   carac = getchar();// getchar() revient exactement à écrire scanf("%c",&lettre)
   carac = toupper(carac);// toupper(caractere) ds ctype.h transforme en majuscule
   while(getchar() != '\n')
   {

   };//boucle jusqua trouver un '\n' taper sur le clavier (entrer)

   return carac;
}

int gagner(int Tableau[],int TailleMot)
{
    int i = 0;
    int GagnerBol = 1;
    for(i = 0; i < TailleMot; i++)
    {
        if(Tableau[i] == 0)
            GagnerBol = 0;
    }
    return GagnerBol;
}
