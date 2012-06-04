pilha	SEGMENT	
	DB	16 DUP('PILHA   ')
pilha	ENDS

dados	SEGMENT
mes	DB	'ITOA$'
dados	ENDS

codigo	SEGMENT
	ASSUME	CS:codigo, DS:dados

strt:
;inicializa DS
	MOV	AX,dados
	MOV	DS,AX
;converte numero que esta em AX, escrevendo na posicao 10h
	MOV	AX,324		;numero a ser convertido
	MOV	CX,0		;contador
	MOV	BX,10		;divisor
R1:
	INC	CX		;incrementa contador
	MOV	DX,0		;estende 32 bits
	DIV	BX		;divide DXAX por BX
	PUSH	DX		;empilha valor do resto
	CMP	AX,0		;verifica se quociente eh 0
	JNE	R1		;se nao eh 0, continua
;agora, desempilhar os valores e escrever o string
	MOV	BX,10h		;base do string - alterar o valor de 10h
R2:
	POP	DX		;desempilha valor
	ADD	DX,0030h	;transforma em caracter
	MOV	DS:[BX],DL	;escreve caracter
	INC	BX		;incrementa base
	DEC	CX		;decrementa contador
	CMP	CX,0		;verifica pilha vazia
	JNE	R2		;se nao pilha vazia, continua
;grava fim de string
	MOV	byte ptr DS:[BX],24h	;grava '$'
;mostra string
	MOV	AH,9
	MOV	DX, 10h
	INT	21h
;termina
	MOV	AH,4Ch
	INT	21h

codigo	ENDS
	END	strt
