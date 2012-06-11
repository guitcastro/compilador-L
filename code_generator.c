#include "code_generator.h"

void initCodeGen (const char * file_name)
{
    currentEnd = 20000;
    tempEnd = 0;
    rotCount=0;
    char * file_ext = strrchr(file_name,'.');
    if (file_ext != NULL)
        strcpy(file_ext, ".asm");
    else
        strcat(file_ext, ".asm");
    file_name = "masm/teste.asm";
    asm_file = fopen(file_name,"w");
    //inicia o segmento de pilha

    fputs ("sseg SEGMENT STACK			; inicio seg. pilha\n",asm_file);
    fputs ("   byte 20000 DUP(?) \n",asm_file);
    fputs ("sseg ENDS				;fim seg. pilha\n",asm_file);
    //inicia o segmento de declarações
    openDseg();
    fputs ("    byte 20000 DUP(?)		; temporarios\n",asm_file);
}

void finishCogGen ()
{
    fputs ("\n;termina o programa\n",asm_file);
    fputs ("    mov ah, 4Ch			; termina o programa\n",asm_file);
    fputs ("    int 21h	            ; fim seg. código\n",asm_file);
    fputs ("cseg ENDS				;\n",asm_file);
    fputs ("END strt					\n",asm_file);
    fclose(asm_file);
}

void openDseg ()
{
    fputs ("\ndseg SEGMENT PUBLIC			; inicio seg. dados\n",asm_file);
}

void closeDseg ()
{
    fputs ("dseg ENDS				; fim seg. dados\n",asm_file);
}

void openCseg()
{
    fputs ("cseg SEGMENT PUBLIC 			; inicio do seg. código\n",asm_file);
    fputs ("    ASSUME CS:cseg, DS:dseg	\n\n",asm_file);
    fputs ("    strt:					; inicio do programa\n",asm_file);
    fputs ("        mov ax, dseg\n",asm_file);
    fputs ("        mov ds, ax\n",asm_file);

}

void defConst( Symbol * identifier, Symbol * constant)
{
    identifier->adress = currentEnd;
    if (compareTokenType(identifier,"byte"))
    {
        fprintf (asm_file,"    byte %s             ;const %s do tipo byte no endereco: %d\n",constant->name,identifier->name,currentEnd);
        //reservou 1  byte na memória
        currentEnd++;
    }
    if (compareTokenType(identifier,"integer"))
    {
        fprintf (asm_file,"    sword %s             ;const %s do tipo integer no endereco: %d\n",constant->name,identifier->name,currentEnd);
        //reservou 1  byte na memória
        currentEnd+=2;
    }
    if (compareTokenType(identifier,"string"))
    {
        fprintf (asm_file,"    byte \"%s$\"             ;const %s do tipo string no endereco: %d\n",constant->name,identifier->name,currentEnd);
        //reservou 1  byte na memória
        currentEnd+= (strlen(constant->name) + 1);
    }
    if (compareTokenType(identifier,"boolean"))
    {
        char * value;
        if (strcmp(constant->name,"TRUE") == 0)
            value = "0ffh";
        else
            value = "000h";
        fprintf (asm_file,"    byte %s             ;const %s do tipo boolean no endereco: %d\n",value,identifier->name,currentEnd);
        //reservou 1  byte na memória
        currentEnd++;
    }
}

void defIdentifier( Symbol * identifier)
{
    identifier->adress = currentEnd;
    if (compareTokenType(identifier,"byte") || compareTokenType(identifier,"boolean"))
    {
        fprintf (asm_file,"    byte ?             ;identifier %s do tipo %s no endereco: %d\n",identifier->name,identifier->type,currentEnd);
        currentEnd++;
    }
    if (compareTokenType(identifier,"integer"))
    {
        fprintf (asm_file,"    sword ?             ;identifier %s do tipo integer no endereco: %d\n",identifier->name,currentEnd);
        currentEnd+=2;
    }
    if (compareTokenType(identifier,"string"))
    {
        fprintf (asm_file,"    byte 256 DUP(?)             ;identifier %s do tipo string no endereco: %d\n",identifier->name,currentEnd);
        currentEnd+= 256;
    }
}

