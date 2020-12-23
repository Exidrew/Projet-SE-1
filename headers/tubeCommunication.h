#ifndef TUBE_COMMUNICATION
#define TUBE_COMMUNICATION

int creerTubeDeCommunication(int tubeDescriptor[2]);
void ecrireVersSortieStandard(int tubeDescriptor[2]);
void lireDepuisEntreeStandard(int tubeDescriptor[2]);
void fermerCoteTubes(int nbArgs, ...);
void fermerTube(int tubeDescriptor[2]);
void lireDepuisTube(int tubeDescriptor[2], char* str, int maxLength);
void ecrireVersTube(int tubeDescriptor[2], char* str);
void lireEcrireVersStandard(int tubeDescriptor[2]);

/*
    Avec argc > 0 et argv[1] = "l", elle va lire les arguments dans le fichier
    fileDescriptor et les mettre dans la chaine arguments.
    Il faudra au moins allouer chaine à une taille de 1, la fonction va
    realloc d'elle même quand nécessaire. Cela a pour but de pouvoir
    free la chaine plus simplement.
*/
void lectureArgumentsDepuisFd(char* arguments, int fileDescriptor, int argc, char* argv[]);

#endif