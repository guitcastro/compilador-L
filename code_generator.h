#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

#define isIntegerOrByte(c) (strcmp(c,"integer") == 0 || strcmp (c,"byte") == 0)
#define areBytes(a,b) (strcmp(a,"byte") == 0 && strcmp (b,"byte") == 0)
#define hasInteger(a,b) (strcmp(a,"integer") == 0 || strcmp (b,"integer") == 0)
#define hasByteAndInteger(a,b) ( (strcmp(a,"byte") == 0 || strcmp (b,"byte") == 0) && !(strcmp(a,"byte") == 0 && strcmp (b,"byte") == 0) )

//endereço inicial
unsigned int currentEnd;
//endereço dos temporários
unsigned int tempEnd;
//
unsigned int rotCount;



/**
 * Arquivo asm
 */
FILE * asm_file;

void initCodeGen (const char * file_name);

void finishCogGen ();

void defConst( Symbol * identifier, Symbol * constant);

void defIdentifier( Symbol * identifier);

void openCseg ();

void openDseg();

void closeDseg();

int genNot ( Symbol * x);

void defTempConst( Symbol * constant);

int genMultiply( Symbol * x, Symbol * y,int isInteger);

int genDivision( Symbol * x, Symbol * y);

int genAnd ( Symbol * x, Symbol * y);

void genNegative( Symbol * x);

unsigned int genOperate (Symbol x, Symbol y, int op);

void loadIntegerAx(Symbol x);

void genWriteln (Symbol s);

void convertToString (Symbol in);

void genReadln(Symbol * s);

int initWhile (Symbol exp);

void finishWhile(int rotInicio,int rotFim);

void printLn();

void loadAxBx (Symbol x,Symbol y);

int genCompareAxBx(Symbol x,Symbol y,int operation);

void genAssign(Symbol  * x, Symbol y);

int initIf (Symbol exp);

int endIf (int rotFalse);

int criarRotulo();

void genStringConcat(Symbol * x,Symbol y);

int compareString (Symbol  x,Symbol y);

#endif // CODE_GENERATOR_H
