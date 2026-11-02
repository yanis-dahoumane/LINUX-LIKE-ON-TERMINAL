#include "fils.h"

void mkdir_cmd(noeud* courant, char* nom);
void touch_cmd(noeud* courant, char* nom);
void ls_cmd(noeud* courant);
void cd_cmd(noeud** courant);
void cd_chemin_cmd(noeud **courant, char *chemin);
void cd_parent_cmd(noeud** courant);
noeud *chemin_cd(noeud *courant, char *chemin);
noeud *chemin_mv(noeud *courant, char *chemin);
void print_cmd(noeud* courant);
void mv_cmd(noeud *courant, char *chem, char *chem2);
void liberer_sous_arbre(noeud *racine);
void rm_cmd(noeud *courant, char *path);
void cp_cmd(noeud *courant, char *chem, char *chem2);
void parser_ligne(noeud **courant, char *ligne);
void parser_fichier(noeud **courant, char *fichier);
void print_cmd(noeud *courant);
void find(noeud *racine,char *chemin);
void mkdir_cmd_p(noeud *courant, char *chemin);
void pwd_cmd(noeud *courant);