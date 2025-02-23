/*
* clock_configuration.c
*
* Author: nielsouvrard
*/

#include "stm32f103xb.h"
#include "clock_configuration.h"

void set_clk_speed(spped_clock speed, bool is_HSE)
{
	// Clear
	RCC->CFGR &= ~(0x03); // Clear SW
	RCC->CR &= ~(0x01); // Clear HSION
	RCC->CR &= ~(0x01 << 16); // Clear HSEON
	RCC->CR &= ~(0x01 << 24); // Clear PLLON
	FLASH->ACR &= ~(0x07); // Clear wait states
	// RCC->CFGR &= ~(0x07 << 8); // Clear APB1 prescaler
	// RCC->CFGR &= ~(0x07 << 11); // Clear APB2 prescaler
	// RCC->CFGR &= ~(0x03 << 14); // Clear ADC prescaler
	// RCC->CFGR &= ~(0x01 << 22); // Clear USB prescaler
	// RCC->CFGR &= ~(0x03 << 24); // Clear MCO

	if (speed <= CLOCK_24MHz) {
		// state 0 by default, do nothing
	} else {
		if (speed <= CLOCK_48MHz) {
			FLASH->ACR |= (0x01); // 1 wait states
		} else {
			FLASH->ACR |= (0x02); // 2 wait states
		}
		FLASH->ACR |= (0x01 << 4); // Enable pre-fetch
		while (!(FLASH->ACR & (0x01 << 5)));// Polling for pre-fetch buffer enable
	}

	if (is_HSE) {
		RCC->CR |= (0x01 << 16); // HSE ON
		while (!(RCC->CR & (0x01 << 17))); // HSERDY flag polling

		if (speed == CLOCK_8MHz) {
			RCC->CFGR |= 0x01; // SW = 1, HSE
			while (!(RCC->CFGR & (0x01 << 2))); // Polling SW status
			return;
		}
	} else {
		RCC->CR |= (0x01); // HSI ON
		while (!(RCC->CR & (0x01 << 1))); // HSIRDY flag polling

		if (speed == CLOCK_8MHz) {
			// SW = 0
			// No polling SW status needed
			return;
		}
	}
	RCC->CFGR |= 0x02; // SW = 2, PLL
	while (!(RCC->CFGR & (0x02 << 2))); // Polling SW status

	RCC->CR |= (0x01 << 24); // PLL ON
	while (!(RCC->CR & (0x01 << 25))); // Polling PLLRDY flag

	// Clear
	RCC->CFGR &= ~(0x01 << 16); // Clear PLLSRC
	RCC->CFGR &= ~(0x01 << 17); // Clear PLLXTPRE
	RCC->CFGR &= ~(0x0F << 18); // Clear PLL multiple
	

	if (is_HSE) {
		RCC->CFGR |= (0x01 << 16); // PLLSRC = HSE
		if (speed % 2) {
			// if speed is odd, divide by 2 the HSE
			RCC->CFGR |= (0x01 << 17); // PLLXTPRE on so we divide by 2 the HSE
		} else {
			speed = speed / 2 - 1; // 8MHz begin so / 2, and -1 because PLLMUL begins at 2
		}
		RCC->CFGR |= (speed << 18); // PLLMUL
	} else {
		// PLLSRC = 0, HSI by default
		RCC->CFGR |= (speed << 18); // PLLMUL
	}
}