void defTempConst(Symbol * s)
{
    fputs ("\n",asm_file);
    s->adress = tempEnd;
    if (strcmp(s->type,"integer") == 0)
    {
        fprintf (asm_file,"    mov ax, %s             ;atribuicao\n",s->name);
        fprintf (asm_file,"    mov DS:[%d], ax\n",tempEnd);
        tempEnd+=2;
    }
    if (strcmp(s->type,"byte") == 0)
    {
        fprintf (asm_file,"    mov al, %s             ;atribuicao\n",s->name);
        fprintf (asm_file,"    mov DS:[%d], al\n",tempEnd);
        tempEnd+=1;
    }
    if (strcmp(s->type,"boolean") == 0)
    {
        char * value;
        if (strcmp(s->name,"TRUE") == 0)
            value = "0ffh";
        else
            value = "000h";
        fprintf (asm_file,"    mov al, %s             ;atribuicao\n",value);
        fprintf (asm_file,"    mov DS:[%d], al\n",tempEnd);
        tempEnd+=1;
    }
    if(strcmp(s->type,"string") == 0)
    {
        openDseg();
        fprintf (asm_file,"    byte \"%s$\"             ;const do tipo string no endereco: %d\n",s->name,currentEnd);
        s->adress = currentEnd;
        currentEnd+= (strlen(s->name) + 1);
        closeDseg();
    }
}

int genMultiply ( Symbol * x, Symbol * y,int isInterger)
{
    fputs ("\n    ;multiplicao\n",asm_file);
    if (isInterger)
    {
        fprintf (asm_file,"    mov ax, DS[%d]\n",x->adress);
        fprintf (asm_file,"    mov bx, DS[%d]\n",y->adress);
        fprintf (asm_file,"    imul bx\n");
        fprintf (asm_file,"    mov DS:[%d], ax\n",tempEnd);
        tempEnd+=2;
        return tempEnd-2;
    }
    else
    {
        fprintf (asm_file,"    mov al, DS[%d]\n",x->adress);
        fprintf (asm_file,"    mov bl, DS[%d]\n",y->adress);
        fprintf (asm_file,"    mul bl\n");
        fprintf (asm_file,"    mov DS:[%d], al\n",tempEnd);
        tempEnd+=1;
        return tempEnd-1;
    }

}

int genDivision ( Symbol * x, Symbol * y)
{
    fputs ("\n    ;divisão\n",asm_file);
    fprintf (asm_file,"    mov ax, DS[%d]\n",x->adress);
    fprintf (asm_file,"    mov bx, DS[%d]\n",y->adress);
    fprintf (asm_file,"    idiv bx\n");
    fprintf (asm_file,"    mov DS:[%d], ax\n",tempEnd);
    tempEnd+=2;
    return tempEnd-2;
}

int genAnd ( Symbol * x, Symbol * y)
{
    fputs ("\n    ;and\n",asm_file);
    fprintf (asm_file,"    mov al, DS[%d]\n",x->adress);
    fprintf (asm_file,"    mov bl, DS[%d]\n",y->adress);
    fprintf (asm_file,"    and al bl\n");
    fprintf (asm_file,"    mov DS:[%d], al\n",tempEnd);
    tempEnd+=1;
    return tempEnd-1;
}

int genNot ( Symbol * x)
{
    fputs ("\n    ;not\n",asm_file);
    fprintf (asm_file,"    mov al, DS[%d]\n              ;negando o %s",x->adress,x->name);
    fputs ("    not al\n",asm_file);
    x->adress = tempEnd;
    fprintf (asm_file,"    mov DS[%d],al\n              ; %s",x->adress,x->name);
    tempEnd+=2;
    return x->adress;
}

void genNegative (Symbol *x)
{
    fputs ("\n    ;convertento para byte\n",asm_file);
    //se for byte transforma em inteiro
    loadIntegerAx(*x);
    fputs ("    neg ax;alterando o sinal\n",asm_file);
    x->adress = tempEnd;
    fprintf (asm_file,"    mov ax, DS[%d]     ;\n",x->adress);
    strcpy(x->type,"integer");
    tempEnd+=2;
}

void loadIntegerAx(Symbol x)
{
    if (strcmp(x.type,"byte") == 0)
    {
        fputs ("\n    ;convertento para inteiro\n",asm_file);
        fputs ("    mov ah, 0;convertento para byte\n",asm_file);//conveter o tipo para byte
        fprintf (asm_file,"    mov al, DS:[%d]\n",x.adress);
    }
    else
        fprintf (asm_file,"    mov ax, DS:[%d]\n",x.adress);
}

