//Lab7
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

void init_lcd(void);
void display1(const char *);
void display2(const char *);
void test_wiring();

int red = 0, blue = 0, grn = 0;
char line1[16] = {"Freq:"};
char line2[16] = {"Duty:"};

int col = 0;
int8_t history[16] = {0};
int8_t lookup[16] = {1,4,7,0xe,2,5,8,0,3,6,9,0xf,0xa,0xb,0xc,0xd};
char char_lookup[16] = {'1','4','7','*','2','5','8','0','3','6','9','#','A','B','C','D'};

int get_key_pressed() {
    int key = get_key_press();
    while(key != get_key_release());
    return key;
}

char get_char_key() {
    int index = get_key_pressed();
    return char_lookup[index];
}

int get_user_freq() {
    int freq = 0;
    int pos = 0;
    while(1) {
        int index = get_key_pressed();
        int key = lookup[index];
        if(key == 0x0d)
            break;
        if(key >= 0 && key <= 9) {
            freq = freq * 10 + key;
            pos++;
            if(pos < 9)
                line1[4+pos] = key + '0';

            display1(line1);
        }
    }

    return freq;
}

void get_pwm_duty() {

    int pos = 0;
    red = 0;
    grn = 0;
    blue = 0;
    while(1) {
        int index = get_key_pressed();
        int key = lookup[index];
        if(key >= 0 && key <= 9) {
            switch(pos) {
            case 0: red = 10 * key;
                    break;
            case 1: red = red + key;
                    break;
            case 2: grn = 10 * key;
                    break;
            case 3: grn = grn + key;
                    break;
            case 4: blue = 10 * key;
                    break;
            case 5: blue = blue + key;
                    break;
            }
            pos++;
            if(pos < 9)
                line2[4+pos] = key + '0';

            display2(line2);
        }

        if(pos == 6)
            break;
    }
}

void prob2() {
    setup_gpio();
    setup_pwm();
    int r, g, b;
    r = g = b = 0;
    int state = 0;
    while(1) {
        if(r == 100) {
            state = 1;
        } if(g == 100) {
            state = 2;
        } if(b == 100) {
            state = 3;
        } if(r == 100) {
            state = 1;
        }

        if(state == 0) {
            r = r + 1;
        }
        if(state == 1) {
            r = r - 1;
            g = g + 1;
        }

        if(state == 2) {
            g = g - 1;
            b = b + 1;
        }

        if(state == 3) {
            r = r + 1;
            b = b - 1;
        }

        update_rgb(r, g, b);
        usleep(10000);
    }
}

void prob3(void)
{
    char keys[16] = {"Key Pressed:"};
    init_lcd();
    init_keypad();
    setup_timer6();

    display1("Problem 3");
    display2(keys);
    while(1) {
        char key = get_char_key();
        if(key != '\0') {
            keys[12] = key;
            display2(keys);
        }
    }
}

// -------------------------------------------
// Section 6.4
// -------------------------------------------
void prob4(void)
{
    init_lcd();
    init_keypad();
    setup_pwm();

    setup_gpio(); //I added this for display purposes

    setup_timer6();
    display1(line1);
    display2(line2);
    int freq, r, g, b;

    while(1) {
        char key = get_char_key();
        //My code
        if(key == '*'){     //if the key pressed is the "*" key
            freq = get_user_freq();
            update_freq(freq);
        }
        else if(key == '#'){     //if the key pressed is the "#" key
            get_pwm_duty();
            r = red;
            b = blue;
            g = grn;
            update_rgb(r, g, b);
        }
        //My code above ^
    }
}


