#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include "liste_chaine.h"

#define CMD_SETVARIABLE "set"

var_locale* listeVariables;

list_var setVariable(char* tabcmd, list_var variables);
char* gestionEspacesCommande(char** tabcmd);
/*
    Fonction qui va ajouter les chaines commencant par début et terminant par fin
    dans le tableau tab
*/
char** gererVariableDepuisCommande(char* commande, char** tab, char debut, char fin);

/*
    Fonction qui va ajouter les chaines terminant par fin dans le tableau tab
*/
char* gererVariableDepuisCommande2(char* commande, char tab[1024], char fin);

void afficherLesCommandesEntrees(char** commandes, int nbCommandes);

void afficherEnBrutLesCommandesEntrees(char** commandes, int nbCommandes);

#endif