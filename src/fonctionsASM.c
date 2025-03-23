
#include <stdlib.h>
#include <stdio.h>
#include "machine.h"
#include "fonctionsASM.h"



void (*TABLEAU_INSTRUCTION[])(Machine* ,InstructionArgs*) = {
	Machine_ASM_add,
	Machine_ASM_sub,
	Machine_ASM_mul,
	Machine_ASM_div,
	Machine_ASM_and,
	Machine_ASM_or ,
	Machine_ASM_xor,
	Machine_ASM_shl,
	NULL,
	NULL,
	Machine_ASM_ldb,
	Machine_ASM_ldw,
	Machine_ASM_stb,
	Machine_ASM_stw,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	Machine_ASM_jmp,
	Machine_ASM_jzs,
	Machine_ASM_jzc,
	Machine_ASM_jcs,
	Machine_ASM_jcc,
	Machine_ASM_jns,
	Machine_ASM_jnc,
	Machine_ASM_in ,
	Machine_ASM_out,
	Machine_ASM_rnd,
	NULL,
	Machine_ASM_hlt
};



//add rd, rn, S
void Machine_ASM_add(Machine* machine, InstructionArgs* args) {

	int valeurOp = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];

	int somme = machine->registre[(unsigned char)args->n] + valeurOp;


    machine->C = (somme & 0x10000)   != 0; /*Calcul de la retenue en comparant les bits de 2^16 et le résultat de la somme des regsitres*/
    machine->N = (somme & 0x8000 )   != 0; /*Calcul du bit de poids fort en comparant les bits de 2^15 et le résultat de la somme des registres*/
    machine->Z =  somme 			 == 0;
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = somme;
    }
    machine->PC += 4;
}

//sub rd, rn, S
void Machine_ASM_sub(Machine* machine, InstructionArgs* args) {
	
	int valeurOp = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
	int difference = machine->registre[(unsigned char)args->n] - valeurOp;
    machine->C = (difference & 0x10000) != 0; /*Calcul de la retenue en comparant les bits de 2^16 et le résultat de la difference des regsitres*/
    machine->N = (difference & 0x8000 ) != 0; /*Calcul du bit de poids fort en comparant les bits de 2^15 et le résultat de la difference des regsitres*/
    machine->Z =  difference 			== 0;
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = difference;
    }

    machine->PC += 4;
}

//mul rd, rn, S
void Machine_ASM_mul(Machine* machine, InstructionArgs* args) {
	
	int valeurOp = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
	int mult = (char)machine->registre[(unsigned char)args->n] * (char)valeurOp;

    machine->C = (mult & 0x10000) != 0; /*Calcul de la retenue en comparant les bits de 2^16 et le résultat du produit des regsitres*/
    machine->N = (mult & 0x8000 ) != 0; /*Calcul du bit de poids fort en comparant les bits de 2^15 et le résultat du produit des regsitres*/
    machine->Z =  mult 			  == 0;
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = mult;
    }

    machine->PC += 4;
}

//div rd, rn, S
void Machine_ASM_div(Machine* machine, InstructionArgs* args) {

    int valeurOp = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
	int division = machine->registre[(unsigned char)args->n] / valeurOp;

    machine->C = (division & 0x10000) != 0; /*Calcul de la retenue en comparant les bits de 2^16 et le résultat de la division des regsitres*/
    machine->N = (division & 0x8000 ) != 0; /*Calcul du bit de poids fort en comparant les bits de 2^15 et le résultat de la division des regsitres*/
    machine->Z = division 			  == 0;
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = division;
    }
    
    machine->PC += 4;
}

//and rd, rn, S
void Machine_ASM_and(Machine* machine, InstructionArgs* args) {
    
    int valeurOp = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
	int and = machine->registre[(unsigned char)args->n] & valeurOp;

     machine->C = (and & 0x10000) != 0; /*Calcul de la retenue en comparant les bits de 2^16 et le résultat de l'opérateur logique & des regsitres*/
     machine->N = (and & 0x8000 ) != 0; /*Calcul du bit de poids fort en comparant les bits de 2^15 et le résultat de l'opérateur logique & des regsitres*/
     machine->Z =  and 			  == 0;

    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = and;
    }

    machine->PC += 4;
}

//or rd, rn, S
void Machine_ASM_or(Machine* machine, InstructionArgs* args) {

    int valeurOp = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
	int or = machine->registre[(unsigned char)args->n] | valeurOp;


    machine->C = (or & 0x10000) != 0; /*Calcul de la retenue en comparant les bits de 2^16 et le résultat de l'opérateur logique | des regsitres*/
    machine->N = (or & 0x8000 ) != 0; /*Calcul du bit de poids fort en comparant les bits de 2^15 et le résultat de l'opérateur logique | des regsitres*/
    machine->Z =  or 		    == 0;
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = or ;
    }

    machine->PC += 4;
}

