#include "parse.h"
#include "code_generator.h"

/**
 * Inicializa o analizador sintÃ¡tico
 * @param file_name nome do arquivo com o cÃ³digo fonte
 */
int initParse (const char * file_name)
{
    initLexicalAnalyzer(file_name);
    currentToken = initialState();
    stateS();
    return 0;
}

/**
 * ImplementaÃ§Ã£o do casa Token
 * @param expectedToken prÃ³xima token esperada
 * @return sucesso: 0 , falha: -1
 */
int readToken (char * expectedToken)
{
    //se a token for a esperada, Ler a prÃ³xima token
    if (currentToken != NULL &&  strcmp(currentToken->name,expectedToken) == 0)
        currentToken = initialState();
    //se a token nÃ£o for a esperada
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
 * LÃª um identificador
 * @param stateExpected 0 : espera-se que a token nÃ£o tenha sido declarada previamente
                        1 : espera-se que a token jÃ¡ tenha sido declarada previamente
 * @return sucesso: 0 , falha: -1
 */
int readIdentifier (int stateExpected)
{
    Symbol * identifier = &*currentToken;
    readClazzToken ("identifier");
    //se a token nÃ£o tiver sido declarada
    if (stateExpected == 0 && findToken(identifier ->name) != NULL)
        printIdentifierAlreadyDeclared(identifier);
    //adicionar a token a tabela de simbolos
    else if (stateExpected == 0)
        addSymbol(identifier);
    //caso a token jÃ¡ tenha sido declarada
    else if(findToken(identifier ->name) == NULL)
        printUndeclaredIdentifier(identifier );
    //retornar 0 caso tenha sucesso
    return 0;
}

void printIncompatibleType ()
{
    char stringError [256];
    strcpy (stringError ,"classe de identificador incompatÃ­vel [");
    strcat (stringError,currentToken->name);
    strcat (stringError,"].");
    printError (stringError);
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
 * Imprimir error quando o identificador nÃ£o foi declarado
 */
void printUndeclaredIdentifier (const Symbol * id)
{
    char stringError [256];
    strcpy (stringError ,"identificador nao declarado [");
    strcat (stringError,id->name);
    strcat (stringError,"].");
    printError (stringError);
}

/**
 * Imprimir error quando o identificador jÃ¡ foi declarado
 */
void printIdentifierAlreadyDeclared (const  Symbol * id)
{
    char stringError [256];
    strcpy (stringError ,"identificador ja declarado [");
    strcat (stringError,id->name);
    strcat (stringError,"].");
    printError (stringError);
}

/**
 * Estado inicial da gramÃ¡tica
 */
void stateS()
{
    while (compareToken(currentToken,"int") || compareToken(currentToken,"boolean") ||  compareToken(currentToken,"byte") || compareToken(currentToken,"string") || compareToken(currentToken,"final"))
    {
        stateD();
    }
    while (compareToken(currentToken,"begin"))
    {
        stateB();
    }
    if (currentToken != NULL)
        printUndefinedLexical();

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

void stateD ()
{
    Symbol * constant;
    if(compareToken(currentToken,"final"))
    {
        readToken("final");
         Symbol * identifier = &*currentToken;
        //verifica se o identificador nÃ£o foi declarado
        readIdentifier(0);
        readToken("=");
        constant = readConst();
        strcpy (identifier->type,constant->type);
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
            typeExpected = "byte";
        }
        else if(compareToken(currentToken,"string"))
        {
            readToken("string");
            typeExpected = "string";
        }
        Symbol * identifier = &*currentToken;
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
            constant = readConst();
            if (strcmp(typeExpected,"integer")==0)
                checkIntegerOrByte(constant->type);
            else if (strcmp(constant->type,typeExpected) != 0)
                printIncompatibleType();
        }
    }
    readToken(";");
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
    Symbol exp;
    if (compareTokenClass(currentToken,"identifier"))
    {
        //certifica-se que o identificador jÃ¡ foi declarado
        Symbol * s = findToken(currentToken->name);
        readIdentifier(1);
        readToken("=");
        if (strcmp(s->clazz,"const")==0)
            printIncompatibleType();

        exp = stateEXP();
        if(strcmp(s->type, exp.type) != 0 && !hasByteAndInteger(s->type,exp.type))
            printError("tipos incompatÃ­veis.");

        else if(strcmp(exp.type,"byte") == 0)
            checkIntegerOrByte(s->type);
        readToken(";");
    }
    else if (compareToken(currentToken,"while"))
    {
        readToken("while");
        exp = stateEXP();
        if(!checkBoolean(exp.type))
            printIncompatibleType();

        if (compareToken(currentToken,"begin"))
            stateB();
        else
            stateC();
    }
    else if (compareToken(currentToken,"if"))
    {
        readToken("if");
        exp = stateEXP();
        checkBoolean(exp.type);

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
        Symbol *s  = findToken(currentToken->name);
        readIdentifier(1);
        if (strcmp (s->type,"boolean") == 0)
            printError("tipos incompatÃ­veis.");
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
        exp = stateEXP();
        while (compareToken(currentToken,","))
        {
            readToken(",");
            exp = stateEXP();
        }
        readToken(";");
    }
}

Symbol stateEXP()
{
     Symbol EXPS = stateEXPS();
     Symbol EXPS2;
    if (compareToken(currentToken,"=="))
    {
        readToken("==");
        EXPS2 = stateEXPS();
        if(strcmp(EXPS.type, "boolean") == 0){
            checkBoolean(EXPS2.type);
        }
        else if(strcmp(EXPS.type, "string") == 0){
            checkString(EXPS2.type);
        }
        else{
            checkIntegerOrByte(EXPS2.type);
        }
        strcpy(EXPS.type,"boolean");
        return EXPS;
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
        EXPS2 = stateEXPS();
        if(strcmp(EXPS.type, "boolean") == 0)
            checkBoolean(EXPS2.type);
        else if(strcmp(EXPS.type, "string") == 0)
            printError("tipos incompatÃ­veis.");
        else
            checkIntegerOrByte(EXPS2.type);
        strcpy (EXPS.type,"boolean");
    }
    return EXPS;
}

Symbol stateEXPS()
{

     Symbol T1;
     Symbol T2;
     Symbol EXPS;
    if (compareToken(currentToken,"-"))
    {
        readToken("-");
        T1 = stateT();
        checkIntegerOrByte(T1.type);
        EXPS = T1;
    }
    else
    {
        EXPS = T1 = stateT();
    }
    while (compareToken(currentToken,"-") || compareToken(currentToken,"+") || compareToken(currentToken,"OR"))
    {
        if (compareToken(currentToken,"-"))
        {
            readToken("-");
            T2 = stateT();
            strcpy(EXPS.type,setIntegerOrByte(T1.type,T2.type));
        }
        else if (compareToken(currentToken,"+"))
        {
            checkIntegerOrByteOrString(T1.type);
            readToken("+");
            T2 = stateT();
            //se o F for string o F1 também deve ser
            if (strcmp(T1.type,"string") == 0)
            {
                if (strcmp(T2.type,"string") != 0)
                    printError("tipos incompativeis.");
            }
            else{
                strcpy(EXPS.type,setIntegerOrByte(T1.type,T2.type));
            }
        }
        else if (compareToken(currentToken,"OR"))
        {
            readToken("OR");
            T2 = stateT();
            if (!checkBooleanExp(T1.type,T2.type)){
                EXPS.adress =-1;
            }
        }
    }
    return EXPS;
}

 Symbol stateT ()
{
     Symbol F1;
     Symbol F2;
     Symbol T = F1 = stateF();

    if (compareToken(currentToken,"*"))
    {
        readToken("*");
        F2 = stateF();
        if (T.type != NULL)
            strcpy(T.type,setIntegerOrByte(F1.type,F2.type));
        else{
            T.adress = -1;
            return T;
        }
    }
    else if (compareToken(currentToken,"/"))
    {
        readToken("/");
        F2 = stateF();
        setIntegerOrByte(F1.type,F2.type);
        strcpy(T.type,"integer");
    }
    else if (compareToken(currentToken,"AND"))
    {
        readToken("AND");
        F2 = stateF();
        if (!checkBooleanExp(F1.type,F2.type))
        {
            T.adress = -1;
            return T;
        }
        else{
            return T;
        }
    }
    return T;
}

 Symbol stateF()
{
     Symbol F = * currentToken;
    if (compareTokenClass(currentToken,"identifier"))
    {
        //se ler o identificador, apenas retornar o mesmo
        readIdentifier(1);
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
        if (!checkBoolean(F.type))
            F.adress = -1;
    }
    else if (compareToken(currentToken,"("))
    {
        readToken ("(");
        F = stateEXP();
        readToken (")");
    }else{
        printUndefinedToken();
    }
    return F;
}

/**
 * verifica os tipos de uma expressÃ£o boolean (em que os dois operandos sÃ£o boolean)
 * ex ( a or b), (a && b)
 * @param typeX tipo a ser verificado
 * @param typeY tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkBooleanExp (char * typeX,char * typeY)
{
    return (checkBoolean(typeX) && checkBoolean(typeY));
}

/**
 * verifica se determinado tipo Ã© um boolean
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkBoolean (char * type)
{
    if (strcmp(type,"boolean") != 0)
    {
        printError("tipos incompatÃ­veis.");
    }
    return 1;
}

/**
 * verifica se determinado tipo Ã© um string
 * @param type tipo a ser verificado
 */
void checkString (char * type)
{
    if (strcmp(type,"string") != 0)
        printError("tipos incompatÃ­veis.");
}

/**
 * Checa se o tipo Ã© byte ou inteiro ou string
 * @param type tipo a ser verificado
 * @return o tipo se ele for string, byte ou inteiro , NULL caso seja diferente
 */
char * checkIntegerOrByteOrString (char * type)
{
    if (strcmp (type,"string") == 0)
        return "string";
    if (strcmp (type,"byte") == 0)
        return "byte";
    if (strcmp (type,"integer") == 0)
        return "integer";
    printError("tipos incompatÃ­veis.");
    return NULL;
}

/**
 * Checa se o tipo Ã© byte ou inteiro
 * @param type tipo a ser verificado
 */
void checkIntegerOrByte (char * type)
{
    if (!isIntegerOrByte(type))
        printError("tipos incompatÃ­veis.");
}

/**
 * Determina qual serÃ¡ o tipo resultado da operaÃ§Ã£o entre bytes em inteiros
 * @param typeX tipo do operando a esquerda
 * @param typeY tipo do operando a direita
 * @return O tipo resultante da operaÃ§Ã£o
 */
char * setIntegerOrByte (char * typeX,char * typeY)
{
    //primeiro verificar se os dois tipos sÃ£o inteiros ou bytes
    checkIntegerOrByte(typeX);
    checkIntegerOrByte(typeY);
    //ou exlcusivo que determina se apenas 1 operador Ã© byte
    //nesse caso o tipo deve ser tranformado em inteiro
    if (hasByteAndInteger(typeX,typeY))
        return "integer";
    //se nÃ£o os dois tem o mesmo tipo (byte, ou inteiro)
    else
        return typeX;
}

