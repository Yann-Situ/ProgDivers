#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#endif // HEADER_H_INCLUDED

typedef struct Moi Moi;
struct Moi
{
    char nom[100];
    char prenom[100];
    char adresse[200];
    int age;
    int garcon; // Booléen : 1 = garçon, 0 = fille
};
typedef struct Coordonnees Coord;
struct Coordonnees
{
    int x;
    int y;
};
typedef enum Volume Volume;
enum Volume
{
    Muet = 0,Bas = 10,Moy = 65,Hau = 90
};
typedef struct Perso Perso;
struct Perso
{
    char Prenom[50];
    char Race[50];
    char Vocation[50];
    int Age;
    int garcon;
};
