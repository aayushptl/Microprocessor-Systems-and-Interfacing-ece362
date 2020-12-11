#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>

//Added from support.c
#define SPI_DELAY 1337

// These are function pointers.  They can be called like functions
// after you set them to point to other functions.
// e.g.  cmd = bitbang_cmd;
// They will be set by the stepX() subroutines to point to the new
// subroutines you write below.
void (*cmd)(char b) = 0;
void (*data)(char b) = 0;
void (*display1)(const char *) = 0;
void (*display2)(const char *) = 0;

int col = 0;
int8_t history[16] = {0};
int8_t lookup[16] = {1,4,7,0xe,2,5,8,0,3,6,9,0xf,0xa,0xb,0xc,0xd};
char char_lookup[16] = {'1','4','7','*','2','5','8','0','3','6','9','#','A','B','C','D'};
extern int count;

// Prototypes for subroutines in support.c
void generic_lcd_startup(void);
void countdown(void);
void step1(void);
void step2(void);
void step3(void);
void step4(void);
void step6(void);

// This array will be used with dma_display1() and dma_display2() to mix
// commands that set the cursor location at zero and 64 with characters.
//
uint16_t dispmem[34] = {
        0x080 + 0,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x080 + 64,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
};

//=========================================================================
// Subroutines for step 2.
//=========================================================================
void spi_cmd(char b) {
    //Send command sequence
    while((SPI2->SR & SPI_SR_TXE) == 0);
    SPI2->DR = b;
    //*(uint8_t *) &(SPI2->DR) = b;
}

void spi_data(char b) {
    while((SPI2->SR & SPI_SR_TXE) == 0);
    SPI2->DR = 0x200 + b;
}

void spi_init_lcd(void) {
//Output enabling
    //Enabling clock to port b
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  //Setting pin 12 (my NSS) to high
    //GPIOB->BSRR = 1<<12; // set NSS high
  //Resetting corresponding ODR bits (13 and 15)
    //GPIOB->BRR = (1<<13) + (1<<15); // set SCK and MOSI low

    //** THESE ARE THE INCORRECT ALTERNATE FUNCTION VALUES, they're actually for analog I believe**//

    // Now configure pins for alternate functions.
    GPIOB->MODER |= (GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER15);
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFRH3;
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFRH4;
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFRH6;
//SPI enabling
    //Enabling clock to SPI2
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    //Bidirectional data mode and output enable in this mode
    SPI2->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
    //Master config and dividing clock by max amount
    SPI2->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR;
    //SS output enable, NSS pulse management, DS = 1001 (9+1 = 10)
    SPI2->CR2 = SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3| SPI_CR2_DS_0;
    //peripheral enables
    SPI2->CR1 |= SPI_CR1_SPE;
//init_lcd stuff
    generic_lcd_startup();
}

//===========================================================================
// Subroutines for step 3.
//===========================================================================

// Display a string on line 1 using DMA.
void dma_display1(const char *s) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    DMA1_Channel1 -> CCR &= ~DMA_CCR_EN;
    DMA1_Channel1 -> CMAR = (uint32_t) s;
    DMA1_Channel1 -> CPAR = (uint32_t) where_its_going;

    DMA1_Channel1 -> CNDTR = sizeof(s);
    DMA1_Channel1 -> CCR |= DMA_CCR_DIR;
    DMA1_Channel1 -> CCR |= DMA_CCR_TCIE;
    DMA1_Channel1 -> CCR &= ~DMA_CCR_HTIE;
    DMA1_Channel1 -> CCR &= ~DMA_CCR_MSIZE;
    DMA1_Channel1 -> CCR &= ~DMA_CCR_PSIZE;
    DMA1_Channel1 -> CCR |= DMA_CCR_MINC;
    DMA1_Channel1 -> CCR |= DMA_CCR_PINC;

    DMA1_Channel1 -> CCR |= probably not mem to mem;

    DMA1_Channel1 -> CCR &= ~DMA_CCR_PL;
    NVIC->ISER[0] = 1 <<DMA1_Channel1;
    DMA1_Channel1 -> CCR |= DMA_CCR_EN;


}

//===========================================================================
// Subroutines for Step 4.
//===========================================================================

void dma_spi_init_lcd(void) {
    // Your code goes here.
}

// Display a string on line 1 by copying a string into the
// memory region circularly moved into the display by DMA.
void circdma_display1(const char *s) {
    // Your code goes here.
}

//===========================================================================
// Display a string on line 2 by copying a string into the
// memory region circularly moved into the display by DMA.
void circdma_display2(const char *s) {
    // Your code goes here.
}

//===========================================================================
// Subroutines for Step 6.
//===========================================================================
void init_keypad() {
    // Your code goes here.
}

void init_tim6(void) {
    // Your code goes here.
}

void TIM6_DAC_IRQHandler() {
    // Your code goes here.
}

int main(void)
{
    //step1();
    step2();
    //step3();
    //step4();
    //step6();
}
