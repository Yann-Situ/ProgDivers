#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

void pause()
{
    int continuer = 1;
    SDL_Event event;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
            continuer = 0;
        }
    }
}


int main(int argc, char *argv[])
{
    /* SDL_Init(mode d'utilisa°) pour utiliser la sdl et allouer de la memoire (peut additionner des modes d'utilisa° avec "|")  renvoi -1 si erreur
                SDL_INIT_VIDEO      Charge le système d'affichage (vidéo). C'est la partie que nous chargerons le plus souvent.
                SDL_INIT_AUDIO      Charge le système de son. Vous permettra donc par exemple de jouer de la musique.
                SDL_INIT_CDROM      Charge le système de CD-ROM. Vous permettra de manipuler votre lecteur de CD-ROM
                SDL_INIT_JOYSTICK   Charge le système de gestion du joystick.
                SDL_INIT_EVERYTHING Charge tous les systèmes listés ci-dessus à la fois.*/
        /*apres ne pas oublier le "SDL_Quit();" pour arreter et liberer la memoire*/

    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur stderr fichier auto pour les erreurs
        exit(EXIT_FAILURE); // On quitte le programme                                // SDL_GetError() renvoie derniere erreur de la SDL
    }//                     EXIT_FAILURE est un Define pour tout OS representant valeur à renvoyer en cas d'échec du programme

    SDL_Surface *screen = NULL, *Rectangle1 = NULL;;// SDL_Surface est une structure pour creer des variables pour les surfaces
                // on crée un pointeur ici pour mettre SDL_setvideomode dedands

    SDL_WM_SetIcon(SDL_LoadBMP("sdl_icone.bmp"), NULL);/*
    SDL_WM_SetIcon(Surface_qui_contient_l'image, info_sur_la_transparence) pour changer l'icone de la fenetre
    /!\ le faire avant SDL_SetVideoMode !   */

    screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    /*  Notre pointeur peut maintenant valoir :

        NULL : ecran vaut NULL si la SDL_SetVideoMode n'a pas réussi à charger le mode vidéo demandé. Cela arrive si
            vous demandez une trop grande résolution ou un trop grand nombre de couleurs que ne supporte pas votre ordinateur ;
        une autre valeur : si la valeur est différente de NULL, c'est que la SDL a pu allouer la surface en mémoire, donc que tout
            est bon ! */

            /*                                      SDL_SetVideoMode() prend 4 parametre :   la largeur de la fenêtre désirée (en pixels) ;
    Flags : SDL_HWSURFACE : mem rapide mais moins de memoire                                 la hauteur de la fenêtre désirée (en pixels) ;
            SDL_SWSURFACE : mem plus lente mais plus de memoire                              le nombre de couleurs affichables (en bits / pixel) ;
            SDL_RESIZABLE : fenetre redimensionable                                          des options (des flags).
            SDL_NOFRAME : pas de bordure et de titre
            SDL_FULLSCREEN
            SDL_DOUBLEBUF : pour deplacement des objets fluides
        SDL_SetVideoMode renvoie un pointeur
    */

    if (screen == NULL) // Si l'ouverture a échoué, on le note et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }// pour gerer  les erreurs

    SDL_WM_SetCaption("Fenetre super swagg !", NULL);// DSL_WM_SetCaption( nom de la fenetre , NULL ) pour changer le nom de la fenetre

    /* SDL_FillRect(surface, partie_de_la_surface, couleur) pour colorier un rectangle  |   NULL pour partie_de_la_surface est la surface entiere */
    // Uint32 stoke un nombre pour une couleur |  cest une structure
    // SDL_MapRGB(format_couleur(ex : 32), quant_rouge, quant_vert, quant_bleu) pour le nom d'une couleur | renvoie un Uint-format-
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 255));// screen->format represente le Nombre de couleurs choisi ds "screen"

    // Maintenant je veux faire d'autre rectangle !!!
    // SDL_CreateRGBSurface( Flags: HWS...ou SWS... , largeur, hauteur, nb_couleur ,0,0,0,0) pour allouer de la memoire pour une surface
    Rectangle1 = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 200, 32, 0,0,0,0); // pas oublier le "SDL_FreeSurface(surface)" pour liberer la memoire

    SDL_FillRect(Rectangle1, NULL, SDL_MapRGB(screen->format, 124, 40, 190)); // on colorie la surface, il ne manque plus que la coller (la "blitter")

    //SDL_BlitSurface(surface, NULL, surface_sur_laquelle_coller (ici l'écran), point_sur_coords) pour blitter une surface
    SDL_Rect Position;// SDL_Rect : struct pour creer des coords il contient x pour abscisse et y pour ordonnée
    Position.x = 640/2-100/2;
    Position.y = 480/2-200/2;
    /*blittage*/
    SDL_BlitSurface(Rectangle1,NULL,screen,&Position);// SDL_BlitSurface(surface, partie_de_la_surface, surface_sur_laquelle_posee, &position)

    SDL_Flip(screen);/* SDL_Flip(ecran) pour Mise à jour de l'écran */

    pause();

    SDL_FreeSurface(Rectangle1); // liberer memoire |   l'écran n'a pas besoin de libérer sa mem, sa se fait automatiquement au SDL_Quit
    SDL_Quit();
    return EXIT_SUCCESS; // EXIT_SUCCESS est un Define pour tout OS representant valeur à renvoyer en cas de réussite du programme.
}
