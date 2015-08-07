#ifndef LEXICAL_ANALYZER_H_INCLUDED
#define LEXICAL_ANALYZER_H_INCLUDED

#include "symbol_table.h"

//macros relacionados ao códigos ascii
#define isNumeric(c) (c > 47 && c < 58)
#define isAlphabetical(c) ((c > 64 && c < 91) || (c > 96 && c < 123))
#define isDelimiter(c) (c == '\n' || c == 32 || c == '\t')
/**
 * Arquivo de código fonte
 */
FILE * source_code;

/**
 * buffer do parse
 */
char buffer [256];

/**
 * Núemero de linhas do arquivo
 */
unsigned int lineNumber;

/**
 * Inicializa o analizador Lexico
 */
int initLexicalAnalyzer (const char * file_name);

/**
 * Estado inicial do Automato
 */
 Symbol * initialState();

/**
* Lê o próximo caracterer da linguagem
*/
char readNextChar ();

/**
 * Lê uma identificador, ou palavra reservada
 */
Symbol * readIdentifiers();

Symbol * readNumber ();

/**
 * imprimir o error "nn:lexema nao identificado [lex]."
 */
void printUndefinedLexical ();

/**
 * imprimir um error no formato "nn:error"
 */
void printError (char * error);

/**
 * Volta em uma posição o ponteiro do arquivo e o buffer
 */
void rewindPointer ();

/**
 * checa se o caracterer lido é válido (esta contindo no alfabeto e não é fim de arquivo)
 */
int isValidAndNotEof (char c);


#endif // LEXICAL_ANALYZER_H_INCLUDED
