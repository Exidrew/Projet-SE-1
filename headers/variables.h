#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include "liste_chaine.h"

#define CMD_SETVARIABLE "set"

list_var listeVariables;

int verifierCommande(char** commande);
int setVariable(char** tabcmd);

#endif