#include "symbol_table.h"

/**
 * adiciona um simbolo a tabela
 */
void addSymbol (char * name){
    struct Symbol *s;
    //alocar memória
    s = malloc(sizeof(struct Symbol));
    s->byte = tableSize();
    strcpy (s->name,name);
    HASH_ADD_INT( symbolTable, byte,s);
}

/**
 * Inicia a tabela de simbolos (chama as funções clean e initializeDefaults)
 */
void init (){
    //inicializa o hash
    symbolTable = NULL;
    addSymbol("boolean");
    //tipos primitivos
    addSymbol("boolean");
    addSymbol("byte");
    addSymbol("int");
    addSymbol("string");
    //controladores
    addSymbol("while");
    addSymbol("if");
    addSymbol("else");
    addSymbol("begin");
    addSymbol("end");
    //operadores
    addSymbol("and");
    addSymbol("or");
    addSymbol("not");
    addSymbol("==");
    addSymbol("=");
    addSymbol("<=");
    addSymbol(">=");
    addSymbol("+");
    addSymbol("-");
    addSymbol("*");
    addSymbol("/");
    addSymbol("<");
    addSymbol(">");
    addSymbol("!=");
    //constantes
    addSymbol("TRUE");
    addSymbol("FALSE");
    //funcoes
    addSymbol("readln");
    addSymbol("write");
    addSymbol("writeln");
    //outros
    addSymbol("final");
    addSymbol(",");
    addSymbol(";");
    addSymbol("(");
    addSymbol(")");
}

/**
 * retorna a quantidade de itens na tabela
 */
unsigned int tableSize(){
    return HASH_COUNT(symbolTable);
}

/**
 * imprimi a tabela de simbolos
 */
void printTable (){
    printf("\n");
    struct Symbol *s;
    for(s=symbolTable; s != NULL; s=s->hh.next) {
        printf("endereco: %d \t nome: %s \n", s->byte, s->name);
    }
}

int main() {
    init();
    printTable();
    return 0;
}

