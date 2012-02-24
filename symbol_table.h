//c libs
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//third-party libs
#include "uthash.h"

struct Symbol {
   int byte;//0 - 255
   char type [256];
   char name [256];
   UT_hash_handle hh;
};

//a tabela hash
struct Symbol *symbolTable;

/**
 * Inicia a tabela de simbolos (chama as funções clean e initializeDefaults)
 */
void init ();

/**
 * adiciona um simbolo a tabela
 */
void addSymbol (char * name);

/**
 * imprimi a tabela de simbolos
 */
void printTable ();

/**
 * Quantidade de itens na tabela
 */
unsigned int tableSize();
