// /**
//  ******************************************************************************
//  * @file           : main.c
//  * @author         : Auto-generated by STM32CubeIDE
//  * @brief          : Main program body
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2025 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
// /* Private includes -----------------------------------------------*/
// #include "stm32f103xb.h"
// #include "core_cm3.h"

// /* Private defines ------------------------------------------------*/

// /* Private macros -------------------------------------------------*/
// #define BIT(n)				(1UL << (n))

// #define __enable_irq() 		__asm volatile ("cpsie i" : : : "memory")
// #define __disable_irq() 	__asm volatile ("cpsid i" : : : "memory")

// /* Private typedefs ------------------------------------------------*/

// /* Private variables -----------------------------------------------*/

// /* Private prototype function --------------------------------------*/
// void SysReset_Check(void);
// void SystemClock_Config(void);
// void MX_GPIO_Init(void);
// void MX_EXTI_Init(void);
// void EXTI1_IRQHandler(void);

// void delay_ms(uint32_t ms);

// uint32_t counter = 0;

// /* Main program ----------------------------------------------------*/
// int main(void)
// {
// 	/* Check system reset */
// 	SysReset_Check();

// 	/* System Clock configuration */
// 	SystemClock_Config();

// 	/* GPIO initialization */
// 	MX_GPIO_Init();

// 	/* EXTI initialization */
// 	MX_EXTI_Init();

// 	/* Application loop forever */
// 	while (1)
// 	{
// 	    EXTI1_IRQHandler();
// 	}
// }


// /* Private function definition -------------------------------------*/
// void SysReset_Check(void)
// {
//     // reset value of RCC_CSR is 0x0C000000
// 	if( RCC->CSR & BIT(31) )
// 	{
// 		// Handle low-power reset
// 	}
// 	else if( RCC->CSR & BIT(30) )
// 	{
// 		// Handle WWDG reset
// 	}
// 	else if( RCC->CSR & BIT(29) )
// 	{
// 		// Handle IWDG reset
// 	}
// 	else if( RCC->CSR & BIT(28) )
// 	{
// 		// Handle Software reset
// 	}
// 	else if( RCC->CSR & BIT(27) )
// 	{
// 		// Handle POR/PDR reset
// 	}
// 	else if( RCC->CSR & BIT(26) )
// 	{
// 		// Handle NRST pin reset
// 	}

// 	// Clear reset flags
// 	RCC->CSR |= BIT(24);
// }

// void SystemClock_Config(void)
// {
//     /* 1. Enable HSE clock */
//     // RCC->CR |= (0x01 << 18); // Set HSEBYP (if using an external clock source)
// 	RCC->CR |= (0x01 << 16);				// Enable HSE signal
// 	while( !(RCC->CR & (0x01 << 17)) ); 	// Polling for HSE Ready

// 	/* 2. Enable pre-fetch buffer */
// 	FLASH->ACR |= (0x01 << 4);			// Enable pre-fetch
// 	while( !(FLASH->ACR & (0x01 << 5)) );// Polling for pre-fetch buffer enable

// 	/* 3. Set wait states */
// 	FLASH->ACR &= ~(0x07);			// Reset wait states
// 	FLASH->ACR |= (0x01 << 1);			// 2 wait states

// 	/* 4. Configure prescaler, multiplier and clock MUX. */
// 	// Prescalers
// 	RCC->CFGR &= ~(0x07 << 8);	// APB1CLK = HCLK/1 = 72MHz (APB1 <= 36MHz)
// 	RCC->CFGR |= (0x04 << 8);	// APB1CLK = HCLK/2 = 36MHz
// 	RCC->CFGR &= ~(0x07 << 11);	// APB2CLK = HCLK/1 = 72MHz (APB1 <= 72MHz)
// 	RCC->CFGR &= ~(0x01 << 22); 		// USBCLK = PLLCLK / 1.5 = 48MHz
// 	RCC->CFGR |= ~(0x03 << 14); // ADCCLK = HCLK / 2 = 36MHz
// 	RCC->CFGR &= ~(0x0F << 4);	// HCLK = SYSCLK / 1 = 72MHz
// 	// MUX
// 	RCC->CFGR &= ~(0x01 << 17);		// HSE not divided for PLL entry (8MHz * 1)
// 	RCC->CFGR |= (0x01 << 16);		// HSE entry on PLL clock to multiply (8MHz)
// 	RCC->CFGR &= ~(0x0F << 18); // Clear PLL multiple (PLLMUL = x2)
// 	RCC->CFGR |= (0x07 << 18);	// PLLMUL = x9 (8MHz * 9 = 72MHz)

// 	/* 5. Enable PLL*/
// 	RCC->CR |= BIT(24);
// 	while( !(RCC->CR & BIT(25)) ); // Polling for PLL locked

// 	/* 6. Set clock source */
// 	RCC->CFGR &= ~(0x03); 				// Clear SW
// 	RCC->CFGR |= BIT(1);				// SYSCLK = PLLCLK = 72MHz
// 	while (!(RCC->CFGR & (0x02 << 2) ));// Polling for system clock switch status
// }

