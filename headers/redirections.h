#ifndef REDIRECTION_H
#define REDIRECTION_H

#define SORTIE_FICHIER_ERASE 1
#define SORTIE_FICHIER_APPEND 2
#define SORTIE_ERREUR_ERASE 3
#define SORTIE_ERREUR_APPEND 4

#define STDOUT_ERASE " > "
#define STDOUT_APPEND " >> "

#define STDERR_ERASE " 2> "
#define STDERR_APPEND " 2>> "

void gererRedirection(char* commande);

// void ecrireFichierSortie(char* commande);

void fermerFichierSortie();

int contientRedirection(char* commande);

int mettreEnBackground(char* commande);

#endif