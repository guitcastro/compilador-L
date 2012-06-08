sseg SEGMENT STACK			; inicio seg. pilha
   byte 20000 DUP(?) 
sseg ENDS				;fim seg. pilha

dseg SEGMENT PUBLIC			; inicio seg. dados
    byte 20000 DUP(?)		; temporarios
    sword ?             ;identifier n do tipo integer no endereco: 20000
    byte 256 DUP(?)             ;identifier nome do tipo string no endereco: 20002
    byte ?             ;identifier naoTerminou do tipo boolean no endereco: 20257
    byte 10             ;const MAXITER do tipo byte no endereco: 20258
dseg ENDS				; fim seg. dados
cseg SEGMENT PUBLIC 			; inicio do seg. código
    ASSUME CS:cseg, DS:dseg	

    strt:					; inicio do programa
        mov ax, dseg
        mov ds, ax


dseg SEGMENT PUBLIC			; inicio seg. dados
    byte "Digite seu nome: $"             ;const do tipo string no endereco: 20259
dseg ENDS				; fim seg. dados
    mov 	dx, ds:[20259] 
    mov ah, 09h
    int	21h

    mov al, 0ffh             ;atribuicao
    mov DS:[0], al

    mov al, 0             ;atribuicao
    mov DS:[1], al

    mov al, 1             ;atribuicao
    mov DS:[2], al

    ;operações simples

    ;convertento para inteiro
    mov ax, DS:[2]
    mov bx, ax    ;move ax para bx

    ;convertento para inteiro
    mov al, DS:[20000]
    cbw    ;convertento para byte
    add ax , bx    ;soma    mov ax, DS[3]
    mov ah, 4Ch			; termina o programa
    int 21h	            ; fim seg. código
cseg ENDS				;

END strt					