unsigned int genOperate (Symbol x, Symbol y, int operation)
{
    fputs ("\n    ;operações simples\n",asm_file);
    //se um valor for byte fazer a conversão
    int end = tempEnd;
    char * regA;
    char * regB;
    if (hasByteAndInteger(x.type,y.type) || hasInteger(x.type,y.type))
    {
        //carrega Y para ax
        loadIntegerAx(y);
        fputs ("    mov bx, ax    ;move ax para bx\n",asm_file);//conveter o tipo para byte
        loadIntegerAx(x);
        regA= "ax";
        regB= "bx";

    }
    else if (areBytes(x.type,y.type))
    {
        fprintf (asm_file,"    mov al, DS[%d]\n",x.adress);
        fprintf (asm_file,"    mov bl, DS[%d]\n",y.adress);
        regA= "al";
        regB= "bl";
    }
    switch (operation)
    {
    case 1:
        fprintf (asm_file,"    sub %s , %s    ;subtração\n",regA,regB);
        break;
    case 2:
        fprintf (asm_file,"    add %s , %s    ;soma\n",regA,regB);
        break;
    case 3:
        fprintf (asm_file,"    or %s , %s    ;OR\n",regA,regB);
        break;
    }
    fprintf (asm_file,"    mov DS:[%d],%s\n",end,regA);
    tempEnd = (strcmp(regA,"al") == 0) ? tempEnd+1 : tempEnd+2;
    return end;
}

void genReadln(Symbol * s){
    int endBuffer = tempEnd;
    tempEnd+=259;
    fprintf (asm_file,"  mov dx,offset DS:[%d]    ;buffer temporario\n",endBuffer);
    fputs ("    mov	al, 0FFh      ;tamanho máximo da string de 255 char\n",asm_file);
    fprintf (asm_file,"    mov	ds:[%d], al\n",endBuffer);
    fputs ("    mov	ah, 0Ah\n",asm_file);
    fputs ("    int	21h\n",asm_file);

    int rotInicio = ++rotCount;
    fprintf (asm_file,"    mov di, offset DS:[%d]	;posição do string\n",endBuffer+2);
    fprintf (asm_file,"    R%d:\n",rotInicio);
    fputs ("        add di, 1    ;incrementar o contador\n",asm_file);
    fputs ("        mov		bl,  ds:[di]		;caractere\n",asm_file);
    fputs ("        cmp bl, 0dh;extende o sinal\n",asm_file);
    fprintf(asm_file,"       jne	R%d      ;continuar até achar o eot end of tape\n",rotInicio);
    fputs("   mov al ,24h\n",asm_file);
    fputs("   mov DS:[di],al\n",asm_file);
    if (compareTokenType(s,"string"))
        s->adress = endBuffer+2;
    else{
        int rotInicio= ++rotCount;
        int rotLoop= ++rotCount;
        int rotFim= ++rotCount;
        fprintf (asm_file,"  mov di,offset DS:[%d]    ;buffer temporario\n",endBuffer+2);
        fputs("   mov		ax, 0			;acumulador\n",asm_file);
        fputs("   mov		cx, 10			;base decimal\n",asm_file);
        fputs("   mov		dx, 1			;valor sinal +\n",asm_file);
        fputs("   mov		bh, 0		\n",asm_file);
        fputs("   mov		bl, ds:[di]		;caractere\n",asm_file);
        fputs("   cmp		bx, 2Dh			;verifica sinal\n",asm_file);
        fprintf(asm_file,"   jne		R%d				;se não negativo\n",rotInicio);
        fputs("   mov		dx, -1			;valor sinal -\n",asm_file);
        fputs("   add		di, 1			;incrementa base\n",asm_file);
        fputs("   mov		bl, ds:[di]		;próximo caractere\n",asm_file);
        fprintf(asm_file,"   R%d:;se não negativo\n",rotInicio);
        fputs("   push	dx				;empilha sinal\n",asm_file);
        fputs("   mov		dx, 0			;reg. multiplicação\n",asm_file);
        fprintf(asm_file,"   R%d:      ;se não negativo\n",rotLoop);
        fputs("   cmp		bx, 24h			;verifica fim string\n",asm_file);
        fprintf(asm_file,"   je		R%d				;salta se fim string\n",rotFim);
        fputs("   imul	cx				;mult. 10\n",asm_file);
        fputs("   add		bx, -48			;converte caractere\n",asm_file);
        fputs("   add		ax, bx			;soma valor caractere\n",asm_file);
        fputs("   add		di, 1			;incrementa base\n",asm_file);
        fputs("   mov		bh, 0\n",asm_file);
        fputs("   mov		bl, ds:[di]		;próximo caractere\n",asm_file);
        fprintf(asm_file,"   jmp		R%d				;loop\n",rotLoop);
        fprintf(asm_file,"   R%d:    ;loop\n",rotFim);
        fputs("   pop		cx				;desempilha sinal\n",asm_file);
        fputs("   imul	cx				;mult. sinal\n",asm_file);

        tempEnd -= 259;//liberar o espaço da string
        if (compareTokenType(s,"integer"))
            fprintf(asm_file,"    mov ds:[%d],ax ;atribuição\n", s->adress);
        else
            fprintf(asm_file,"    mov ds:[%d],al ;atribuição\n", s->adress);

    }
    printLn();
}

