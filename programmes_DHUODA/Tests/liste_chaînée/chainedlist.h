#ifndef CHAINEDLIST_H_INCLUDED
#define CHAINEDLIST_H_INCLUDED

//STRUCTURES :
typedef struct elemt elemt;
struct elemt
{
    //Donn�es : ex :
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
elemt* cl_initelemt(/*donn�es*/);// initialiser un �l�ment d'une list
elemtList* cl_initlist(elemt* element);// initialiser une list avec son premier �l�ment
void cl_addelemt(elemtList* list, elemt* element, elemt* pre);// ajouter un �l�ment ds une list apr�s l'�l�ment 'pre'
void cl_deleteelemt(elemtList* list, elemt* pre);// suprimer un �l�ment apr�s l'�l�ment 'pre'
void cl_printlist(elemtList* list);// afficher le contenu des �l�ments dunne list
int cl_getsizelist(elemtList* list);// retourne la taille d'une list
elemt* cl_getpreelemt(elemtList* list, elemt* element);// retourne un pointeur sur l'�l�ment pr�c�dent dans la list
#endif // CHAINEDLIST_H_INCLUDED
