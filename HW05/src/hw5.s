.cpu cortex-m0
.thumb
.syntax unified
.fpu softvfp

.equ RCC,       0x40021000
.equ GPIOA,     0x48000000
.equ GPIOC,     0x48000800
.equ AHBENR,    0x14
.equ APB2ENR,   0x18
.equ APB1ENR,   0x1c
.equ IOPAEN,    0x20000
.equ IOPCEN,    0x80000
.equ SYSCFGCOMPEN, 1
.equ TIM3EN,    2
.equ MODER,     0x00
.equ OSPEEDR,   0x08
.equ PUPDR,     0x0c
.equ IDR,       0x10
.equ ODR,       0x14
.equ BSRR,      0x18
.equ BRR,       0x28
.equ PC8,       0x100

// SYSCFG control registers
.equ SYSCFG,    0x40010000
.equ EXTICR2,   0x0c

// NVIC control registers
.equ NVIC,      0xe000e000
.equ ISER,      0x100

// External interrupt control registers
.equ EXTI,      0x40010400
.equ IMR,       0
.equ RTSR,      0x8
.equ PR,        0x14

.equ TIM3,      0x40000400
.equ TIMCR1,    0x00
.equ DIER,      0x0c
.equ TIMSR,     0x10
.equ PSC,       0x28
.equ ARR,       0x2c

// Iinterrupt number for EXTI4_15 is 7.
.equ EXTI4_15_IRQn,  7
// Interrupt number for Timer 3 is 16.
.equ TIM3_IRQn,  16

//=====================================================================
// Q1
//=====================================================================
.global euclidean
euclidean:
    push {r4-r7, lr}
    cmp r1, r0
    BHI bHigher     //if b > a, (or a < b)
    
    cmp r0, r1
    BLS equal       //if a==b, (a < b, already covered)
aGreater:           //This is just for me to follow along
    subs r0, r1
    bl euclidean
    pop {r4-r7, pc}
equal:
    pop {r4-r7, pc}
bHigher:
    subs r1, r0
    bl euclidean
    pop {r4-r7, pc}

