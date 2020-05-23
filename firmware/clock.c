#include <stdint.h>
#include <stm32f303x8.h>
#include <core_cm4.h>
#include "system_stm32f3xx.h"
#include "clock.h"


volatile uint32_t g_ticks = 0;

void SysTick_Handler()
{
	g_ticks += 10;
}

void clockInit()
{
	// Set flash latency to 2 wait states
	// Needed for higher clock frequencies
	FLASH->ACR |= 2;

	// Turn off PLL
	RCC->CR &= ~RCC_CR_PLLON;

	// Configure PLL, HSI/2 * 14 = 56MHz
	RCC->CFGR &= ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC);
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL14;

	// Configure Prediv = 2
	RCC->CFGR2 |= 1;

	// Turn on the PLL and wait for stability
	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0)
		;

	// Set PLL as clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0)
		;

	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 100);
	__enable_irq();
}

uint32_t clockGetTicks()
{
	return g_ticks;
}
