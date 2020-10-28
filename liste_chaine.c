#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/liste_chaine.h"

list_var ajouterEnFin(list_var liste, char *nom, char *valeur)
{
    var_locale* nouvelleVar_locale = malloc(sizeof(var_locale));
    nouvelleVar_locale->nom = nom;
    nouvelleVar_locale->valeur = valeur;
    var_locale* variableExistante = var_existe(liste, nom);
    if(variableExistante != NULL){
        variableExistante->valeur = valeur;
        return liste;
    }
    if(liste == NULL)
    {
        return nouvelleVar_locale;
    }
    else
    {
        nouvelleVar_locale->suivant = liste;
        return nouvelleVar_locale;
    }
}

list_var supprimerVar(list_var liste, char *nom){
    var_locale* tmp = liste;
    var_locale* ptmp = liste;
    if(liste == NULL)
        return NULL;
 
    if(liste->suivant == NULL)
    {
        if(strcmp(liste->nom, nom) == 0)
        {
            free(liste);
            return NULL;
        }
    }
    if(strcmp(liste->nom, nom) == 0)
    {
        liste = liste->suivant;
        free(tmp);
        return liste;
    }
    while(tmp != NULL)
    {
        ptmp = tmp;
        tmp = tmp->suivant;
        if(strcmp(tmp->nom, nom) == 0){
            if(tmp->suivant == NULL){
                ptmp->suivant = NULL;
                free(tmp);
                return liste;
            }
            else {
                ptmp->suivant = tmp->suivant;
                free(tmp);
                return liste;
            }
        }
    }
}

list_var var_existe(list_var liste, char *nom)
{
    var_locale* tmp = liste;
    if(liste == NULL){
        return NULL; 
    }
    while(tmp != NULL){
        if(strcmp(tmp->nom, nom) == 0)
            return tmp;
        tmp = tmp->suivant;
    }
    return NULL;
}

void afficher_variables(list_var liste)
{
    var_locale* tmp = liste;
    if(liste == NULL){
        printf("pas de variable locale en memoire\n");
    }
    else{
        while(tmp != NULL){
            printf("%s : %s\n",tmp->nom, tmp->valeur);
            tmp = tmp->suivant;
        }
    }
}
