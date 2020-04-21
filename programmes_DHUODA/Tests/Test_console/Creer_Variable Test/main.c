#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

int main()
{
printf("Hello world!\n");
Perso Joueur1 = {"","","",0,0};

printf("Bonjour, Comment vous appelez vous ?\n");
scanf("%s", Joueur1.Prenom);

printf("Vous vous appelez donc %s\nQuel est votre race ?\n", Joueur1.Prenom);
scanf("%s", Joueur1.Race);

printf("Et votre Vocation ?\n");
scanf("%s", Joueur1.Vocation);

printf("\nVous etes donc %s le %s et vous etes un %s", Joueur1.Prenom,Joueur1.Vocation,Joueur1.Race);
Coord point = {0,0};
point.x = 10;
point.y = 20;
Volume Musique = Moy;
return 0;
}
