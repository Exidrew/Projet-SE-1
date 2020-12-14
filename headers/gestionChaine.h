#ifndef GESTION_CHAINE_H
#define GESTION_CHAINE_H

#define sizelgcmd 2048
#define sizeWord 1024

/* Alloue une mémoire suffisante pour les commandes passées au tinyshell */
char** allouerMemoireCommandes();

/* Affiche les commande qu'on vient d'entrer dans le shell formaté comme une liste*/
void afficherLesCommandesEntrees(char** commandes, int nbCommandes);

/* Affiche les commandes qu'on vient d'entrer dans le shell sans formatage */
void afficherEnBrutLesCommandesEntrees(char** commandes, int nbCommandes);

/* Demande une commande qu'on entre dans le double pointeur commande
Retourne :
- La commande entrée
- Le nombre de commande dans le pointeur passé en parametre */
char** demanderCommande(char** commande, int* nbCommandes);

/* Retire les espaces de la commande commandeSansEspaces passée en parametre
Retourne :
- La commande formatée */
char** retirerEspaces(char commande[sizelgcmd], char** commandeSansEspaces, int* nbCommandes);

/* Remplace les variables dans les commandes et indique le status dans le pointeur status
passé en paramètres
Retour :
- Les commandes formatées et avec les noms des variables remplacées par leur valeur
- Le status */
char** remplacerLesVariablesDansLesCommandes(char** commandes, int nbCommandes, int* status);

/* Vide la commande pour accueillir une nouvelle commande par la suite */
void viderCommande(char** commandes);

/* Get le chemin du repertory/directory courant
Retourne :
- 1 si fail, 0 si succès
- Le chemin dans le char* repertory */
int getPwd(char *repertory);

/* Get le nom de l'ordinateur
Retourne :
- 1 si fail, 0 si succès
- Le nom dans le char* hostName */
int getComputerName(char *hostName);

/* Affiche le nom de l'ordinateur et le chemin dans le shell */
void affichageLigneShell();

int recupererArguments(char* args[], char* commande);

void recupererNomProgramme(char nomProgramme[100], char* commande);
#endif