
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
 * LÃª um identificador
 * Se o stateExpected for 0, e o identificador jÃ¡ tiver sido declarado ele imprmir um error
 * caso contrÃ¡rio se o identificador nÃ£o tiver sido declarado ele imprime um error
 */
int readIdentifier (int stateExpected);

/**
 * Imprimir error quando o identificador nÃ£o foi declarado
 */
void printUndeclaredIdentifier (const  Symbol * id);

/**
 * Imprimir error quando o identificador jÃ¡ foi declarado
 */
void printIdentifierAlreadyDeclared (const  Symbol * id);

/**
 * verifica os tipos de uma expressÃ£o boolean (em que os dois operandos sÃ£o boolean)
 * ex ( a or b), (a && b)
 * @param typeX tipo a ser verificado
 * @param typeY tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkBooleanExp (char * typeX,char * typeY);

/**
 * verifica se determinado tipo Ã© um boolean
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int  checkBoolean (char * type);

/**
 * verifica se determinado tipo Ã© um string
 * @param type tipo a ser verificado
 */
void checkString (char * type);

/**
 * Checa se o tipo Ã© byte ou inteiro ou string
 * @param type tipo a ser verificado
 * @return o tipo se ele for string, byte ou inteiro , NULL caso seja diferente
 */
char * checkIntegerOrByteOrString (char * type);

/**
 * Checa se o tipo Ã© byte ou inteiro
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
void checkIntegerOrByte (char * type);

/**
 * Determina qual serÃ¡ o tipo resultado da operaÃ§Ã£o entre bytes em inteiros
 * @param typeX tipo do operando a esquerda
 * @param typeY tipo do operando a direita
 * @return O tipo resultante da operaÃ§Ã£o
 */
char * setIntegerOrByte (char * typeX,char * typeY);


/**
 * LÃª uma constante
 */
 Symbol * readConst ();

/**
 * Estado inicial da gramÃ¡tica
 */
void stateS();
void stateB();
void stateD();
void stateC ();
void stateEXP ();
void stateEXPS();
Symbol stateT();
Symbol stateF();

#endif // PARSE_H_INCLUDED
