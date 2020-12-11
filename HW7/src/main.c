
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
			
extern autotest(void);
void tim1_init();
void tim3_init();

int rdir = 1;
int gdir = 1;
int bdir = 1;

int main(void) {
    autotest();

    tim1_init();
    tim3_init();
    while(1) asm("wfi");
    return 0;
}

void tim1_init(){
    //Enable timer one
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    //Prescaller output 2 mhz
    TIM1->PSC = 24-1;
    //Output from ARR is 1000 hz
    TIM1->ARR = 2000-1;
    //Edge alligned and upcounting (don't think I need to specify these)
    //Setting the channels for output mode and configure them to stay high until count value is reached
        //Configuring channels output mode as HIGH as long as TIMx_CNT < TIMx_CCR2, and enabling pre-load enable (no need to clear, resetVal = '0)
        TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
        TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
        TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
    //Enabling output for channel 1, 2, and 3
        TIM1->CCER |= TIM_CCER_CC1E;
        TIM1->CCER |= TIM_CCER_CC2E;
        TIM1->CCER |= TIM_CCER_CC3E;
    //Default values for CCRx
        TIM1->CCR1 = 120;
        TIM1->CCR2 = 666;
        TIM1->CCR3 = 1333;
    //Setting MOE bit of BDTR to allow us to generate output on any channel of TIM1
        TIM1->BDTR |= TIM_BDTR_MOE;
    //Enable clock
        TIM1->CR1 |= TIM_CR1_CEN;
//Enabling GPIO
        RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
        //Clear the mode for pins 8,9,10
        GPIOA->MODER &= ~GPIO_MODER_MODER8;   //pin 8
        GPIOA->MODER &= ~GPIO_MODER_MODER9;   //pin 9
        GPIOA->MODER &= ~GPIO_MODER_MODER10;  //pin 10
        //Setting pins 8,9,10 to alternate function
        GPIOA->MODER |= GPIO_MODER_MODER8_1;   //pin 8
        GPIOA->MODER |= GPIO_MODER_MODER9_1;   //pin 9
        GPIOA->MODER |= GPIO_MODER_MODER10_1;  //pin 10
        //Configure the Alternate High register so that each pin is on AF2
        GPIOA->AFR[1] &= ~0xfff;
        GPIOA->AFR[1] |= 2<<0;          //pin 8
        GPIOA->AFR[1] |= 2<<(1*4);      //pin 9
        GPIOA->AFR[1] |= 2<<(2*4);      //pin 10
}

void tim3_init(){
    //Enable clock to timer
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    //Prescaller output 1 mhz
    TIM3->PSC = 48-1;
    //Output from ARR is 1000 hz
    TIM3->ARR = 2000-1;
    //Enable update counter
    TIM3->DIER |= TIM_DIER_UIE;
    //Enable counter
    TIM3->CR1 |= TIM_CR1_CEN;
    //Enable the interrupt
    //NVIC->ISER[0] |= 1<< TIM3_IRQn;
    NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler() {
    //Acknowledging the interrupt
    TIM3->SR &= ~TIM_SR_UIF;
    int dummy;

    if(rdir == 1){
        TIM1->CCR3 = (TIM1->CCR3 * 517) >> 9;
        if(TIM1->CCR3 >= ((TIM1->ARR+1)*0.99))
            rdir = -1;
    }
    else{
        TIM1->CCR3 = (TIM1->CCR3 * 506) >> 9;
        if(TIM1->CCR3 <= ((TIM1->ARR+1)*0.06))
            rdir = 1;
    }

    if(gdir == 1){
        TIM1->CCR2 = (TIM1->CCR2 * 517) >> 9;
        if(TIM1->CCR2 >= ((TIM1->ARR+1)*0.99))
            gdir = -1;
        }
    else{
        TIM1->CCR2 = (TIM1->CCR2 * 506) >> 9;
        if(TIM1->CCR2 <= ((TIM1->ARR+1)*0.06))
            gdir = 1;
        }

    if(bdir == 1){
        TIM1->CCR1 = (TIM1->CCR1 * 517) >> 9;
        if(TIM1->CCR1 >= ((TIM1->ARR+1)*0.99))
            bdir = -1;
        }
    else{
        TIM1->CCR1 = (TIM1->CCR1 * 506) >> 9;
        if(TIM1->CCR1 <= ((TIM1->ARR+1)*0.06))
            bdir = 1;
        }

}
