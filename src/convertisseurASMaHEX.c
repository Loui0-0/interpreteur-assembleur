#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "convertisseurASMaHEX.h"

#define TAILLELIGNEMAX 512
#define INSTRUCTIONHEXLENGTH 9


char* ASMtoHex(char* nomFichier){

	char* nomNouveauFichier = "hexa.txt";

	FILE* fptr = fopen(nomFichier,"r"); 
	
	if (fptr == NULL){
		printf("erreur fichier introuvable \n");
		return NULL;
	}

	//recuperation du nombre de lignes du fichier
	unsigned int nbLigne = getNombreLigneNonVide(fptr);

	if (nbLigne == 0){
		printf("le fichier specifié est vide\n");		
		return NULL;
	}

	//recuperation des etiquettes dans le fichier
	char** etiquettes = extractionEtiquette(fptr);
	if (etiquettes == NULL){
		printf("erreur d'exctration des etiquettes\n");
		return NULL;
	}
	
	char* programmeHex = malloc(sizeof(char) * INSTRUCTIONHEXLENGTH * nbLigne + 1);

	if (programmeHex == NULL){
		return NULL;
	}
	
	programmeHex[INSTRUCTIONHEXLENGTH * nbLigne] = '\0';

	
	//place le curseur au debut du fichier
	fseek(fptr, 0, SEEK_SET);

	int convSucce = 1;
	
	//la taille d'une ligne ne peut depasser TAILLELIGNEMAX 
	char ligne[TAILLELIGNEMAX];
	char cpyligne[TAILLELIGNEMAX];
	int nombreInstruction = 0;
	int numeroLigneReel = 0;


	while (fgets(ligne, sizeof(ligne), fptr) != NULL) {

		strncpy(cpyligne, ligne, TAILLELIGNEMAX);

		if (estVide(ligne)){
			numeroLigneReel++;
			continue;
		}

		//recherche dans la ligne la 1ere occurence de ':' 
		char* debutInstruction = strchr( ligne, ':' );

		//fait debutInstruction commencer au 1er char apres ':' sinon au debut de la ligne
		debutInstruction = debutInstruction ? debutInstruction+1 : ligne; 



		char* itsrComp[] = {NULL,NULL,NULL,NULL}; 
		int codeSortie = instructionVersToken(debutInstruction, itsrComp);

		if (codeSortie == -1){
			convSucce = 0;
			afficheMessageErreur(-7, numeroLigneReel, cpyligne);
		}

		long long int encode = encodageInstruction(itsrComp, etiquettes, nbLigne);

		if (encode < 0){
			convSucce = 0;
			afficheMessageErreur(encode, numeroLigneReel, cpyligne);
		}


		char hex[INSTRUCTIONHEXLENGTH+1];
		snprintf(hex,INSTRUCTIONHEXLENGTH+1 , "%08X\n", (unsigned int)encode);
		
		strncpy(programmeHex + nombreInstruction * INSTRUCTIONHEXLENGTH ,hex , INSTRUCTIONHEXLENGTH);
		
		nombreInstruction++;
		numeroLigneReel++;

	}
	
	fclose(fptr);

	if (convSucce){
		creationFichier(nomNouveauFichier, programmeHex);
	}

	for (int i = 0; i < nbLigne; i++)
	{
		if (etiquettes[i]){
			free(etiquettes[i]);
		}
	}

	free(etiquettes);
	free(programmeHex);

	return convSucce ? nomNouveauFichier : NULL;
}

