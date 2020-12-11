// hw8.c
//
// All of these exercises should be completed using the Standard Peripheral
// firmware.  Use only direct I/O register manipulation to implement these
// subroutines.  (Do not use Standard Peripheral macros.)  You should also
// assume that the system clock frequency, and the input to all timers, is
// 48 MHz.
//

// This homework requires you to look up hardware configuration information
// in the Family Reference Manual as well as the STM32F051R8T6 datasheet, to
// which you will have access during the lab practical exam.  Various lecture
// materials can guide you to the appropriate tables in the documentation.
// Remember that you will not have access to the lecture notes (or Piazza)
// during your lab practical exam.  For any problem where you need to
// consult the lecture notes, try to find examples you need in the appropriate
// section of Appendix A of the Family Reference Manual.

// This homework will be tested automatically.  At some point in the future,
// a test module will give you information only on what works and what doesn't.
// If you connect a CFAL1602 OLED LCD to SPI2 channel as you did for Lab 8,
// it will report information when the autotest() subroutine is invoked.

// Unless explicitly stated, you should not assume that any subroutine you
// write is invoked before another one.  You should also not assume that
// I/O register values are in their default (reset) state.  For instance, an
// I/O register field that is expected to be 00 on reset, may be set to 11
// or 10 or 01.  It is your responsibility to clear such a field before
// ORing in a new value.

// Finally, neither this nor any other homework exercise is not meant to be
// a collaborative effort with other students.  Do it on your own.  This will
// be important not only for your preparation for the practical, but to
// ensure you are not cited for copying your solution from other students.

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <math.h>

int16_t wavetable[256];
int offset;
int counter;

// Problem 1:
// Fill in the function to initialize GPIO Port C so that:
//  - PC9  is an output, using medium speed, push-pull drivers
//  - PC10 is an output, using high speed, open-drain drivers
//  - PC11 is an input, with a pull-up resistor
//  - PC12 is an input, with a pull-down resistor
// Do not change the configuration of any other pin on any other port.
void problem1(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    //Clearing all pins (inadvertently setting 11 & 12 to input)
    GPIOC->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER11);
    //Setting pins 9 & 10 to output
    GPIOC->MODER |= (GPIO_MODER_MODER9_0 |GPIO_MODER_MODER10_0);
    //Clearing pins 9 & 10 speeds, then setting 9 to med and 10 to high
    GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10);
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR10);
    //Clearing pins 9 & 10 drivers, then setting 9 to push-pull and 10 to open-drain
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_9);
    GPIOC->OTYPER |= GPIO_OTYPER_OT_10;
    //Clearing pins 11 & 12 resistor config, then setting 11 to pull-up and 12 to pull down
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR11 | GPIO_PUPDR_PUPDR12);
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR11_0 | GPIO_PUPDR_PUPDR12_1);
}

// Problem 2:
// Fill in the function to initialize GPIO Port C so that:
//  - PC8 is an output, using high speed, push-pull drivers, using the
//    alternate function that connects it to Timer 3, channel 3
//  - PC9 is an output, using high speed, push-pull drivers, using the
//    alternate function that connects it to Timer 3, channel 4
void problem2(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    //Clearing then setting to 2'b01 for output
    GPIOC->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
    GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
    //Configuring for high speed
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR10);
    //Configuring for push-pull drivers
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

    //I'm not sure if this will work
    GPIOC->AFR[1] &= ~(GPIO_AFRH_AFRH0 | GPIO_AFRH_AFRH1);
    //GPIOC->AFR[1] |= ((0 << 0) | (0 << 4));
}

// Problem 3:
// Fill in the function to initialize Timer 3 so that:
//  - The counter is up-counting.
//  - The counter goes from 0 to 99.
//  - The counter will be reset to zero 300 times per second.

//  - Channels 3 and 4 are set for PWM mode 1.

//  - Preload enable is set for CCR3 and CCR4.

//  - Outputs for channel 3 and 4 are enabled.

