#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lexical_analyzer.h"

extern int error;

struct Symbol * currentToken;

/**
 * Inicializa o analizador sintático
 */
int initParse (const char * file_name);

/**
 * Implementação do casa Token
 */
int readToken (char * expected);

/**
 * Implementação do casa Token, esperando por um tipo em especifico
 */
int readTypedToken (char * expectedType);

/**
 * Imprimir error quando o casa token lê um simbolo inesperado
 */
void printUndefinedToken ();

/**
 * Estado inicial da gramática
 */
void stateS();

void stateD();
void stateC ();
void stateEXP ();
void stateEXPS();
void stateT();
void stateF();

#endif // PARSE_H_INCLUDED