int instructionVersToken(char* str, char** token){
	/*
		fonction specifique a la syntaxe requise donc tres peut modulable une meilleur implementation
		serait de programmer un vrai tokenizer ou d'utilisé un systeme d'expressions reguliéres
		puisque la syntaxe est tres basique cette solution suffira
		
	*/

	//init a NULL les token
    for (int i = 0; i < 4; i++) {
        token[i] = NULL;
    }

	//supression des espaces du debut
	char* tok = str;
	while(isspace(*tok)){tok++;}
	
	//delimite le 1er token (il se termine par un espace)
	char* prochainEspace = strchr(tok, ' ');
	if (prochainEspace == NULL){
		
		supprimeCaracteresVide(tok);
		token[0] = tok;
		return 0;
	}

	*prochainEspace = '\0';
	token[0] = tok;
	tok = prochainEspace+1;

	supprimeCaracteresVide(tok);

	int DernierTokAtteint = 0;
	for (int i = 1; i < 4; i++)
	{
		char* prochainSep = strchr(tok, ',');
		
		if (prochainSep == NULL){
			DernierTokAtteint = 1;
			token[i] = *tok=='\0' ? NULL: tok;
			tok = str + strlen(str);

		}else{
			*prochainSep = '\0'; 
			token[i] = tok;
			tok = prochainSep+1;

		}
	}

	//cas particulier ou une une ou plusieur virgule termine la ligne 
	if (!DernierTokAtteint){
		return -1;
	}

	return 0;
}

void supprimeCaracteresVide(char* str){

	char tmp[strlen(str)+1]; //utilisation des VLA
	int size = 0;
	char* ptr = str;

	while (*ptr){
		if (!isspace(*ptr)){
			tmp[size] = *ptr;
			size++;
		}
		ptr++;
	}
	tmp[size] = '\0';

	strcpy(str, tmp); //ici strcoy ne pose pas de prblm puisque taille tmp <= taille srt
}

void afficheMessageErreur(long long int codeErreur, int numLigne, char* ligne){
	if (codeErreur > 0 || codeErreur < -7){
		printf("ligne %d:\n%s\n~~~~~~~~~~~~~~~~\n erreur inconnue\n", numLigne, ligne);
	}
	char* msgsErreurs[] = {
		"",
		"aucune instructions specifié",
		"l'instruction specifié n'existe pas",
		"nombre insuffisant d'arguments",
		"syntaxe d'une instruction de lecture/ecriture memoire erronée",
		"argument(s) specifié(s) inexistant(s)",
		"trop d'arguments specifiés",
		"syntaxe inconnue"
	};

	if (ligne){

		//supprime les caracteres "vide" au debut de la ligne
		while (isspace(*(ligne))){ligne++;};
		printf("[ligne %d] %s:\n%s~~~~~~~~~~~~~~~~\n\n", numLigne, msgsErreurs[abs(codeErreur)], ligne);
	}
}

int creationFichier(char* nomFichier, char* contenu){

	FILE* fptr = fopen(nomFichier,"w");
	
	if (fptr == NULL){
		printf("impossible de creer un nouveau fichier\n");
		return 0;
	}

	fprintf(fptr ,"%s", contenu);
	fclose(fptr);
	return 1;
}

int getNombreLigneNonVide(FILE* fptr){
	/*
	 revoie le nombre de ligne d'un fichier
	*/
	
	long savedPosition = ftell(fptr);
	
	fseek(fptr, 0, SEEK_SET);
	
	char ligne[TAILLELIGNEMAX];

	int ligneCount = 0;
	while (fgets(ligne, sizeof(ligne), fptr) != NULL) {
		ligneCount += !estVide(ligne);
	}


	fseek(fptr, savedPosition, SEEK_SET);
	return ligneCount;
}

int estVide(char* ligne){
	char* ptr = ligne;

	while(*ptr){
		if (!isspace(*ptr)){
					return 0;
		}
		ptr++;
	}

	return 1;
}

