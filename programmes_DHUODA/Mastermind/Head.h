#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED
#endif // HEAD_H_INCLUDED

void tirercode(long Tableau[], int longueur)
{
    int i = 0;
    for(i = 0; i < longueur; i++)
    {
        Tableau[i] = (rand() % (9 - 1 + 1)) + 1;
    }
}

int gagner(long TableauProp[], long TableauBon[])
{
    int i = 0;
    int oui = 1;
    while(i != 4)
    {
        if(TableauBon[i] != TableauProp[i])
            oui = 0;
        i++;
    }
    return oui;
}

void entrerCode(long Tableau[])
{
    int i = 0;
    while(i != 4)
    {
        Tableau[i] = lireLong();
        i++;
    }
}

void place(long TabProp[], long TabBon[])
{
    int i = 0, j = 0, r1 = 0, r2 = 0;
    int BolUtilise1[4] = {0}, BolUtilise2[4] = {0};
    for(j = 0; j < 4; j++)
    {
        if(TabProp[j] == TabBon[j])
        {
            BolUtilise1[j] = 1;
            BolUtilise2[j] = 1;
            r1++;
        }
    }
    printf("%d bien place | ", r1);

    for(j = 0; j < 4; j++)
    {
        for(i = 0; i < 4; i++)
        {
            if(TabProp[j] == TabBon[i] && BolUtilise1[j] == 0 && BolUtilise2[i] == 0)
            {
                BolUtilise1[j] = 1;
                BolUtilise2[i] = 1;
                r2++;
            }
        }
    }
    printf("%d place au mauvaise endroit", r2);
}
