/*
 * lcd.c
 *
 *  Created on: Feb 20, 2025
 *      Author: nielsouvrard
 */


#include "lcd.h"

#define BIT(n)				(1UL << (n))
#define __NOP() __asm volatile ("nop")  // Lasts 1 clock cycle of the STM32f103

static void LCD_trigger_enable_pin(void)
{
	// put E for 1ms
	GPIOB->ODR |= (0x01 << 5); // E = 1

	__NOP();
	__NOP();
	__NOP();
	__NOP();

	__NOP();
	__NOP();
	__NOP();
	__NOP();
	
	GPIOB->ODR &= ~(0x01 << 5); // E = 0
}

void LCD_Init(uint8_t dbWidth)
{
	// Configure GPIOs ============	RCC->APB2ENR |= BIT(2) | BIT(3); // Enable GPIOA and GPIOB clock

	// PB5 - PB7, output mode, max speed 2 MHz, push-pull
	GPIOB->CRL &= ~(0xFFF << 20); // Clear CNF5, MODE5, CNF6, MODE6, CNF7, MODE7
	GPIOB->CRL |= (0x222 << 20);  // Set MODE5, MODE6, MODE7 to output mode, max speed 2 MHz

	// PA0 - PA7, output mode, max speed 2 MHz, push-pull
	GPIOA->CRL &= ~(0xFFFFFFFF);  // Clear CNF0, MODE0, CNF1, MODE1, CNF2, MODE2, CNF3, MODE3, CNF4, MODE4, CNF5, MODE5, CNF6, MODE6, CNF7, MODE7
	GPIOA->CRL |= 0x22222222;     // Set MODE0, MODE1, MODE2, MODE3, MODE4, MODE5, MODE6, MODE7 to output mode, max speed 2 MHz

	// Perform init sequence: see https://cdn.sparkfun.com/assets/9/5/f/7/b/HD44780.pdf
	// See pag.
	if(dbWidth == LCD_8B_INTERFACE)
	{
		// 8-bit interface: see pag. 22, and 45
		// rise VCC to 4.5V, wait 40ms
		delay_ms(50);

		// RS = 0, R/W = 0, DB7 = 0, DB6 = 0, DB5 = 1, DB4 = 1...
		// B7 = 0, B6 = 0,  A7 = 0,  A6 = 0,  A5 = 1,  A4 = 1
		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
        // PB5 -> E
		GPIOA->ODR = (GPIOA->ODR & ~(0xFF)) | (LCD_CMD_FUNCTION_SET | LCD_8B_INTERFACE);

		LCD_trigger_enable_pin();
	
		// Wait for more than 4.1 ms
		delay_ms(5);

		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
		GPIOA->ODR = (GPIOA->ODR & ~(0xFF)) | (LCD_CMD_FUNCTION_SET | LCD_8B_INTERFACE);

		LCD_trigger_enable_pin();

		// Wait for more than 100 µs
		delay_ms(1);

		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
	
		GPIOA->ODR = (GPIOA->ODR & ~(0xFF)) | (LCD_CMD_FUNCTION_SET | LCD_8B_INTERFACE);

		LCD_trigger_enable_pin();

		// Wait for more than 100 µs
		delay_ms(1);


        // *  NOW THE SETTINGS
        
        // LCD_Write(0x38, 1);

    
		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0

		GPIOA->ODR = (GPIOA->ODR & ~(0xFF)) | (LCD_TWO_LINES | LCD_5X8_FONT);
		LCD_trigger_enable_pin();

        
		// Wait for more than 100 µs
		delay_ms(1);
        
		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
		GPIOA->ODR = (GPIOA->ODR & ~(0xFF)) | (LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_CURSOR_NO_BLINK);

        
		LCD_trigger_enable_pin();

        
		// Wait for more than 100 µs
		delay_ms(1);
        
		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
		GPIOA->ODR = (GPIOA->ODR & ~(0xFF)) | (LCD_CMD_CLEAR_DISP);

        
		LCD_trigger_enable_pin();

        
		// Wait for more than 100 µs
		delay_ms(1);
        
		GPIOB->ODR &= ~(0x01 << 7); // RS = 0
		GPIOB->ODR &= ~(0x01 << 6); // R/W = 0
		GPIOA->ODR = (GPIOA->ODR & ~(0xFF)) | (LCD_CMD_RETURN_HOME | LCD_CURSOR_MOVE);

		LCD_trigger_enable_pin();

		// Wait for more than 100 µs
		delay_ms(1);

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
	GPIOB->ODR |= (0x01 << 7); // RS = 0
	GPIOB->ODR &= ~(0x01 << 6); // R/W = 0

	GPIOA->ODR &= ~(0xFF);
	GPIOA->ODR |= (0x30);

	LCD_trigger_enable_pin();

	
	// Wait for more than 100 µs
	delay_ms(1);


	GPIOB->ODR |= (0x01 << 7); // RS = 0
	GPIOB->ODR &= ~(0x01 << 6); // R/W = 0

	GPIOA->ODR &= ~(0xFF);
	GPIOA->ODR |= (data);

	LCD_trigger_enable_pin();

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


void LCD_Goto_XY (int x, int y)
{
    // RS 0
    // RW 0
    GPIOB->ODR &= ~(0x01 << 7);
    GPIOB->ODR &= ~(0x01 << 6);

    // DB7 1
    // adress to the cursor (0x00 -> 0x0F) (0x40 -> 0x4F)
    uint8_t value = 0b10000000;
    value &= x;
    if (y) {
        value &= 0x40;
    }

}

void LCD_Print(const char *str)
{

}
