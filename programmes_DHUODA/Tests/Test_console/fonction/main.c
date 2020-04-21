#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Minutes(int *Sec, int *Min, int *Heu);
int main()
{
   int NbSecondes = 0, NbMinutes = 0, NbHeures = 0;
   printf("Hello guys!\n");
   printf("Entrer un nombre de secondes :");
   scanf("%d", &NbSecondes);
   Minutes(&NbSecondes, &NbMinutes, &NbHeures);
   printf("\n\t%d h %d m %d s.", NbHeures, NbMinutes, NbSecondes);
   return 0;
}
void Minutes(int *Sec, int *Min, int *Heu)
{
    while(*Sec > 59)
    {
        *Sec -= 60;
        *Min = *Min + 1;
    }
    while(*Min > 59)
    {
        *Min -= 60;
        *Heu = *Heu + 1;
    }
}