void genWriteln (Symbol s)
{
    if (strcmp(s.type,"string") != 0)
        convertToString(s);
    else{
        int rotInicio = ++rotCount;
        int rotFim = ++rotCount;
        fprintf (asm_file,"    mov di, %d \n",s.adress);
        fprintf (asm_file,"    R%d: \n",rotInicio);
        fputs ("        mov	dl, ds:[di]\n",asm_file);
        fputs ("        add	di, 1\n",asm_file);
        fputs ("        cmp dl, 024h\n",asm_file);
        fprintf (asm_file,"        je R%d\n",rotFim);
        fputs ("        mov ah, 02h\n",asm_file);
        fputs ("        int	21h\n",asm_file);
        fprintf (asm_file,"        jmp R%d\n",rotInicio);
        fprintf (asm_file,"    R%d: \n",rotFim);
    }
}

void convertToString (Symbol in)
{
    loadIntegerAx(in);
    int rotInicio = ++rotCount;
    int rotPositivo = ++rotCount;
    int rotPrint = ++rotCount;
    fputs ("    cmp 	ax,0		;verifica sinal\n",asm_file);
    fprintf (asm_file,"    jge 	R%d			;salta se número positivo\n",rotPositivo);
    fputs ("    mov		bx, ax		;guardar o valor de ax–\n",asm_file);
    fputs ("    mov		dl, 2Dh		;senão, escreve sinal –\n",asm_file);
    fputs ("    mov ah,	2h ; character output\n",asm_file);
    fputs ("    int 21h ; call ms-dos output character	\n",asm_file);
    fputs ("    mov ax,bx      ;voltar com o valor\n",asm_file);
    fputs ("    neg 	ax			;toma módulo do número\n",asm_file);
    fprintf (asm_file,"    R%d:\n \n",rotPositivo);
    fputs ("    mov cx, 0		;contador\n",asm_file);
    fputs ("    mov	bx, 10		;divisor\n",asm_file);
    fprintf (asm_file,"    R%d:\n \n",rotInicio);
    fputs ("        add		cx, 1		;incrementa contador.\n",asm_file);
    fputs ("        mov dx, 0   ;Zero extend unsigned value in AX to DX.\n",asm_file);
    fputs ("        div bx		  	;divide AX por 10\n",asm_file);
    fputs ("        push 	dx			;empilha o char\n",asm_file);
    fputs ("    	cmp 	al, 0		;verifica se quoc. é 0\n",asm_file);
    fprintf (asm_file,"    	jne	R%d			;se não é 0, continua\n",rotInicio);
    fprintf (asm_file,"    R%d:\n",rotPrint);
    fputs ("    	pop 	dx			;desempilha valor\n",asm_file);
    fputs ("        add dl, 30h		;transforma em char\n",asm_file);
    fputs ("    	add 	cx, -1		;decrementa contador\n",asm_file);
    fputs ("    	mov ah,	2h ; character output\n",asm_file);
    fputs ("    	int 21h ; call ms-dos output character	\n",asm_file);
    fputs ("    	cmp 	cx, 0		;verifica pilha vazia\n",asm_file);
    fprintf (asm_file,"    	jne	R%d			;se não pilha vazia, loop\n",rotPrint);
}

