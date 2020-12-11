.cpu cortex-m0
.thumb
.syntax unified
.fpu softvfp

//============================================================================
// Q1: hello
//============================================================================
.global hello
hello:
	push {lr}
	ldr r0, =0xfffffffe
	//bl serial_init
	ldr r0, =q1Format
	bl printf
	pop {pc}

q1Format:
	.string "Hello, world!\n"
	.align 2
//============================================================================
// Q2: add2
//============================================================================
.global add2
add2:
	push {lr}
	movs r2, r1
	movs r1, r0
	ldr r0,=q2Format
	adds r3, r1, r2
	bl printf

	pop {pc}
q2Format:
	.string "%d + %d = %d\n"
	.align 2

//============================================================================
// Q3: add3
//============================================================================
.global add3
add3:
	push {lr}
	movs r3, r2
	movs r2, r1
	movs r1, r0
	ldr r0, =q3Format
	adds r4, r3, r2
	adds r4, r1
	push {r4}
	bl printf
	pop {r4}
	pop {pc}

q3Format:
	.string "%d + %d + %d = %d\n"
	.align 2
//============================================================================
// Q4: rotate6
//============================================================================
.global rotate6
rotate6:
	//push {r4, r5, r6, r7, lr}
	pop {r4, r5} //Pulling the last two arguments off the stack
	//mov r7, sp
	//adds r7, #20
	//ldr r4, [r7]
	//adds r7, #4		//Adding to MY stackp makes it go "down"
	//ldr r5, [r7]
	push {lr}
	//subs r7, #24		//Subtracting makes it go "up"

	CMP r0, #0		//Comparing the first argument to zero
	bne recursive		//If they're equal then don't need to do recur. call
defaultCase:
	//The first value being a zero is the default case
	subs r5, r0
	subs r5, r1
	subs r5, r2
	subs r5, r3
	subs r5, r4
	movs r0, r5
	pop {pc}

recursive:
	//If the first argument IS NOT zero
	movs r6, r5
	movs r5, r4
	movs r4, r3
	movs r3, r2
	movs r2, r1
	movs r1, r0
	movs r0, r6

	push {r4, r5}
	b rotate6

	b defaultCase

	pop {pc}

//============================================================================
// Q5: low_pattern
//============================================================================
.type compare, %function  // You knew you needed this line.  Of course you did!
compare:
        ldr  r0,[r0]
        ldr  r1,[r1]
        subs r0,r1
        bx lr

.global low_pattern
low_pattern:
	push {r4-r7,lr}
	ldr r6, =0x320		//320 is the hex of 800
	//sub sp, r6		//Allocate 200 integers onto the stack
		ldr r7, [sp]
		subs r7, r6
		str r7, [sp]
	//mov r7, sp
	movs r6, #0			//r6 is the sum
	movs r5, #0			//r5 is x
	ldr r2, [sp]		//this is my permanent head of array {KEEP}

loop2:
	adds r5, #1
	movs r3, #0xff
	movs r6, r5
	muls r6, r3
	ands r6, r3
	str r6, [sp]
	adds r7, #4
	str r7, [sp]

	cmp r5, #200
	BLT loop2

postLoop2:
	movs r4, r0			//holding the "nth" input in this register
	movs r0, r2
	movs r1, #200
	movs r2, #4
    ldr r3, =compare

    bl qsort

	muls r4, r1
	subs r2, r4
	str r2, [sp]
	ldr r0, [sp]
	//add sp, #0x320
	ldr r7, [sp]
	ldr r6, =0x0320
	adds r7, r6
	str r7, [sp]
	pop {r4-r7, pc}

//============================================================================
// Q6: get_name
//============================================================================
.global get_name
get_name:
	push {lr}
	ldr r0, =q6Format
	bl printf

	sub  sp,#80      // Allocate 80 chars
	mov  r0,sp        // R7 is the beginning of "array"
	bl gets

	ldr r1, [r0]
	ldr r0, =q6Second
	mov r1, sp
	bl printf
	add sp,#80
	pop {pc}

q6Format:
	.string "Enter your name: "
	.align 2
q6Second:
	.string "Hello, %s\n"
	.align 2
//============================================================================
// Q7: random_sum
//============================================================================
.global random_sum
random_sum:
	push {lr}
	sub sp, #80
    ldr r7, [sp]        //r7 is my movable stack pointer
    movs r6, #0         //My sum variable
    movs r5, #1         //My x variable
    bl random

    //str r0, [r7]
    str r0, [sp]
    //subs r7, #4
    movs r4, #4          //Just holding onto 4
    movs r3, #1         //my offset variable
loop3:
    movs r3, #1
    muls r3, r4
    muls r3, r5
    mov r6, r7			//The old stack pointer
    subs r7, r3			//the new stack pointer
    str r2, [r6]

    str r3, [r7]

    adds r5, #1
    cmp r5, #20
    BLE loop3

loop4:

	add sp,#80
	pop {pc}
//============================================================================
// Q8: fibn
//============================================================================
.global fibn
fibn:
	push {lr}
	sub sp, #240
	sub sp, #240
	mov r7, sp

	movs r1, #0
	movs r2, #1

	str r1, [r7]
	adds r7, #4
	str r2, [r7]
	adds r7, #4

	movs r6, #2		//initializing x


loop8:
	subs r7, #8
	ldr r1, [r7]
	adds r7, #4
	ldr r2, [r7]
	adds r7, #4

	adds r1, r2
	str r1, [r7]
	adds r7, #4

	adds r6, #1
	cmp r6, #120
	blt loop8

mov r7, sp
movs r6, #4
muls r0, r4
adds r0,#4
subs r7, r0
ldr r0, [r7]

	add sp, #240
	add sp, #240
	pop {pc}

//============================================================================
// Q9: fun
//============================================================================
.global fun
fun:
push {lr}

sub sp,#400
mov r7, sp	    //my "movable" stack pointer
movs r2, #1     //The x variable
movs r3, #0     //The sum variable (garbage in the first loop)
movs r4, #0     //my working variable to assign into array
str r4, [r7]
movs r6, #37
loop9:
    ldr r4, [r7]        //The old arr value "arr[x-1]"
    movs r3, #7         //moving 7 into register
    adds r3, r2         //adding the current x an 7
    muls r3, r6         //doing (x+7) * 37
    adds r4, r3         //adding old arr value plus above value

    subs r7, #4         //Incrementing the stack pointer up, so that I acn store new value
    str r4, [r7]        //Putting calculated value (r4) into the array

    adds r2, #1         //Incrementing x
    CMP r2, #100
    BLT loop9           //If x is less than 100 keep looping

movs r3, #0     //sum variable
movs r2, r0     //re initialize x to "a"
mov r7, sp     //move my "movable" stack pointer back to the front
movs r0, #4
muls r0, r1
subs r7, r0
loop10:
    ldr r6, [r7]
    adds r3, r6
    subs r7, #4

    adds r2, #1
    cmp r2, r1
    BLE loop10

movs r0, r3
add sp,#400
pop {pc}

//============================================================================
// Q10: sick
//============================================================================
.global sick
sick:


