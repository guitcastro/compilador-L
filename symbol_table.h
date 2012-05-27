#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

//c libs
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//third-party libs
#include "uthash.h"

extern int error;

struct Symbol {
   int byte;//0 - 255
   char type [256];
   char clazz [256];
   char name [256];
   UT_hash_handle hh;
};

//a tabela hash
struct Symbol *symbolTable;

/**
 * Inicia a tabela de simbolos (chama as funções clean e initializeDefaults)
 */
void initSymbolTable ();

/**
 * Cria uma token
 */
struct Symbol * createSymbol (char * name);

/**
 * Cria uma token com o nome e o tipo especificados
 */
struct Symbol * createTypedSymbol (char * name,char * type);

/**
 * adiciona um simbolo a tabela
 */
void addSymbol (struct Symbol * s);

/**
 * adiciona um simbolo a tabela
 */
void addConstantSymbol (char * name,char * type);

/**
 * imprimi a tabela de simbolos
 */
void printTable ();

/**
 * Quantidade de itens na tabela
 */
unsigned int tableSize();

/**
 * Imprimi uma token
 */
void printSymbol (struct Symbol *s);

/**
 * retorna o simbolo encontrado de acordo com o nome especificado, ou null quando não encontrado
 */
struct Symbol * findToken (char * name);

/**
 * Compara se duas token são iguais
 */
int compareToken (struct Symbol *s1,char * tokenName);

/**
 * Compara se duas token tem o mesmo tipo
 */
int compareTokenType (struct Symbol *s1,char * tokenType);

#endif // SYMBOLTABLE_H
