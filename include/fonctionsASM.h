
#ifndef FONCTIONASM_H
#define FONCTIONASM_H

#include "machine.h"

typedef struct {
    char d;
    char n;
    char valeurImmediate;
    short int S;
} InstructionArgs;	

void Machine_ASM_add(Machine* machine, InstructionArgs* args);
void Machine_ASM_sub(Machine* machine, InstructionArgs* args);
void Machine_ASM_mul(Machine* machine, InstructionArgs* args);
void Machine_ASM_div(Machine* machine, InstructionArgs* args);
void Machine_ASM_and(Machine* machine, InstructionArgs* args);
void Machine_ASM_or (Machine* machine, InstructionArgs* args);
void Machine_ASM_xor(Machine* machine, InstructionArgs* args);
void Machine_ASM_shl(Machine* machine, InstructionArgs* args);
void Machine_ASM_ldb(Machine* machine, InstructionArgs* args);
void Machine_ASM_ldw(Machine* machine, InstructionArgs* args);
void Machine_ASM_stb(Machine* machine, InstructionArgs* args);
void Machine_ASM_stw(Machine* machine, InstructionArgs* args);
void Machine_ASM_jmp(Machine* machine, InstructionArgs* args);
void Machine_ASM_jzs(Machine* machine, InstructionArgs* args);
void Machine_ASM_jzc(Machine* machine, InstructionArgs* args);
void Machine_ASM_jcs(Machine* machine, InstructionArgs* args);
void Machine_ASM_jcc(Machine* machine, InstructionArgs* args);
void Machine_ASM_jns(Machine* machine, InstructionArgs* args);
void Machine_ASM_jnc(Machine* machine, InstructionArgs* args);
void Machine_ASM_in (Machine* machine, InstructionArgs* args);
void Machine_ASM_out(Machine* machine, InstructionArgs* args);
void Machine_ASM_rnd(Machine* machine, InstructionArgs* args);
void Machine_ASM_hlt(Machine* machine, InstructionArgs* args);

void Machine_ASM_execInstruction(Machine* machine, int instructionComplete);

int Machine_ASM_execProg(Machine* machine);

void InstructionArgs_afficheArgDebug(InstructionArgs* args); 



#endif
