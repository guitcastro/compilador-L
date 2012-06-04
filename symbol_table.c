#include "symbol_table.h"

/**
 * adiciona um simbolo a tabela
 */
void addSymbol (struct Symbol * s)
{
    HASH_ADD_STR(symbolTable, name,s);
}

/**
 * Cria uma token com o nome e o tipo especificados
 */
struct Symbol * createSymbol (char * name, char * clazz, char * type)
{
    struct Symbol *s;
    //alocar memória
    s = malloc(sizeof(struct Symbol));
    s->adress = 0;
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
    addSymbol(createSymbol("boolean","reserverd_word",NULL));
    addSymbol(createSymbol("byte","reserverd_word",NULL));
    addSymbol(createSymbol("int","reserverd_word",NULL));
    addSymbol(createSymbol("string","reserverd_word",NULL));
    //controladores
    addSymbol(createSymbol("while","reserverd_word",NULL));
    addSymbol(createSymbol("if","reserverd_word",NULL));
    addSymbol(createSymbol("else","reserverd_word",NULL));
    addSymbol(createSymbol("begin","reserverd_word",NULL));
    addSymbol(createSymbol("end","reserverd_word",NULL));
    //operadores
    addSymbol(createSymbol("AND","reserverd_word",NULL));
    addSymbol(createSymbol("OR","reserverd_word",NULL));
    addSymbol(createSymbol("NOT","reserverd_word",NULL));
    addSymbol(createSymbol("==","reserverd_word",NULL));
    addSymbol(createSymbol("=","reserverd_word",NULL));
    addSymbol(createSymbol("<=","reserverd_word",NULL));
    addSymbol(createSymbol(">=","reserverd_word",NULL));
    addSymbol(createSymbol("+","reserverd_word",NULL));
    addSymbol(createSymbol("-","reserverd_word",NULL));
    addSymbol(createSymbol("*","reserverd_word",NULL));
    addSymbol(createSymbol("/","reserverd_word",NULL));
    addSymbol(createSymbol("<","reserverd_word",NULL));
    addSymbol(createSymbol(">","reserverd_word",NULL));
    addSymbol(createSymbol("!=","reserverd_word",NULL));
    //constantes
    addSymbol(createSymbol("TRUE","reserverd_word","boolean"));
    addSymbol(createSymbol("FALSE","reserverd_word","boolean"));
    //funcoes
    addSymbol(createSymbol("readln","reserverd_word",NULL));
    addSymbol(createSymbol("write","reserverd_word",NULL));
    addSymbol(createSymbol("writeln","reserverd_word",NULL));
    //outros
    addSymbol(createSymbol("final","reserverd_word",NULL));
    addSymbol(createSymbol(",","reserverd_word",NULL));
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
struct Symbol * findToken (const char * name)
{
    struct Symbol *s;
    HASH_FIND_STR (symbolTable, name, s );
    return s;
}

/**
 * Imprimi uma token
 */
void printSymbol (struct Symbol *s)
{
    if (s->type == NULL)
        printf("end: %d \t nome: %s \n", s->adress, s->name);
    else if (s->clazz == NULL)
        printf("end: %d \t nome: %s \t tipo: %s \n", s->adress, s->name,s->type);
    else
        printf("end: %d \t nome: %s \t tipo: %s \t classe: %s \n", s->adress, s->name,s->type,s->clazz);
}

/**
 * imprimi a tabela de simbolos
 */
void printTable ()
{
    printf("\n");
    struct Symbol *s;
    for(s=symbolTable; s != NULL; s=s->hh.next)
        printSymbol(s);
}

/**
 * Compara se duas token são iguais
 */
int compareToken (struct Symbol *s1,char * tokenName)
{
    if (error != 0 || s1 == NULL)
        return 0;
    return ( strcmp (s1->name,tokenName) == 0 );
}

/**
 * Compara se duas token tem o mesmo tipo
 */
int compareTokenType (struct Symbol *s1,char * tokenType)
{
    if (error != 0 || s1 == NULL)
        return 0;
    return (strcmp(s1->type,tokenType) == 0);
}

/**
 * Compara se duas token tem a mesma classe
 */
int compareTokenClass (struct Symbol *s1,char * tokenClazz)
{
    if (error != 0 || s1 == NULL)
        return 0;
    return (strcmp(s1->clazz,tokenClazz) == 0);
}
