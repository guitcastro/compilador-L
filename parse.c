#include "parse.h"

/**
 * Inicializa o analizador sintático
 */
int initParse (const char * file_name)
{
    initLexicalAnalyzer(file_name);
    currentToken = initialState();
    stateS();
    return 0;
}

/**
 * Implementação do casa Token
 */
int readToken (char * expectedToken)
{
    //se as tokens forem iguais
    if (strcmp(currentToken->name,expectedToken) == 0)
        currentToken = initialState();
    else
    {
        printUndefinedToken();
        return -1;
    }
    return 0;
}

/**
 * Implementação do casa Token, esperando por um tipo em especifico
 */
int readTypedToken (char * expectedType)
{
    //se as tokens forem iguais
    if (strcmp(currentToken->type,expectedType) == 0)
        currentToken = initialState();
    else
    {
        printUndefinedToken();
        return -1;
    }
    return 0;
}

/**
 * Imprimir error quando o casa token lê um simbolo inesperado
 */
void printUndefinedToken ()
{
    char stringError [256];
    strcpy (stringError ,"token nao esperado [");
    strcat (stringError,currentToken->name);
    strcat (stringError,"].");
    printError (stringError);
}

/**
 * Estado inicial da gramática
 */
void stateS()
{
    while (currentToken != NULL)
    {
        if (compareToken(currentToken,"int") || compareToken(currentToken,"boolean") ||  compareToken(currentToken,"byte") || compareToken(currentToken,"string") || compareToken(currentToken,"final"))
        {
            stateD();
        }
        else if (compareToken(currentToken,"begin"))
        {
            readToken("begin");
            while (compareTokenType(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
            {
                stateC();
            }
            readToken("end");
        }
    }
}

void stateD ()
{
    if(compareToken(currentToken,"final"))
    {
        readToken("final");
        readTypedToken("identifier");
        readToken("=");
        if (compareToken(currentToken,"-"))
            readToken("-");
        if (compareTokenType(currentToken,"int"))
            readTypedToken("int");
        else if (compareTokenType(currentToken,"hexa"))
            readTypedToken("hexa");
        else if (compareTokenType(currentToken,"string"))
            readTypedToken("string");
        else if (compareToken(currentToken,"TRUE"))
            readToken("TRUE");
        else if (compareToken(currentToken,"FALSE"))
            readToken("FALSE");
    }
    else
    {
        if (compareToken(currentToken,"int"))
            readToken("int");
        else if (compareToken(currentToken,"boolean"))
            readToken("boolean");
        else if(compareToken(currentToken,"byte"))
            readToken("byte");
        else if(compareToken(currentToken,"string"))
            readToken("string");


        readTypedToken("identifier");
        if (compareToken(currentToken,","))
        {
            while (compareToken(currentToken,","))
            {
                readToken(",");
                readTypedToken("identifier");
            }
        }
        else if (compareToken(currentToken,"="))
        {
            readToken("=");
            if (compareToken(currentToken,"-"))
                readToken("-");
            if (compareTokenType(currentToken,"int"))
                readTypedToken("int");
            else if (compareTokenType(currentToken,"hexa"))
                readTypedToken("hexa");
            else if (compareTokenType(currentToken,"string"))
                readTypedToken("string");
            else if (compareToken(currentToken,"TRUE"))
                readToken("TRUE");
            else if (compareToken(currentToken,"FALSE"))
                readToken("FALSE");
        }
    }
    readToken(";");
}


void stateC ()
{
    if (compareTokenType(currentToken,"identifier"))
    {
        readTypedToken("identifier");
        readToken("=");
        stateEXP();
        readToken(";");
    }
    else if (compareToken(currentToken,"while"))
    {
        readToken("while");
        stateEXP();
        if (compareToken(currentToken,"begin"))
        {
            readToken("begin");
            while (compareTokenType(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
            {
                stateC();
            }
            readToken("end");
        }
        else
        {
            stateC();
        }
    }
    else if (compareToken(currentToken,"if"))
    {
        readToken("if");
        stateEXP();
        if (compareToken(currentToken,"begin"))
        {
            readToken("begin");
            while (compareTokenType(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
            {
                stateC();
            }
            readToken("end");
        }
        else
        {
            stateC();
        }
        if (compareToken(currentToken,"else"))
        {
            readToken("else");
            stateEXP();
            if (compareToken(currentToken,"begin"))
            {
                readToken("begin");
                while (compareTokenType(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
                {
                    stateC();
                }
                readToken("end");
            }
            else
            {
                stateC();
            }

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
        readTypedToken("identifier");
        readToken(";");
    }
    else if (compareToken(currentToken,"write"))
    {
        readToken("write");
        readToken(",");
        stateEXP();
        while (compareToken(currentToken,",")){
            readToken(",");
            stateEXP();
        }
        readToken(";");
    }
    else if (compareToken(currentToken,"writeln"))
    {
        readToken("writeln");
        readToken(",");
        stateEXP();
        while (compareToken(currentToken,",")){
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
    else if (compareToken(currentToken,"!="))
    {
        readToken("!=");
        stateEXPS();
    }
    else if (compareToken(currentToken,"<"))
    {
        readToken("<");
        stateEXPS();
    }
    else if (compareToken(currentToken,">"))
    {
        readToken(">");
        stateEXPS();
    }
    else if (compareToken(currentToken,"<="))
    {
        readToken("<=");
        stateEXPS();
    }
    else if (compareToken(currentToken,">="))
    {
        readToken(">=");
        stateEXPS();
    }
}

void stateEXPS()
{
    if (compareToken(currentToken,"-"))
        readToken("-");
    stateT();
    while (compareToken(currentToken,"-") || compareToken(currentToken,"+") || compareToken(currentToken,"OR"))
    {
        if (compareToken(currentToken,"-"))
            readToken("-");
        else if (compareToken(currentToken,"+"))
            readToken("+");
        else if (compareToken(currentToken,"OR"))
            readToken("OR");
        stateT();
    }
}

void stateT ()
{
    stateF();
    while (compareToken(currentToken,"*") || compareToken(currentToken,"/") || compareToken(currentToken,"AND"))
    {
        if (compareToken(currentToken,"*"))
            readToken("*");
        else if (compareToken(currentToken,"/"))
            readToken("/");
        else if (compareToken(currentToken,"AND"))
            readToken("AND");
        stateF();
    }
}

void stateF()
{
    if (compareTokenType(currentToken,"identifier") || compareTokenType(currentToken,"hexa") || compareTokenType(currentToken,"int") || compareTokenType(currentToken,"string")|| compareToken(currentToken,"TRUE") || compareToken(currentToken,"FALSE"))
    {
        if (compareTokenType(currentToken,"identifier"))
            readTypedToken("identifier");
        else if (compareTokenType(currentToken,"hexa"))
            readTypedToken("hexa");
        else if (compareTokenType(currentToken,"int"))
            readTypedToken("int");
        else if (compareTokenType(currentToken,"string"))
            readTypedToken("string");
        else if (compareToken(currentToken,"TRUE"))
            readToken("TRUE");
        else if (compareToken(currentToken,"FALSE"))
            readToken("FALSE");
    }else if (compareToken(currentToken,"NOT")){
        readToken ("NOT");
        stateF();
    }
    else if (compareToken(currentToken,"(")){
        readToken ("(");
        stateEXP();
        readToken (")");
    }
}
