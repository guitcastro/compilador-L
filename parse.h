
#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lexical_analyzer.h"


Symbol * currentToken;

/**
 * Inicializa o analizador sintÃ¡tico
 * @param file_name nome do arquivo com o cÃ³digo fonte
 */
int initParse (const char * file_name);

/**
 * ImplementaÃ§Ã£o do casa Token
 * @param expectedToken prÃ³xima token esperada a ser lida
 * @return sucesso: 0 , falha: -1
 */
int readToken (char * expected);

/**
 * ImplementaÃ§Ã£o do casa Token, esperando por uma token com o tipo especifico
 * Os tipos esperados podem ser "boolean" , "integer" e "string"
 * @param expectedType nome do tipo da token esperada
 * @return sucesso: 0 , falha: -1
 */

int readTypedToken (char * expectedType);


/**
 * ImplementaÃ§Ã£o do casa Token, esperando por uma token com a classe especÃ­fica
 * Os tipos esperados podem ser "identifier" e "const"
 * @param expectedClazz nome da classe da token esperada
 * @return sucesso: 0 , falha: -1
 */int readClazzToken (char * expectedClazz);

/**
 * Imprimir error quando o casa token lÃª um simbolo inesperado
 */
 void printUndefinedToken ();

/**
 * LÃª uma constante
 */
 Symbol * readConst ();

/**
 * Estado inicial da gramÃ¡tica
 */
void stateS();
void stateD();
void stateEXP ();
void stateEXPs();
void stateInstructions();

#endif // PARSE_H_INCLUDED
