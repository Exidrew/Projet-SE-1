#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include "liste_chaine.h"

#define CMD_SETVARIABLE "set"
#define CMD_DELVARIABLE "del"

var_locale* listeVariables;

void setVariableLocale(char* tabcmd, TableauVariables* variables);

void delVariableLocale(char* tabcmd, TableauVariables* variables);

char* gestionEspacesCommande(char** tabcmd);

/*
    Fonction qui va ajouter les chaines terminant par fin dans le tableau tab
*/
char* gererVariableLocaleDepuisCommande(char* commande, char tab[1024], char fin);

void afficherLesCommandesEntrees(char** commandes, int nbCommandes);

void afficherEnBrutLesCommandesEntrees(char** commandes, int nbCommandes);

#endif