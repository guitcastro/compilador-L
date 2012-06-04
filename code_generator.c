#include "code_generator.h"

void initCodeGen (const char * file_name)
{
    currentEnd = 20000;
    tempEnd = 0;
    char * file_ext = strrchr(file_name,'.');
    if (file_ext != NULL)
        strcpy(file_ext, ".asm");
    else
        strcat(file_ext, ".asm");
    asm_file = fopen(file_name,"w");
    //inicia o segmento de pilha

    fputs ("sseg SEGMENT STACK			; inicio seg. pilha\n",asm_file);
    fputs ("   byte 20000 DUP(?) \n",asm_file);
    fputs ("sseg ENDS				;fim seg. pilha\n",asm_file);
    //inicia o segmento de declarações
    openDseg();
    fputs ("    byte 20000 DUP(?)		; temporarios\n",asm_file);
}

void openDseg (){
    fputs ("\ndseg SEGMENT PUBLIC			; inicio seg. dados\n",asm_file);
}

void closeDseg (){
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

void defConst(struct Symbol * identifier,struct Symbol * constant)
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
            value = "FFh";
        else
            value = "00h";
        fprintf (asm_file,"    byte %s             ;const %s do tipo boolean no endereco: %d\n",value,identifier->name,currentEnd);
        //reservou 1  byte na memória
        currentEnd++;
    }
}

void defIdentifier(struct Symbol * identifier)
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
        currentEnd+= (255);
    }
}

void defTempConst(struct Symbol * s){
    fputs ("\n",asm_file);
    s->adress = tempEnd;
    if (strcmp(s->type,"integer") == 0){
        fprintf (asm_file,"    mov ax, %s             ;atribuicao\n",s->name);
        fprintf (asm_file,"    mov DS:[%d], ax\n",tempEnd);
        tempEnd+=2;
    }
    if (strcmp(s->type,"byte") == 0){
        fprintf (asm_file,"    mov al, %s             ;atribuicao\n",s->name);
        fprintf (asm_file,"    mov DS:[%d], al\n",tempEnd);
        tempEnd+=1;
    }
    if (strcmp(s->type,"boolean") == 0){
        char * value;
        if (strcmp(s->name,"TRUE") == 0)
            value = "FFh";
        else
            value = "00h";
        fprintf (asm_file,"    mov al, %s             ;atribuicao\n",value);
        fprintf (asm_file,"    mov DS:[%d], al\n",tempEnd);
        tempEnd+=1;
    }if(strcmp(s->type,"string") == 0) {
        openDseg();
        fprintf (asm_file,"    byte \"%s$\"             ;const do tipo string no endereco: %d\n",s->name,currentEnd);
        currentEnd+= (strlen(s->name) + 1);
        closeDseg();
    }
}

int genMultiply (struct Symbol * x,struct Symbol * y,int isInterger){
    fputs ("\n    ;multiplicao\n",asm_file);
    if (isInterger){
        fprintf (asm_file,"    mov ax, DS[%d]\n",x->adress);
        fprintf (asm_file,"    mov bx, DS[%d]\n",y->adress);
        fprintf (asm_file,"    imul bx\n");
        fprintf (asm_file,"    mov DS:[%d], ax\n",tempEnd);
        tempEnd+=2;
    }else{
        fprintf (asm_file,"    mov al, DS[%d]\n",x->adress);
        fprintf (asm_file,"    mov bl, DS[%d]\n",y->adress);
        fprintf (asm_file,"    mul bl\n");
        fprintf (asm_file,"    mov DS:[%d], al\n",tempEnd);
        tempEnd+=1;
    }
    return tempEnd;
}

int genDivision (struct Symbol * x,struct Symbol * y){
    fputs ("\n    ;divisão\n",asm_file);
    fprintf (asm_file,"    mov ax, DS[%d]\n",x->adress);
    fprintf (asm_file,"    mov bx, DS[%d]\n",y->adress);
    fprintf (asm_file,"    idiv bx\n");
    fprintf (asm_file,"    mov DS:[%d], ax\n",tempEnd);
    tempEnd+=2;
    return tempEnd;
}
