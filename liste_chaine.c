#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/liste_chaine.h"

void main(int argc, char **argv)
{
	list_var liste_variables = NULL;
	liste_variables = ajouterEnFin(liste_variables, "a", "foo");
	liste_variables = ajouterEnFin(liste_variables, "b", "sebBG");
	liste_variables = ajouterEnFin(liste_variables, "d", "leoBG");
	liste_variables = ajouterEnFin(liste_variables, "c", "2eme test");
	var_locale *test = var_existe(liste_variables, "aff");
	if(test != NULL)
		printf("var_existe  %s : %s\n", test->nom, test->valeur);
	else printf("variable introuvable\n");
	var_locale *test2 = var_existe(liste_variables, "c");
	if(test2 != NULL)
		printf("var_existe  %s : %s\n", test2->nom, test2->valeur);
	else printf("variable introuvable\n");
	liste_variables = supprimerVar(liste_variables, "d");
	afficher_variables(liste_variables);
}

list_var ajouterEnFin(list_var liste, char *nom, char *valeur)
{
    var_locale* nouvelleVar_locale = malloc(sizeof(var_locale));
    nouvelleVar_locale->nom = nom;
    nouvelleVar_locale->valeur = valeur;
    nouvelleVar_locale->suivant = NULL;
 
    if(liste == NULL)
    {
        return nouvelleVar_locale;
    }
    else
    {
        var_locale* temp = liste;
        if(temp->suivant == NULL){
        	temp->suivant = nouvelleVar_locale;
        	return liste;
        }
        while(temp->suivant != NULL)
        {
            temp = temp->suivant;
        }
        temp->suivant = nouvelleVar_locale;
        return liste;
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