char** extractionEtiquette(FILE* fptr){
	/* 
	 revoie un tableau de la taille du nombre de ligne du fichier
	 tel que a l'index i si il y a une etiq alors l'etiq pointe dans le prog a l'adresse i*4  
    */
	
	long savedPosition = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	char** etiquettePosition = malloc( sizeof(char*) * getNombreLigneNonVide(fptr));

	if (etiquettePosition == NULL){
		return NULL;
	}


	char ligne[TAILLELIGNEMAX];
	int nbInstruction = 0;

	while (fgets(ligne, sizeof(ligne), fptr) != NULL) {
	//lit le fichier ligne par ligne et stock a chaque iteration la ligne ds ligne

		if (estVide(ligne)){
			continue;
		}


		//trouve la fin de l'etiquette si il y en a une
		char* debutInstruction = strchr( ligne, ':' );
		
		//cas particulié ou les 2 points sont au debut de ligne ":add r0..." interdit
		if (debutInstruction == ligne){
			return NULL;
		}

		if (debutInstruction){

			//cas particulié ou etiq n'est pas immédiatement suivie de ':' "etiq :add" interdit  
			if (*(debutInstruction-1) == ' '){
				return NULL;
			}


			/*
			"etiq: add ......" -> "etiq\0 add ....." donc strlen renvoie la taille de etiq
			et ainsi etiquettePosition[i] est de la taille d'etiq
			on gere le cas ou il y a des espaces au debuts avec strtok
			*/

			*debutInstruction = '\0';
			char* ligneSansEsp = ligne;
			ligneSansEsp = strtok(ligneSansEsp, " \t");
			
			etiquettePosition[nbInstruction] = malloc(strlen(ligne));
			
			if (etiquettePosition[nbInstruction]==NULL){
				return NULL;
			} 
			
			strcpy(etiquettePosition[nbInstruction], ligneSansEsp);
			/*
			puisque etiq est place au debut de ligne et que etiquettePosition[nbInstruction] est de
			taille etiq lorsque la copie seul etiq est copié ds le tableau
			*/

		} else {

			etiquettePosition[nbInstruction] = NULL;
		}

		nbInstruction++;
	}


	fseek(fptr, savedPosition, SEEK_SET);	
	return etiquettePosition;
}

int getIndex(char* str,char* T[],int sizeT){

    int i = 0;
    while(i < sizeT){
    	char* currentInstr = T[i];
    	if (currentInstr){
    	    if (strcmp(currentInstr,str) == 0){
    	   		return i;
    	   	}
    	}
        i++;
    }
    return -1;
}

int getInstructionId(char* instruction){

	//nom des instructions ordonné de tel sorte que leur index soit leur identifiant
	char* nomInstructions[] = {
		"add",
		"sub",
		"mul", 
		"div", 
		"and", 
		"or", 
		"xor", 
		"shl",
		NULL, 
		NULL, 
		"ldb", 
		"ldw", 
		"stb", 
		"stw", 
		NULL, 
		NULL, 
		NULL,
		NULL, 
		NULL, 
		NULL, 
		"jmp", 
		"jzs", 
		"jzc", 
		"jcs", 
		"jcc", 
		"jns",
		"jnc", 
		"in", 
		"out", 
		"rnd", 
		NULL, 
		"hlt"
	};

	int id = getIndex(instruction,nomInstructions,32);

	return id;
}

int getRegistreId(char* registreArg){
	//registre toujour de la forme "rn" 0 <= n <= 31
	
	if (registreArg[0] != 'r')
		return -1;

	int IdRegistre = (int)strtol(registreArg+1, NULL, 10); 

	if (IdRegistre < 0 || IdRegistre > 31)
		return -2;
	
	return IdRegistre; 
}

int estValeurImmediate(char* S){
	return S[0]=='#';
}

int getValeurImmediateOuRegistreId(char* S){
	/*
		permet de gerer S lorsque de la forme #m ou rm
	*/
	if (estValeurImmediate(S)){
		char* representationNombre;
		int base;
		switch (S[1]) {
			case 'h':
				base = 16;
				representationNombre = S+2;
				break;
			case 'b':
				base = 2;
				representationNombre = S+2;
				break;
			default:
				representationNombre = S+1;
				base = 10;
				break;
		}
		int valeurImmediate = strtol(representationNombre, NULL, base);

		if (valeurImmediate < -32768 || valeurImmediate > 32767)
			return 0x0FFFFFFF;
		return valeurImmediate;

	}else{
		int IdRegistre = getRegistreId(S);
		if (IdRegistre == -1){
			return 0x0FFFFFFF;
		}
		return IdRegistre;
	}
}

