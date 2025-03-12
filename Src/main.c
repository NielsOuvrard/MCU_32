/*
 * main.c
 *
 *  Created on: Feb 23, 2025
 *      Author: nielsouvrard
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 16
#define NUMBER_LINES 2
#define NUMBER_GPIO 4

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    char *name;
} GPIO_Config;

GPIO_Config gpios[] = {
    {GPIOC, 13, "LED"},
    {GPIOA, 11, "Nothing"},
	{GPIOA, 12, "Nothing"},
	{GPIOA, 15, "Nothing"}
};

char screen_buffer[32] = {0};
uint8_t index_display = 0;

void write_one_gpio(GPIO_Config *gpio, bool second_line, uint8_t index)
{
	char buffer[SCREEN_WIDTH] = {0};
	sprintf(buffer, "Output %d: %s", index, (gpio->port->ODR & (1 << gpio->pin)) ? "ON" : "OFF");
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		if (screen_buffer[i + (second_line * SCREEN_WIDTH)] != buffer[i]) {
			LCD_Goto_XY(i, second_line);
			LCD_Write(buffer[i] == 0 ? ' ' : buffer[i], 0);
		}
	}
	sprintf(screen_buffer + (second_line * SCREEN_WIDTH), buffer);
}

void write_outputs(void)
{
	for (int i = 0; i < NUMBER_LINES; i++) {
		write_one_gpio(&gpios[(index_display + i) % NUMBER_GPIO], i % 2, ((index_display + i) % NUMBER_GPIO) + 1);
	}
}

// Button
// 400ms -> move the menu
// 2s -> change the value
// each output should be a led or a motor

int main(void)
{
	RCC->APB2ENR |= (0x01 << 4); // Enable GPIOC clock
	GPIOC->CRH &= ~(0xF << 20); // Clear CNF13 & MODE13
    GPIOC->CRH |= (0x01 << 20); // output mode, max speed 10 MHz

	set_clk_speed(CLOCK_8MHz, true);
    change_clk(8);
    LCD_Init(LCD_8B_INTERFACE);

	int loop = 1;

	/* Application loop forever */
	while (1)
	{
		write_outputs();
	
		if (loop % 10 == 0) {
			index_display += 1;
		}

		GPIOC->ODR ^= (1 << 13);
		delay_ms(1000);
		loop++;
	}
}
