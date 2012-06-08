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
    R0: 
        mov	dl, ds:[di]
        add	di, 1
        cmp dl, 024h
        je R1
        mov ah, 02h
        int	21h
        jmp R0
    R1: 

    ;convertento para inteiro
    mov al, DS:[20259]
    cbw    ;convertento para byte
    mov cx, 0		;contador
    mov	bx, 10		;divisor
    cmp 	ax,0		;verifica sinal
    jge 	R1			;salta se número positivo
    mov		bl, 2Dh		;senão, escreve sinal –
    neg 	ax			;toma módulo do número
    R1:
 
        add		cx, 1		;incrementa contador.
        mov dx, 0   ;Zero extend unsigned value in AX to DX.
        div bx		  	;divide AX por 10
        add dl, 30h		;transforma em char
        push 	dx			;empilha o char
    	cmp 	al, 0		;verifica se quoc. é 0
    	jne	R1			;se não é 0, continua
    R2:
    	pop 	dx			;desempilha valor
    	add 	cx, -1		;decrementa contador
    	mov ah,	2h ; character output
    	int 21h ; call ms-dos output character	
    	cmp 	cx, 0		;verifica pilha vazia
    	jne	R2			;se não pilha vazia, loop


dseg SEGMENT PUBLIC			; inicio seg. dados
    byte "Digite seu nome: $"             ;const do tipo string no endereco: 20278
dseg ENDS				; fim seg. dados
    mov di, 20278 
    R2: 
        mov	dl, ds:[di]
        add	di, 1
        cmp dl, 024h
        je R3
        mov ah, 02h
        int	21h
        jmp R2
    R3: 

    ;convertento para inteiro
    mov al, DS:[20259]
    cbw    ;convertento para byte
    mov cx, 0		;contador
    mov	bx, 10		;divisor
    cmp 	ax,0		;verifica sinal
    jge 	R1			;salta se número positivo
    mov		bl, 2Dh		;senão, escreve sinal –
    neg 	ax			;toma módulo do número
    R3:
 
        add		cx, 1		;incrementa contador.
        mov dx, 0   ;Zero extend unsigned value in AX to DX.
        div bx		  	;divide AX por 10
        add dl, 30h		;transforma em char
        push 	dx			;empilha o char
    	cmp 	al, 0		;verifica se quoc. é 0
    	jne	R3			;se não é 0, continua
    R4:
    	pop 	dx			;desempilha valor
    	add 	cx, -1		;decrementa contador
    	mov ah,	2h ; character output
    	int 21h ; call ms-dos output character	
    	cmp 	cx, 0		;verifica pilha vazia
    	jne	R4			;se não pilha vazia, loop

    mov al, 0ffh             ;atribuicao
    mov DS:[0], al

    mov al, 0             ;atribuicao
    mov DS:[1], al

    mov al, 1             ;atribuicao
    mov DS:[2], al

    ;operações simples

    ;convertento para inteiro
    mov al, DS:[2]
    cbw    ;convertento para byte
    mov bx, ax    ;move ax para bx

    ;convertento para inteiro
    mov ax, DS:[20000]
    add ax , bx    ;soma    mov ax, DS[3]
    mov ah, 4Ch			; termina o programa
    int 21h	            ; fim seg. código
cseg ENDS				;

END strt					
