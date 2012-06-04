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

    mov al, FFh             ;atribuicao
    mov DS:[0], al

    mov al, 0             ;atribuicao
    mov DS:[1], al


dseg SEGMENT PUBLIC			; inicio seg. dados
    byte "Ola' $"             ;const do tipo string no endereco: 20277
dseg ENDS				; fim seg. dados

    mov al, 1             ;atribuicao
    mov DS:[2], al
