.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

//=============================================================================
// ECE 362 Lab Experiment 5
// Timers
//
//=============================================================================

.equ  RCC,      0x40021000
.equ  APB1ENR,  0x1C
.equ  AHBENR,   0x14
.equ  TIM6EN,	0x10
.equ  GPIOCEN,  0x00080000
.equ  GPIOBEN,  0x00040000
.equ  GPIOAEN,  0x00020000
.equ  GPIOC,    0x48000800
.equ  GPIOB,    0x48000400
.equ  GPIOA,    0x48000000
.equ  MODER,    0x00
.equ  PUPDR,    0x0c
.equ  IDR,      0x10
.equ  ODR,      0x14
.equ  PC0,      0x01
.equ  PC1,      0x04
.equ  PC2,      0x10
.equ  PC3,      0x40
.equ  PIN8,     0x00000100

// NVIC control registers...
.equ NVIC,		0xe000e000
.equ ISER, 		0x100
.equ ICER, 		0x180
.equ ISPR, 		0x200
.equ ICPR, 		0x280

// TIM6 control registers
.equ  TIM6, 	0x40001000
.equ  CR1,		0x00
.equ  DIER,		0x0C
.equ  PSC,		0x28
.equ  ARR,		0x2C
.equ  TIM6_DAC_IRQn, 17
.equ  SR,		0x10

