#include <stdio.h>
#include <stdlib.h>
#include "fonction.h"
#include <string.h>

int main()
{
    printf("YOLOOO\n");

    char Nom[20] = {0};
    printf("Quel est votre nom ?\n");
    if(lire(Nom,20))
    {
        printf("votre nom est %s",Nom);
    }
    else
    {
        printf("prob");
    }
    return 0;
}
