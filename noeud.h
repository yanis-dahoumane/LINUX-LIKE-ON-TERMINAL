#include <stdbool.h>
#include <stdio.h>

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

struct noeud {
    bool est_dossier;
    char nom[100];
    noeud* pere;
    noeud* racine;
    liste_noeud* fils;
};

struct liste_noeud {
    noeud* no;
    liste_noeud* succ;
};

noeud* creer_noeud(char* nom, bool est_dossier, noeud* pere);