//=====================================================================
// Q2
//=====================================================================
.global enable_porta
enable_porta:
    push {r4-r7, lr}
    
    ldr r0, =RCC
    ldr r1, [r0, #AHBENR]
    ldr r2, =IOPAEN
    orrs r1, r2
    str r1, [r0, #AHBENR]
    
    pop {r4-r7, pc}

//=====================================================================
// Q3
//=====================================================================
.global enable_portc
enable_portc:
    push {r4-r7, lr}
    
    ldr r0, =RCC
    ldr r1, [r0, #AHBENR]
    ldr r2, =IOPCEN
    orrs r1, r2
    str r1, [r0, #AHBENR]
    
    pop {r4-r7, pc}

//=====================================================================
// Q4
//=====================================================================
.global setup_pa4
setup_pa4:
    push {r4-r7, lr}
    
    ldr r0, =GPIOA
    ldr r1, [r0, #MODER]
    ldr r2, =0x300           //Turns off the two bits that control pin 4
    bics r1, r2
    str r1, [r0, #MODER]
    
    ldr r1, [r0, #PUPDR]
    ldr r2, =0x300
    bics r1, r2
    ldr r2, =0x100
    orrs r1, r2
    str r1, [r0, #PUPDR]
    
    pop {r4-r7, pc}

//=====================================================================
// Q5
//=====================================================================
.global setup_pa5
setup_pa5:
    push {r4-r7, lr}
    
    ldr r0, =GPIOA
    ldr r1, [r0, #MODER]
    ldr r2, =0xc00           //Turns off the two bits that control pin 5
    bics r1, r2
    str r1, [r0, #MODER]
    
    ldr r1, [r0, #PUPDR]
    ldr r2, =0xc00
    bics r1, r2
    ldr r2, =0x800
    orrs r1, r2
    str r1, [r0, #PUPDR]
    
    pop {r4-r7, pc}

//=====================================================================
// Q6
//=====================================================================
.global setup_pc8
setup_pc8:
    push {r4-r7, lr}
    
    ldr r0, =GPIOC
    ldr r1, [r0, #MODER]
    ldr r2, =0x30000           //Turns off the two bits that control pin 8
    bics r1, r2
    ldr r2, =0x10000
    orrs r1, r2
    str r1, [r0, #MODER]
    
    //Controlling output speed (High speed)
    ldr r1, [r0, #OSPEEDR]
    ldr r2, =0x30000
    orrs r1, r2
    str r1, [r0, #OSPEEDR]
    
    pop {r4-r7, pc}

//=====================================================================
// Q7
//=====================================================================
.global setup_pc9
setup_pc9:
    push {r4-r7, lr}
    
    ldr r0, =GPIOC
    ldr r1, [r0, #MODER]
    ldr r2, =0xc0000           //Turns off the two bits that control pin 8
    bics r1, r2
    ldr r2, =0x40000
    orrs r1, r2
    str r1, [r0, #MODER]
    
    //Controlling output speed (High speed)
    ldr r1, [r0, #OSPEEDR]
    ldr r2, =0xc0000
    bics r1, r2
    ldr r2, =0x40000
    orrs r1, r2
    str r1, [r0, #OSPEEDR]
    
    pop {r4-r7, pc}
//=====================================================================
// Q8
//=====================================================================
.global action8
action8:
    push {r4-r7, lr}
    
    ldr r0, =GPIOA
    ldr r1, [r0, #IDR]
    ldr r2, [r0, #IDR]
    lsrs r1, #4
    lsrs r2, #5
    ldr r7, =0xfffffffe
    bics r1, r7
    bics r2, r7
    movs r3, #1
    cmp r1, r3
    BEQ continue1
    B PC8toOne
continue1:
    cmp r2, r3
    BNE PC8toZero
    B PC8toOne
PC8toZero:
	ldr r0, =GPIOC
    ldr r1, [r0, #ODR]
    ldr r2, =0x100
    bics r1, r2
    str r1, [r0, #ODR]
    pop {r4-r7, pc}
PC8toOne:
    ldr r0, =GPIOC
    ldr r1, [r0, #ODR]
    ldr r2, =0x100
    orrs r1, r2
    str r1, [r0, #ODR]
    pop {r4-r7, pc}

//=====================================================================
// Q9
//=====================================================================
.global action9
action9:
    push {r4-r7, lr}
        ldr r0, =GPIOA
        ldr r1, [r0, #IDR]
        ldr r2, [r0, #IDR]
        //Shift and isolate the single relevant bit
        lsrs r1, #4
        lsrs r2, #5
        ldr r7, =0xfffffffe
        bics r1, r7
        bics r2, r7
        //Compare the PA4 to check if it's LOW (or not HIGH)
        movs r3, #1
        cmp r1, r3
        BNE continueb1
        B PC9toOne
    continueb1:
        //Check PS5 to check if it's HIGH
        cmp r2, r3
        BEQ PC9toZero
        B PC9toOne
    PC9toZero:
        ldr r0, =GPIOC
        ldr r1, [r0, #ODR]
        ldr r2, =0x200
        bics r1, r2
        str r1, [r0, #ODR]
        pop {r4-r7, pc}
    PC9toOne:
        ldr r0, =GPIOC
        ldr r1, [r0, #ODR]
        ldr r2, =0x200
        orrs r1, r2
        str r1, [r0, #ODR]
        pop {r4-r7, pc}

//=====================================================================
// Q10
//=====================================================================
.type EXTI4_15_IRQHandler, %function
.global EXTI4_15_IRQHandler
EXTI4_15_IRQHandler:
    push {r4-r7, lr}
    //Incrementing counter
    ldr r0, =counter
    ldr r1, [r0]
    adds r1, #1
    str r1, [r0]
    //Acknowledging interrupt
    ldr r0, =EXTI
    ldr r1, [r0, #PR]
    movs r2, #1
    ldr r3, =EXTI4_15_IRQn
    lsls r2, r3
    //toggle the interrupt
    eors r1, r2
    str r1, [r0, #PR]
    
    pop {r4-r7, pc}

//=====================================================================
// Q11
//=====================================================================
.global enable_exti4
enable_exti4:
    push {r4-r7, lr}
    //Enable clock to SYSCFG subsystem
    ldr r0, =RCC
    ldr r1, [r0, #APB2ENR]
    ldr r2, =SYSCFGCOMPEN
    orrs r1, r2
    str r2, [r0, #APB2ENR]
    //Set up SYSCFG external interrupt config. reg 2 to use PA4 as source
    ldr r0, =SYSCFG
    ldr r1, [r0, #EXTICR2]
    ldr r2, =0x0000ffff
    bics r1, r2
    ldr r2, =0x00004000
    orrs r1, r2
    str r1, [r0, #EXTICR2]
    //configure EXTI_RTSR to trigger on rising edge
    ldr r0, =EXTI
    ldr r1, [r0, #RTSR]
    ldr r2, =0x10
    orrs r1, r2
    str r1, [r0, #RTSR]
    //set EXTI_IMR to not ignore pin number 4
    ldr r0, =EXTI
    ldr r1, [r0, #IMR]
    ldr r2, =0x10
    orrs r1, r2
    str r1, [r0, #IMR]
    //configure the NVIC to enable the interrupt
    ldr r0, =NVIC
    ldr r7, =0x100
    ldr r1, [r0, r7]
    ldr r2, =EXTI4_15_IRQn
    movs r3, #1
    lsls r3, r2
    orrs r1, r3
    str r1, [r0, r7]
    
    pop {r4-r7, pc}
//=====================================================================
// Q12
//=====================================================================
.type TIM3_IRQHandler, %function
.global TIM3_IRQHandler
TIM3_IRQHandler:
    push {r4-r7, lr}
//Toggling PC9 (my green LED)
	//Giving the toggling some time
    movs r6, #0
    ldr r5, =1000000
top:
    adds r6, #1
    cmp r6, r5
    BLT top
	//The actual toggling
    ldr r0, =GPIOC
    ldr r1, [r0, #ODR]
    movs r2, #1
    lsls r2, #9
    eors r1, r2
    str r1, [r0, #ODR]
    
    //Acknowledging the interrupt
    ldr r0, =EXTI
    ldr r1, [r0, #PR]
    movs r2, #1
    ldr r3, =TIM3_IRQn
    lsls r2, r3
    eors r1, r2
    str r1, [r0, #PR]

    pop {r4-r7, pc}
//=====================================================================
// Q13
//=====================================================================
.global enable_tim3
enable_tim3:
    push {r4-r7, lr}
    //Enabling the clock to TIM3 subsystem
    ldr r0, =RCC
    ldr r1, [r0, #APB1ENR]
    ldr r2, =0x2
    orrs r1, r2
    str r1, [r0, #APB1ENR]
    //Configuring ARr and PSC to give update twice a second
    ldr r0, =TIM3
    ldr r1, =2399
    str r1, [r0, #PSC]
    ldr r1, =9999
    str r1, [r0, #ARR]
    //Setting DIER of TIM3 so that interrupt occurs on an update event
    ldr r1, [r0, #DIER]
    movs r2, #1
    orrs r1, r2
    str r1, [r0, #DIER]
    //Write the bit to NVIC ISER so that the interrupt for TIM3 is enabled
    ldr r0, =NVIC
    ldr r7, =ISER
    ldr r1, [r0, r7]
    movs r2, #1
    ldr r3, =TIM3_IRQn
    lsls r2, r3
    orrs r1, r2
    str r1, [r0, r7]
    //Enable counter for TIM3
    ldr r0, =TIM3
    ldr r1, [r0, #TIMCR1]
    movs r2, #1
    orrs r1, r2
    str r1, [r0, #TIMCR1]
    
    pop {r4-r7, pc}
