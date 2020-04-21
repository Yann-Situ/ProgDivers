#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

void CreatValeurTableau(int Tableau[],int TailleTableau);
void affichageTable(int Tableau[],int TailleTableau);
void Initialisation(int Tableau[], int TailleTableau);
void SommeTableau(int Tableau[], int TailleTableau);

void CreatValeurTableau(int Tableau[],int TailleTableau)
{
    int io = 0;
    while (io < TailleTableau)
    {
        printf("Veuiller entrer la %d valeur du tableau : \n", io+1);
        scanf("%d", &Tableau[io]);
        io++;
    }
}
void affichageTable(int Tableau[],int TailleTableau)
{
    int i = 0;
    printf("Tableau :\n");
    for (i = 0; i < TailleTableau; i++)
    {
        printf("\t%d\n", Tableau[i]);
    }
}

void Initialisation(int Tableau[], int TailleTableau)
{
    int i = 0;
    for (i = 0; i < TailleTableau; i++)
    {
        Tableau[i] = 0;
    }
}

void SommeTableau(int Tableau[], int TailleTableau)
{
    int i = 0, Somme = 0;
    for (i = 0;i < TailleTableau; i++)
    {
        Somme += Tableau[i];
    }
    printf("Somme : %d\n", Somme);
}


#endif // MAIN_H_INCLUDED
