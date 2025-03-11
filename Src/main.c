/*
 * main.c
 *
 *  Created on: Feb 23, 2025
 *      Author: nielsouvrard
 */

#include "main.h"

int main(void)
{
	RCC->APB2ENR |= (0x01 << 4); // Enable GPIOC clock
	GPIOC->CRH &= ~(0xF << 20); // Clear CNF13 & MODE13
    GPIOC->CRH |= (0x01 << 20); // output mode, max speed 10 MHz

	set_clk_speed(CLOCK_8MHz, true);
    change_clk(8);
    LCD_Init(LCD_8B_INTERFACE);

	LCD_Print("Hello, World!");
	LCD_Print("Thixxxxxx");


	/* Application loop forever */
	while (1)
	{
		GPIOC->ODR ^= (1 << 13);
		delay_ms(1000);
	}
}
