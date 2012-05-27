#include "symbol_table.h"

/**
 * adiciona um simbolo a tabela
 */
void addSymbol (struct Symbol * s)
{
    HASH_ADD_STR(symbolTable, name,s);
}

/**
 * Cria uma token
 */
struct Symbol * createSymbol(char * name)
{
    return createTypedSymbol(name,"reserved token");
}

/**
 * Cria uma token com o nome e o tipo especificados
 */
struct Symbol * createTypedSymbol (char * name, char * clazz, char * type)
{
    struct Symbol *s;
    //alocar memória
    s = malloc(sizeof(struct Symbol));
    s->byte = tableSize();
    strcpy (s->name,name);
    if (type != NULL)
        strcpy (s->type,type);
    if (type != NULL)
        strcpy (s->type,type);
    if (type != NULL)
        strcpy (s->type,type);
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
    addSymbol(createSymbol("boolean"));
    addSymbol(createSymbol("byte"));
    addSymbol(createSymbol("int"));
    addSymbol(createSymbol("string"));
    //controladores
    addSymbol(createSymbol("while"));
    addSymbol(createSymbol("if"));
    addSymbol(createSymbol("else"));
    addSymbol(createSymbol("begin"));
    addSymbol(createSymbol("end"));
    //operadores
    addSymbol(createSymbol("AND"));
    addSymbol(createSymbol("OR"));
    addSymbol(createSymbol("NOT"));
    addSymbol(createSymbol("=="));
    addSymbol(createSymbol("="));
    addSymbol(createSymbol("<="));
    addSymbol(createSymbol(">="));
    addSymbol(createSymbol("+"));
    addSymbol(createSymbol("-"));
    addSymbol(createSymbol("*"));
    addSymbol(createSymbol("/"));
    addSymbol(createSymbol("<"));
    addSymbol(createSymbol(">"));
    addSymbol(createSymbol("!="));
    //constantes
    addSymbol(createSymbol("TRUE"));
    addSymbol(createSymbol("FALSE"));
    //funcoes
    addSymbol(createSymbol("readln"));
    addSymbol(createSymbol("write"));
    addSymbol(createSymbol("writeln"));
    //outros
    addSymbol(createSymbol("final"));
    addSymbol(createSymbol(","));
    addSymbol(createSymbol(";"));
    addSymbol(createSymbol("("));
    addSymbol(createSymbol(")"));
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
struct Symbol * findToken (char * name)
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
        printf("endereco: %d \t nome: %s \n", s->byte, s->name);
    else
        printf("endereco: %d \t nome: %s \t tipo: %s \n", s->byte, s->name,s->type);
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
    if (error != 0)
        return -1;
    return ( strcmp (s1->name,tokenName) == 0 );
}

/**
 * Compara se duas token tem o mesmo tipo
 */
int compareTokenType (struct Symbol *s1,char * tokenType)
{
    if (error != 0)
        return -1;
    return (strcmp(s1->type,tokenType) == 0);
}
