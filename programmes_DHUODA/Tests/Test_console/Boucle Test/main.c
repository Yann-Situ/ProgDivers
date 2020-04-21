#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    srand(time(NULL));
    int NBMYST;
    const int MAX = 100, MIN = 1;
    NBMYST = (rand() % (MAX - MIN + 1))+MIN;
    printf("Que puisse demarrer LE PROGRAMME!!!\n");
    //YOLO
    int Compt = 10;
    while (Compt > 0)
    {
        printf("%d \t", Compt);
        Compt--;
    }
    //fin du YOLO
    printf("Commencons\n");
    printf("%d ", NBMYST);
    int NBENTRER;
    do
    {
        printf("Nombre mystere ?\n");
        scanf("%d", &NBENTRER);
        if (NBENTRER < NBMYST)
        {
            printf("C'est plus\n");
        }
        else if (NBENTRER > NBMYST)
        {
            printf("C'est moins\n");
        }
        else
        {
            printf("Bravo\n");
        }
    } while (NBENTRER != NBMYST);

    return 0;
}
