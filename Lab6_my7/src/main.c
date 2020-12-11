//Lab6
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RATE 100000
#define N 1000
#define M_PI 3.14159

short int wavetable[N];
int volatile offset = 0;
int volatile offset2 = 0;
int step = 554.365 * N / RATE * (1<<16);
int step2 = 698.456 * N / RATE * (1<<16);

void init_wavetable(void)
{
  int x;
  for(x=0; x<N; x++)
    wavetable[x] = 32767 * sin(2 * M_PI * x / N);
}

// This function
// 1) enables clock to port A,
// 2) sets PA0, PA1, PA2 and PA4 to analog mode
void setup_gpio() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOC->MODER |= 3<<(2*0);
    GPIOC->MODER |= 3<<(2*1);
    GPIOC->MODER |= 3<<(2*2);
    GPIOC->MODER |= 3<<(2*4);
}

// This function should enable the clock to the
// onboard DAC, enable trigger,
// setup software trigger and finally enable the DAC.
void setup_dac() {
//Enable clock
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

// This function should,
// enable clock to timer6,
// setup pre scalar and arr so that the interrupt is triggered every 10us,
// enable the timer 6 interrupt and start the timer.
void setup_timer6() {
//Enabling clock of TIM6
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
//Assumption: 1 interrupt every 10us = 0.000 01s (480 / 48,000,000 = 0.000010)
//Setting pre scalar to
    TIM6->PSC = 23;     //(24-1)
//Setting arr to
    TIM6->ARR = 19;     //(20-1)
//Enabling TIM6 interrupt and starting the timer
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] |= 1<<TIM6_DAC_IRQn;
    //or
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}



// This function should enable the clock to ADC,
// turn on the clocks, wait for ADC to be ready.
void setup_adc() {
//Enable clock to the ADC
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//turn on the high speed clock
    RCC->CR2 |= RCC_CR2_HSI14ON;
//Enable the ADC
    ADC1->CR |= ADC_CR_ADEN;
//Waiting for the ADC to be ready
    while(!(RCC->CR2 & RCC_CR2_HSI14ON));   //wait for 14 MHz clock to be ready
    while(!(ADC1->ISR & ADC_ISR_ADRDY));    //wait for ADC to be ready
    while(ADC1->CR & ADC_CR_ADSTART);       //wait for ADC start to be zero
}

// This function should return the value from the ADC
// conversion of the channel specified by the channel variable.
// Make sure to set the right bit in channel selection
// register and do not forget to start adc conversion.
int read_adc_channel(unsigned int channel) {
    int returnVal;
//Unselect all channels, then select the one I want to read frm
    ADC1->CHSELR = 0;
    ADC1->CHSELR |= (1 << channel);
//Waiting to be able to start the conversion, then starting
    while(!(ADC1->ISR & ADC_ISR_ADRDY));    //wait for ADC to be ready
    ADC1->CR |= ADC_CR_ADSTART;
//Waiting for conversion to be done, then reading value into variable
    while(!(ADC1->ISR & ADC_ISR_EOC));      //wait for End Of Conversion

    //Assumption: if the analog value is 1.5 for example, it will be read
    //as 0x800, which will go into my integer as a value of 2^11
    //The only way to read it as it's proper voltage would be to multiply
    //ADC1->DR by 3/4095.0, but I'm not using floats
    returnVal = ADC1->DR;
    //return Val / Actual voltage
    //2^11 / 1.5
    //2^10 / 0.75
    //2^11 + 2^10 / 2.25
    //etc.
    return returnVal;
}

void TIM6_DAC_IRQHandler() {
//Waiting for the DAC to be ready for another conversion
    while((DAC->SWTRIGR & DAC_SWTRIGR_SWTRIG1) == DAC_SWTRIGR_SWTRIG1);
    //DAC->DHR12R1 = ; this step was already done in dac interrupt
//Initiating the conversion (asserting the software trigger)
    DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
//Acknowledging the interrupt
    //EXTI->PR ^= (1 << TIM6_DAC_IRQn);
    TIM6->SR &= ~TIM_SR_UIF;
//for the DAC portion
    offset += step;
    if ((offset>>16) >= N) // If we go past the end of the array,
        offset -= N<<16; // wrap around with same offset as overshoot.
    int sample = wavetable[offset>>16]; // get a sample
    sample = sample / 16 + 2048; // adjust for the DAC range
    DAC->DHR12R1 = sample;

//Combining a second signal in DAC portion
    offset2 += step2;
        if ((offset2>>16) >= N) // If we go past the end of the array,
            offset2 -= N<<16; // wrap around with same offset as overshoot.
        int sample2 = wavetable[offset2>>16]; // get a sample
        sample2 = sample2 / 16 + 2048; // adjust for the DAC range
        //Directly adding the two samples together
        DAC->DHR12R1 += sample2;
//If I'd like to combine them and then make sure they're within range, last 2 steps

        sample += sample2;
        sample = sample / 16 + 2048; // adjust for the DAC range
        DAC->DHR12R1 = sample;

}

int main(void)
{
    init_wavetable();
    setup_gpio();
    setup_dac();
    setup_adc();
    setup_timer6();
    int a1, a2;
    while(1){
        asm("wfi");

        //Figure out how to change the values of step and step2 when the input voltages change
        //Note: a1 and a2 are changing properly in the main function
        a1 = read_adc_channel(1);  //channel one is my pa0
        a2 = read_adc_channel(2);  //channel two is my pa1
        step = (200 + (a1/4095.0)*800) * N / RATE * (1<<16);
        step2 = (200 + (a2/4095.0)*800) * N / RATE * (1<<16);
    }
}