//xor rd, rn, S
void Machine_ASM_xor(Machine* machine, InstructionArgs* args) {

    int valeurOp = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
	int xor = machine->registre[(unsigned char)args->n] ^ valeurOp;

    machine->C = (xor&0x10000) != 0; /*Calcul de la retenue en comparant les bits de 2^16 et le résultat de l'opérateur logique ^ des registres*/
    machine->N = (xor&0x8000 ) != 0; /*Calcul du bit de poids fort en comparant les bits de 2^15 et le résultat de l'opérateur logique ^ des regsitres*/
    machine->Z =  xor 		   == 0;
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = xor;
    }

    machine->PC += 4;
}

//shl rd, rn, S
void Machine_ASM_shl(Machine* machine, InstructionArgs* args) {
    int decalageVal = args->valeurImmediate ? abs(args->S) : machine->registre[(unsigned short)args->S];
    int decalage = 0;
    if (args->S < 0) {
        decalage = (machine->registre[(unsigned char)args->n] >> (decalageVal - 1));
        machine->C = (decalage & 1)      != 0;  // on fait le décalage n-1 fois vers la droite puis on récupère le dernier bit qui va disparaître
        decalage = (machine->registre[(unsigned char)args->n] >> 1);
        machine->N = (decalage & 0x8000) != 0; /*on compare la nouvelle valeur décaler avec la puissance de deux suppérieur à celle du décalage avec moins args->S pour compenser le décalage*/
    } else {
        decalage = (machine->registre[(unsigned char)args->n] << decalageVal);
        machine->C = (decalage & 0x10000) != 0;  // on compare la nouvelel valeur du registre décalée avec 2^16 pour avoir le dernier bit qui a disparu*/
        machine->N = (decalage & 0x8000 ) != 0;    /*on compare la nouvelle valeur du registre décalée avec 2^15 pour avoir le nouveau bit de poids fort*/
    }
    
    machine->Z = decalage == 0;

    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = decalage;
    }

    machine->PC += 4;
}

//ldb rd, (rn)S
void Machine_ASM_ldb(Machine* machine, InstructionArgs* args) {

    int ajout = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    int valeurACharger = machine->memoire[(unsigned char)machine->registre[(unsigned char)args->n] + ajout];

    int PoidsFaible = (0xff & valeurACharger);  //       On extrait les 8 bits de poids faible en comparant avec 1111 1111
    int PoidFortExtrait = (0x8000 & valeurACharger);       // On extrait le bit de poid fort
    
    int BitsdePoidsFort = PoidFortExtrait ? 0xff : 0;
    
    int nouvelleval = ((BitsdePoidsFort << 8) | PoidsFaible);  // concaténation des résultat
    
    machine->N = (nouvelleval & 0x8000) != 0;
    machine->Z =  nouvelleval 			== 0;
    machine->C = 0;  // Car il n'y a ni bit qui disparait ni retenue
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = nouvelleval;
    }

    machine->PC += 4;
}

//ldw rd, (rn)S
void Machine_ASM_ldw(Machine* machine, InstructionArgs* args) {  // On est parti du principe qu'un octet se lit de droite à gauche comme les bits
    int ajout = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    
    char valeurAChargerPoidFort = machine->memoire[(unsigned char)machine->registre[(unsigned char)args->n] + ajout];
    char valeurAChargerPoidFaible = machine->memoire[(unsigned char)machine->registre[(unsigned char)args->n] + ajout + 1];
    
    short int PoidsFort   = valeurAChargerPoidFort << 8;      
    short int PoidsFaible = valeurAChargerPoidFaible& 0x00FF;

    short int nouvelleval = PoidsFort | PoidsFaible;
    
    machine->N = (nouvelleval & 0x8000) != 0;
    machine->Z =  nouvelleval 			== 0;
    machine->C = 0;  // Car il n'y a ni bit qui disparait ni retenue
    
    if (args->d != 0) { 
    	machine->registre[(unsigned char)args->d] = nouvelleval;
    }

    machine->PC += 4;
}

//stb (rd)S, rn
void Machine_ASM_stb(Machine* machine, InstructionArgs* args) {

    int ajout = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    char copie = machine->registre[(unsigned char)args->n];
   
    machine->N = (0x80 & copie) != 0;
    machine->Z =  copie 		== 0;
    machine->C = 0;  // Car il n'y a ni bit qui disparait ni retenue
   
    machine->memoire[(unsigned char)machine->registre[(unsigned char)args->d] + ajout] = copie;
    
    machine->PC += 4;
}

//stw (rd)S, rn
void Machine_ASM_stw(Machine* machine, InstructionArgs* args) {
    int ajout = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    short int copie = machine->registre[(unsigned char)args->n];
    machine->N = (0x8000 & copie) != 0;
    machine->Z =  copie 		  == 0;
    machine->C = 0;  // Car il n'y a ni bit qui disparait ni retenue d'apres Lazard
    
    char PoidsFort =  (copie & 0xFF00) >> 8;
    char PoidsFaible = copie & 0x00FF;
    machine->memoire[(unsigned char)machine->registre[(unsigned char)args->d] + ajout] = PoidsFort;
    machine->memoire[(unsigned char)machine->registre[(unsigned char)args->d] + ajout + 1] = PoidsFaible;
    
    machine->PC += 4;
}


