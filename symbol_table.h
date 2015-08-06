#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

//c libs
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//third-party libs
#include "uthash.h"

typedef struct {
    char type [256];
    char clazz [256];
    char name [256];
    UT_hash_handle hh;
}Symbol;

//a tabela hash
 Symbol *symbolTable;

/**
 * Inicia a tabela de simbolos (chama as funções clean e initializeDefaults)
 */
void initSymbolTable ();

/**
 * Cria uma token com o nome, tipo e classe especificados
 */
 Symbol * createSymbol (char * name, char * clazz, char * type);

/**
 * adiciona um simbolo a tabela
 */
void addSymbol ( Symbol * s);

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
void printSymbol ( Symbol *s);

/**
 * retorna o simbolo encontrado de acordo com o nome especificado, ou null quando não encontrado
 */
 Symbol * findToken (const char * name);

/**
 * Compara se duas token são iguais
 */
int compareToken ( Symbol *s1,char * tokenName);

/**
 * Compara se duas token tem o mesmo tipo
 */
int compareTokenType ( Symbol *s1,char * tokenType);

/**
 * Compara se duas token tem a mesma classe
 */
int compareTokenClass ( Symbol *s1,char * tokenClazz);

#endif // SYMBOLTABLE_H
