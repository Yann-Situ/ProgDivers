#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#endif // HEADER_H_INCLUDED
//"r" : lecture seule. Vous pourrez lire le contenu du fichier, mais pas y écrire. Le fichier doit avoir été créé au préalable.
//"w" : écriture seule. Vous pourrez écrire dans le fichier, mais pas lire son contenu. Si le fichier n'existe pas, il sera créé.
//"a" : mode d'ajout. Vous écrirez dans le fichier, en partant de la fin du fichier. Vous ajouterez donc du texte à la fin du
//      fichier. Si le fichier n'existe pas, il sera créé.
//"r+": lecture et écriture. Vous pourrez lire et écrire dans le fichier. Le fichier doit avoir été créé au préalable.
//"w+": lecture et écriture, avec suppression du contenu au préalable. Le fichier est donc d'abord vidé de son contenu,
//      vous pouvez y écrire, et le lire ensuite. Si le fichier n'existe pas, il sera créé.
//"a+": ajout en lecture / écriture à la fin. Vous écrivez et lisez du texte à partir de la fin du fichier. Si le fichier n'existe
//      pas, il sera créé.

//fputc :   écrit un caractère dans le fichier (UN SEUL caractère à la fois) ;                          fputc(caractere,pointeur_fichier)
//fputs :   écrit une chaîne dans le fichier ;                                                          fputs(chaine,pointeur_fichier)
//fprintf : écrit une chaîne « formatée » dans le fichier, fonctionnement quasi-identique à printf.     fprintf(pointteur_fichier,-comme un printf-)

//fgetc : lit un caractère ;                                                                            fgetc(pointeur_fichier)
//fgets : lit une chaîne ;                                                                              fgets(chn_receveur,nb_carac,pointeur_fichier)
//fscanf : lit une chaîne formatée.                                                                     fscanf(pointeur_fichier,-comme un scanf-)

//ftell : indique à quelle position vous êtes actuellement dans le fichier ;                            ftell(pointeur_fichier)
//fseek : positionne le curseur à un endroit précis ;     deplacement negatif/nul/positif               fseek(pointeur_fichier,deplacement,origine)
//origine     SEEK_SET : indique le début du fichier ;  SEEK_CUR : indique la position actuelle du curseur ;    SEEK_END : indique la fin du fichier.
//rewind: remet le curseur au début du fichier (c'est équivalent à demander à la fonction fseek de      rewind(pointeur_fichier)
//          positionner le curseur au début).

//rename : renomme un fichier ;                                                                         rename(ancien_nom,nouveau_nom)
//remove : supprime un fichier.                                                                         remove(fichier a suprimer)
