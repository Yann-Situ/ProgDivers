#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Ecriture_secu.h"
#include "Head.h"

int main()
{
    printf("Hello world!\n");
    srand(time(NULL));
    long C_rep[4] = {0};
    long C_pro[4] = {0};
    int i = 0, NbVie = 10;

    tirercode(C_rep, 4);
    for(i = 0; i < 4; i++)
    {
        printf("%ld",C_rep[i]);
    }
 while( NbVie > 0 && !gagner(C_pro, C_rep))
 {
     printf("Entrez un Code de 4 chiffre allant de 1 a 8 : \n");
     entrerCode(C_pro);
    place(C_pro,C_rep);

 }
    return 0;
}