// Student Code goes below, do not modify code above this
// -------------------------------------------
// Section 6.2
// -------------------------------------------
// Should enable clock to GPIO port A, configure the modes of the three
// pins corresponding to TIM1_CH1, TIM1_CH2 and TIM1_CH3 as alternate function.
void setup_gpio() {
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

// Should use TIM1 to PSC so that the clock is 1 KHz, and choose the
// value of ARR so that the PWM frequency is 10 Hz. The duty cycle of each
// channel will be set by writing a value between 0 and 99 to the CCRx
// registers. Note since we have a common anode configuration,
// CCRx of 100 will result in an off LED and
// a CCRx of 0 will result in maximum brightness.
void setup_pwm() {
//Enabling TIM1 so that it's noncentered and upcounting
    //Enabling clock to the timer
    RCC->APB2ENR = RCC_APB2ENR_TIM1EN;
//Setting Prescaler value, to output 100 khz
    //48,000,000 / 480 = 100 khz
    TIM1->PSC = 480-1;
    //100,000 / 100 = 1 khz
    TIM1->ARR = 100 - 1;
//Setting the channels for output mode and configure them to stay high until count value is reached
    //Configuring channels output mode as HIGH as long as TIMx_CNT < TIMx_CCR2, and enabling pre-load enable (no need to clear, resetVal = '0)
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
    TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
    TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
//Enabling output for channel 1, 2, and 3
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->CCER |= TIM_CCER_CC2E;
    TIM1->CCER |= TIM_CCER_CC3E;
//Setting MOE bit of BDTR to allow us to generate output on any channel of TIM1
    TIM1->BDTR |= TIM_BDTR_MOE;
//Enable clock
    TIM1->CR1 |= TIM_CR1_CEN;
}


// This function accepts an integer argument that is used to update the
// TIM1_PSC to produce the requested frequency (as close as possible) on
// the output pins. Remember that the output frequency will be 100 times
// slower than the TIM1_PSC due to TIM1_ARR always being 100-1 (99).
// The formula for calculating the output frequency is then:
//          freq = 48,000,000.0 / (TIM1_PSC + 1) / 100.0
// You should determine the formula to use to put the proper value
// into TIM1_PSC given the frequency
void update_freq(int freq) {


     float holder = 0;
    holder = 48000000.0/ (freq*100.0);
    if((holder-1) > 65535)
        TIM1->PSC = 65635;
    else
        TIM1->PSC = holder - 1;

    //TIM1->PSC = freq;
}

// This function accepts three arguments---the red, green, and blue values used
// to set the CCRx registers. The values should never be smaller than zero or
// larger than 100. The value can be assigned directly to the appropriate
// CCR registers. E.g. the red LED is connected to channel 1.
void update_rgb(int r, int g, int b) {
    //Red control
    if(r > 100)
        TIM1->CCR3 = 100;
    else if (r < 0)
        TIM1->CCR3 = 0;
    else
        TIM1->CCR3 = r;
    //Green control
    if(g > 100)
        TIM1->CCR2 = 100;
    else if (g < 0)
        TIM1->CCR2 = 0;
    else
        TIM1->CCR2 = g;
    //Blue control
    if(b > 100)
        TIM1->CCR1 = 100;
    else if (b < 0)
        TIM1->CCR1 = 0;
    else
        TIM1->CCR1 = b;
}

// -------------------------------------------
// Section 6.3
// -------------------------------------------
// This function should enable the clock to port A, configure pins 0, 1, 2 and
// 3 as outputs (we will use these to drive the columns of the keypad).
// Configure pins 4, 5, 6 and 7 to have a pull down resistor
// (these four pins connected to the rows will being scanned
// to determine the row of a button press).
void init_keypad() {
//Enable port A clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//Clearing the modes for pins 0,1,2,3
    GPIOA->MODER &= ~GPIO_MODER_MODER0;
    GPIOA->MODER &= ~GPIO_MODER_MODER1;
    GPIOA->MODER &= ~GPIO_MODER_MODER2;
    GPIOA->MODER &= ~GPIO_MODER_MODER3;
//Setting pins 0,1,2,3 as outputs
    GPIOA->MODER |= GPIO_MODER_MODER0_0;
    GPIOA->MODER |= GPIO_MODER_MODER1_0;
    GPIOA->MODER |= GPIO_MODER_MODER2_0;
    GPIOA->MODER |= GPIO_MODER_MODER3_0;
//Clearing the modes for pins 4,5,6,7 (so that they are inputs)
    GPIOA->MODER &= ~GPIO_MODER_MODER4;
    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER &= ~GPIO_MODER_MODER6;
    GPIOA->MODER &= ~GPIO_MODER_MODER7;

//Configure pins 4,5,6,7 with pull down resistors
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_1;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR7_1;
}

// This function should,
// enable clock to timer6,
// setup pre scalar and arr so that the interrupt is triggered every
// 1ms, enable the timer 6 interrupt, and start the timer.
void setup_timer6() {
//Enabling clock to TIM6
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
//Configuring PSC and ARR for 1000 hz (once every 0.001s)
    //48,000,000 / (48 * 1,000) = 1000
    TIM6->PSC = 48-1;
    TIM6->ARR = 1000-1;
//Enable update counter
    TIM6->DIER |= TIM_DIER_UIE;
//Enabling the counter
    TIM6->CR1 |= TIM_CR1_CEN;
//Enabling TIM6 interrupt
    NVIC->ISER[0] |= 1<<TIM6_DAC_IRQn;
}

// The functionality of this subroutine is described in the lab document
int get_key_press() {
    while(1){
        for(int i = 0; i <16; i++){
            if(history[i] == 1)
                return i;
        }
    }
}

// The functionality of this subroutine is described in the lab document
int get_key_release() {
    while(1){
        for(int i = 0; i<16; i++)
            if(history[i] == -2)
                return i;
    }
}


// See lab document for the instructions as to how to fill this
void TIM6_DAC_IRQHandler() {
//Acknowledging the interrupt
    TIM6->SR &= ~TIM_SR_UIF;
    
    int row = (GPIOA->IDR >> 4) & 0xf;
    int index = col << 2;

    history[index] <<= 1;
    history[index+1] <<= 1;
    history[index+2] <<= 1;
    history[index+3] <<= 1;

    history[index] |= (row>> 0) & 0x1;
    history[index+1] |= (row>> 1) & 0x1;
    history[index+2] |= (row>> 2) & 0x1;
    history[index+3] |= (row>> 3) & 0x1;

    //Increment col, not letting it get above 3
    col = (col + 1) & 3;

    //Set the ODR to col*2
    GPIOA->ODR = (1 << col);

}

int main(void)
{
    //test_wiring();
    //prob2();
    //prob3();
    prob4();    //if I uncomment the setup_gpio in prob 4 code, I can see the output on led,
                //I know the duty cycle change works because I can see that but not sure about the frequency
                //I think I need to use the setup_gpio so that the output channels can be configured to output

    //setup_gpio();
    //setup_pwm();
    //TIM1->CCR1 = 99;
    //TIM1->CCR2 = 0;
    //TIM1->CCR3 = 0;
}
