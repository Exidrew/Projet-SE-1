#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include "liste_chaine.h"

#define CMD_SETVARIABLE "set"

var_locale* listeVariables;

int verifierCommande(char** tabcmd);
list_var setVariable(char** tabcmd, list_var variables);
char* gestionEspacesCommande(char** tabcmd);
/*
    Fonction qui va ajouter les chaines commencant par début et terminant par fin
    dans le tableau tab
*/
char** gererVariableDepuisCommande(char* commande, char** tab, char debut, char fin);

#endif