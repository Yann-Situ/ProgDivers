#ifndef CHAINEDLIST_H_INCLUDED
#define CHAINEDLIST_H_INCLUDED

//STRUCTURES :
typedef struct elemt elemt;
struct elemt
{
    //Données : ex :
    /*
    int nb;
    char name[20];*/

    elemt *su;
};
typedef struct elemtList elemtList;
struct elemtList
{
    int size;
    elemt *first;
};

//PROTOTYPE DES FONCTIONS :
elemt* cl_initelemt(/*données*/);// initialiser un élément d'une list
elemtList* cl_initlist(elemt* element);// initialiser une list avec son premier élément
void cl_addelemt(elemtList* list, elemt* element, elemt* pre);// ajouter un élément ds une list après l'élément 'pre'
void cl_deleteelemt(elemtList* list, elemt* pre);// suprimer un élément après l'élément 'pre'
void cl_printlist(elemtList* list);// afficher le contenu des éléments dunne list
int cl_getsizelist(elemtList* list);// retourne la taille d'une list
elemt* cl_getpreelemt(elemtList* list, elemt* element);// retourne un pointeur sur l'élément précédent dans la list
#endif // CHAINEDLIST_H_INCLUDED
