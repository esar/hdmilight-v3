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

#define POWER_LED_PORT       GPIOC
#define POWER_LED_PIN        1
#define POWER_1V8_PORT       GPIOC
#define POWER_1V8_PIN        2
#define POWER_1V2_PORT       GPIOC
#define POWER_1V2_PIN        3



void powerLEDOn()
{
	// LED pin output high
	POWER_LED_PORT->BSRR |= 1 << POWER_LED_PIN;
}

void powerLEDOff()
{
	// LED pin output low
	POWER_LED_PORT->BRR |= 1 << POWER_LED_PIN;
}

void power1V8On()
{
	// 1V8 pin output high
	POWER_1V8_PORT->BSRR |= 1 << POWER_1V8_PIN;
}

void power1V8Off()
{
	// 1V8 pin output low
	POWER_1V8_PORT->BRR |= 1 << POWER_1V8_PIN;
}

void power1V2On()
{
	// 1V2 pin output high
	POWER_1V2_PORT->BSRR |= 1 << POWER_1V2_PIN;
}

void power1V2Off()
{
	// 1V2 pin output low
	POWER_1V2_PORT->BRR |= 1 << POWER_1V2_PIN;
}

void powerInit()
{
	// Enable GPIOC
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Set LED power to output low
	POWER_LED_PORT->BRR   |= 1 << POWER_LED_PIN;
	POWER_LED_PORT->MODER |= 1 << (POWER_LED_PIN * 2);

	// Set vreg power pins to open-drain output high
	POWER_1V8_PORT->OTYPER |= 1 << POWER_1V8_PIN;
	POWER_1V8_PORT->BSRR   |= 1 << POWER_1V8_PIN;
	POWER_1V8_PORT->MODER  |= 1 << (POWER_1V8_PIN * 2);

	POWER_1V2_PORT->OTYPER |= 1 << POWER_1V2_PIN;
	POWER_1V2_PORT->BSRR   |= 1 << POWER_1V2_PIN;
	POWER_1V2_PORT->MODER  |= 1 << (POWER_1V2_PIN * 2);
}

