#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commande.h"

void mkdir_cmd(noeud *courant, char *nom)
{

    liste_noeud *temp = courant->fils;

    while (temp != NULL)
    {
        if (strcmp(temp->no->nom, nom) == 0)
        {
            printf("XXX déjà existant XXX\n");
            return;
        }
        temp = temp->succ;
    }

    noeud *n = creer_noeud(nom, true, courant);
    ajouter_fils(courant, n);
    printf("Le dossier %s a bien été crée !\n", nom);
}

void mkdir_cmd_p(noeud *courant, char *chemin)
{
    char tmp[1000];
    strcpy(tmp, chemin);
    char *partie = strtok(tmp, "/");

    while (partie != NULL)
    {
        mkdir_cmd(courant,partie);
        cd_chemin_cmd(&courant,partie);
    }

    cd_cmd(&courant);
}

void touch_cmd(noeud *courant, char *nom)
{

    liste_noeud *temp = courant->fils;

    while (temp != NULL)
    {
        if (strcmp(temp->no->nom, nom) == 0)
        {
            printf("XXX déjà existant XXX\n");
            return;
        }
        temp = temp->succ;
    }

    noeud *n = creer_noeud(nom, false, courant);
    ajouter_fils(courant, n);
    printf("Le fichier %s a bien été crée !\n", nom);
}

void ls_cmd(noeud *courant)
{
    afficher_fils(courant);
}

void pwd_cmd(noeud *courant)
{
    if (courant == courant->pere)
    {
        printf("/");
        return;
    }

    pwd_cmd(courant->pere);
    printf("%s/", courant->nom);
}

void cd_cmd(noeud **courant)
{
    *courant = (*courant)->racine;
}

// une fonction chemin qui permets de retrouver le noeud (Idée reçue de la part d’un camarade, Nantha Sorubakantan, mais il n’y a pas eu d’échange de code.)
noeud *chemin_cd(noeud *courant, char *chemin)
{
    noeud *actuel;
    int trouve;

    if (chemin[0] == '/')
    {
        actuel = courant->racine;
    }

    else
    {
        actuel = courant;
    }

    char tmp[1000];
    strcpy(tmp, chemin);
    char *partie = strtok(tmp, "/");

    while (partie != NULL)
    {
        if (strcmp(partie, ".") == 0)
        {
            partie = strtok(NULL, "/");
        }

        else if (strcmp(partie, "..") == 0)
        {
            actuel = actuel->pere;
        }

        else
        {
            liste_noeud *fils = actuel->fils;
            trouve = 0;

            while (fils != NULL)
            {
                if (strcmp(fils->no->nom, partie) == 0)
                {
                    actuel = fils->no;
                    trouve = 1;
                    break;
                }
                fils = fils->succ;
            }
        }

        if (!trouve)
        {
            printf("XXX chemin pas possible XXX\n");
            return NULL;
        }

        if (!actuel->est_dossier)
        {
            printf("XXX pas un dossier XXX\n");
            return NULL;
        }

        partie = strtok(NULL, "/");
    }

    return actuel;
}

// la meme fonction mais qui accepte les fichiers
noeud *chemin_mv(noeud *courant, char *chemin)
{
    noeud *actuel;
    int trouve;

    if (chemin[0] == '/')
    {
        actuel = courant->racine;
    }

    else
    {
        actuel = courant;
    }

    char tmp[1000];
    strcpy(tmp, chemin);
    char *partie = strtok(tmp, "/");

    while (partie != NULL)
    {
        if (strcmp(partie, ".") == 0)
        {
            partie = strtok(NULL, "/");
        }

        else if (strcmp(partie, "..") == 0)
        {
            actuel = actuel->pere;
        }

        else
        {
            liste_noeud *fils = actuel->fils;
            trouve = 0;

            while (fils != NULL)
            {
                if (strcmp(fils->no->nom, partie) == 0)
                {
                    actuel = fils->no;
                    trouve = 1;
                    break;
                }
                fils = fils->succ;
            }
        }

        if (!trouve)
        {
            printf("XXX chemin pas possible XXX\n");
            return NULL;
        }

        partie = strtok(NULL, "/");
    }

    return actuel;
}

