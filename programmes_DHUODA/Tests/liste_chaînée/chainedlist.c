#include <stdio.h>
#include <stdlib.h>
#include "chainedlist.h"

// FONCTIONS

elemt* cl_initelemt(/*données*/)// initialiser un élément d'une list
{
    elemt* element = malloc(sizeof(elemt*));
    if(element == NULL)
        exit(EXIT_FAILURE);
    //INITIALISATION DES DONNEES DE LA STRUCT :

    element->su = NULL;

    return element;
}

elemtList* cl_initlist(elemt* element)// initialiser une list avec son premier élément
{
    elemtList* list = malloc(sizeof(elemtList*));

    if(list == NULL || element == NULL)
        exit(EXIT_FAILURE);

    element->su = NULL;
    list->first = element;
    list->size = 1;
    return list;
}

void cl_addelemt(elemtList* list, elemt* element, elemt* pre)// ajouter un élément ds une list après l'élément 'pre'
{
    if(list == NULL || element == NULL)
        exit(EXIT_FAILURE);
    //INSERTION :
    if(pre != NULL)
    {
        element->su = pre->su;
        pre->su = element;
    }
    else
    {
        element->su = list->first;
        list->first = element;
    }
    list->size ++;
}

void cl_deleteelemt(elemtList* list, elemt* pre)// suprimer un élément après l'élément 'pre'
{
    if(list == NULL)
        exit(EXIT_FAILURE);

    if(list->first != NULL)
    {
        elemt* element;
        //SUPRESSION :
        if(pre != NULL)
        {
            element = pre->su;
            pre->su = pre->su->su;
        }
        else
        {
            element = list->first;
            list->first = list->first->su;
        }
        free(element);
        list->size --;
    }
}

void cl_printlist(elemtList* list)// afficher le contenu des éléments dunne list
{
    if(list == NULL)
        exit(EXIT_FAILURE);

    elemt* element = list->first;
    while(element !=NULL)
    {
        //AFFICHAGE DES DONNEES DUN ELEMENT :

        element = element->su;
    }
}

int cl_getsizelist(elemtList* list)// retourne la taille d'une list
{
    if(list == NULL)
        exit(EXIT_FAILURE);
    if(list->first != NULL)
    {
        int i = 0;
        elemt* element = list->first;
        while(element != NULL)
        {
            element = element->su;
            i++;
        }
        return i;
    }
    return 0;
}

elemt* cl_getpreelemt(elemtList* list, elemt* element)// retourne un pointeur sur l'élément précédent dans la list
{
    if(list->first != element)
    {
        elemt* pre = list->first;
        while(pre->su != element)
        {
            pre = pre->su;
        }
        return pre;

    }
    else
        return NULL;
}
