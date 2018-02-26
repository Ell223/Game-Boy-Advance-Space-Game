I@ the gba stuff tends to use thumb rather than arm for asm
@ this means we can only use registers r0-r7 except with add, mov and cmp
@ and we have a restricted instruction set

@ parameters passed from c function calls are stored in order in r0-r3 (automatically uses the stack after 4 parameters)
@ to return a value we must make sure we pass a pointer from c and write to it via the corresponding asm register



@ refer to this: http://www.coranac.com/tonc/text/asm.htm
@ also this: http://www.heyrick.co.uk/assembler/qfinder.html

.THUMB							@ turn on thumb
.ALIGN  2							@ no idea why we do this
.GLOBL  addCount			@ name of first function goes here


.THUMB_FUNC					@ we are about to declare a thumb function
										@ c equivalent: void myfunc(int* answer);
										@ the address passed into "answer" will be stored in r0 upon calling the function
					
addCount:							@ function start

push { r4-r7, lr }					@ push r4-r7 and link register onto stack. Your function might use these
										@ registers, so we need to preserve the values just in case!
										@ we don't need to worry about r0-r3 as it is assumed they will be regularly messed up anyway
					
										@ r0 will automatically contain the address of the first parameter passed from C.
	ldr r4, [r0]						@ dereference it into r4 - use square brackets when dealing with addresses!
	ldr r5, [r1]
	ldr r6, [r2]
	ldr r7, [r3]
	
LOOPSTART:

	cmp r4, #10					@Compare count1 to 10
	BGE LOOP1					@If its greater than or equal to 10 go to loop1
	BLT LOOPCHECK			@if not go to loopcheck
	

LOOP1:								
	
	add r5, r5, #1					@add 1 to count2 if count1 is equal or greater than 10
	sub r7, r7, #10				@take 10 off of score
	mov r4, r7						@make count1 equal score, i.e count1 = 0
	cmp r4, r4						@A NOP to make the loop start over
	BEQ LOOPSTART
	
LOOPCHECK:						

	cmp r5, #10					@Compare count2 to 10
	BGE LOOP2					@jump to loop2 if greater than or equal to ten
	BLT ENDLOOP				@otherwise end the loop
	
LOOP2:

	add r6, r6, #1					@add 1 to count3
	sub r5, r5, #10				@ take ten off of count2
	cmp r5, r5
	BEQ LOOPSTART
	
ENDLOOP:
	
	str r4, [r0] 						@ store data in r4 at r0 address
	str r5, [r1]
	str r6, [r2]
	
	bl addEquals

pop { r4-r7 }						@ pop first 4 values from stack back into r4-r7, and also
pop { r3 }							@ pop the next value from stack (stored value for lr) into some unused register, e.g. r3 - we cannot overwrite lr so we have to do it via a normal register
bx r3									@ "branch and exchange" (return) back to C, using the previous value for lr stored in r3

.THUMB_FUNC					@ we are about to declare a thumb function
										@ c equivalent: void myfunc(int* answer);
										@ the address passed into "answer" will be stored in r0 upon calling the function
					
addEquals:							@ function start

push { r4-r7, lr }

	ldr r4, [r0]						@ dereference it into r4 - use square brackets when dealing with addresses!
	ldr r5, [r1]
	ldr r6, [r2]
	
	add r4, #32
	add r5, #32
	add r6, #32
	
	str r4, [r0] 						@ store data in r4 at r0 address
	str r5, [r1]
	str r6, [r2]

pop { r4-r7 }		@ pop first 4 values from stack into r4-r7, and also
pop { r3 }			@ pop the next value from stack (old value for lr) into r3 - we cannot overwrite lr so we have to do it via a normal register

bx r3 				@ "branch and exchange" (return) back to calling function, using the previous value for lr stored in r3