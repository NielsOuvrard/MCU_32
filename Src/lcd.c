/*
 * lcd.c
 *
 *  Created on: Feb 20, 2025
 *      Author: User123
 */


/* Private includes -----------------------------------------------*/
#include "stm32f103xb.h"

/* Private defines ------------------------------------------------*/

/* Private macros -------------------------------------------------*/
#define BIT(n)				(1UL << (n))
#define __NOP() __asm volatile ("nop")  // Lasts 1 clock cycle of the STM32f103

/* Private typedefs ------------------------------------------------*/

/* Private variables -----------------------------------------------*/

/* Private prototype function --------------------------------------*/


void delay_ms(uint32_t ms)
{
    // Declared as volatile to avoid compiler optimization
    volatile uint32_t cycles = 0;
    while(ms--)
    {
        /*
         * This for loop takes 10 cycles per iteration
         * The outer while takes 4 cycles per iteration
         * Ideally, for a delay of 1ms we need a for loop
         * from 0 to Fclk / 1000 clock cycles if we assume
         * that each iteration takes 1 clock cycle.
         *
         * If Fclk = 8MHz -> 8MHz/1000 = 8000
         *
         * However, at low-level, each iteration of the for
         * loop takes around 10 clock cycles, therefore
         * instead of iterating up to Fclk / 1000, it
         * should be Fclk / (1000*CYCLES_PER_ITER), i.e.,
         * 8MHz/(1000*10) = 800
         */
        for (cycles = 0; cycles < 800; cycles++)
        {
        }
    }
}

/* Exported reference function -------------------------------------*/
void LCD_Init(uint8_t dbWidth)
{
	// Configure GPIOs ============
	RCC->APB2ENR |= BIT(2); // Enable GPIOA clock
	RCC->APB2ENR |= BIT(3); // Enable GPIOB clock

	// PB5 - PB7, output mode, max speed 10 MHz, push-pull
	GPIOB->CRL &= ~(0xFFF); // Clear CNF5, MODE5, CNF6, MODE6, CNF7, MODE7
	GPIOB->CRL |= (0b01 << 20); // output mode, max speed 10 MHz, 5
	GPIOB->CRL |= (0b11 << 24); // output mode, max speed 10 MHz, 6
	GPIOB->CRL |= (0b11 << 28); // output mode, max speed 10 MHz, 7

	// PA0 - PA7, output mode, max speed 10 MHz, push-pull
	GPIOA->CRL &= ~(0xFFFFFFFF); // Clear CNF0, MODE0, CNF1, MODE1, CNF2, MODE2, CNF3, MODE3
	GPIOB->CRL |= (0b01 << 0); // output mode, max speed 10 MHz, 0
	GPIOB->CRL |= (0b11 << 4); // output mode, max speed 10 MHz, 1
	GPIOB->CRL |= (0b11 << 8); // output mode, max speed 10 MHz, 2
	GPIOB->CRL |= (0b11 << 12); // output mode, max speed 10 MHz, 3
	GPIOB->CRL |= (0b11 << 10); // output mode, max speed 10 MHz, 4
	GPIOB->CRL |= (0b11 << 14); // output mode, max speed 10 MHz, 5
	GPIOB->CRL |= (0b11 << 18); // output mode, max speed 10 MHz, 6
	GPIOB->CRL |= (0b11 << 22); // output mode, max speed 10 MHz, 7

	// Perform init sequence: see https://cdn.sparkfun.com/assets/9/5/f/7/b/HD44780.pdf
	// See pag.
	if(dbWidth == LCD_8B_INTERFACE)
	{
		// 8-bit interface: see pag. 22, and 45
		// rise VCC to 4.5V, wait 40ms
		delay_ms(40);

		// RS = 0, R/W = 0, DB7 = 0, DB6 = 0, DB5 = 1, DB4 = 1...
		// B7 = 0, B6 = 0,  A7 = 0,  A6 = 0,  A5 = 1,  A4 = 1
		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
		GPIOA->ODR &= ~(0x01 << 7); // DB7 = 0
		GPIOA->ODR &= ~(0x01 << 6); // DB6 = 0
		GPIOA->ODR |= (0x01 << 5); // DB5 = 1
		GPIOA->ODR |= (0x01 << 4); // DB4 = 1
		GPIOA->ODR |= (0x01 << 5); // DB5 = 1
		GPIOA->ODR |= (0x01 << 4); // DB4 = 1
		GPIOA->ODR &= ~(0x01 << 3); // DB3 = 0
		GPIOA->ODR &= ~(0x01 << 2); // DB2 = 0
		GPIOA->ODR &= ~(0x01 << 1); // DB1 = 0
		GPIOA->ODR &= ~(0x01 << 0); // DB0 = 0


		// put E for 1 sec
		GPIOB->ODR |= (0x01 << 5); // E = 1

		// Wait for more than 4.1 ms
		delay_ms(5);

		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
		GPIOA->ODR &= ~(0x01 << 7); // DB7 = 0
		GPIOA->ODR &= ~(0x01 << 6); // DB6 = 0
		GPIOA->ODR |= (0x01 << 5); // DB5 = 1
		GPIOA->ODR |= (0x01 << 4); // DB4 = 1
		GPIOA->ODR &= ~(0x01 << 3); // DB3 = 0
		GPIOA->ODR &= ~(0x01 << 2); // DB2 = 0
		GPIOA->ODR &= ~(0x01 << 1); // DB1 = 0
		GPIOA->ODR &= ~(0x01 << 0); // DB0 = 0

		// Wait for more than 100 µs
		delay_ms(1);

		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
		GPIOA->ODR &= ~(0x01 << 7); // DB7 = 0
		GPIOA->ODR &= ~(0x01 << 6); // DB6 = 0
		GPIOA->ODR |= (0x01 << 5); // DB5 = 1
		GPIOA->ODR |= (0x01 << 4); // DB4 = 1
		GPIOA->ODR &= ~(0x01 << 3); // DB3 = 0
		GPIOA->ODR &= ~(0x01 << 2); // DB2 = 0
		GPIOA->ODR &= ~(0x01 << 1); // DB1 = 0
		GPIOA->ODR &= ~(0x01 << 0); // DB0 = 0

	}
	else if(dbWidth == LCD_4B_INTERFACE)
	{
		// 4-bit interface: see pag. 22, and 46
	}
}

// Write sequence
void LCD_Write(uint8_t data, uint8_t isCmd)
{
	// Write mode (RW = 0)

	// Write data/instruction (RS = 1/0)

	// Configure DB pins as outputs

	// Write data into DB pins

	// Send EN pulse: see Bus Timing Characteristics on pag. 49

	// Set DB pins to high
}

// Read sequence
uint8_t LCD_Read(uint8_t isData)
{
	uint8_t dout = 0x00;

	// Read mode (RW = 1)

	// Read data/busy&DDRAM (RW = 1/0)

	// Configure DB pins as floating inputs

	// Pull EN to HIGH: see Bus Timing Characteristics on pag. 49

	// Read data from DB pins

	// Pull EN to LOW: see Bus Timing Characteristics on pag. 49

	// Set DB pins to high

	return dout;
}

/* Private reference function --------------------------------------*/

/* Main program ----------------------------------------------------*/
int main(void)
{
    LCD_Init();


	/* Application loop forever */
	while (1)
	{
	}
}
