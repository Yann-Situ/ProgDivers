#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp


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
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur stderr fichier auto pour les erreurs
        exit(EXIT_FAILURE); // On quitte le programme                                // SDL_GetError() renvoie derniere erreur de la SDL
    }//                     EXIT_FAILURE est un Define pour tout OS representant valeur à renvoyer en cas d'échec du programme
    SDL_Surface *screen = NULL, *Zozor = NULL;
    SDL_Event Event;
    int Continuer = 1, i = 0;
    SDL_Rect PositionZozor;
    PositionZozor.x = 800/2-32/2;
    PositionZozor.y = 600/2-32/2;

    SDL_WM_SetIcon(IMG_Load("link_icon.png"), NULL);/*
    SDL_WM_SetIcon(Surface_qui_contient_l'image, info_sur_la_transparence) pour changer l'icone de la fenetre
    /!\ le faire avant SDL_SetVideoMode !   */
    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);// SDL_DOUBLEBUF pour enlever les scintillement de l'ecran
    SDL_WM_SetCaption("Gerer les evenement en SDL", NULL);

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,200,200));

    Zozor = IMG_Load("Zozor.bmp");
    SDL_SetColorKey(Zozor, SDL_SRCCOLORKEY, SDL_MapRGB(Zozor->format, 0, 0,255));
    SDL_BlitSurface(Zozor,NULL,screen, &PositionZozor);

    SDL_Flip(screen);

    SDL_EnableKeyRepeat(500, 10);// SDL_EnableKeyRepeat(delai avant repet°(ms), delai entre chaque repet°(ms)) pour activer la repet de touche

    while (Continuer)
    {
        SDL_WaitEvent(&Event); /* Récupération de l'événement dans event
*/
        switch(Event.type) /* Test du type d'événement */
        {
            case SDL_QUIT: /* Si c'est un événement de type "Quitter" */
                Continuer = 0;
                break;
            case SDL_KEYDOWN:// appuyage de touche au clavier
                switch(Event.key.keysym.sym)// type de touche au clavier
                {
                    case SDLK_ESCAPE:// SDLK_ESCAPE define qui represente la touche "Echap"
                        Continuer = 0;
                        break;
                    case SDLK_UP:
                        PositionZozor.y --;
                        break;
                    case SDLK_DOWN:
                        PositionZozor.y ++;
                        break;
                    case SDLK_LEFT:
                        PositionZozor.x --;
                        break;
                    case SDLK_RIGHT:
                        PositionZozor.x ++;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP: // pour relachement de clic
                switch(Event.button.button)// type de clic (gauche droit milieu...)
                {
                    case SDL_BUTTON_MIDDLE:
                        Continuer = 0;
                        break;
                    case SDL_BUTTON_LEFT:
                        PositionZozor.x = Event.button.x;
                        PositionZozor.y = Event.button.y;// event.button.x ou y pour les coords de la souris
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                PositionZozor.x = Event.motion.x;
                PositionZozor.y = Event.motion.y;// event.motion.x ou y pour les coord de la souris /!\ pas pareil que MOUSEBUTTON !
                break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, i/5,i/5,i/5));
        SDL_BlitSurface(Zozor,NULL,screen, &PositionZozor);
        SDL_Flip(screen);
        i++;
        if(i>255*5)
            Continuer = 0;
    }

    SDL_FreeSurface(Zozor);
    SDL_Quit();
    return 0;
}


/*              AUTRE FONCTION UTILISABLE ET UTILE
* SDL_ShowCursor(SDL_DISABLE); ou SDL_ShowCursor(SDL_ENABLE); pour masquer la souris ds la fenetre (a faire avant la boucle principale)
* SDL_WarpMouse(x, y); pour placer la souris à un endroit précis \ par exemple : SDL_WarpMouse(ecran->w / 2, ecran->h / 2); pour au milieu

* Avec SDL_RESIZABLE la fenetre peut changer de taille, si cest le cas, un événement de type SDL_VIDEORESIZE est généré;
    On peut récupérer : la nouvelle largeur dans event.resize.w et la nouvelle hauteur dans event.resize.h

* L'événement SDL_ACTIVEEVENT est généré lorsque la visibilité de la fenêtre change. Cela peut être dû à de nombreuses choses :
    * la fenêtre est réduite en barre des tâches ou restaurée ;
    * le curseur de la souris se trouve à l'intérieur de la fenêtre ou en sort ;
    * la fenêtre est active (au premier plan) ou n'est plus active.
                On peut récuperer :
    * event.active.gain, 0 si elle n'est plus au 1er plan et 1 si elle l'est devenue
    * event.active.state qui contient des flags avec les évenements produits pour la perte ou le gain :
                                * SDL_APPMOUSEFOCUS: le curseur de la souris vient de rentrer ou de sortir de la fenêtre.
                                * SDL_APPINPUTFOCUS : l'application vient de recevoir le focus du clavier ou de le perdre.
                                * SDL_APPACTIVE : l'application a ete réduite dans la barre des tâches ou remise dans son état normal (gain = 1).
    /!\ attention, code difficile !
    par exemple: "if ((event.active.state & SDL_APPMOUSEFOCUS) == SDL_APPMOUSEFOCUS)" ou "if ((event.active.state & SDL_APPACTIVE) == SDL_APPACTIVE)"
        ::: code pour mettre en pause lorsque la fenetre est réduite :
            if ((event.active.state & SDL_APPACTIVE) == SDL_APPACTIVE)
            {
                if (event.active.gain == 0)
                    pause = 1;
                else if (event.active.gain == 1)
                    pause = 0;
            }
            il faut aussi rajouter un moyen de tester et de mettre la pause
        */
