#include <stdio.h>
#include <stdlib.h>
#define POINT_VIE 20 // juste des espaces apres variables
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600
#define NOMBRE_PIXELS (LARGEUR_FENETRE * HAUTEUR_FENETRE)// mettre les calculs entre parentheses
#define PRESENTATION() printf("Coucou, je m'appelle Dark\n"); \
printf("J'habite a l'etoile noir\n"); \
printf("J'aime la force\n\n");
#define MAJORITE(age) if(age<18) \
printf("Vous etes mineur"); \
else printf("Vous etes Majeur\n");// les "\" pour continuer le define apres la ligne
//          ________Pour tous les fichier.h_________
//#ifndef DEF_NOMDUFICHIER // Si la constante n'a pas été définie le fichier n'a jamais été inclus
//#define DEF_NOMDUFICHIER // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus
/*   # Contenu de votre fichier .h (autres include, prototypes, define...) */
//#endif

//Il est possible d'écrire des conditions en langage préprocesseur pour choisir ce qui sera compilé. On utilise notamment les mots-clés
//      #if, #elif et #endif.

int main()
{
    printf("QUE PUISSE COMMENCER LE PROGRAMME !!!\n\n");
    PRESENTATION()
    int Age = 0;
    printf("Votre age : ");
    scanf("%d", &Age);
    MAJORITE(Age)
    return 0;
}
