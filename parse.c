#include "parse.h"

/**
 * Inicializa o analizador sintático
 * @param file_name nome do arquivo com o código fonte
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
 * @param expectedToken próxima token esperada
 * @return sucesso: 0 , falha: -1
 */
int readToken (char * expectedToken)
{
    //excetuar o casa token apenas se não houver error
    if (error != 0)
        return -1;
    //se a token for a esperada, Ler a próxima token
    if (strcmp(currentToken->name,expectedToken) == 0)
        currentToken = initialState();
    //se a token não for a esperada
    else
    {
        error = 1;
        printUndefinedToken();
        return -1;
    }
    //retorna 0 caso tenha sucesso
    return 0;
}

/**
 * Implementação do casa Token, esperando por uma token com o tipo especifico
 * Os tipos esperados podem ser "boolean" , "integer" e "string"
 * @param expectedType nome do tipo da token esperada
 * @return sucesso: 0 , falha: -1
 */
int readTypedToken (char * expectedType)
{
    //excetuar o casa token apenas se não houver error
    if (error != 0)
        return -1;
    //se a token tiver o tipo esperado, ler a próxima token
    if (strcmp(currentToken->type,expectedType) == 0)
        currentToken = initialState();
    //se a token não tiver o tipo esperado
    else
    {
        printUndefinedToken();
        return -1;
    }
    //retorna 0 caso tenha sucesso
    return 0;
}

/**
 * Implementação do casa Token, esperando por uma token com a classe específica
 * Os tipos esperados podem ser "identifier" e "const"
 * @param expectedClazz nome da classe da token esperada
 * @return sucesso: 0 , falha: -1
 */
int readClazzToken (char * expectedClazz)
{
    //excetuar o casa token apenas se não houver error
    if (error != 0)
        return -1;
    //se a token tiver o a classe esperada, ler a próxima token
    if (strcmp(currentToken->clazz,expectedClazz) == 0)
        currentToken = initialState();
    //se a token não tiver a classe esperada
    else
    {
        printUndefinedToken();
        return -1;
    }
    //retorna 0 caso tenha sucesso
    return 0;
}

/**
 * Lê um identificador
 * @param stateExpected 0 : espera-se que a token não tenha sido declarada previamente
                        1 : espera-se que a token já tenha sido declarada previamente
 * @return sucesso: 0 , falha: -1
 */
int readIdentifier (int stateExpected)
{
    struct Symbol * identifier = &*currentToken;
    readClazzToken ("identifier");
    //se a token não tiver sido declarada
    if (stateExpected == 0 && findToken(identifier ->name) != NULL)
    {
        printUndeclaredIdentifier(identifier );
        return -1;
    }
    //adicionar a token a tabela de simbolos
    else if (stateExpected == 0)
        addSymbol(identifier);
    //caso a token já tenha sido declarada
    else if(findToken(identifier ->name) == NULL)
    {
        printIdentifierAlreadyDeclared(identifier);
        return -1;
    }
    //retornar 0 caso tenha sucesso
    return 0;
}