void Machine_ASM_jmp(Machine* machine, InstructionArgs* args) {
    machine->PC = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];  
}


void Machine_ASM_jzs(Machine* machine, InstructionArgs* args) {
    if (machine->Z) {
		machine->PC = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];

    } else {
    	machine->PC += 4;

    }
}


void Machine_ASM_jzc(Machine* machine, InstructionArgs* args) {
    if (machine->Z == 0) {
   		machine->PC = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    
    } else {
    	machine->PC += 4;

    }
}


void Machine_ASM_jcs(Machine* machine, InstructionArgs* args) {
    if (machine->C) {
    	machine->PC = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    } else {
     	machine->PC += 4;

    }
}


void Machine_ASM_jcc(Machine* machine, InstructionArgs* args) {
    if (machine->C == 0) {
	    machine->PC = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
  
    } else {
    	machine->PC += 4;

    }
}


void Machine_ASM_jns(Machine* machine, InstructionArgs* args) {
    if (machine->N) {

	    machine->PC = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    
    } else {
    	machine->PC += 4;

    }
}


void Machine_ASM_jnc(Machine* machine, InstructionArgs* args) {
    if (machine->N == 0) {
	    machine->PC = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S];
    
    } else {
    	machine->PC += 4;

    }
}

//in rd
void Machine_ASM_in(Machine* machine, InstructionArgs* args) {

    short int val;
    scanf("%hd",&val);

    machine->N = (val & 0x8000) != 0;
    machine->Z =  val        	== 0;
    machine->C =  0;  // Car il n'y a ni bit qui disparait ni retenue

    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = val;
    }
    
    machine->PC += 4;
}

//out rd
void Machine_ASM_out(Machine* machine, InstructionArgs* args) {
    int contenu = machine->registre[(unsigned char)args->d];

    printf("%d\n", contenu);
    
    machine->N = (contenu & 0x8000) != 0;
    machine->Z =  contenu 			== 0;
    machine->C = 0;  // Car il n'y a ni bit qui disparait ni retenue
    
    machine->PC += 4;
}

//rnd rd, rn, S
void Machine_ASM_rnd(Machine* machine, InstructionArgs* args) {

	int borne2 = args->valeurImmediate ? args->S : machine->registre[(unsigned short)args->S]; 
    int sup = (machine->registre[(unsigned char)args->n] >= borne2) ? machine->registre[(unsigned char)args->n] : borne2 - 1;
    int inf = (machine->registre[(unsigned char)args->n] <  borne2) ? machine->registre[(unsigned char)args->n] : borne2 - 1;
    
    int aleatoire = rand() % (sup - (inf - 1)) + (inf);  // Marche pour toutes les inégalités on prend l'intervalle [0 ; supp-inf+1(=taille de l'intervalle)] auquel on rajoute l'inf pour atteindre la borne inf sup
    machine->N = (aleatoire & 0x80000000) != 0;
    machine->Z =  aleatoire 			  == 0;
    machine->C = 0;  // Car il n'y a ni bit qui disparait ni retenue
    
    if (args->d != 0) {
        machine->registre[(unsigned char)args->d] = aleatoire;
    }
    
    machine->PC += 4;
}


void Machine_ASM_hlt(Machine* machine, InstructionArgs* args) {
    machine->PC = -1;
}


void Machine_ASM_execInstruction(Machine* machine, int instructionComplete){

	if(machine == NULL){
		return;
	}
	
	unsigned char instructionID = (instructionComplete & 0xf8000000 ) >> 27;
	
	InstructionArgs args = {
		(char)((instructionComplete & 0x7c00000) >> 22),
		(char)((instructionComplete & 0x3e0000 ) >> 17),
		(char)((instructionComplete & 0x10000  ) >> 16),
		(short)(instructionComplete & 0xffff   )
	};

    void (*fonctionInstruction)(Machine*, InstructionArgs*);

	fonctionInstruction = TABLEAU_INSTRUCTION[instructionID];
    
    //dans le cas extreme ou l'instruction lu par PC n'existe pas on arrete manuelement le programme
    if (fonctionInstruction == NULL){
        
        Machine_ASM_hlt(machine, NULL);
    } else {

        fonctionInstruction(machine, &args);
    }


}



int Machine_ASM_execProg(Machine* machine){
    //execute le programme contenue a l'adresse 0 de la memoire
	machine->PC = 0;
	int instructionCourante = Machine_recupereMemoire(machine, machine->PC, 4);

	while(machine->PC >= 0){

		Machine_ASM_execInstruction(machine, instructionCourante);
		
		instructionCourante = Machine_recupereMemoire(machine, machine->PC, 4);

	}

	return 0;
}

void InstructionArgs_afficheArgDebug(InstructionArgs* args) {
    printf("d: %d\n", args->d);
    printf("n: %d\n", args->n);
    printf("valeurImmediate: %d\n", args->valeurImmediate);
    printf("S: %d\n", args->S);
}