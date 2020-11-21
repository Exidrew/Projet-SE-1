#ifndef DEF_VARIABLES
#define DEF_VARIABLES

#include "variables.h"

#define CMD_SETVARIABLE "set"
#define CMD_DELVARIABLE "del"

/* Ajout d'une variable locale en fonction d'une commande */
int setVariableLocale(char* tabcmd);

/* Suppression d'une variable locale en fonction d'une commande */
int delVariableLocale(char* tabcmd);

/*
    Fonction qui va ajouter les chaines terminant par fin dans le tableau tab
*/
char* gererVariableLocaleDepuisCommande(char* commande, char tab[1024], char fin);

#endif