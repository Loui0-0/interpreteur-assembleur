#ifndef MACHINE_H
#define MACHINE_H

typedef struct {
	int PC; //pointeur sur l'instruction courante
	char Z, N, C; //le registre interressant ici definit sur 3 char peu efficace mais permet de simplifier l'implementation
	

	/*
	pr la memoire et les registree il faut une taille d'element specifique
	1 octet pr la memoire et 2 octets pr les registres dcp je les def comme 
	des char et short int mais c'est pas dit leur taille ne chage pas si on exec le 
	code sur une autre machine donc il faudra remedier a ca plus tard
	genre avec <stdint.h> pr fixer la taille des types
	

	*/
	short int *registre;//2 octet 16bit
	char *memoire; 		//1 octet 8bit
} Machine;


Machine* newMachine();
int Machine_chargementProgrammeDepuisFichier(Machine* machine, char* nomFichier);
void Machine_afficheMemoire(Machine* machine,int adresseDebut, int longueur, int taillePaquet);
int Machine_recupereMemoire(Machine* machine, int adresseDebut, int tailleMot);
void Machine_afficheToutDebug(Machine* machine);
void Machine_free(Machine* machine);

#endif