void cd_parent_cmd(noeud **courant)
{
    *courant = (*courant)->pere;
}

void cd_chemin_cmd(noeud **courant, char *chemin)
{
    noeud *tmp = chemin_cd(*courant, chemin);
    if (tmp!=NULL)
    {
        *courant=tmp;
    }
}

void mv_cmd(noeud *courant, char *chem, char *chem2)
{
    noeud *base = chemin_mv(courant, chem);

    char copie[1000];
    strcpy(copie, chem2);

    char *dernier_slash = strrchr(copie, '/');

    noeud *nouveau_parent;
    char *nouveau_nom;

    if (dernier_slash == NULL)
    {
        nouveau_parent = courant;
        nouveau_nom = copie;
    }
    else
    {
        nouveau_nom = dernier_slash + 1;

        if (dernier_slash == &copie[0])
        {
            nouveau_parent = courant->racine;
        }
        else
        {
            *dernier_slash = '\0';
            nouveau_parent = (strcmp(copie, "..") == 0) ? courant->pere : chemin_cd(courant, copie);
        }
    }

    retirer_fils(base->pere, base);

    strcpy(base->nom, nouveau_nom);
    base->pere = nouveau_parent;

    ajouter_fils(nouveau_parent, base);
}

// fonction utilitaire pour le rm
void liberer_sous_arbre(noeud *racine)
{
    if (racine == NULL)
        return;
    liste_noeud *tmp = racine->fils;
    while (tmp != NULL)
    {
        liste_noeud *suivant = tmp->succ;
        liberer_sous_arbre(tmp->no);
        free(tmp);
        tmp = suivant;
    }
    free(racine);
}

// rm
void rm_cmd(noeud *courant, char *path)
{
    noeud *cible;
    noeud *parent;

    cible = chemin_mv(courant, path);

    if (cible == NULL)
        return ;

    if (cible == cible->racine)
    {
        printf("XXX impossible de supprimer la racine XXX\n");
        return ;
    }

    parent = cible->pere;

    retirer_fils(parent, cible);
    liberer_sous_arbre(cible);
}

void cp_cmd(noeud *courant, char *chem, char *chem2)
{

    noeud *base = chemin_mv(courant, chem);

    char copie[1000];
    strcpy(copie, chem2);

    char *dernier_slash = strrchr(copie, '/');

    noeud *nouveau_parent;
    char *nouveau_nom;

    if (dernier_slash == NULL)
    {
        nouveau_parent = courant;
        nouveau_nom = copie;
    }
    else
    {
        nouveau_nom = dernier_slash + 1;

        if (dernier_slash == &copie[0])
        {
            nouveau_parent = courant->racine;
        }
        else
        {
            *dernier_slash = '\0';
            nouveau_parent = (strcmp(copie, "..") == 0) ? courant->pere : chemin_cd(courant, copie);
        }
    }
    
    noeud *nouveau_noeud = creer_noeud(nouveau_nom, base->est_dossier, nouveau_parent);
    ajouter_fils(nouveau_parent, nouveau_noeud);
    copier_fils(base, nouveau_noeud);
}

void print_cmd(noeud *courant)
{
    static int niveau = 0;

    if (courant == NULL)
    {
        return;
    }

    for (int i = 0; i < niveau; i++)
    {
        printf("\t");
    }

    printf("%s %s\n", courant->est_dossier ? "Dossier " : "Fichier ", strcmp(courant->nom, "") == 0 ? "racine" : courant->nom);

    liste_noeud *actuel = courant->fils;

    niveau++;
    while (actuel != NULL)
    {
        print_cmd(actuel->no);
        actuel = actuel->succ;
    }
    niveau--;
}

void find(noeud *courant, char *cible)
{
    if (courant == NULL)
    {
        return;
    }
    if (strcmp(courant->nom, cible) == 0)
    {
        pwd_cmd(courant);
        printf("\n");
    }
    liste_noeud *tmp = courant->fils;

    while (tmp != NULL)
    {
        liste_noeud *suivant = tmp->succ;
        find(tmp->no, cible);
        tmp = suivant;
    }
}

