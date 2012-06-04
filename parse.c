#include "parse.h"
#include "code_generator.h"

/**
 * Inicializa o analizador sintático
 * @param file_name nome do arquivo com o código fonte
 */
int initParse (const char * file_name)
{
    initLexicalAnalyzer(file_name);
    initCodeGen(file_name);
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

void printIncompatibleType ()
{
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
    while (compareToken(currentToken,"int") || compareToken(currentToken,"boolean") ||  compareToken(currentToken,"byte") || compareToken(currentToken,"string") || compareToken(currentToken,"final"))
    {
        stateD();
    }
    closeDseg();
    openCseg();
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
    struct Symbol * constant;
    if(compareToken(currentToken,"final"))
    {
        readToken("final");
        struct Symbol * identifier = &*currentToken;
        //verifica se o identificador não foi declarado
        readIdentifier(0);
        readToken("=");
        constant = readConst();
        strcpy (identifier->type,constant->type);
        strcpy (identifier->clazz,"const");
        defConst(identifier,constant);
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
        struct Symbol * identifier = &*currentToken;
        strcpy(identifier->type,typeExpected);
        readIdentifier(0);
        defIdentifier(identifier);
        if (compareToken(currentToken,","))
        {
            while (compareToken(currentToken,","))
            {
                readToken(",");
                identifier = &*currentToken;
                strcpy(identifier->type,typeExpected);
                readIdentifier(0);
                defIdentifier(identifier);
            }
        }
        else if (compareToken(currentToken,"="))
        {
            readToken("=");
            constant = readConst();
            if (strcmp(constant->type,typeExpected) != 0)
                printIncompatibleType();
        }
    }
    readToken(";");
}

/**
 * Lê uma constante
 */
struct Symbol * readConst ()
{
    struct Symbol * constant;
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

char * stateC ()
{
    if (compareTokenClass(currentToken,"identifier"))
    {
        //certifica-se que o identificador já foi declarado
        struct Symbol * s = findToken(currentToken->name);
        readIdentifier(1);
        readToken("=");
        if (strcmp(s->clazz,"const")==0)
        {
            printIncompatibleType();
            return NULL;
        }
        char * typeEXP = stateEXP();
        if(strcmp(s->type, typeEXP) != 0 && strcmp(typeEXP,"byte") != 0)
        {
            printError("tipos incompatíveis.");
            return NULL;
        }
        else if(strcmp(typeEXP,"byte") == 0)
        {
            if(!checkIntegerOrByte(s->type))
                return NULL;
        }
        readToken(";");
    }
    else if (compareToken(currentToken,"while"))
    {
        readToken("while");
        char * typeEXP = stateEXP();
        if(!checkBoolean(typeEXP)){
            printIncompatibleType();
            return NULL;
        }
        if (compareToken(currentToken,"begin"))
            stateB();
        else
            stateC();
    }
    else if (compareToken(currentToken,"if"))
    {
        readToken("if");
        char * typeEXP = stateEXP();
        if(!checkBoolean(typeEXP))
            return NULL;
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
        struct Symbol *s  = findToken(currentToken->name);
        readIdentifier(1);
        if (strcmp (s->type,"boolean") == 0)
        {
            printError("tipos incompatíveis.");
            return NULL;
        }
        readToken(";");
    }
    else if (compareToken(currentToken,"write") || compareToken(currentToken,"writeln"))
    {
        if(compareToken(currentToken, "write"))
            readToken("write");
        else if(compareToken(currentToken, "writeln"))
            readToken("writeln");
        readToken(",");
        char * typeEXP = stateEXP();
        if(!checkString(typeEXP))
            return NULL;
        while (compareToken(currentToken,","))
        {
            readToken(",");
            typeEXP = stateEXP();
            if(!checkString(typeEXP))
                return NULL;

        }
        readToken(";");
    }
    return NULL;
}

char * stateEXP()
{
    struct Symbol EXPS1 = stateEXPS();
    struct Symbol EXPS2;
    if (compareToken(currentToken,"=="))
    {
        readToken("==");
        EXPS2 = stateEXPS();
        if(strcmp(EXPS1.type, "boolean"))
        {
            if (!checkBoolean(EXPS2.type));
            return NULL;
        }
        else if(strcmp(EXPS1.type, "string"))
        {
            if(!checkString(EXPS2.type))
                return NULL;
        }
        else if(!checkIntegerOrByte(EXPS2.type))
            return NULL;

        return "boolean";

    }
    else if (compareToken(currentToken,"!=") || compareToken(currentToken,"<") || compareToken(currentToken,">") || compareToken(currentToken,"<=") || compareToken(currentToken,">=") )
    {
        if (compareToken(currentToken,"!="))
            readToken("!=");
        else if (compareToken(currentToken,"<"))
            readToken("<");
        else if (compareToken(currentToken,">"))
            readToken(">");
        else if (compareToken(currentToken,"<="))
            readToken("<=");
        else if (compareToken(currentToken,">="))
            readToken(">=");
        EXPS2 = stateEXPS();
        if(strcmp(EXPS1.type, "boolean") == 0)
        {
            if (!checkBoolean(EXPS2.type));
            return NULL;
        }
        else if(strcmp(EXPS1.type, "string") == 0)
        {
            printError("tipos incompatíveis.");
            return NULL;
        }
        else if(!checkIntegerOrByte(EXPS2.type))
            return NULL;

        return "boolean";
    }
    char * x = EXPS1.type;
    return x;
}

struct Symbol stateEXPS()
{

    struct Symbol T1;
    struct Symbol T2;
    struct Symbol EXPS;
    if (compareToken(currentToken,"-"))
    {
        readToken("-");
        T1 = stateT();
        if (!checkIntegerOrByte(T1.type)){
            strcpy(T1.type,"error");
            return T1;
        }
        strcpy(T1.type,"integer");
        //Um valor negado (-) se transforma em inteiro.
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
            if (!checkIntegerOrByteOrString(T1.type)){
                 error = -1;
                 T1.adress = -1;
                 return T1;
            }
            readToken("+");
            T2 = stateT();
            //se o F for string o F1 também deve ser
            if (strcmp(T1.type,"string") == 0)
            {
                if (strcmp(T2.type,"string"))
                {
                    printError("tipos incompatíveis.");
                    T1.adress = -1;
                    error = -1;
                    return T1;
                }
            }
            else
                strcpy(EXPS.type,setIntegerOrByte(T1.type,T2.type));
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

struct Symbol stateT ()
{
    struct Symbol T = stateF();
    struct Symbol F1;
    struct Symbol F2;

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
        int isInteger = (strcmp(T.type,"integer") == 0) ? 1 : 0;
        T.adress = genMultiply(&F1,&F2,isInteger);
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
        else
            return T;
    }
    return T;
}

struct Symbol stateF()
{
    struct Symbol F = * currentToken;
    if (compareTokenClass(currentToken,"identifier"))
    {
        struct Symbol * s = findToken(currentToken->name);
        readIdentifier(1);
        strcpy(F.type,s->type);
    }
    else if (compareTokenType(currentToken,"integer"))
    {
        int num = atoi(currentToken->name);
        readTypedToken("integer");
        (num >= 0 && num <= 255) ? strcpy (F.type,"byte") : strcpy (F.type,"integer");
        defTempConst(&F);
    }
    else if (compareTokenType(currentToken,"string"))
    {
        readTypedToken("string");
        strcpy(F.type,"string");
        defTempConst(&F);
    }
    else if (compareToken(currentToken,"TRUE"))
    {
        readToken("TRUE");
        strcpy(F.type,"boolean");
        defTempConst(&F);
    }
    else if (compareToken(currentToken,"FALSE"))
    {
        readToken("FALSE");
        strcpy(F.type,"boolean");
        defTempConst(&F);
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
        char * type = stateEXP();
        strcpy(F.type,type);
        readToken (")");
    }
    return F;
}

/**
 * verifica os tipos de uma expressão boolean (em que os dois operandos são boolean)
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
 * verifica se determinado tipo é um boolean
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkBoolean (char * type)
{
    if (strcmp(type,"boolean") != 0)
    {
        printError("tipos incompatíveis.");
        error = -1;
        return 0;
    }
    return 1;
}

/**
 * verifica se determinado tipo é um string
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkString (char * type)
{
    if (strcmp(type,"string") != 0)
    {
        printError("tipos incompatíveis.");
        error = -1;
        return 0;
    }
    return 1;
}

/**
 * Checa se o tipo é byte ou inteiro ou string
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
    printError("tipos incompatíveis.");
    error = -1;
    return NULL;
}

/**
 * Checa se o tipo é byte ou inteiro
 * @param type tipo a ser verificado
 * @return 1: sucesso
 *         0: error
 */
int checkIntegerOrByte (char * type)
{
    if (!isIntegerOrByte(type))
    {
        printError("tipos incompatíveis.");
        error = -1;
        return 0;
    }
    return 1;
}

/**
 * Determina qual será o tipo resultado da operação entre bytes em inteiros
 * @param typeX tipo do operando a esquerda
 * @param typeY tipo do operando a direita
 * @return O tipo resultante da operação
 */
char * setIntegerOrByte (char * typeX,char * typeY)
{
    //primeiro verificar se os dois tipos são inteiros ou bytes
    if (!checkIntegerOrByte(typeX))
        return 0;
    if (!checkIntegerOrByte(typeY))
        return 0;
    //ou exlcusivo que determina se apenas 1 operador é byte
    //nesse caso o tipo deve ser tranformado em inteiro
    else if (hasByte(typeX,typeY))
        return "integer";
    //se não os dois tem o mesmo tipo (byte, ou inteiro)
    else
        return typeX;
}
