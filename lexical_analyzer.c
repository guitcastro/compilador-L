#include "lexical_analyzer.h"

/**
 * Inicializa o analizador lexico
 */
int initLexicalAnalyzer (const char * file_name)
{
    //inicializar a tabela de simbolos
    initSymbolTable ();
    //carrega o arquivo de código fonte
    source_code = fopen(file_name,"r");
    if (source_code == NULL)
    {
        perror ("Error ao abrir o arquivo");
        return -1;
    }
    lineNumber = 1;
    return 0;
}

/**
 * Lê o próximo caracterer da linguagem
 */
char readNextChar ()
{
    if (feof(source_code))
        return '\0';
    char character = fgetc (source_code);
    if (feof(source_code))
        return '\0';
    //checa se foi encontradado algum error ao ler o arquivo e caso tenha sido encontrado retornar um caracterer nulo
    if (ferror(source_code))
    {
        perror ("Error ao ler o arquivo");
        return '\0';
    }
    //checa se é final, e caso seja incrementa o contado de número de linhas do arquivo
    if (character == '\n')
        lineNumber++;
    //se o próximo caracter for 13, pular o mesmo
    if (character == 13)
        return readNextChar();
    //verificar se o caracter é um caracter válido no alfabeto ou EOF ou Fim de linha
    if (isAlphabetical(character) || isNumeric(character) || isDelimiter(character)
        || character == '+' || character == '-'
        || character == '=' || character == '<' || character == '>'
        || character == ':'
        || character == ';'
        || character == ','
        || character == '*' || character == '/'
        || character == EOF)
    {
        //adicona o caractere ao buffer
        buffer[strlen(buffer)]= character;
        buffer[strlen(buffer)+1]= '\0';
        return character;
    }
    //caso contrário o caracterer não pertence a linguagem
    else
    {
        printError ("caractere invalido.");
        return '\0';
    }
}

/**
 * Estado inicial do automato
 */
 Symbol * initialState()
{
    memset (buffer,0,256);
    Symbol *token = NULL;
    char character = readNextChar();
    //caso começar com _ , a-z, A-Z ler o identificador (ou palavra reservada)
    if (isAlphabetical(character))
        token = readIdentifiers();
    //caso contrário
    else if (character == '+' || character == '-'){
        char character = readNextChar();
        if (!isNumeric(character))
        {
            rewindPointer();
            token = findToken(buffer);
        }
        else
        {
            token = readNumber();
        }
    }
    //se for um delimitar ignorar o mesmo
    else if (character == EOF || isDelimiter(character))
        return initialState();
    //operadores com apenas um caractere
    else if (character == ';'|| character == ',' || character == '*' ||
             character == '(' ||  character == ')' || character == '=' || character == '/')
    {
        token = findToken(buffer);
    }
    //operadores com dois caracteres
    else if (character == '<' ||character == '>' ||character == ':')
    {
        char aux = character;
        character  = readNextChar();
        if (character != '=')
        {
            if (!(aux == '<' && character == '>'))
            {
                rewindPointer();
            }
        }

        token = findToken(buffer);
    }
    //fechar arquivo
    if (token == NULL)
        fclose(source_code);
    return token;
}

/**
 * Lê uma identificador, ou palavra reservada
 */
 Symbol * readIdentifiers ()
{
    char c = readNextChar();
    Symbol *s;
    while (isAlphabetical(c) || isNumeric(c))
        c = readNextChar();
    if (c != '\0')
        rewindPointer();
    //remover o caracter lido
    if (findToken(buffer) != NULL)
        s = findToken(buffer);
    else
        s = createSymbol(buffer,"identifier", NULL);
    return s;
}

/**
 * Read digits and concatenate it to the buffer until find a non digit character
 * @return the last read character or '\0' in case of error
*/
char readDigits(){
    char c;
    do
    {
        c = readNextChar();
        if (!isValidAndNotEof(c))
            return '\0';
    }
    while isNumeric(c);
    return c;
}


/**
 * Read a integer number or a real number
 */
Symbol * readNumber ()
{
    char c = readDigits();
    if (c == '.')
    {
        c = readDigits();
    }
    rewindPointer();
    return createSymbol(buffer,"const","integer");
}

void printError (char * error)
{
    printf("%d:%s",lineNumber,error);
    exit (EXIT_FAILURE);
}

void printUndefinedLexical ()
{
    char stringError [256];
    strcpy (stringError ,"lexema nao identificado [");
    strcat (stringError,buffer);
    strcat (stringError,"].");
    printError (stringError);
}

/**
 * checa se o caracterer lido é válido (esta contindo no alfabeto e não é fim de arquivo)
 */
int isValidAndNotEof (char c)
{
    if (c == EOF )
        printError("fim de arquivo nao esperado.");
    return (c != EOF && c != '\0');
}

/**
 * Volta em uma posição o ponteiro do arquivo e o buffer
 */
void rewindPointer ()
{
    if (buffer[strlen(buffer)-1] == '\n')
    {
        lineNumber--;
    }
    buffer[strlen(buffer)-1]= '\0';

    fseek ( source_code ,- 1 , SEEK_CUR);
}
