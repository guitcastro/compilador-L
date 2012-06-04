#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

//endereço inicial
unsigned int currentEnd;
//endereço dos temporários
unsigned int tempEnd;

/**
 * Arquivo asm
 */
FILE * asm_file;

void initCodeGen (const char * file_name);

void defConst(struct Symbol * identifier,struct Symbol * constant);

void defIdentifier(struct Symbol * identifier);

void openCseg ();

void openDseg();

void closeDseg();

void defTempConst(struct Symbol * constant);

int genMultiply(struct Symbol * x,struct Symbol * y,int isInteger);


#endif // CODE_GENERATOR_H