//=======================================================
// 6.1: Configure timer 6
//=======================================================
.global init_TIM6
init_TIM6:
	push {lr}
	//Enabling clock for TIM6
	ldr r0, =RCC	//RCC
	ldr r1, [r0, #APB1ENR]
	ldr r2, =TIM6EN
	orrs r1, r2
	str r1, [r0, #APB1ENR]
	//Setting PSC and APR to achieve update every 1 ms (2000 *24) /48,000,000 = 0.001 s
	ldr r0, =TIM6
	ldr r1, =1999   	//2000 - 1 = 0x7CF
	str r1,[r0, #ARR]
	ldr r1, =23			//24 - 1
	str r1,[r0, #PSC]
	//Enabling UIE in the DIER register of TIM6
    ldr r0, =TIM6
	ldr r1, [r0, #DIER]
	movs r2, #1
	orrs r1, r2
	str r1, [r0, #DIER]
	//Enabling TIM6 interrupt in NVIC ISER register
		//Enabling timer counter
	ldr r1, [r0, #CR1]
	movs r2, #1
	orrs r1, r2
	str r1, [r0, #CR1]

	ldr r0, =NVIC
	ldr r1, =ISER
	ldr r2, =(1 << TIM6_DAC_IRQn)
	str r2, [r0,r1]
	pop {pc}
//=======================================================
// 6.2: Confiure GPIO
//=======================================================
.global init_GPIO
init_GPIO:
    push {lr}
    //Enabling clock to port C and A
    ldr r0, =RCC
    ldr r1, [r0, #AHBENR]
    ldr r2, =GPIOCEN
    ldr r3, =GPIOAEN
    orrs r1, r2
    orrs r1, r3
    str r1, [r0, #AHBENR]
    //Set PC0, PC1, PC2, PC3, and PC8 as outputs
    ldr r0, =GPIOC
    ldr r1, [r0, #MODER]
    ldr r2, =0x10055
    orrs r1, r2
    str r1, [r0, #MODER]
    //Set PA0, PA1, PA2, and PA3 as outputs
    ldr r0, =GPIOA
    ldr r1, [r0, #MODER]
    ldr r2, =0x55
    orrs r1, r2
    str r1, [r0, #MODER]
    //Set up pull down resistance on pins PA4, PA5, PA6, and PA7
    ldr r0, =GPIOA
    ldr r1, [r0, #PUPDR]
    ldr r2, =0xaa00
    orrs r1, r2
    str r1, [r0, #PUPDR]
    pop {pc}
//=======================================================
// 6.3 Blink blue LED using Timer 6 interrupt
// Write your interrupt service routine below.
//IF I WANT TO COME BACK AND FIX THIS MY IDEA IS THIS:
	//Go to the area of the code where I store my value into the history array and change it to loading and storing by bytes
	//Instead of loading and storing the entire 32 bit register, because that is where it's messing up
//=======================================================
.type TIM6_DAC_IRQHandler, %function
.global TIM6_DAC_IRQHandler
TIM6_DAC_IRQHandler:
	push {lr}
	//incrementing col, but keeping it less than 4
	ldr r0, =col
	ldr r1, [r0]
	adds r1, #1
	movs r2, #3
	ands r1, r2
	str r1, [r0]        //SAVE*** (col), until i get to row
	//GPIOA->ODR = (1 << col)
	ldr r0, =GPIOA
	movs r2, #1
	lsls r2, r1
	str r2, [r0, #ODR]
	//index = col << 2
	movs r3, #0     //initializing r3, my index register
	movs r3, r1
	lsls r3, #2     //SAVE until i get to "case statement"
	//left Shift all the history variables
	ldr r0, =history

	movs r2, #0
	movs r4, #4
	movs r5, #8
	movs r6, #12

	cmp r3, r2
	beq zeroDef
	cmp r3, r4
	beq fourDef
	cmp r3, r5
	beq eightDef
	B twelveDef

zeroDef:
	movs r7, #0
	B doWork
fourDef:
	movs r7, #4
	B doWork
eightDef:
	movs r7, #8
twelveDef:
	movs r7, #12
doWork:
	//FIRST shifting history[r7] - history[r7+3] by 1 to the left
	//Index r7
		ldr r2, [r0, r7]
		ldr r4, =0x000000ff
		ands r4, r2
		lsls r4, #1
		ldr r5, =0x000000ff
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]
	//Index r7+1
		ldr r2, [r0, r7]
		ldr r4, =0x0000ff00
		ands r4, r2
		lsls r4, #1
		ldr r5, =0x0000ff00
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]
	//Index r7+2
		ldr r2, [r0, r7]
		ldr r4, =0x00ff0000
		ands r4, r2
		lsls r4, #1
		ldr r5, =0x00ff0000
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]
	//Index r7+3
		ldr r2, [r0, r7]
		ldr r4, =0xff000000
		ands r4, r2
		lsls r4, #1
		ldr r5, =0xff000000
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]
	//Changing r1 now since I want to save the row for a while
	ldr r6, =GPIOA
	ldr r1, [r6, #IDR]
	lsrs r1, #4
	ldr r2, =0xf
	ands r1, r2         //SAVE*** (row)
	//NOW "ORing" history with new key samples [SAVES: r1, r3]
	//Index r7
		//loading in necessary value and isolating the 8 bit number
		ldrb r2, [r0, r7]
		//moving row into a dummy reg, shifting it right by zero, ANDing it with one, ORing it with history[index]
		movs r3, r1
		lsrs r3, #0
		ldr r5, =0x1
		ands r3, r5
		orrs r4, r3
		//Re-isolating the 8-bit number, clearing the destination 8 bits, adding my value, storing my value
		ldr r5, =0x000000ff
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]
	//Index r7+1
		//loading in necessary value and isolating the 8 bit number
		adds r7, #1
		ldrb r2, [r0, r7]
		//moving row into a dummy reg, shifting it right by one, ANDing it with one, ORing it with history[index]
		movs r3, r1
		lsrs r3, #1
		ldr r5, =0x1
		ands r3, r5
		orrs r4, r3
		//Re-isolating the 8-bit number, clearing the destination 8 bits, adding my value, storing my value
		ldr r5, =0x0000ff00
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]
	//Index r7+2
		//loading in necessary value and isolating the 8 bit number
		ldr r2, [r0, r7]
		ldr r4, =0x00ff0000
		ands r4, r2
		//moving row into a dummy reg, shifting it right by one, ANDing it with one, ORing it with history[index]
		movs r3, r1
		lsrs r3, #2
		ldr r5, =0x1
		ands r3, r5
		orrs r4, r3
		//Re-isolating the 8-bit number, clearing the destination 8 bits, adding my value, storing my value
		ldr r5, =0x00ff0000
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]
	//Index r7+3
		//loading in necessary value and isolating the 8 bit number
		ldr r2, [r0, r7]
		ldr r4, =0xff000000
		ands r4, r2
		//moving row into a dummy reg, shifting it right by one, ANDing it with one, ORing it with history[index]
		movs r3, r1
		lsrs r3, #3
		ldr r5, =0x1
		ands r3, r5
		orrs r4, r3
		//Re-isolating the 8-bit number, clearing the destination 8 bits, adding my value, storing my value
		ldr r5, =0xff000000
		ands r4, r5
		bics r2, r5
		orrs r2, r4
		str r2, [r0, r7]

    //loading the value of tick, and incrementing it
    ldr r0, =tick
    ldr r1, [r0]
    adds r1, #1
    //checking if tick is greater than or equal to 1000, if so jump to that case
    ldr r2, =1000
    cmp r1, r2
    BGE tickProtocol

    //If tick is NOT greater than or equal to 1000
    str r1, [r0]
        //need to acknowledge the interrupt somehow
        ldr r0, =TIM6
        ldr r3, =SR
        ldr r1, [r0, r3]
        ldr r2, =0x1
        eors r1, r2
        str r1, [r0, r3]

    pop {pc}

tickProtocol:
/*
    //Setting tick back to zero
    movs r1, #0
    str r1, [r0]
    //Toggling bit 8 of GPIOC ODR
    ldr r0, =GPIOC
    ldr r1, [r0, #ODR]
    ldr r2, =0x100
    EORS r1, r2
    str r1, [r0, #ODR]
    */
//Need to acknowledge the interrupt somehow
    ldr r0, =TIM6
    ldr r3, =SR
    ldr r1, [r0, r3]
   	ldr r2, =0x1
    eors r1, r2
    str r1, [r0, r3]
    pop {pc}
//
//=======================================================
// 6.5 Debounce keypad
//=======================================================
.global getKeyPressed
    getKeyPressed:
    push {lr}
start1:
    movs r1, #0         //r1 is my counter, i
    movs r2, #16        //r2 is my stopper, "16"
    innerloop1:
    ldr r4, =history    //loading the array "history" into register 4
    ldrb r5, [r4, r1]    //loading the value from array with index r1 (i)

    movs r6, #1
    cmp r5, r6          //If the value at the given index is one, then return the index value
    BEQ done1

    adds r1, #1         //my increment, for loop check, and ret to top of loop
    cmp r1, r2
    BLT innerloop1

    B start1
done1:
    ldr r2, =tick
    movs r3, #0
    str r3, [r2]
    movs r0, r1
    pop {pc}

.global getKeyReleased
getKeyReleased:
    push {lr}
start2:
    movs r1, #0         //r1 is my counter, i
    movs r2, #16        //r2 is my stopper, "16"

    innerloop2:
    ldr r4, =history    //loading the array "history" into register 4
    ldrb r5, [r4, r1]    //loading the value from array with index r1 (i)
    ldr r6, =0xfffffffe		//0xfffffffe = -2
    cmp r5, r6          //If the value at the given index is "-2", then return the index value
    BEQ done2
    adds r1, #1         //my increment, for loop check, and ret to top of loop
    cmp r1, r2
    BLT innerloop2
    B start2
done2:
    ldr r2, =tick
    movs r3, #0
    str r3, [r2]
    movs r0, r1
    pop {pc}
