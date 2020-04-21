#include <stdio.h>
#include <stdlib.h>

int main()
{
    typedef struct coordo coordo;
    struct coordo
    {
    int x;
    int y;
    };

    printf("QUE PUISSE COMMENCER LE PROGRAMME!\n");
    printf("int : %d\n", sizeof(int));
    printf("char : %d\n", sizeof(char));
    printf("double : %d\n", sizeof(double));
    printf("long : %d\n", sizeof(long));
    printf("Coordo : %d\n\n", sizeof(coordo));
    // "malloc" permission memoire
    // "free"   liberer de la memoire
    int *PointeurMemAllouee = NULL;
    PointeurMemAllouee = malloc(sizeof(int));// malloc(octet_a_liberer) renvoie un pointeur
    if (PointeurMemAllouee == NULL)
        exit(0);
    printf("Votre age :");
    scanf("%d", PointeurMemAllouee);
    printf("Ah, vous avez %d", *PointeurMemAllouee);
    free(PointeurMemAllouee);// free(mem_a_liberer)
    int NBamis = 0, i = 0;
    int *PointeurTabAmis = NULL;
    printf("\nCombien d'amis avez vous ?");
    scanf("%d", &NBamis);
    PointeurTabAmis = malloc(NBamis * sizeof(int));
    if (PointeurTabAmis == NULL)
        exit(0);
    while( i != NBamis)
    {
        printf("Votre ami numero %d a :", i+1);
        scanf("%d", &PointeurTabAmis[i]);
        i++;
    }
    printf("\n");
    for (i = 0 ; i < NBamis ; i++)
    {
        printf("Votre amis numero %d a %d ans\n", i+1, PointeurTabAmis[i]);
    }
    free(PointeurTabAmis);
    printf("\nLE PROGRAMME EST TERMINEE!");
    return 0;
}