// void MX_GPIO_Init(void)
// {
// 	// Enable clock for GPIOA, GPIOC, GPIOD peripherals
// 	RCC->APB2ENR |= BIT(2) | BIT(4) | BIT(5);

// 	// PA5 as push-pull output (2 MHz)
// 	GPIOA->CRL &= ~(0x0F << 20); // Clear MODE5 & CNF5
// 	GPIOA->CRL |= BIT(21);
// 	// general purpose output push-pull = 0x00

//    	// LED
//     GPIOC->CRH &= ~(0xF << 20); // Clear CNF13 & MODE13
//     GPIOC->CRH |= (0x01 << 20); // output mode, max speed 10 MHz
//     // GPIOC->CRH |= (0x00 << 22); // general purpose output push-pull
// }

// void MX_EXTI_Init(void)
// {
// 	// Enable EXTI clock by enabling the AFIO clock (APB2)
// 	RCC->APB2ENR |= (0x01 << 0);

// 	// PA0 -> 10 -> 0A
// 	// PB0 -> 18 -> 1A
// 	// Connect EXTI0 line to PA0/PB0 on the AFIO's EXTICRs registers
// 	AFIO->EXTICR[0] &= ~(0x0F); // Clear EXTI0
// 	AFIO->EXTICR[0] |= (0x00); // Connect EXTI0 to PA0
// 	// AFIO->EXTICR[0] |= (0x01 << 4); // Connect EXTI0 to PB0

// 	// Disable the 'rising edge' trigger on EXTI RTSR register
// 	EXTI->RTSR &= (0x00);

// 	// Enable/disable the 'falling edge' trigger on EXTI FTSR register
// 	EXTI->FTSR |= (0x00);

// 	// Setup EXTI1 line as an interrupt on EXTI IMR register

// 	/* Set priority: The priority field holds an 8-bit priority value.
// 	 * The lower the value, the greater the priority of the
// 	 * corresponding interrupt. The processor implements only bits[7:4]
// 	 * of each field, bits[3:0] read as zero and ignore writes.
// 	 *
// 	 * Although the NVIC_IPx are 32-bit registers (x = 0,...,16)
// 	 * the library define these registers as an array of 8-bit
// 	 * registers, where the location of the interrupt source is the
// 	 * index on the array. In this case the location of EXTI1 in the
// 	 * interrupt vector table is 7.
// 	*/

// 	/* Enable EXTI1 interrupt: NVIC ISER1-ISER3 are read-set register, i.e.,
// 	 * writing zero does not take effect on the register value.
// 	 * Location	Set-enable	Clear-enable	Set-pending	Clear-pending	Active Bit
// 	 * 0-31 	ISER[0] 	ICER[0] I		SPR[0] 		ICPR[0] 		IABR[0]
// 	 * 32-63 	ISER[1] 	ICER[1] 		ISPR[1] 	ICPR[1] 		IABR[1]
// 	 * 64-80 	ISER[2] 	ICER[2] 		ISPR[2] 	ICPR[2] 		IABR[2]
// 	 *
// 	 * Since EXTI1 is located at position 7, to enable its interrupt
// 	 * we need to set bit 7 of ISER[0].
// 	 */
// 	// EXTI->ISER[0] |= (0x01 << 6); // Enable EXTI1 interrupt
// 	// for the practice, set EXIT1 instead of EXTI0
// 	NVIC->ISER[0] |= (0x01 << 7); // Enable EXTI1 interrupt
// }


// void delay_ms(uint32_t ms)
// {
// 	// Declared as volatile to avoid compiler optimization
// 	volatile uint32_t cycles = 0;
// 	while(ms--)
// 	{
// 		/*
// 		 * This for loop takes 10 cycles per iteration
// 		 * The outer while takes 4 cycles per iteration
// 		 * Ideally, for a delay of 1ms we need a for loop
// 		 * from 0 to Fclk / 1000 clock cycles if we assume
// 		 * that each iteration takes 1 clock cycle.
// 		 *
// 		 * If Fclk = 8MHz -> 8MHz/1000 = 8000
// 		 *
// 		 * However, at low-level, each iteration of the for
// 		 * loop takes around 10 clock cycles, therefore
// 		 * instead of iterating up to Fclk / 1000, it
// 		 * should be Fclk / (1000*CYCLES_PER_ITER), i.e.,
// 		 * 8MHz/(1000*10) = 800
// 		 */
// 		for (cycles = 0; cycles < 800; cycles++)
// 		{
// 		}
// 	}
// }




// /* Interrupt Handlers ------------------------------------------------*/
// void EXTI1_IRQHandler(void)
// {
// 	// Check EXTI1 pending flag
// 	if (EXTI->PR & BIT(0))
// 	{
// 		// Clear the EXTI interrupt status
// 		EXTI->PR = BIT(0);

//         GPIOC->ODR ^= (1 << 13);
//         delay_ms(10000);

// 		counter++;
// 	} else {
//        	GPIOC->ODR ^= (1 << 13);
//         delay_ms(1000);
// 	}
// }
