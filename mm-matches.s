@ This ARM Assembler code should implement a matching function, for use in the MasterMind program, as
@ described in the CW2 specification. It should produce as output 2 numbers, the first for the
@ exact matches (peg of right colour and in right position) and approximate matches (peg of right
@ color but not in right position). Make sure to count each peg just once!
	
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
.global         loader
loader: 
	LDR  r2, =secret	@ pointer to secret sequence
	LDR  r3, =guess		@ pointer to guess sequence
	mov r4, #0 @ approx. matches
	mov r5, #0 @ exact matches
	mov r6, #0 @ i
	mov r8, #0 @ j 
	mov r9, #4 @ new number
	b matches
	



exit:
	MOV r7, #1		@ load system call code
	SWI 0		@ return this value

@ -----------------------------------------------------------------------------
@ sub-routines

@ this is the matching fct that should be callable from C	
matches:			@ Input: R0, R1 ... ptr to int arrays to match ; Output: R0 ... exact matches (10s) and approx matches (1s) of base COLORS
		cmp r6, #3
		beq addmatches
		bl access
		cmp r11, r12
		beq  exact
		b approx
exact:
	 	add r5, r5, #10 @exact + 10
		add r6, r6, #1 @ i++
		mov r9, #1 
		add r2, r9, lsl #2 @ inc r2
		mov r9, #4
		str r9, [r3] @ r3[current] = 4
		add r3, r9, lsl #2 @ inc r2
		b matches
approx:
		sub r3, r6, lsl #2 @ r3[i] -> r3[0]
		bl access
		cmp r11, r12
		beq incApprox
		add r10, r10, #1 @ j++
		mov r9, #1
		add r3, r9, lsl #2 @ inc r3
		bl access
		cmp r11, r12
		beq incApprox
		add r10, r10, #1 @ j++
		add r3, r9, lsl #2 @ inc r3
		bl access
		cmp r11, r12
		beq incApprox
		@ no matches at all go back
		mov r10, #0 @ j = 0
		mov r9, #2
		sub r3, r9, lsl #2 @ set r3[2] back to r3[0]
		add r3, r6, lsl #2 @ set r3 back to r3[i]
		add r6, r6, #1 @ i++
		b matches
incApprox: 
		mov r9, #4
		str r9, [r3] @ r3[j] = 4
		sub r3, r10, lsl #2 @ set r3[j] back to r3[0]
		mov r10, #0 @ j = 0
		add r4, r4, #1 @ approx++
		add r6, r6, #1 @ i++
		mov r9, #1
		add r3, r6, lsl #2 @ r3[0] -> r3[i]
		add r2, r9, lsl #2 @ r2[i-1] -> r2[i]
		b matches
addmatches:
		add r5, r5, r4 @ r5 = exact + approx
		mov r0, r5 @ r0 = r5
		b exit
access:
		ldr r11, [r2]
		ldr r12, [r3]
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