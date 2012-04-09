#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lexical_analyzer.h"

struct Symbol * currentToken;

/**
 * Inicializa o analizador sintático
 */
int initParse (const char * file_name);

/**
 * Inicializa o analizador sintático
 */
int readToken (struct Symbol * expected);

#endif // PARSE_H_INCLUDED
