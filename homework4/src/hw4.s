.cpu cortex-m0
.thumb
.syntax unified
.fpu softvfp

//============================================================================
// Q1: hello
//============================================================================
.global hello
hello:
	bl serial_init
	ldr r0, =helloworld
	bl printf
	wfi
//============================================================================
// Q2: add2
//============================================================================
.global add2
add2:

//============================================================================
// Q3: add3
//============================================================================
.global add3
add3:

//============================================================================
// Q4: rotate6
//============================================================================
.global rotate6
rotate6:

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

//============================================================================
// Q6: get_name
//============================================================================
.global get_name
get_name:

//============================================================================
// Q7: random_sum
//============================================================================
.global random_sum
random_sum:

//============================================================================
// Q8: fibn
//============================================================================
.global fibn
fibn:

//============================================================================
// Q9: fun
//============================================================================
.global fun
fun:

//============================================================================
// Q10: sick
//============================================================================
.global sick
sick:


helloworld:
	.string "hello world\n"
	.align 2