void help_cmd()
{

    printf("ls\n");
    printf("Affiche le contenu du dossier courant.\n\n");

    printf("pwd\n");
    printf("Affiche le chemin courant.\n\n");

    printf("mkdir <nom>\n");
    printf("Cree un dossier.\n\n");

    printf("touch <nom>\n");
    printf("Cree un fichier.\n\n");

    printf("cd <chemin>\n");
    printf("Change de dossier.\n\n");

    printf("cd ..\n");
    printf("Va dans le dossier parent.\n\n");

    printf("cd\n");
    printf("Retourne a la racine.\n\n");

    printf("rm <chemin>\n");
    printf("Supprime un fichier ou dossier.\n\n");

    printf("mv <source> <destination>\n");
    printf("Deplace ou renomme un fichier/dossier.\n\n");

    printf("cp <source> <destination>\n");
    printf("Copie un fichier ou dossier.\n\n");

    printf("find <nom>\n");
    printf("Recherche un fichier ou dossier.\n\n");

    printf("print\n");
    printf("Affiche tout l'arbre.\n\n");

}


//////////// PARSER /////////////


void parser_ligne(noeud **courant, char *ligne)
{
    char *commande = strtok(ligne, " \t\r\n");

    printf("\nCOMMANDE ACTUEL : %s\n", commande);

    if (strcmp(commande, "ls") == 0)
    {
        ls_cmd(*courant);
    }

    else if (strcmp(commande, "touch") == 0)
    {
        char *nom = strtok(NULL, " \t\r\n");
        touch_cmd(*courant, nom);
    }

    else if (strcmp(commande, "mkdir") == 0)
    {
        char *nom = strtok(NULL, " \t\r\n");
        mkdir_cmd(*courant, nom);
    }

    else if (strcmp(commande, "pwd") == 0)
    {
        pwd_cmd(*courant);
        printf("\n");
    }

    else if (strcmp(commande, "print") == 0)
    {
        print_cmd(*courant);
    }

     else if (strcmp(commande, "help") == 0)
    {
        help_cmd();
    }

    else if (strcmp(commande, "rm") == 0)
    {
        char *nom = strtok(NULL, " \t\r\n");
        rm_cmd(*courant, nom);
    }

    else if (strcmp(commande, "cp") == 0)
    {
        char *chem = strtok(NULL, " \t\r\n");
        char *chem2 = strtok(NULL, " \t\r\n");
        cp_cmd(*courant, chem, chem2);
        printf("Copie effectué !\n");
    }

    else if (strcmp(commande, "mv") == 0)
    {
        char *chem = strtok(NULL, " \t\r\n");
        char *chem2 = strtok(NULL, " \t\r\n");
        mv_cmd(*courant, chem, chem2);
        printf("Mv effectué !\n");
    }

    else if (strcmp(commande, "find") == 0)
    {
        char *chem = strtok(NULL, " \t\r\n");
        find((*courant)->racine, chem);
    }

    else if (strcmp(commande, "cd") == 0)
    {
        char *nom = strtok(NULL, " \t\r\n");

        if (nom == NULL)
        {
            cd_cmd(courant);
            printf("CD effectué ! Noeud actuel : %s\n", (*courant)->nom);
        }

        else if (strcmp(nom, "..") == 0)
        {
            cd_parent_cmd(courant);
            printf("CD effectué ! Noeud actuel : %s\n", (*courant)->nom);
        }

        else
        {
            cd_chemin_cmd(courant, nom);
            printf("CD effectué ! Noeud actuel : %s\n", (*courant)->nom);
        }
    }
    else {
           printf("commande inexistante\n");
        }
}

void parser_fichier(noeud **courant, char *nom_fichier)
{
    FILE *fichier = fopen(nom_fichier, "r");

    char ligne[1000];

    if (fichier == NULL)
    {
        printf("XXX impossible d'ouvrir le fichier :%s car inexistant XXX'.\n", nom_fichier);
        return;
    }

    while (fgets(ligne, 1000, fichier) != NULL)
    {
        parser_ligne(courant, ligne);
    }
    fclose(fichier);
}