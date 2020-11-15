#ifndef TUBE_COMMUNICATION
#define TUBE_COMMUNICATION

int creerTubeDeCommunication(int tubeDescriptor[2]);
void ecrireVersSortieStandard(int tubeDescriptor[2]);
void lireDepuisEntreeStandard(int tubeDescriptor[2]);
void fermerCoteTubes(int nbArgs, ...);
void fermerTube(int tubeDescriptor[2]);
void lireDepuisTube(int tubeDescriptor[2], char* str, int maxLength);
void ecrireVersTube(int tubeDescriptor[2], char* str);
void ecrireVariableVersTube(int tubeDescriptor[2], TableauVariables* tab);
void lireVariableDepuisTube(int tubeDescriptor[2], TableauVariables* tab);

#endif