void printLn (){
    fputs("    mov		ah, 02h\n",asm_file);
    fputs("    mov		dl, 0Dh\n",asm_file);
    fputs("    int		21h\n"    ,asm_file);
    fputs("    mov		DL, 0Ah\n",asm_file);
    fputs("    int		21h\n"    ,asm_file);
}

void loadAxBx (Symbol x,Symbol y){
    loadIntegerAx(y);
    fputs("mov bx, ax\n",asm_file);
    loadIntegerAx(x);
}

int genCompareAxBx(Symbol x,Symbol y,int operation){
    loadAxBx (x,y);
    fputs("cmp ax,bx\n ",asm_file);
    int rotTrue = ++rotCount;
    int rotEnd = ++rotCount;
    int resultAdress = tempEnd;
    tempEnd++;
        switch (operation){
        case 0:
            fprintf(asm_file,"je R%d\n",rotTrue);
            break;
        case 1:
            fprintf(asm_file,"jne R%d\n",rotTrue);
            break;
        case 2:
            fprintf(asm_file,"jl R%d\n",rotTrue);
            break;
        case 3:
            fprintf(asm_file,"jg R%d\n",rotTrue);
            break;
        case 4:
            fprintf(asm_file,"jle R%d\n",rotTrue);
            break;
        case 5:
            fprintf(asm_file,"jge R%d\n",rotTrue);
            break;
        }
        fputs("    mov al, 0 ;como default a comparacao é falsa\n",asm_file);
        fprintf(asm_file,"jmp R%d;\n",rotEnd);
        fprintf(asm_file,"R%d:     ;caso a expressao seja verdadeira\n",rotTrue);
        fputs("        mov al, 0ffh ; exp = true\n",asm_file);
        fprintf(asm_file,"R%d:\n",rotEnd);
        fprintf(asm_file,"    mov DS:[%d], al     ;atribuir o resultado da expressao\n",resultAdress);
        return resultAdress;
}

int initWhile (Symbol exp){
    int rotInicio = ++rotCount;
    int rotFim = ++rotCount;
    fprintf(asm_file,"    R%d:     ;\n",rotInicio);
    fprintf(asm_file,"         mov al,ds:[%d]    ;carregar conteúdo da exp\n",exp.adress);
    fputs("        cmp al, 0    ;verificar se a expressão é falsa\n",asm_file);
    fprintf(asm_file,"        je R%d    ;se falsa, pular o comando\n",rotFim);
    return rotInicio;
}

void finishWhile (int rot){
    fprintf(asm_file,"     jmp R%d    ;Voltar ao loop\n",rot);
    fprintf(asm_file,"     R%d:    ;marca o fim do loop\n",rot+1);
}

void genAssgin(Symbol *x, Symbol y){
    if (compareTokenType (x,"integer")){
        fprintf(asm_file,"     mov    ax,ds:[%d]     ;ax = y\n",y.adress);
        fprintf(asm_file,"     mov    ds:[%d],ax     ;x = ax = y\n",x->adress);
    }else{
        fprintf(asm_file,"     mov    al,ds:[%d]     ;al = y\n",y.adress);
        fprintf(asm_file,"     mov    ds:[%d],al     ;x = al = y\n",x->adress);
    }
}


int initIf (Symbol exp){
    int rotFalse = ++rotCount;
    fprintf(asm_file,"         mov al,ds:[%d]    ;carregar conteúdo da exp\n",exp.adress);
    fputs("        cmp al, 0ffh    ;verificar se a expressão é falsa\n",asm_file);
    fprintf(asm_file,"     jne R%d    ;pula o comando\n",rotFalse);
    return rotFalse;
}

int endIf (int rotFalse){
    int rotEnd = ++rotCount;
    fprintf(asm_file,"     jmp R%d    ;pula o comando\n",rotEnd);
    fprintf(asm_file,"     R%d:    ;\n",rotFalse);
    return rotEnd;
}

void criarRotulo (int rot){
    fprintf(asm_file,"     R%d:\n",rot);
}
