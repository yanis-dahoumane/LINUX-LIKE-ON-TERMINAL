#include "fils.h"
#include <stdlib.h>

void ajouter_fils(noeud *parent, noeud *fils)
{
    liste_noeud *nouveau = malloc(sizeof(liste_noeud));
    if (nouveau == NULL)
    {
        printf("Erreur malloc \n");
        exit(2);
    }

    nouveau->no = fils;
    nouveau->succ = parent->fils;
    parent->fils = nouveau;
}

void retirer_fils(noeud *parent, noeud *fils) {
    liste_noeud *tmp = parent->fils;
    liste_noeud *prec = NULL;

    while (tmp != NULL) {
        if (tmp->no == fils) {
            if (prec == NULL) {
                parent->fils = tmp->succ;
            } else {
                prec->succ = tmp->succ;
            }

            free(tmp);
            return;
        }

        prec = tmp;
        tmp = tmp->succ;
    }
}

void afficher_fils(noeud *n) {
    liste_noeud *temp = n->fils;

    while (temp != NULL) {
        printf("%s ", temp->no->nom);
        temp = temp->succ;
    }
    printf("\n");
}

void copier_fils(noeud *n1, noeud *n2) {
    liste_noeud *courant = n1->fils;

    while(courant != NULL) {
        noeud *fils_actuel = courant->no;
        noeud *nouveau_fils = creer_noeud(courant->no->nom, courant->no->est_dossier, n2);
        ajouter_fils(n2, nouveau_fils);
        copier_fils(fils_actuel, nouveau_fils);
        courant = courant->succ;
    }
}