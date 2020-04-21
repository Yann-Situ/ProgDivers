#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED
#endif // FONCTION_H_INCLUDED
#include <string.h>

void viderBuffer()// pour vider ce qui est en plus ds la chaine
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();// getchar() renvoie un int
    }
}

int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;
    if (fgets(chaine, longueur, stdin) != NULL)
    {// char *fgets( char *str, int num, FILE *stream ); soit   fgets(Pointeur_chn, Longueur_Chaine, Mode_de_saisie(stdin = clavier))return un point
        positionEntree = strchr(chaine, '\n');// strchr(Point_chaine, carac) renvoie un pointeur sur lemplacement du caractere dans la chaine
                                                // ou NULL si ça na pas fonctionné
        if (positionEntree != NULL)
        {
            *positionEntree = '\0';
        }
        else
        {
            viderBuffer();
        }
        return 1;
    }
    else
    {
        viderBuffer();
        return 0;
    }
}

long lireLong()// elle renvoie le nombre convertit ou 0
{
    char Chaine[50] = {0};
    if(lire(Chaine,50))
    {
        return strtol(Chaine,NULL,10);// strtol(chaine, NULL, base de nombre(10)) convertit une chaine en long si elle commence par un nombre
                                      //elle renvoie ce nombre ou un 0
    }
    else
    {
        return 0;
    }
}
