
#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "fonctionsASM.h"




Machine* newMachine(){

	Machine* machine = (Machine*)malloc(sizeof(Machine));
	if (machine == NULL){
		return NULL;
	}
	machine->PC = 0;
	machine->Z  = 0;
	machine->N  = 0;
	machine->C  = 0;

	char* memoire = malloc(65536 * sizeof(char));

	short int* registres = malloc(32 * sizeof(short int));

	if (memoire==NULL || registres==NULL){
		return NULL;
	}

	for (int i = 0; i < 32; ++i)
	{
		registres[i] = 0;
	}

	for (int i = 0; i < 65536; ++i)
	{
		memoire[i] = 0;
	}

	machine->memoire = memoire;
	machine->registre = registres;
	return machine;

}

int Machine_recupereMemoire(Machine* machine, int adresseDebut, int tailleMot){
	/*
		recupere dans la memoire les n octets specifié ds tailleMot et les revoies

		fonctionnement:
		on remplie motMem avec le nb d'octet specifier ex:
		si tailleMot = 4
		et machine->memoire[adresseDebut  ] = 1010 1111 
		et machine->memoire[adresseDebut+1] = 0001 1000
		et machine->memoire[adresseDebut+2] = 1100 0011
		et machine->memoire[adresseDebut+3] = 0111 1000
		alors 
		motMem =   m[i+0]  |  m[i+1]   |  m[i+2]  | m[i+3]
		motMem = 1010 1111   0001 1000   1100 0011   0111 1000


	*/


	if (adresseDebut+tailleMot > 65536){
		return -1;
	}

	int motMem = 0;

	for (int i = 0; i < tailleMot; ++i)
	{	

		motMem |= (unsigned char)machine->memoire[adresseDebut+i] << 8*(tailleMot - i - 1);
		
	}
	return motMem;

}

int Machine_chargementProgrammeDepuisFichier(Machine* machine, char* nomFichier){

	FILE* fptr = fopen(nomFichier,"r");
	
	if (fptr == NULL){
		printf("erreur fichier introuvable \n");
		return -1;
	}
	char line[1024];
	int i = 0;

	while (fgets(line, sizeof(line), fptr) != NULL) {
		line[8] = '\0';
	    unsigned int instructionComplete = strtoll(line, NULL, 16);

	    machine->memoire[i  ] = instructionComplete >> 24;
	    machine->memoire[i+1] = instructionComplete >> 16;
	    machine->memoire[i+2] = instructionComplete >> 8 ;
	    machine->memoire[i+3] = instructionComplete      ;

	    i += 4;


	}
	fclose(fptr);
	return 0;
}

void Machine_afficheMemoire(Machine* machine,int adresseDebut, int longueur, int tailleMot){
	/*
		permet d'afficher une partie de la memoire en specifiant l'adresse du debut de la 
		lecture adressé de 0 a 65535, 
		La longueur en octet de la partie affiché
		Et la taille des paquet a afficher en octet de 1 a 4
		on peut aller au dessus mais le resultat sera faux
	
	*/

	
	for (int i = adresseDebut; i < adresseDebut+longueur; i += tailleMot)
	{
		//on affiche le mot comme un nombre en base 16, avec par default un taille de 4 oct
		printf("%08X\n", Machine_recupereMemoire(machine, i, tailleMot));
	}
}

void Machine_afficheToutDebug(Machine* machine){
	printf("------------------------------------\n");
	printf("|    MACHINE DEBUG INFO START      |\n");
	printf("------------------------------------\n");
	printf("|PC: %d\n", machine->PC);
	printf("|instructionID : %08X\n", Machine_recupereMemoire(machine, machine->PC, 4));
	printf("|Z : %d\n", machine->Z);
	printf("|N : %d\n", machine->N);
	printf("|C : %d\n", machine->C);
	printf("|registres(0-31):\n[");
	for (int i = 0; i < 31; ++i)
	{
		printf("%hd, ",machine->registre[i]);
	}
	printf("%hd]\n",machine->registre[31]);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("|    MACHINE DEBUG INFO END        |\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void Machine_free(Machine* machine){
	free(machine->memoire);
	free(machine->registre);
	free(machine);
}