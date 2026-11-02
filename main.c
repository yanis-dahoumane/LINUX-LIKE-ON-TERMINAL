#include <stdio.h>
#include <string.h>
#include "commande.h"

int main(int argc, char **argv)
{
    noeud *racine = creer_noeud("racine", true, NULL);
    noeud *courant = racine;

    if (argc > 1)
    {
        parser_fichier(&courant, argv[1]);
    }
    else
    {
        char ligne[1000];
        while (1)
        {
            pwd_cmd(courant);
            printf(" : ");
            if (fgets(ligne, sizeof(ligne), stdin) == NULL)
            {
                break;
            }
            if (strcmp(ligne, "exit\n") == 0)
            {
                break;
            }
            parser_ligne(&courant, ligne);
        }
    }

    liberer_sous_arbre(racine);

    return 0;
}