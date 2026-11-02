#include <stdlib.h>
#include <string.h>
#include "noeud.h"

noeud *creer_noeud(char *nom, bool est_dossier, noeud *pere)
{
    noeud *n = malloc(sizeof(noeud));

    if (n == NULL)
    {
        printf("Erreur malloc\n");
        exit(2);
    }

    n->est_dossier = est_dossier;
    strcpy(n->nom, nom);
    n->pere = (pere == NULL) ? n : pere;
    n->racine = (pere == NULL) ? n : pere->racine;
    n->fils = NULL;

    return n;
}