void printIncompatibleType (){
    char stringError [256];
    strcpy (stringError ,"classe de identificador incompatível [");
    strcat (stringError,currentToken->name);
    strcat (stringError,"].");
    printError (stringError);
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
 * Imprimir error quando o identificador não foi declarado
 */
void printUndeclaredIdentifier (const struct Symbol * id)
{
    char stringError [256];
    strcpy (stringError ,"identificador nao declarado [");
    strcat (stringError,id->name);
    strcat (stringError,"].");
    printError (stringError);
}

/**
 * Imprimir error quando o identificador já foi declarado
 */
void printIdentifierAlreadyDeclared (const struct Symbol * id)
{
    char stringError [256];
    strcpy (stringError ,"identificador ja declarado [");
    strcat (stringError,id->name);
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
            while (compareTokenClass(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
            {
                stateC();
            }
            readToken("end");
        }
    }
}

void stateD ()
{
    const char * type;
    if(compareToken(currentToken,"final"))
    {
        readToken("final");
        struct Symbol * identifier = &*currentToken;
        //verifica se o identificador não foi declarado
        readIdentifier(0);
        readToken("=");
        type = readConst();
        strcpy (identifier->type,type);
        strcpy (identifier->clazz,"const");
    }
    else
    {
        const char * typeExpected;
        if (compareToken(currentToken,"int"))
        {
            readToken("int");
            typeExpected = "integer";
        }
        else if (compareToken(currentToken,"boolean"))
        {
            readToken("boolean");
            typeExpected = "boolean";
        }
        else if(compareToken(currentToken,"byte"))
        {
            readToken("byte");
            typeExpected = "integer";
        }
        else if(compareToken(currentToken,"string"))
        {
            readToken("string");
            typeExpected = "string";
        }
        struct Symbol * identifier = &*currentToken;
        strcpy(identifier->type,typeExpected);
        readIdentifier(0);
        if (compareToken(currentToken,","))
        {
            while (compareToken(currentToken,","))
            {
                readToken(",");
                identifier = &*currentToken;
                strcpy(identifier->type,typeExpected);
                readIdentifier(0);
            }
        }
        else if (compareToken(currentToken,"="))
        {
            readToken("=");
            type = readConst();
            if (strcmp(type,typeExpected) != 0)
                printIncompatibleType();
        }
    }
    readToken(";");
}

/**
 * Lê uma constante
 */
const char *  readConst ()
{
    char type [20];
    if (compareToken(currentToken,"-"))
    {
        readToken("-");
        char temp [256] = "-";
        strcat (temp,currentToken->name);
        strcpy (currentToken->name,temp);
    }
    if (compareTokenType(currentToken,"integer"))
    {
        readTypedToken("integer");
        strcpy (type,"integer");
    }
    else if (compareTokenType(currentToken,"string"))
    {
        readTypedToken("string");
        strcpy (type,"string");
    }
    else if (compareToken(currentToken,"TRUE"))
    {
        readToken("TRUE");
        strcpy (type,"boolean");
    }
    else if (compareToken(currentToken,"FALSE"))
    {
        readToken("FALSE");
        strcpy (type,"boolean");
    }
    const char * bla = type;
    return  bla;
}

void stateC ()
{
    if (compareTokenClass(currentToken,"identifier"))
    {
        //certifica-se que o identificador já foi declarado
        readIdentifier(1);
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
            while (compareTokenClass(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
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
                while (compareTokenClass(currentToken,"identifier") || compareToken(currentToken,"while") ||  compareToken(currentToken,"if") || compareToken(currentToken,"readln") || compareToken(currentToken,"write")|| compareToken(currentToken,"writeln"))
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
        readIdentifier(1);
        readToken(";");
    }
    else if (compareToken(currentToken,"write"))
    {
        readToken("write");
        readToken(",");
        stateEXP();
        while (compareToken(currentToken,","))
        {
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
    if (compareToken(currentToken,"*") || compareToken(currentToken,"/") || compareToken(currentToken,"AND"))
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
    if (compareTokenClass(currentToken,"identifier") || compareTokenType(currentToken,"integer") || compareTokenType(currentToken,"string")|| compareTokenType(currentToken,"boolean"))
    {
        if (compareTokenClass(currentToken,"identifier"))
            readIdentifier(1);
        else if (compareTokenType(currentToken,"integer"))
            readTypedToken("integer");
        else if (compareTokenType(currentToken,"string"))
            readTypedToken("string");
        else if (compareToken(currentToken,"TRUE"))
            readToken("TRUE");
        else if (compareToken(currentToken,"FALSE"))
            readToken("FALSE");
    }
    else if (compareToken(currentToken,"NOT"))
    {
        readToken ("NOT");
        stateF();
    }
    else if (compareToken(currentToken,"("))
    {
        readToken ("(");
        stateEXP();
        readToken (")");
    }
}
