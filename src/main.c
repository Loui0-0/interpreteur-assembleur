#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "machine.h"
#include "convertisseurASMaHEX.h"
#include "fonctionsASM.h"

int main(int argc, char *argv[]){
	

	srand(time(NULL));

	if (argc == 0){
		printf("Aucun fichier specifié\n");
		return -1;
	}
	
	char* nomFichierSortie = ASMtoHex(argv[1]);
	
	if (nomFichierSortie == NULL){		
		printf("erreur de convertion creation fichier annulée\n");
		return -1;
	}

	Machine* machineSimul = newMachine();

	if (machineSimul == NULL){
		printf("erreur crea machine\n");
	}
	
	Machine_chargementProgrammeDepuisFichier(machineSimul,nomFichierSortie);
	Machine_ASM_execProg(machineSimul);
	Machine_free(machineSimul);

	return 0;
}

