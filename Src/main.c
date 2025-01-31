/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
// ctrl + space = get library help

// RCC
#define RCC_BASE 0x40021000

// GPIO
#define GPIOA_BASE 0x40010800
#define GPIOB_BASE 0x40010C00
#define GPIOC_BASE 0x40011000
#define GPIOD_BASE 0x40011400
#define GPIOE_BASE 0x40011800
#define GPIOF_BASE 0x40011C00
#define GPIOG_BASE 0x40012000

// ADC
#define ADC1_BASE 0x40012400
#define ADC2_BASE 0x40012800

// TIM
#define TIM1_BASE 0x40012C00
#define TIM2_BASE 0x40000000

// I2C
#define I2C2_BASE 0x40005800
#define I2C1_BASE 0x40005400

// UART
#define UART5_BASE 0x40005000
#define UART4_BASE 0x40004C00
#define USART3_BASE 0x40004800
#define USART2_BASE 0x40004400

// I2S
#define SPI3_BASE 0x40003C00
#define SPI2_BASE 0x40003800

// Real time clock
#define RTC_BASE 0x40002800

#define GPIO_CRL_OFFSET 0x00
#define GPIO_ODR_OFFSET 0x0c

#define GPIOB_CRL *(volatile uint32_t *) (GPIOB_BASE + GPIO_CRL_OFFSET)

#define GPIOC_CRL *(volatile uint32_t *) (GPIOC_BASE + 0x00)
#define GPIOC_CRH *(volatile uint32_t *) (GPIOC_BASE + 0x04)
#define GPIOC_IDR *(volatile uint32_t *) (GPIOC_BASE + 0x08)
#define GPIOC_ODR *(volatile uint32_t *) (GPIOC_BASE + 0x0C)
#define GPIOC_BSR *(volatile uint32_t *) (GPIOC_BASE + 0x10)
#define GPIOC_BRR *(volatile uint32_t *) (GPIOC_BASE + 0x14)
#define GPIOC_LCKR *(volatile uint32_t *) (GPIOC_BASE + 0x18)

#define RCC_APB2ENR *(volatile uint32_t *)(RCC_BASE + 0x18)
#define RCC_APB1ENR *(volatile uint32_t *)(RCC_BASE + 0x1C)

#define MODE_INPUT 00
#define MODE_OUTPUT_10MHZ 01
#define MODE_OUTPUT_2MHZ 10
#define MODE_OUTPUT_50MHZ 11

// Output type
#define CNF_OUTPUT_PUSHPULL 00
#define CNF_OUTPUT_OPENDRAIN 01
#define CNF_AF_PUSHPULL 10
#define CNF_AF_OPENDRAIN 11

// Input type
#define CNF_INPUT_ANALOG 00
#define CNF_INPUT_FLOATING 01
#define CNF_INPUT_PULLUP_DOWN 10
#define CNF_RESERVED 11

int main(void)
{
    // enable clock for GPIOC & GPIOB (see ENABLING CLOCK)
    // GPIOC = 0x10
    // GPIOB = 0x08
    RCC_APB2ENR |= 0x18;

    // clean and set the mode of the pin 13 of the GPIOC
    GPIOC_CRH &= ~(0x3 << 20);
    GPIOC_CRH |= (MODE_OUTPUT_50MHZ << 20); // with & door, we can only put at 0

    // clean and set the output type of the pin 13 of the GPIOC
    GPIOC_CRH &= ~(0x3 << 22);
    GPIOC_CRH |= (CNF_OUTPUT_OPENDRAIN << 22);

    //...
    GPIOB_CRL &= ~(0x3 << 8);
    GPIOB_CRL |= (MODE_INPUT << 8);
    GPIOB_CRL &= ~(0x3 << 10);
    GPIOB_CRL |= (CNF_INPUT_PULLUP_DOWN << 10);

    /* Loop forever */
    while (1);
}