long long int encodageInstruction(char* args[], char* etiquettes[], int tailleProgramme){
	/* 
		le but de cette fonction est de convertir une instruction sous la forme 
		d'un array en sa version encodé sur 32bit
		cette partie du code a aussi pour but de trouver les erreurs de syntaxes
		on revoie un long long int car tous les cas legaux sont utilisé par les 32 bits de l'int 
		et il faut 1 bit supplementaire pour les codes erreurs donc on doit étendre la taille du renvoie
	*/
	
	//erreur: pas d'instruction specifié apres une etiquettes
	if (args[0] == NULL){
		return -1;
	}

	//recuperation de l'id de l'instruction
	unsigned int instructionId = getInstructionId(args[0]);

	//erreur: l'instruction specifié n'existe pas
	if (instructionId == -1){
		return -2;
	}

	//on fait du cas par cas en fonction du type d'instruction
	if ((0 <= instructionId && instructionId <= 7) || instructionId == 29){
		/*
			de 0 a 7 les instructions on pour syntaxe : 
			"*instruction* rd, rn, S"
			ou rd et rn sont tjr des registres, 
			S est soit "#m" soit "rm"

			l'instruction rnd (29) a la meme syntaxe
			
		*/

		//erreur: le nombre d'arguments specifié est insuffisant
		if (args[3] == NULL){
			return -3;
		}

		//on recupere d dans "rd"
		int IdDest = getRegistreId(args[1]);

		//on recupere n dans "rn"
		int IdSrc1 = getRegistreId(args[2]);

		int Imm = estValeurImmediate(args[3]);

		//on recupere m dans rm ou #m
		int S = getValeurImmediateOuRegistreId(args[3]);
		
		//erreur: argument specifé inexistant
		if (IdDest == -1 || IdSrc1 == -1 || S == 0x0FFFFFFF){
			return -5;
		}

		/*
		construction de l'instruction complete sur 32 bits
		ex avec "shl r1, r24, #10"
		instructionId = 7 		(0000 0000 0000 0000 0000 0000 0000 0111)
		IdDest = 1 		  		(0000 0000 0000 0000 0000 0000 0000 0001)
		IdSrc1 = 24       		(0000 0000 0000 0000 0000 0000 0001 1000)
		Imm = 1           		(0000 0000 0000 0000 0000 0000 0000 0001)
		S = -10		  		    (1111 1111 1111 1111 1111 1111 1111 0110) representation Comp 2
		
		instructionId <<= 27;	(00111 00000 00000 0 0000 0000 0000 0000)
		IdDest <<= 22;			(00000 00001 00000 0 0000 0000 0000 0000)
		IdSrc1 <<= 17;			(00000 00000 11000 0 0000 0000 0001 1000)
		Imm <<= 16;				(00000 00000 00000 1 0000 0000 0000 0000)
		S $= 0x0000FFFF;        (00000 00000 00000 0 1111 1111 1111 0110)

		IdDest|IdSrc1|Imm|S     (00111 00001 11000 1 1111 1111 1111 0110)
		IdDest|IdSrc1|Imm|S     (00111 00001 11000 1 1111 1111 1111 0110)


		*/
		instructionId <<= 27;
		IdDest <<= 22;
		IdSrc1 <<= 17;
		Imm <<= 16;
		S &= 0x0000FFFF;

		return instructionId | IdDest | IdSrc1 | Imm | S;

	} else if (10 <= instructionId && instructionId <= 11){
		/*
			de 10 a 11 les instruction on pour syntaxe : 

			"*instruction* rd, (rn)S"
			ou rd et rn sont tjr des registre, 
			S est soit "#m" soit "rm"
			et on veut encoder comme tel:

			instructionId | d | n | Imm | m;	
	
		*/

		//erreur: nb args 
		if (args[2] == NULL){
			return -3;
		}

		// recuperation de d
		int IdDest = getRegistreId(args[1]);

		//erreur: syntaxe des instruction memoires 
		if (args[2][0] != '('){
			return -4;
		}

		//exctraction de rn et S
		char* registreEtS[2] = {NULL,NULL};
		registreEtS[0] = strtok( args[2], "()");
		registreEtS[1] = strtok( NULL, "()");
		
		//erreur: syntaxe specifique mem
		if (registreEtS[0] == NULL || registreEtS[1] == NULL){
			return -4;
		}

		//rn tjr un registre on peut utiliser getRegistreId
		int IdRegistreSource = getRegistreId(registreEtS[0]);
		int Imm = estValeurImmediate(registreEtS[1]);

		//recuperation de m dans rm ou #m
		int S = getValeurImmediateOuRegistreId(registreEtS[1]);

		//erreur: argument specifé inexistant
		if (IdDest == -1 || IdRegistreSource == -1 || S == 0x0FFFFFFF){
			return -5;
		}

		instructionId    <<= 27;
		IdDest 		     <<= 22;
		IdRegistreSource <<= 17;
		Imm 		     <<= 16;
		S &= 0x0000FFFF;

		return instructionId | IdDest | IdRegistreSource | Imm | S;

	} else if (12 <= instructionId && instructionId <= 13){

		/*
			de 12 a 13 les instruction on pour syntaxe : 

			"*instruction* (rd)S, rn"

			ou rd et rn sont tjr des registre, 
			S est soit "#m" soit "rm"
			et on veut encoder comme tel:

			instructionId | d | n | Imm | m;
	
		*/

		//erreur: nb args
		if (args[2] == NULL){
			return -3;
		}

		//erreur: syntaxe mem
		if (args[1][0] != '('){
			return -4;
		}

		//exctraction de rd et S
		char* registreEtS[2] = {NULL,NULL};
		registreEtS[0] = strtok( args[1], "()");
		registreEtS[1] = strtok( NULL, "()");

		//erreur: syntaxe mem
		if (registreEtS[0] == NULL || registreEtS[1] == NULL){
			return -3;
		}

		//recuperation de d
		int IdDest = getRegistreId(registreEtS[0]);

		int Imm = estValeurImmediate(registreEtS[1]);

		//recuperation de m dans rm ou #m
		int S = getValeurImmediateOuRegistreId(registreEtS[1]);

		//recuperation de n
		int IdRegistreSource = getRegistreId(args[2]);

		
		//erreur: argument specifé inexistant
		if (IdDest == -1 || IdRegistreSource == -1 || S == 0x0FFFFFFF){
			return -5;
		}

		instructionId <<= 27;
		IdDest <<= 22;
		IdRegistreSource <<= 17;
		Imm <<= 16;
		S &= 0x0000FFFF;

		return instructionId | IdDest | IdRegistreSource | Imm | S;

	} else if (20 <= instructionId && instructionId <= 26){
		/*
			de 20 a 26 les instruction on pour syntaxe : 

			"*instruction* S" 
			S est soit "#m" soit "rm" soit "etiq" 
			ou si S = "etiq" il faut remplacer par l'adresse pointé par etiq 	
			et on veut encoder comme tel: (m 16 bit)
			instructionId | 00000 | 00000 | Imm | m;	
		*/

		//erreur: trop d'arguments specifié
		if (args[1] == NULL || args[2] != NULL){
			return -6;
		}

		//on interprete S comme une etiquette
		int S = getIndex(args[1], etiquettes, tailleProgramme);
		int Imm = 1;

		if (S >= 0){
			S *= 4;

		} else {
			Imm = estValeurImmediate(args[1]);
			S = getValeurImmediateOuRegistreId(args[1]);
			
		}

		//erreur: arg inex
		if (S == 0x0FFFFFFF || Imm == -1){
			return -5;
		}

		instructionId <<= 27;
		Imm <<= 16;
		S &= 0x0000FFFF;

		return instructionId | Imm | S;

	} else if (27 <= instructionId && instructionId <= 28){
		/*
			de 27 a 28 les instruction on pour syntaxe : 

			"*instruction* rd"
			ou rd est tjr un registre
			instructionId | d | 00000 | 0 | 0000000000000000;
	
		*/

		//erreur: trop d'arg
		if (args[1] == NULL || args[2] != NULL){
			return -6;
		}


		int IdRegistre = getRegistreId(args[1]);

		//erreur: registre inex
		if (IdRegistre == -1){
			return -5;
		}

		instructionId <<= 27;
		IdRegistre <<= 22;
		return instructionId | IdRegistre;

	} else if (instructionId == 31){
		/*
			si l'instruction est hlt on renvoit 11111 | 27 bites a 0
		*/
		//erreur: trop d'arg
		if (args[1] != NULL){
			return -6;
		}
		return instructionId<<27;

	} else {
		//erreur: inconnue
		return -7;
		
	}
}


