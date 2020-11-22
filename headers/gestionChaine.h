#ifndef GESTION_CHAINE_H
#define GESTION_CHAINE_H

#define sizelgcmd 2048
#define sizeWord 1024

char** allouerMemoireCommandes();
void afficherLesCommandesEntrees(char** commandes, int nbCommandes);
void afficherEnBrutLesCommandesEntrees(char** commandes, int nbCommandes);
char** demanderCommande(char** commandeSansEspaces, int* nbCommandes);
char** retirerEspaces(char commande[sizelgcmd], char** commandeSansEspaces, int* nbCommandes);
char* retirerEspaceCommandeUnique(char* commande);
char** remplacerLesVariablesDansLesCommandes(char** commandes, int nbCommandes, int* status);
void viderCommande(char** commandes);
int getPwd(char *repertory);
int getComputerName(char *hostName);
void affichageLigneShell();
#endif