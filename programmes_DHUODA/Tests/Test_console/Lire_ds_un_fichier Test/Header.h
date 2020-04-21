#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#endif // HEADER_H_INCLUDED
//"r" : lecture seule. Vous pourrez lire le contenu du fichier, mais pas y �crire. Le fichier doit avoir �t� cr�� au pr�alable.
//"w" : �criture seule. Vous pourrez �crire dans le fichier, mais pas lire son contenu. Si le fichier n'existe pas, il sera cr��.
//"a" : mode d'ajout. Vous �crirez dans le fichier, en partant de la fin du fichier. Vous ajouterez donc du texte � la fin du
//      fichier. Si le fichier n'existe pas, il sera cr��.
//"r+": lecture et �criture. Vous pourrez lire et �crire dans le fichier. Le fichier doit avoir �t� cr�� au pr�alable.
//"w+": lecture et �criture, avec suppression du contenu au pr�alable. Le fichier est donc d'abord vid� de son contenu,
//      vous pouvez y �crire, et le lire ensuite. Si le fichier n'existe pas, il sera cr��.
//"a+": ajout en lecture / �criture � la fin. Vous �crivez et lisez du texte � partir de la fin du fichier. Si le fichier n'existe
//      pas, il sera cr��.

//fputc :   �crit un caract�re dans le fichier (UN SEUL caract�re � la fois) ;                          fputc(caractere,pointeur_fichier)
//fputs :   �crit une cha�ne dans le fichier ;                                                          fputs(chaine,pointeur_fichier)
//fprintf : �crit une cha�ne � format�e � dans le fichier, fonctionnement quasi-identique � printf.     fprintf(pointteur_fichier,-comme un printf-)

//fgetc : lit un caract�re ;                                                                            fgetc(pointeur_fichier)
//fgets : lit une cha�ne ;                                                                              fgets(chn_receveur,nb_carac,pointeur_fichier)
//fscanf : lit une cha�ne format�e.                                                                     fscanf(pointeur_fichier,-comme un scanf-)

//ftell : indique � quelle position vous �tes actuellement dans le fichier ;                            ftell(pointeur_fichier)
//fseek : positionne le curseur � un endroit pr�cis ;     deplacement negatif/nul/positif               fseek(pointeur_fichier,deplacement,origine)
//origine     SEEK_SET : indique le d�but du fichier ;  SEEK_CUR : indique la position actuelle du curseur ;    SEEK_END : indique la fin du fichier.
//rewind: remet le curseur au d�but du fichier (c'est �quivalent � demander � la fonction fseek de      rewind(pointeur_fichier)
//          positionner le curseur au d�but).

//rename : renomme un fichier ;                                                                         rename(ancien_nom,nouveau_nom)
//remove : supprime un fichier.                                                                         remove(fichier a suprimer)
