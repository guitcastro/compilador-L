#include "parse.h"

/**
 * Init the sintax analyser
 * @param file_name name of the source file
 */
int initParse (const char * file_name)
{
    initLexicalAnalyzer(file_name);
    currentToken = initialState();
    stateS();
    return 0;
}

/**
 * Get the next Token
 * @param expectedToken the expected token
 * @return  0 if success and -1 if fail
 */
int readToken (char * expectedToken)
{
    // case is the expected toke, go ahead
    if (currentToken != NULL &&  strcmp(currentToken->name,expectedToken) == 0)
        currentToken = initialState();
    // otherwise
    else
    {
        printUndefinedToken();
        return -1;
    }
    //retorna 0 caso tenha sucesso
    return 0;
}

/**
 * ImplementaÃ§Ã£o do casa Token, esperando por uma token com o tipo especifico
 * Os tipos esperados podem ser "boolean" , "integer" e "string"
 * @param expectedType nome do tipo da token esperada
 * @return sucesso: 0 , falha: -1
 */
int readTypedToken (char * expectedType)
{
    //se a token tiver o tipo esperado, ler a prÃ³xima token
    if (currentToken != NULL && strcmp(currentToken->type,expectedType) == 0)
        currentToken = initialState();
    //se a token nÃ£o tiver o tipo esperado
    else
    {
        printUndefinedToken();
        return -1;
    }
    //retorna 0 caso tenha sucesso
    return 0;
}

/**
 * ImplementaÃ§Ã£o do casa Token, esperando por uma token com a classe especÃ­fica
 * Os tipos esperados podem ser "identifier" e "const"
 * @param expectedClazz nome da classe da token esperada
 * @return sucesso: 0 , falha: -1
 */
int readClazzToken (char * expectedClazz)
{
    //se a token tiver o a classe esperada, ler a prÃ³xima token
    if (currentToken != NULL && strcmp(currentToken->clazz,expectedClazz) == 0)
        currentToken = initialState();
    //se a token nÃ£o tiver a classe esperada
    else
    {
        printUndefinedToken();
        return -1;
    }
    //retorna 0 caso tenha sucesso
    return 0;
}

/**
 * Imprimir error quando o casa token lÃª um simbolo inesperado
 */
void printUndefinedToken ()
{
    if (currentToken == NULL)
        printError ("fim de arquivo nao esperado.");
    char stringError [256];
    strcpy (stringError ,"token nao esperado [");
    strcat (stringError,currentToken->name);
    strcat (stringError,"].");
    printError (stringError);
}

/**
 * Estado inicial da gramÃ¡tica
 */
void stateS()
{
    readToken("inicio");
    readClazzToken("identifier");
    readToken(";");
    while (compareTokenClass(currentToken, "identifier"))
    {
        readClazzToken("identifier");
        if (compareToken(currentToken, ","))
        {
            readToken(",");
        }
        else if (compareToken(currentToken, ":"))
            stateD();
        else
            break;
    }
    /*
    * Especial case where the identifier is read before
    */
    if (compareToken(currentToken,":="))
    {
        readToken(":=");
        stateEXPs();
        readToken(";");
    }
    stateInstructions();
    if (currentToken != NULL)
        printUndefinedLexical();

}

void stateD ()
{
    readToken(":");
    if (compareToken(currentToken, "inteiro") || compareToken(currentToken, "real") ||
        compareToken(currentToken, "logico") || compareToken(currentToken, "caracter"))
    {
        readToken(currentToken->name);
    }
    else{
        printUndefinedToken();
    }
    readToken(";");
}
void stateInstructions(){
    while (compareTokenClass(currentToken, "identifier") ||
           compareToken(currentToken, "se") ||
           compareToken(currentToken, "leia") || compareToken(currentToken, "escreva") ||
           compareToken(currentToken, "enquanto") )
    {
        stateInstruction();
        if (compareToken(currentToken, ";")){
            readToken(";");
        }
        else{
            break;
        }
    }
}
void stateInstruction()
{
    if (compareTokenClass(currentToken, "identifier")){
        readClazzToken("identifier");
        readToken(":=");
        stateEXPs();
    }
    else if (compareToken(currentToken, "enquanto"))
    {
        readToken("enquanto");
        stateEXPs();
        readToken("faca");
        readToken("inicio");
        stateInstructions();
        readToken("fim");
    }
    else if (compareToken(currentToken, "se"))
    {
        readToken("se");
        stateEXPs();
        readToken("entao");
        readToken("inicio");
        stateInstructions();
        readToken("fim");
        if (compareToken(currentToken, "senao")){
            readToken("senao");
            readToken("inicio");
            stateInstructions();
            readToken("fim");
        }
    }else if (compareToken(currentToken, "leia")){
        readToken("leia");
        readToken("(");
        while (compareTokenClass(currentToken, "identifier")){
            readClazzToken("identifier");
            if (compareToken(currentToken, ",")){
                    readToken(",");
            }else{
                break;
            }
        }
        readToken(")");
    }
    else if (compareToken(currentToken, "escreva")){
        readToken("escreva");
        readToken("(");
        stateEXPs();
        readToken(")");
    }
}

int isRELOP(){
    return compareToken(currentToken, "=") || compareToken(currentToken, "<") ||
           compareToken(currentToken, "<=") || compareToken(currentToken, ">") ||
           compareToken(currentToken, ">=") || compareToken(currentToken, "<>");
}

int isADDOP(){
    return compareToken(currentToken, "+") || compareToken(currentToken, "-") ||
        compareToken(currentToken, "or");
}
int isMULOP  (){
    return compareToken(currentToken, "*")|| compareToken(currentToken, "/") ||
           compareToken(currentToken, "div") || compareToken(currentToken, "mod") ||
           compareToken(currentToken, "and");
}

void stateEXPs(){
    stateEXP();
    if (isRELOP() || isADDOP() || isMULOP()){
        readToken(currentToken->name); // read the operator
        stateEXPs();
    }
}

void stateEXP()
{
    if (compareTokenType(currentToken, "integer")){
        readTypedToken("integer");
    }
    else if (compareTokenClass(currentToken, "identifier")){
        readClazzToken("identifier");
    }
    else if (compareToken(currentToken, "(")){
        readToken("(");
        stateEXPs();
        readToken(")");
    }
}
