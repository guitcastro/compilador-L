
#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lexical_analyzer.h"

extern int error;

struct Symbol * currentToken;

/**
 * Inicializa o analizador sintático
 * @param file_name nome do arquivo com o código fonte
 */
int initParse (const char * file_name);

/**
 * Implementação do casa Token
 * @param expectedToken próxima token esperada a ser lida
 * @return sucesso: 0 , falha: -1
 */
int readToken (char * expected);

/**
 * Implementação do casa Token, esperando por uma token com o tipo especifico
 * Os tipos esperados podem ser "boolean" , "integer" e "string"
 * @param expectedType nome do tipo da token esperada
 * @return sucesso: 0 , falha: -1
 */

int readTypedToken (char * expectedType);


/**
 * Implementação do casa Token, esperando por uma token com a classe específica
 * Os tipos esperados podem ser "identifier" e "const"
 * @param expectedClazz nome da classe da token esperada
 * @return sucesso: 0 , falha: -1
 */int readClazzToken (char * expectedClazz);

/**
 * Imprimir error quando o casa token lê um simbolo inesperado
 */
 void printUndefinedToken ();

/**
 * Lê um identificador
 * Se o stateExpected for 0, e o identificador já tiver sido declarado ele imprmir um error
 * caso contrário se o identificador não tiver sido declarado ele imprime um error
 */
int readIdentifier (int stateExpected);

/**
 * Imprimir error quando o identificador não foi declarado
 */
void printUndeclaredIdentifier (const struct Symbol * id);

/**
 * Imprimir error quando o identificador já foi declarado
 */
void printIdentifierAlreadyDeclared (const struct Symbol * id);


/**
 * Lê uma constante
 */
const char * readConst ();

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
