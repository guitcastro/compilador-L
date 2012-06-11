sseg SEGMENT STACK			; inicio seg. pilha
   byte 20000 DUP(?) 
sseg ENDS				;fim seg. pilha

dseg SEGMENT PUBLIC			; inicio seg. dados
    byte 20000 DUP(?)		; temporarios
    sword ?             ;identifier n do tipo integer no endereco: 20000
    byte 256 DUP(?)             ;identifier nome do tipo string no endereco: 20002
    byte ?             ;identifier naoTerminou do tipo boolean no endereco: 20258
    byte 10             ;const MAXITER do tipo byte no endereco: 20259
dseg ENDS				; fim seg. dados
cseg SEGMENT PUBLIC 			; inicio do seg. código
    ASSUME CS:cseg, DS:dseg	

    strt:					; inicio do programa
        mov ax, dseg
        mov ds, ax


dseg SEGMENT PUBLIC			; inicio seg. dados
    byte "Digite seu nome: $"             ;const do tipo string no endereco: 20260
dseg ENDS				; fim seg. dados
    mov di, 20260 
    R1: 
        mov	dl, ds:[di]
        add	di, 1
        cmp dl, 024h
        je R2
        mov ah, 02h
        int	21h
        jmp R1
    R2: 
  mov dx,offset DS:[0]    ;buffer temporario
    mov	al, 0FFh      ;tamanho máximo da string de 255 char
    mov	ds:[0], al
    mov	ah, 0Ah
    int	21h
    mov di, offset DS:[2]	;posição do string
    R3:
        add di, 1    ;incrementar o contador
        mov		bl,  ds:[di]		;caractere
        cmp bl, 0dh;extende o sinal
       jne	R3      ;continuar até achar o eot end of tape
   mov al ,24h
   mov DS:[di],al
    mov		ah, 02h
    mov		dl, 0Dh
    int		21h
    mov		DL, 0Ah
    int		21h

    mov al, 0ffh             ;atribuicao
    mov DS:[259], al
     mov    al,ds:[259]     ;al = y
     mov    ds:[20258],al     ;x = al = y

    mov al, 0             ;atribuicao
    mov DS:[260], al
     mov    ax,ds:[260]     ;ax = y
     mov    ds:[20000],ax     ;x = ax = y
    R4:     ;
         mov al,ds:[20258]    ;carregar conteúdo da exp
        cmp al, 0    ;verificar se a expressão é falsa
        je R5    ;se falsa, pular o comando


dseg SEGMENT PUBLIC			; inicio seg. dados
    byte "Ola' $"             ;const do tipo string no endereco: 20278
dseg ENDS				; fim seg. dados
    mov di, 20278 
    R6: 
        mov	dl, ds:[di]
        add	di, 1
        cmp dl, 024h
        je R7
        mov ah, 02h
        int	21h
        jmp R6
    R7: 
    mov di, 2 
    R8: 
        mov	dl, ds:[di]
        add	di, 1
        cmp dl, 024h
        je R9
        mov ah, 02h
        int	21h
        jmp R8
    R9: 
    mov		ah, 02h
    mov		dl, 0Dh
    int		21h
    mov		DL, 0Ah
    int		21h
