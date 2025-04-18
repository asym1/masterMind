@ This ARM Assembler code should implement a matching function, it should produce as output 2 numbers,
@ the first for the exact compare (peg of right colour and in right position) and approximate compare
@ (peg of right color but not in right position). Make sure to count each peg just once!
	
@ Example (first sequence is secret, second sequence is guess):
@ 1 2 1
@ 3 1 3 ==> 0 1
@ You can return the result as a pointer to two numbers, or two values
@ encoded within one number
@
@ -----------------------------------------------------------------------------

.text
@ this is the matching fct that should be called from the C part of the CW	
.global         matches
@ use the name `main` here, for standalone testing of the assembler code
@ when integrating this code into `master-mind.c`, choose a different name
@ otw there will be a clash with the main function in the C code

matches:
	push {lr}q
	mov r3, #0 @ approx. compare
	mov r4, #0 @ exact compare
	mov r5, #0 @ i
	mov r6, #0 @ j 
	mov r9, #4
	b compare

exit: 
	MOV r0, r4
	pop {lr}
	bx lr		@ return this value

@ -----------------------------------------------------------------------------
@ sub-routines

@ this is the matching fct that should be callable from C	
compare:			@ Input: R0, R1 ... ptr to int arrays to match ; Output: R0 ... exact compare (10s) and approx compare (1s) of base COLORS
		cmp r5, #3
		beq addmatches

		bl accessI
		cmp r11, r12
		beq  exact
		b approx
exact:
	 	add r4, r4, #10 @exact + 10
		str r9, [r1, r5, lsl #2] @ r1[current] = 4
		add r5, r5, #1 @ i++
		b compare
approx:
		mov r6, #0 		@ j = 0
		bl accessJ
		cmp r11, r12
		beq incApprox
		add r6, r6, #1 	@ j++
		bl accessJ
		cmp r11, r12
		beq incApprox
		add r6, r6, #1 	@ j++
		bl accessJ
		cmp r11, r12
		beq incApprox
		@ no matches
		add r5, r5, #1 	@ i++
		b compare
incApprox:
		str r9, [r1, r6, lsl #2] @ r1[j] = 4
		add r5, r5, #1 @ i++
		add r3, r3, #1 @ approx++
		b compare
addmatches:
		add r4, r4, r3 @ r4 = exact + approx
		b exit		@ go to exit
accessI:
		ldr r11, [r0, r5, lsl #2] @ r11 = seq1[i]
		ldr r12, [r1, r5, lsl #2] @ r12 = seq2[i]
		bx lr
accessJ:
		ldr r12, [r1, r6, lsl #2] @ r12 = seq2[j]
		bx lr

.data

@ constants about the basic setup of the game: length of sequence and number of colors	
.equ LEN, 3
.equ COL, 3
.equ NAN1, 8
.equ NAN2, 9

@ a format string for printf that can be used in showseq
f4str: .asciz "Seq:    %d %d %d\n"

@ a memory location, initialised as 0, you may need this in the matching fct
n: .word 0x00
	
@ INPUT DATA for the matching function
.align 4
secret: .word 1 
	.word 2 
	.word 1 

.align 4
guess:	.word 3 
	.word 1 
	.word 3 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 0 1
.align 4
expect: .byte 0
	.byte 1

.align 4
secret1: .word 1 
	 .word 2 
	 .word 3 

.align 4
guess1:	.word 1 
	.word 1 
	.word 2 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 1
.align 4
expect1: .byte 1
	 .byte 1

.align 4
secret2: .word 2 
	 .word 3
	 .word 2 

.align 4
guess2:	.word 3 
	.word 3 
	.word 1 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 0
.align 4
expect2: .byte 1
	 .byte 0
