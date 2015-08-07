#include "symbol_table.h"

/**
 * adiciona um simbolo a tabela
 */
void addSymbol ( Symbol * s)
{
    HASH_ADD_STR(symbolTable, name,s);
}

/**
 * Cria uma token com o nome e o tipo especificados
 */
 Symbol * createSymbol (char * name, char * clazz, char * type)
{
     Symbol *s;
    //alocar memória
    s = malloc(sizeof( Symbol));
    strcpy (s->name,name);
    if (type != NULL)
        strcpy (s->type,type);
    else
        memset (s->type,0,256);
    if (clazz != NULL)
        strcpy (s->clazz,clazz);
    else
        memset (s->clazz,0,256);
    return s;
}

/**
 * Inicia a tabela de simbolos (chama as funções clean e initializeDefaults)
 */
void initSymbolTable ()
{
    //inicializa o hash
    symbolTable = NULL;
    //tipos primitivos
    addSymbol(createSymbol("caracter","reserverd_word",NULL));
    addSymbol(createSymbol("logico","reserverd_word",NULL));
    addSymbol(createSymbol("real","reserverd_word",NULL));
    addSymbol(createSymbol("inteiro","reserverd_word",NULL));
    //controladores
    addSymbol(createSymbol("inicio","reserverd_word",NULL));
    addSymbol(createSymbol("se","reserverd_word",NULL));
    addSymbol(createSymbol("senao","reserverd_word",NULL));
    addSymbol(createSymbol("entao","reserverd_word",NULL));
    addSymbol(createSymbol("fim","reserverd_word",NULL));
    addSymbol(createSymbol("enquanto","reserverd_word",NULL));
    addSymbol(createSymbol("faca","reserverd_word",NULL));
    //operadores
    addSymbol(createSymbol("+","reserverd_word",NULL));
    addSymbol(createSymbol("-","reserverd_word",NULL));
    addSymbol(createSymbol("or","reserverd_word",NULL));
    addSymbol(createSymbol("*","reserverd_word",NULL));
    addSymbol(createSymbol("div","reserverd_word",NULL));
    addSymbol(createSymbol("mod","reserverd_word",NULL));
    addSymbol(createSymbol("and","reserverd_word",NULL));
    addSymbol(createSymbol("=","reserverd_word",NULL));
    addSymbol(createSymbol("<","reserverd_word",NULL));
    addSymbol(createSymbol("<=","reserverd_word",NULL));
    addSymbol(createSymbol("/","reserverd_word",NULL));
    addSymbol(createSymbol(">","reserverd_word",NULL));
    addSymbol(createSymbol("<>","reserverd_word",NULL));
    addSymbol(createSymbol(">=","reserverd_word",NULL));
    addSymbol(createSymbol(":=","reserverd_word",NULL));
    //funcoes
    addSymbol(createSymbol("escreva","reserverd_word",NULL));
    addSymbol(createSymbol("leia","reserverd_word",NULL));
    //outros
    addSymbol(createSymbol("inicio","reserverd_word",NULL));
    addSymbol(createSymbol("fim","reserverd_word",NULL));
    addSymbol(createSymbol(",","reserverd_word",NULL));
    addSymbol(createSymbol(":","reserverd_word",NULL));
    addSymbol(createSymbol(";","reserverd_word",NULL));
    addSymbol(createSymbol("(","reserverd_word",NULL));
    addSymbol(createSymbol(")","reserverd_word",NULL));
}

/**
 * retorna a quantidade de itens na tabela
 */
unsigned int tableSize()
{
    return HASH_COUNT(symbolTable);
}

/**
 * retorna o simbolo encontrado de acordo com o nome especificado, ou null quando não encontrado
 */
 Symbol * findToken (const char * name)
{
    Symbol *s;
    HASH_FIND_STR (symbolTable, name, s );
    return s;
}

/**
 * Imprimi uma token
 */
void printSymbol ( Symbol *s)
{
    printf("nome: %s \t tipo: %s \t classe: %s \n",  s->name,s->type,s->clazz);
}

/**
 * imprimi a tabela de simbolos
 */
void printTable ()
{
    printf("\n");
     Symbol *s;
    for(s=symbolTable; s != NULL; s=s->hh.next)
        printSymbol(s);
}

/**
 * Compara se duas token são iguais
 */
int compareToken ( Symbol *s1,char * tokenName)
{
    if (s1 == NULL)
        return 0;
    return ( strcmp (s1->name,tokenName) == 0 );
}

/**
 * Compara se duas token tem o mesmo tipo
 */
int compareTokenType ( Symbol *s1,char * tokenType)
{
    if (s1 == NULL)
        return 0;
    return (strcmp(s1->type,tokenType) == 0);
}

/**
 * Compara se duas token tem a mesma classe
 */
int compareTokenClass ( Symbol *s1,char * tokenClazz)
{
    if (s1 == NULL)
        return 0;
    return (strcmp(s1->clazz,tokenClazz) == 0);
}
