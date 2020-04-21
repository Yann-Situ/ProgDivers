#ifndef DEF_FICHIERS
#define DEF_FICHIERS
    int chargerNiveau(int carte[][NB_BLOC_H],int Aventure, char *Nomlevel);
    int sauvegarderNiveau(int carte[][NB_BLOC_H],int Aventure, char Nomlevel[]);
    int comparerChaine(char *chn1, char *chn2);
    int nombredeNiveaux(int Aventure);
    int mettreNomdsTableau(InfoLevel level[],int Aventure, int NbLvL);

#endif // DEF_FICHIERS


