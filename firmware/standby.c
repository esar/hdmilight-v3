/*

   Copyright (C) 2020 Stephen Robinson
  
   This file is part of HDMI-Light
  
   HDMI-Light is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.
  
   HDMI-Light is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this code (see the file names COPING).  
   If not, see <http://www.gnu.org/licenses/>.
  
*/

#include <stm32f303x8.h>
#include <core_cm4.h>
#include "printf.h"
#include "ambilight.h"


static int g_enterStandby = 0;


void EXTI0_IRQHandler()
{
	g_enterStandby = 1;
	EXTI->PR |= EXTI_PR_PR0;
}

void standbyPoll()
{
	if(g_enterStandby)
	{
		volatile int x;
		printf("standby...\n");
		for(x = 0; x < 1000000; ++x)
			;

		suspend();

		__disable_irq();

		// Disable system clock
		SysTick->CTRL = 0;

		// Clear all interrupts
		EXTI->PR = 0xFFFFFFFF;

		PWR->CR |= PWR_CR_CWUF;            // Clear the WUF flag after 2 clock cycles
		PWR->CR &= ~( PWR_CR_PDDS );       // Enter stop mode when the CPU enters deepsleep
		SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // Low-power mode = stop mode
		__WFI();                           // Enter low-power mode

		clockInit();
		__enable_irq();

		for(x = 0; x < 1000000; ++x)
			;
		printf("resuming\n");

		resume();

		g_enterStandby = 0;
	}
}

void standbyInit()
{
	
	// Enable GPIOA for standby button
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Ensure all pins start in suspended state
	suspend();

	// Standby button is input
	GPIOA->MODER &= ~GPIO_MODER_MODER0;

	// Connect EXTI0 to GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0_Msk;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;

	EXTI->IMR |= 1 << 0;            // enable EXTI0
	EXTI->RTSR |= EXTI_RTSR_TR0;    // enable rising edge
	EXTI->FTSR &= ~EXTI_FTSR_TR0;   // disable falling edge

	NVIC_EnableIRQ(EXTI0_IRQn);

	resume();
}

