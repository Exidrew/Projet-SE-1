#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include "liste_chaine.h"

#define CMD_SETVARIABLE "set"
#define CMD_DELVARIABLE "del"

var_locale* listeVariables;

/* Ajout d'une variable locale en fonction d'une commande */
void setVariableLocale(char* tabcmd, TableauVariables* variables);

/* Suppression d'une variable locale en fonction d'une commande */
void delVariableLocale(char* tabcmd, TableauVariables* variables);

/*
    Fonction qui va ajouter les chaines terminant par fin dans le tableau tab
*/
char* gererVariableLocaleDepuisCommande(char* commande, char tab[1024], char fin);

#endif