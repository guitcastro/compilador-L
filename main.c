#include "parse.h"

extern int error = 0;

int main(int argc, const char *argv[])
{
    //inicializar a tabela de simbolos
    initParse(argv[1]);
    //printTable();
    return 0;
}
