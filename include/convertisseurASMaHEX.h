#ifndef CONVERTISSEURASMAHEX_H
#define CONVERTISSEURASMAHEX_H

char* ASMtoHex(char* nomFichier);

int instructionVersToken(char* str, char** token);

void supprimeCaracteresVide(char* str);

int contientChar(char* str, char c);

int getNombreLigneNonVide(FILE* fptr);

int estVide(char* ligne);

char** extractionEtiquette(FILE* fptr);

int getIndex(char* str,char* T[],int sizeT);

int getInstructionId(char* instruction);

int getRegistreId(char* registreArg);

int estValeurImmediate(char* S);

int getValeurImmediateOuRegistreId(char* S);

long long int encodageInstruction(char* args[], char* etiquettes[], int tailleProgramme);

void afficheMessageErreur(long long int codeErreur, int numLigne, char* ligne);

int creationFichier(char* nomFichier, char* contenu);

#endif