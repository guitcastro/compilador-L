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
    if ((character > 31 && character < 126 && character != 35 && character != 36 && character != 37 && character != 38 && character != 64 && character != 94 && character != 96 )|| character == EOF || character == '\n' || character == '\t')
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
    //caso começar com aspas, ler uma string
    if (character == '"')
        token = readString ();
    //caso começar com _ , a-z, A-Z ler o identificador (ou palavra reservada)
    else if (character == '_' || isAlphabetical(character))
        token = readIdentifiers();
    //caso seja comentário, ignorar o mesmo
    else if (character == '{')
    {
        readBracesComment();
        return initialState();
    }
    //caso seja digito decimal ou hexadecimal
    else if (character == '0')
        token =  readDecOrHexa();
    //caso contrário
    else if (isNumeric(character))
        token = readInteger();
    //se for um delimitar ignorar o mesmo
    else if (character == EOF || isDelimiter(character))
        return initialState();
    //operadores com apenas um caractere
    else if (character == ';'|| character == '+'||character == '-'|| character == ',' || character == '*' || character == '(' ||  character == ')' )
        token = findToken(buffer);
    //operadores com dois caracteres, sendo o segundo '='
    else if (character == '='|| character == '<'||character == '>')
    {
        character  = readNextChar();
        if (character != '=')
            rewindPointer();
        token = findToken(buffer);
    }
    //caso único da '!'
    else if (character == '!')
    {
        character  = readNextChar();
        if (character != '=')
        {
            printUndefinedLexical();
            return NULL;
        }
        token = findToken(buffer);
    }
    //caso seja um divisor ou comentário
    else if (character == '/')
    {
        character = readNextChar();
        //caso seja divisor
        if (character != '*')
        {
            rewindPointer();
            token = findToken("/");
        }
        //caso seja comentário
        else
        {
            readComment();
            return initialState();
        }
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
    while (c == '_' || isAlphabetical(c) || isNumeric(c))
        c = readNextChar();
    if (c != '\0')
        rewindPointer();
    //remover o caracter lido
    if (isReservedWord(buffer))
        s = findToken(buffer);
    else
        s = createSymbol(buffer,"identifier", NULL);
    return s;
}

/**
 * Lê um comentário, no formato { comentário }
 */
int readBracesComment()
{
    char c = readNextChar();
    while (c != '}')
    {
        c = readNextChar();
        if (!isValidAndNotEof(c))
            return -1;
    }
    return 0;
}

/**
 * Lê um digito hexadecimal ou decimal
 */
 Symbol * readDecOrHexa ()
{
    char c;
    c = readNextChar();
    if (!isValidAndNotEof(c))
        return NULL;
    if (c == 'x')
        return readHexa();
    rewindPointer();
    return readInteger();
}

/**
 * Lê um número inteiro
 */
 Symbol * readInteger ()
{
    char c;
    do
    {
        c = readNextChar();
        if (!isValidAndNotEof(c))
            return NULL;
    }
    while isNumeric(c);
    rewindPointer();
    return createSymbol(buffer,"const","integer");
}

/**
 * Lê um digito no formato hexadecimal
 */
 Symbol * readHexa ()
{
    memset (buffer,0,256);
    char c;
    int i;
    for (i=0; i<2; i++)
    {
        c =  readNextChar();
        if (!isValidAndNotEof(c))
            return NULL;
        //verfica se é numero ou A-F
        if (!isNumeric(c) && (c > 70 || c < 65))
        {
            printUndefinedLexical();
            return NULL;
        }
    }
    int num;
    sscanf (buffer,"%X",&num);
    snprintf(buffer,256,"%d",num);
    return createSymbol(buffer,"const","integer");
}

/**
 * Lê um comentário, ou o operador de divisão
 */
int readComment()
{
    char temp = buffer[0];
    char c = readNextChar();
    if (!isValidAndNotEof(c))
        return -1;
    while (c != '/' || temp != '*')
    {
        temp = c;
        c = readNextChar();
        if (!isValidAndNotEof(c))
            return -1;

    }
    return 0;
}

/**
* Ler uma constante string do código fonte
*/
 Symbol * readString ()
{
    //remover aspa inicial da string:
    buffer[0] = '\0';
    //ler a string até o final
    char c;
    do
    {
        c = readNextChar();
        if (!isValidAndNotEof(c))
            return NULL;
        if (c == '\n')
        {
            printUndefinedLexical();
            return NULL;
        }
    }
    while (c != '"');
    //remover a aspa final da string
    buffer[strlen(buffer)-1]= '\0';
    return createSymbol(buffer,"const","string");
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
