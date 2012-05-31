
#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lexical_analyzer.h"

#define isIntegerOrByte(c) (strcmp(c,"integer") == 0 || strcmp (c,"byte") == 0)
#define hasByte(a,b) ( (strcmp(a,"byte") == 0 || strcmp (b,"byte") == 0) && !(strcmp(a,"byte") == 0 && strcmp (b,"byte") == 0) )

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
 * verifica os tipos de uma expressão boolean (em que os dois operandos são boolean)
 * ex ( a or b), (a && b)
 * @param typeX tipo a ser verificado
 * @param typeY tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkBooleanExp (char * typeX,char * typeY);

/**
 * verifica se determinado tipo é um boolean
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int  checkBoolean (char * type);

/**
 * verifica se determinado tipo é um string
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkString (char * type);

/**
 * Checa se o tipo é byte ou inteiro ou string
 * @param type tipo a ser verificado
 * @return o tipo se ele for string, byte ou inteiro , NULL caso seja diferente
 */
char * checkIntegerOrByteOrString (char * type);

/**
 * Checa se o tipo é byte ou inteiro
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int  checkIntegerOrByte (char * type);

/**
 * Determina qual será o tipo resultado da operação entre bytes em inteiros
 * @param typeX tipo do operando a esquerda
 * @param typeY tipo do operando a direita
 * @return O tipo resultante da operação
 */
char * setIntegerOrByte (char * typeX,char * typeY);


/**
 * Lê uma constante
 */
const char * readConst ();

/**
 * Estado inicial da gramática
 */
void stateS();
void stateB();
void stateD();
char * stateC ();
char * stateEXP ();
char * stateEXPS();
char * stateT();
char * stateF();

#endif // PARSE_H_INCLUDED
