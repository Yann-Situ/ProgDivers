#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL_image.h>// pour SDL_image pour mettre des images autre que bmp
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "fonction_0.h"

// FONCTIONS
//__________________________
    //fonctions classique
    void checkerror(SDL_Surface *s, FILE *fichier, int type);
    Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y);
    void SetAlpha(SDL_Surface *surface, Uint8 alpha);
    int add_timer(C_timer *timer);
    int freeOBJF(OBJF *smain);
    //fonctions animation
    int animation(Sanim *anim);
    int clignot(SDL_Surface *s, int blink ,int ecart);
    int fondublanc(SDL_Surface *s, int blink, int ecart);
    int fondunoir(SDL_Surface *s, int blink, int ecart);
    Sanim* chargerAnim(Sanim *animSelect, int Nbl,int Nbt,int Nul,int Nut,int h,int w,int x,int y,int interv,char *tilesetName);
    //fonctions rect
    SDL_Rect recttoSDL(Srect r);
    Srect SDLtorect(SDL_Rect r);

//::::::::::::::::::::::::::
    //fonctions classique
    void checkerror(SDL_Surface *s, FILE *fichier, int type)
    {
        if(s == NULL&& type == SDL)
        {
            fprintf(stderr,"\tSDL error:%s!\n",SDL_GetError());
            exit(EXIT_FAILURE);
        }
        else if(s == NULL&& type == IMG)
        {
            fprintf(stderr,"\tIMG error:%s!\n",IMG_GetError());
            exit(EXIT_FAILURE);
        }
        else if(fichier == NULL&& type == FICHIERS)
        {
            fprintf(stderr,"\tFILE error, le fichier n'existe pas ou n'a pas été ouvert!\n");
            exit(EXIT_FAILURE);
        }
    }

    int add_timer(C_timer *timer)
    {
        int tmps = SDL_GetTicks();
        if(tmps < timer->tempsPre + timer->intervalle)
            return 0;
        else
        {
            timer->tempsPre = tmps;
            return 1;
        }
    }

    Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y)
    {
        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

        switch(bpp) {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
        }
    }

    void SetAlpha(SDL_Surface *surface, Uint8 alpha)
    {
        SDL_PixelFormat* fmt = surface->format;

        // If surface has no alpha channel, just set the surface alpha.
        if( fmt->Amask == 0 )
        {
            SDL_SetAlpha( surface, SDL_SRCALPHA, alpha );
        }
        // Else change the alpha of each pixel.
        else
        {
            unsigned bpp = fmt->BytesPerPixel;
            // Scaling factor to clamp alpha to [0, alpha].
            SDL_LockSurface(surface);
            int y,x;
            for(y = 0; y < surface->h; ++y)
            {
                for(x = 0; x < surface->w; ++x)
                {
                    // Get a pointer to the current pixel.
                    Uint32* pixel_ptr = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + x * bpp);
                    // Get the old pixel components.
                    Uint8 r, g, b, a;
                    SDL_GetRGBA(*pixel_ptr, fmt, &r, &g, &b, &a);
                    // Set the pixel with the new alpha.
                    *pixel_ptr = SDL_MapRGBA(fmt, r, g, b, alpha);
                }
            }
            SDL_UnlockSurface(surface);
        }
    }

    int freeOBJF(OBJF *smain)
    {
        fprintf(stderr,"*debut freeobjf*\n");
        TTF_CloseFont(smain->policeTitle);
        TTF_CloseFont(smain->police1);
        TTF_CloseFont(smain->police2);
        TTF_CloseFont(smain->police3);
        FMOD_System_Close(smain->systeme);// _____________________________
        FMOD_System_Release(smain->systeme);//pour liberer l'objet system
        free(smain);
        fprintf(stderr,"*fin freeobjf*\n");
        return 1;
    }

    //fonctions animation
    int animation(Sanim *anim)
    {
        if(anim->NumloopACT >= anim->NBloop && anim->NBloop != -1)
        {

        }
        else if(add_timer(&anim->timer))
        {
            anim->NumTileACT++;
            if(anim->NumTileACT > anim->NBTile-1)
            {
                anim->NumTileACT = 0;
                if(anim->NBloop != -1)
                    anim->NumloopACT ++;

            }
        }
        anim->rectTile.x = anim->rectTile.w * anim->NumTileACT;
        return anim->NumTileACT;
    }

    int clignot(SDL_Surface *s, int blink, int ecart)
    {
        blink += ecart;
        if(blink >= 255)
            blink = -255;
        SetAlpha(s,abs(blink));
        return blink;
    }

    int fondublanc(SDL_Surface *s, int blink, int ecart)
    {
        if(blink < 0)
            blink = -blink;
        if(blink < 255)
        {
            blink += ecart;
            if(blink > 255)
                blink = 255;
            SetAlpha(s,abs(blink));
        }
        return blink;
    }

    int fondunoir(SDL_Surface *s, int blink, int ecart)
    {
        if(blink > 0)
            blink = -blink;
        if(blink < 0)
        {
            blink += ecart;
            if(blink > 0)
                blink = 0;
            SetAlpha(s,abs(blink));
        }
        return blink;
    }

    Sanim* chargerAnim(Sanim *animSelect, int Nbl,int Nbt,int Nul,int Nut,int h,int w,int x,int y,int interv,char *tilesetName)
    {//                          NBloop , NBtiles , Numloop , numtile , rect tileset , intervalle timer , nom fichier
        animSelect->NBloop = Nbl; animSelect->NBTile = Nbt; animSelect->NumloopACT = Nul;  animSelect->NumTileACT = Nut;
        animSelect->rectTile.h = h;  animSelect->rectTile.w = w;  animSelect->rectTile.x = x;  animSelect->rectTile.y = y;
        animSelect->timer.intervalle = interv; animSelect->timer.tempsPre = 0;
        animSelect->sprites = IMG_Load(tilesetName);
        checkerror(animSelect->sprites,NULL,IMG);
        return animSelect;
    }

    //fonctions rect
    SDL_Rect recttoSDL(Srect r)
    {
        SDL_Rect s;
        s.h = r.h;
        s.w = r.w;
        s.x = r.x;
        s.y = r.y;
        return s;
    }

    Srect SDLtorect(SDL_Rect r)
    {
        Srect s;
        s.h = r.h;
        s.w = r.w;
        s.x = r.x;
        s.y = r.y;
        return s;
    }
