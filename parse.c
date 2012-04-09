#include "parse.h"

/**
 * Inicializa o analizador sintático
 */
int initParse (const char * file_name){
    initLexicalAnalyzer(file_name);
    currentToken = initialState();
    return 0;
}
