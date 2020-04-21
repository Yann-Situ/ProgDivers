#include <stdio.h>
#include <stdlib.h>
#define TAILLE_MAX 100
//On appelle la fonction d'ouverture de fichier "fopen" qui nous renvoie un pointeur sur le fichier.
//On vérifie si l'ouverture a réussi (c'est-à-dire si le fichier existait) en testant la valeur du pointeur qu'on a reçu. Si le
//pointeur vaut NULL, c'est que l'ouverture du fichier n'a pas fonctionné, dans ce cas on ne peut pas continuer (il faut
//afficher un message d'erreur).
//Une fois qu'on a terminé de travailler sur le fichier, il faut penser à le « fermer » avec la fonction fclose.
//      fopen( fichier_a_ouvrir , mode_d'ouverture )
// "fclose" renvoie un "int" : 0 si ca a marché ou "EOF" ( c un define )
int main()
{
    char Prenom[30] = "", Chaine[TAILLE_MAX] = "";
    printf("QUE PUISSE DEMARRER LE PROGRAMME!\n");
    FILE* Fichier = NULL;// Fichier renvoie un pointeur sur FILE qui est une structure zarbi
    Fichier = fopen("test.txt", "r+");//fopen("fichier","mode_lecture")
    if (Fichier != NULL)
    {
// On peut lire et écrire dans le fichier

        while (fgets(Chaine, TAILLE_MAX, Fichier) != NULL)
        {// ce while : continuer tant que fgets renvoi qqchose de different que NULL (pointeur) c-a-dire jusqu'a la fin du texte
                printf("%s", Chaine);
        }
        fputc('C',Fichier);
        fputs("oucou",Fichier);

        printf("    Votre nom : ");
        scanf("%s", Prenom);
        fprintf(Fichier," %s.\n", Prenom);

        fclose(Fichier);//pas de "" car renvoi un int
    }
    else
    {
        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier test.txt");
    }
    printf("\nLE PROGRAMME EST TERMINE!");
    return 0;
}