//  - The CCR3 register is set to 12.
//  - The CCR4 register is set to 84.
//  - Enable the timer.
//  (Change no other configuration registers for any other functionality.)
void problem3(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    //Ensuring timer is off
    TIM3->CR1 &= ~TIM_CR1_CEN;

    TIM3->CR1 &= ~TIM_CR1_DIR;
    TIM3->ARR = 100-1;
    TIM3->PSC = 1600-1;

    //TIM3->

    TIM3->CCMR2 |= (TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE);
    TIM3->CCR3 = 12;
    TIM3->CCR4 = 84;
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

// Problem 4:
// Fill in the function below so that it uses floating-point arithmetic
// to initialize the wavetable array (at the top of the file) so that
// it approximates a sine wave with the following formula:
//
// for(x=0; x < sizeof wavetable / sizeof wavetable[0]; x += 1)
//     wavetable[x] = 32767 * sin(x * 2 * M_PI / 256);
//
// This should form a sinusoid with an amplitude of 32767 that would
// repeat after 256 steps.  Its maximum value will be 32767, and its
// minimum value will be -32767.
void problem4(void) {
    int i;
    for(i=0; i<256; i++)
        wavetable[i] = 32767.0 * sin(i * 2 * M_PI /256);
}

// Problem 5:
// Fill in the function to initialize the DAC so that:
//  - its output is routed to its output pin.
//  - it is triggered by software.
//  - the trigger is enabled.
//  - it is enabled.
// You should be able to "listen to" the DAC by plugging in a speaker.
void problem5(void) {
    /*
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    //DAC->CR &= DAC_CR_BOFF1;

    //DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;

    // These were included in the demo slides but if the channel is already disabled and the buffer already on, they're unnecessary
             // DAC->CR &= ~DAC_CR_EN1;
             // DAC->CR &= ~DAC_CR_BOFF1;
             //

    //Enabling channel 1 trigger
    DAC->CR |= DAC_CR_TEN1;
    //Selecting a software trigger
    DAC->CR |= DAC_CR_TSEL1;
    //DAC channel 1 enable
    DAC->CR |= DAC_CR_EN1;
    */
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
        /* These were included in the demo slides but if the channel is already disabled and the buffer already on, they're unnecessary
         * DAC->CR &= ~DAC_CR_EN1;
         * DAC->CR &= ~DAC_CR_BOFF1;
         */
    //Enable the trigger
        DAC->CR |= DAC_CR_TEN1;
    //Setup software trigger
        DAC->CR |= DAC_CR_TSEL1;
    //Enable the DAC
        DAC->CR |= DAC_CR_EN1;
}

// Problem 6:
// Fill in the function below to output the value of
// (wavetable[offset] + 32768)>>4  to the DAC.  You can assume the wavetable
// array contains values that were initialized by problem4().
// The addition will shift each entry in the wavetable to the
// positive range of values in the range 1 ... 65535.
// The ">>4" will divide each value by 16 to produce values
// in the range 0 ... 4095.
// Then:
//  - Write the converted result to DHR12R1.
//  - Software trigger the DAC.
void problem6(void) {

}

// Problem 7:
// Write the interrupt service routine for Timer 2 below.
// You should be able to determine the name for it.
// The ISR should do the following:
//  - Acknowledge the source of the interrupt.  (See Problem 8.)
//  - If the 'offset' variable is greater than or equal to the
//    number of elements in wavetable, set it to zero.
//  - Invoke problem6().
//  - Increment the 'offset' variable.
// Remember that you can find the names of ISRs in startup/startup_stm32f0xx.S.


// Problem 8:
// Fill in the function below to initialize Timer 2 so that:
//  - The counter is set back to zero 64000 times per second.
//  - The ARR register is 1.
//  - The timer generates a interrupt on each update event.
//  - Enable the timer.
//  - Ensure that interrupt is enabled.
//  (Change no other configuration registers for any other functionality.)
// Remember that the value you assign to the NVIC ISER is
//   1 << interrupt_name_IRQn
//
// If you run
//    problem4(); problem5(); problem6(); problem8();
// you should hear a 250 Hz sine wave on the DAC output.
void problem8(void) {

}

// Problem 9:
// Fill in the function below so that it uses floating-point arithmetic
// to initialize the wavetable array (at the top of the file) so that
// it approximates a sine wave with the following formula:
//
// for(x=0; x < sizeof wavetable / sizeof wavetable[0]; x += 1)
//     wavetable[x] = (32767 * sin(x * 2 * M_PI / 256) + 32768) / 16;
//
// This should form a sinusoid with an amplitude of 2047 that is offset
// by 2048.  Its maximum value will be 4095, and its minimum value will be 0.
void problem9(void) {

}

// Problem 10:
// Fill in the function below to set up a DMA channel such that:
//  - The peripheral output is the DAC DHR12R1 register.
//  - The input is the memory region for wavetable.
//  - The count is the number of elements in wavetable.
//  - Ensure it is copying from memory to the peripheral.
//  - Circular transfers are enabled.
//  - You must determine the values for the MSIZE and PSIZE fields.
//  - The DMA channel is triggered by the timer 15 update event.
// You must determine which DMA channel to use!  (Which DMA channel
// can be triggered by the Timer 15 update event?)
void problem10(void) {

}

// Problem 11:
// Fill in the function below to set up timer 15 so that:
//  - It issues an update event at a rate of 160 kHz.
//  - It triggers a DMA channel on an update event.
//  - Set the MMS field so that the update event is selected for TRGO.
//  - Set the auto-reload preload enable flag.
//  - Enable the timer.
// FURTHERMORE:
//  - Reconfigure the DAC so it is triggered by Timer 15 TRGO.
//  - Turn off Timer 2.
//
// ERRATA:
//  - An earlier version asked you to enable DMA in the DAC CR.  Don't do that.
//
// If you run
//    problem5(); problem6(); problem9(); problem10(); problem11();
// you should hear a 625 Hz tone on the DAC output.
void problem11(void) {

}

// Problem 12:
// Write an interrupt service routine to be invoked by the DMA channel
// you used for Problem 10.
// It should do the following:
//  - Acknowledge the source of the interrupt.  (See Problem 13.)
//  - Increment the 'counter' variable.
//  - If counter is greater than or equal to 2:
//    - set counter to 0
//    - if the rate of timer 15 is 160 kHz, set it to 320 kHz
//    - else set the timer 15 rate to 160 kHz
// Remember that you can find names of ISRs in startup/startup_stm32f0xx.S.
//
// This should cause the DAC to output two cycles of a sinusoid at one
// frequency, and then two cycles of the same sinusoid at twice the
// frequency.


// Problem13:
// Fill in the function below to:
//  - Enable the interrupt for the ISR named in Problem 12.
//  - Set the DMA channel used in Problem 10 to invoke an interrupt
//    each time there is a transfer complete event for the DMA.
// (Remember that the value you should assign to the NVIC ISER is
//  1 << interrupt_name_IRQn )
//
// If you run
//   problem5(); problem6(); problem9(); problem10(); problem11(); problem13();
// you should hear a strange tone and see shifting frequencies on a scope.
void problem13(void) {

}

// Problem 14:
// Fill in the function below to initialize the wavetable array such that:
//  - wavetable[0] is 5
//  - wavetable[1] is 5
//  - wavetable[2] is 5
//  - wavetable[3] is 5
//  - wavetable[4] is 5
//  - wavetable[5] is 95
void problem14(void) {
    int i;
    for(i=0; i<5; i++)
        wavetable[i] = 5;
    wavetable[5] = 95;
}

// Problem 15:
// Fill in the function below to initialize a DMA channel so that:
//  - The peripheral output is the CCR register for timer 3, channel 3.
//  - The input is the memory region for wavetable.
//  - The count 6.
//  - Ensure it is copying from memory to the peripheral.
//  - You must determine the values for the MSIZE and PSIZE fields.
//  - Circular copying is enabled.
//  - The DMA channel is triggered by the timer 3 channel 3 event.
// You must determine which DMA channel to use!
void problem15(void) {

}

// Problem 16:
// Fill in the function below to update timer 3 so that:
//  - It triggers a DMA channel on a channel 3 event.
// You should assume Timer 3 is already running as it was in problem 3.
//
// If you run
//     problem2(); problem3(); problem14(); problem15(); problem16();
// You should see a distinctive variable duty-cycle square wave output on PC8.
void problem16(void) {

}

// Problem 17:
// Fill in the function below to set up the ADC so that:
//  - It uses the high-speed internal clock.
//  - It can read analog values from PC0.
void problem17(void) {

}

// Problem 18:
// Fill in the function below to set up SPI channel 1 so that:
//  - It is first disabled.
//  - Its pins are set to use PA4 through PA7.
//  - The baud rate is 6mbit/sec.
//  - Clock polarity is "1 when idle".
//  - The second clock transition is the first data capture edge.
//  - It is set for Master mode.
//  - Software slave management is enabled.
//  - The internal slave select is currently disabled.
//  - Frames are sent LSB first.
//  - It has a data word size of 9 bits.
//  - Slave select output is enabled.
//  - It is enabled once all other fields are set.
//
// ERRATA:
//  - An earlier version told you to:
//    . Enable bidirectional data mode.  Don't do that.
//    . Enable output for bidirectional mode.  Don't do that.
//  - An earlier version forgot to tell you to enable slave select output.
//    It turns out that this is necessary in order be able to set the SPE bit.
//
void problem18(void) {

}

// Problem 19:
// Fill in the function below to set up I2C2
//  - It uses PB10 and PB11 for external I/O.
//  - It uses the 8MHz internal "HSI" clock
//  - The analog noise filter is turned on.
//  - No interrupts are enabled.
//  - Clock stretching is disabled.
//  - It uses the recommended timing register values for 400 kHz operation.
//  - The "own address 1" and "own address 2" are both disabled.
//  - It uses 7-bit addresses.
//  - Auto end is enabled.
//  - NACK is set.
void problem19(void) {

}

// Problem 20:
// Fill in the function below to set up USART2 so that:
//  - It uses PA2 and PA3 for TxD and RxD.
//  - It is first disabled.
//  - It uses 16x oversampling.
//  - The baud rate is 48000.
//  - It uses an 8 bits word.
//  - It uses 1 stop bit.
//  - It uses odd parity.
//  - It is enabled.
void problem20(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    //Turning off the USART
    USART2->CR1 &= ~(USART_CR1_UE);

    USART2->CR1 &= ~(USART_CR1_OVER8);

    USART2->BRR = 0x3e8;

    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 |GPIO_MODER_MODER3_1);




}

// Just look at all the things you learned how to do in this class.
