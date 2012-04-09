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
    return createConstantSymbol(name,NULL);
}

/**
 * Cria uma token com o nome e o tipo especificados
 */
struct Symbol * createConstantSymbol (char * name, char * type)
{
    struct Symbol *s;
    //alocar memória
    s = malloc(sizeof(struct Symbol));
    s->byte = tableSize();
    strcpy (s->name,name);
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
    addSymbol(createSymbol("and"));
    addSymbol(createSymbol("or"));
    addSymbol(createSymbol("not"));
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
void printT able ()
{
    printf("\n");
    struct Symbol *s;
    for(s=symbolTable; s != NULL; s=s->hh.next)
        printSymbol(s);
}
