/*
 * main.c
 *
 *  Created on: Feb 23, 2025
 *      Author: nielsouvrard
 */

#include "main.h"

int mhz_clk = 8; // by default, 8MHz

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
         * If Fclk = XMHz -> XMHz/1000 = X000
         *
         * However, at low-level, each iteration of the for
         * loop takes around 10 clock cycles, therefore
         * instead of iterating up to Fclk / 1000, it
         * should be Fclk / (1000*CYCLES_PER_ITER), i.e.,
         * XMHz/(1000*10) = X00
         */
        for (cycles = 0; cycles < mhz_clk * 100; cycles++)
        {
        }
    }
}


int main(void)
{
	RCC->APB2ENR |= BIT(4);
	GPIOC->CRH &= ~(0xF << 20); // Clear CNF13 & MODE13
    GPIOC->CRH |= (0x01 << 20); // output mode, max speed 10 MHz

	set_clk_speed(CLOCK_72MHz, true);
    mhz_clk = 72;
    LCD_Init(LCD_8B_INTERFACE);


	/* Application loop forever */
	while (1)
	{
		GPIOC->ODR ^= (1 << 13);
		delay_ms(1000);
	}
}
