#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*sur une variable, comme la variable age :
    age signifie : « Je veux la valeur de la variable age »,
    &age signifie : « Je veux l'adresse à laquelle se trouve la variable age » ;
  sur un pointeur, comme pointeurSurAge :
    pointeurSurAge signifie : « Je veux la valeur de pointeurSurAge » (cette valeur étant une adresse),
    *pointeurSurAge signifie : « Je veux la valeur de la variable qui se trouve à l'adresse contenue dans pointeurSurAge ».*/

int main()
{
    printf("\tHello world!\n\n");

    int NbChaine1 = 0, NbChaine2 = 0;
    char Chaine1[50], Chaine2[50], Chaine3[50] = {0}, *suiteCh1 = {0};

    printf("Entrer une premiere Chaine :");
    scanf("%s", Chaine1);// pas les "[]" ni & car tableau char
    printf("\nEntrer une deuxieme Chaine :");
    scanf("%s", Chaine2);
    printf("\nLa Chaine 1 = %s\n", Chaine1);//pas de [] ni de * car tableau char
    printf("La Chaine 2 = %s\n\n", Chaine2);

    NbChaine1 = strlen(Chaine1);// fonction "strlen(chaine)" pour la longueur dune chaine
    NbChaine2 = strlen(Chaine2);
    printf("La Chaine 1 possede %d caractres\n", strlen(Chaine1));
    printf("La Chaine 2 possede %d caractres\n\n", NbChaine2);

    strcpy(Chaine3, Chaine1);// fonction "strcpy(Copy, Chaine a Copier)" pour copier une chaine
    printf("La Chaine 3 (copie de la Chaine 1) = %s\n", Chaine3);

    strcat(Chaine3, Chaine2);// fonction "strcat(Chaine a Changer, Chaine a inclure)" pour fusionner deux chaine
    printf("Je met Chaine 2 dans Chaine 3\nLa Chaine 3 = %s\n", Chaine3);
    NbChaine1 = strlen(Chaine3);
    printf("La Chaine 3 possede %d caractres\n\n", NbChaine1);

    suiteCh1 = strchr(Chaine1,'a');// fonction "strchr(Chaine, caractere a rechercher)" pour rechercher un caractere ds une chaine, renvoie NULL ou
    //                     la fin de la chaine apres le premier caractere trouvé /!\ la Chaine ou on enregistre la suite doit s'ecrir "*suiteChaine"
   //                       strchr renvoie un pointeur !
    if(suiteCh1 != NULL)
    {
        printf("La suite de Chaine 1 apres le premier 'a' est %s\n", suiteCh1);
    }
    else
    {
        printf("Il n'y a pas de 'a' dans Chaine 1\n");
    }
    return 0;
}
