@ the gba stuff tends to use thumb rather than arm for asm
@ this means we can only use registers r0-r7 except with add, mov and cmp
@ and we have a restricted instruction set

@ parameters passed from c function calls are stored in order in r0-r3 (automatically uses the stack after 4 parameters)
@ to return a value we must make sure we pass a pointer from c and write to it via the corresponding asm register



@ refer to this: http://www.coranac.com/tonc/text/asm.htm
@ also this: http://www.heyrick.co.uk/assembler/qfinder.html

.THUMB							@ turn on thumb
.ALIGN  2							@ no idea why we do this
.GLOBL  swapPalette					@ name of first function goes here

.THUMB_FUNC

swapPalette:

push { r4-r7, lr }

	ldrh r4, [r0]
	ldrh r5, [r1]
	
	push {r4}
	push {r5}
	pop {r4}
	pop {r5}
	
	strh r4, [r0]
	strh r5, [r1]
	
pop { r4-r7 }		@ pop first 4 values from stack into r4-r7, and also
pop { r3 }			@ pop the next value from stack (old value for lr) into r3 - we cannot overwrite lr so we have to do it via a normal register

bx r3 				@ "branch and exchange" (return) back to calling function, using the previous value for lr stored in r3