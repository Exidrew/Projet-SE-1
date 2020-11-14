#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/liste_chaine.h"

list_var ajouter(list_var liste, char nom[1024], char valeur[1024])
{
    var_locale* nouvelleVar_locale = calloc(1, sizeof(var_locale));
    for (int i = 0; i < strlen(nom); i++) nouvelleVar_locale->nom[i] = nom[i];
    for (int i = 0; i < strlen(valeur); i++) nouvelleVar_locale->valeur[i] = valeur[i];
    var_locale* variableExistante = var_existe(liste, nom);

    if (variableExistante != NULL){
        memset(variableExistante->valeur, '\0', strlen(variableExistante->valeur));
        for (int i = 0; i < strlen(valeur); i++) variableExistante->valeur[i] = valeur[i];

        free(nouvelleVar_locale);
        return liste;
    }

    nouvelleVar_locale->suivant = liste;
    return nouvelleVar_locale;
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
    return NULL;
}

list_var var_existe(list_var liste, char *nom)
{
    var_locale* tmp = liste;
    if(liste == NULL){
        return NULL; 
    }
    while(tmp != NULL && tmp->nom != NULL){
        if(strcmp(tmp->nom, nom) == 0)
            return tmp;
        tmp = tmp->suivant;
    }
    return NULL;
}

void afficher_variables(list_var liste)
{
    var_locale* tmp = liste;
    if(tmp == NULL){
        printf("pas de variable locale en memoire\n");
    }
    else{
        while (tmp != NULL) {
            printf("%s : %s\n", tmp->nom, tmp->valeur);
            tmp = tmp->suivant;
        }
    }
}

int tailleChaine(var_locale* liste) {
    var_locale* tmp = liste;
    int total = 0;
    if(liste == NULL){
        return total;
    }
    else{
        while(tmp != NULL){
            total++;
            tmp = tmp->suivant;
        }
    }
    return total;
}

void freeVariables(list_var liste) {
    list_var tmp;
    while (liste != NULL) {
        tmp = liste;
        liste = liste->suivant;
        free(tmp);
    }
}
