#ifndef GESTION_CHAINE_H
#define GESTION_CHAINE_H

#define sizelgcmd 2048
#define sizeWord 1024

char** allouerMemoireCommandes();
void afficherLesCommandesEntrees(char** commandes, int nbCommandes);
char** demanderCommande(char** commandeSansEspaces, int* nbCommandes);
char** retirerEspaces(char commande[sizelgcmd], char** commandeSansEspaces, int* nbCommandes);
void viderCommande(char** commandes, int nbCommande) ;

#endif