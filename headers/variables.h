#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include "liste_chaine.h"

#define CMD_SETVARIABLE "set"

list_var listeVariables;

int verifierCommande(char** tabcmd);
int setVariable(char** tabcmd);
char* gestionEspacesCommande(char** tabcmd);
/*
    Fonction qui va ajouter les chaines commencant par début et terminant par fin
    dans le tableau tab
*/
char** gererVariableDepuisCommande(char* commande, char** tab, char debut, char fin);

#endif