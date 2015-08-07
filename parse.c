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
    while (readClazzToken("identifier") == 0)
    {
        if (compareToken(currentToken, ","))
        {
            readToken(",");
        }
        else if (compareToken(currentToken, ":"))
            stateD();
        else
            break;
    }
    while (compareToken(currentToken,"begin"))
    {
        stateB();
    }
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

void stateB()
{
    readToken("begin");
    while (compareTokenClass(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
    {
        stateC();
    }
    readToken("end");
}

/**
 * LÃª uma constante
 */
 Symbol * readConst ()
{
     Symbol * constant;
    if (compareToken(currentToken,"-"))
    {
        readToken("-");
        char temp [256] = "-";
        strcat (temp,currentToken->name);
        strcpy (currentToken->name,temp);
    }
    constant = &* currentToken;
    if (compareTokenType(currentToken,"integer"))
    {
        int num = atoi(currentToken->name);
        readTypedToken("integer");
        if (num >= 0 && num <= 255)
            strcpy (constant->type,"byte");
        else
            strcpy (constant->type,"integer");
    }
    else if (compareTokenType(currentToken,"string"))
    {
        readTypedToken("string");
        strcpy (constant->type,"string");
    }
    else if (compareToken(currentToken,"TRUE"))
    {
        readToken("TRUE");
        strcpy (constant->type,"boolean");
    }
    else if (compareToken(currentToken,"FALSE"))
    {
        readToken("FALSE");
        strcpy (constant->type,"boolean");
    }
    return constant;
}

void stateC ()
{
    if (compareTokenClass(currentToken,"identifier"))
    {
        readClazzToken("identifier");
        readToken("=");
        stateEXP();
        readToken(";");
    }
    else if (compareToken(currentToken,"while"))
    {
        readToken("while");
        stateEXP();

        if (compareToken(currentToken,"begin"))
            stateB();
        else
            stateC();
    }
    else if (compareToken(currentToken,"if"))
    {
        readToken("if");
        stateEXP();

        if (compareToken(currentToken,"begin"))
            stateB();
        else
            stateC();
        if (compareToken(currentToken,"else"))
        {
            readToken("else");

            if (compareToken(currentToken,"begin"))
                stateB();
            else
                stateC();
        }
    }
    else if (compareToken(currentToken,";"))
    {
        readToken(";");
    }
    else if (compareToken(currentToken,"readln"))
    {
        readToken("readln");
        readToken(",");
        readClazzToken("identifier");
        readToken(";");
    }
    else if (compareToken(currentToken,"write") || compareToken(currentToken,"writeln"))
    {
        compareToken(currentToken,"writeln");
        if(compareToken(currentToken, "write"))
            readToken("write");
        else if(compareToken(currentToken, "writeln"))
            readToken("writeln");
        readToken(",");
        stateEXP();
        while (compareToken(currentToken,","))
        {
            readToken(",");
            stateEXP();
        }
        readToken(";");
    }
}

void stateEXP()
{
     stateEXPS();
    if (compareToken(currentToken,"=="))
    {
        readToken("==");
        stateEXPS();
    }
    else if (compareToken(currentToken,"!=") || compareToken(currentToken,"<") || compareToken(currentToken,">") || compareToken(currentToken,"<=") || compareToken(currentToken,">=") )
    {
        if (compareToken(currentToken,"!=")){
            readToken("!=");
        }
        else if (compareToken(currentToken,"<")){
            readToken("<");
        }
        else if (compareToken(currentToken,">")){
            readToken(">");
        }
        else if (compareToken(currentToken,"<=")){
            readToken("<=");
        }
        else if (compareToken(currentToken,">=")){
            readToken(">=");
        }
        stateEXPS();
    }
}

void stateEXPS()
{
    if (compareToken(currentToken,"-"))
    {
        readToken("-");
        stateT();
    }
    else
    {
        stateT();
    }
    while (compareToken(currentToken,"-") || compareToken(currentToken,"+") || compareToken(currentToken,"OR"))
    {
        if (compareToken(currentToken,"-"))
        {
            readToken("-");
            stateT();
        }
        else if (compareToken(currentToken,"+"))
        {
            readToken("+");
            stateT();
        }
        else if (compareToken(currentToken,"OR"))
        {
            readToken("OR");
            stateT();
        }
    }
}

 Symbol stateT ()
{
     Symbol T = stateF();

    if (compareToken(currentToken,"*"))
    {
        readToken("*");
        stateF();
    }
    else if (compareToken(currentToken,"/"))
    {
        readToken("/");
        stateF();
    }
    else if (compareToken(currentToken,"AND"))
    {
        readToken("AND");
        stateF();
    }
    return T;
}

 Symbol stateF()
{
     Symbol F = * currentToken;
    if (compareTokenClass(currentToken,"identifier"))
    {
        //se ler o identificador, apenas retornar o mesmo
        readClazzToken("identifier");
        F = *findToken(F.name);
    }
    else if (compareTokenType(currentToken,"integer"))
    {
        int num = atoi(currentToken->name);
        readTypedToken("integer");
        //determinar se a constante nÃºmerica Ã© do tipo byte ou integer
        (num >= 0 && num <= 255) ? strcpy (F.type,"byte") : strcpy (F.type,"integer");
        //definir a constante
    }
    else if (compareTokenType(currentToken,"string"))
    {
        readTypedToken("string");
        strcpy(F.type,"string");
        //definir a constante
    }
    else if (compareToken(currentToken,"TRUE"))
    {
        readToken("TRUE");
        strcpy(F.type,"boolean");
        //definir a constante
    }
    else if (compareToken(currentToken,"FALSE"))
    {
        readToken("FALSE");
        strcpy(F.type,"boolean");
        //definir a constante
    }
    else if (compareToken(currentToken,"NOT"))
    {
        readToken ("NOT");
        F = stateF();
    }
    else if (compareToken(currentToken,"("))
    {
        readToken ("(");
        stateEXP();
        readToken (")");
    }else{
        printUndefinedToken();
    }
    return F;
}

