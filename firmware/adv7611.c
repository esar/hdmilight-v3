/*

   Copyright (C) 2019 Stephen Robinson
  
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

#include <stdint.h>
#include <stm32f303xe.h>
#include <core_cm4.h>
#include "printf.h"
#include "ambilight.h"
#include "config_hdmi.h"
#include "config_edid.h"

#define RESET_ADV7611_PORT   GPIOA
#define RESET_ADV7611_PIN    8


static void adv7611ResetOn()
{
	// ADV7611 reset pin is low output (pull down)
	RESET_ADV7611_PORT->BRR |= 1 << RESET_ADV7611_PIN;
	RESET_ADV7611_PORT->MODER |= 1 << (RESET_ADV7611_PIN * 2);
}

static void adv7611ResetOff()
{
	// ADV7611 reset pin is input (float high)
	RESET_ADV7611_PORT->MODER &= ~(3 << (RESET_ADV7611_PIN * 2));
}

static void adv7611WriteEdid(const char* edid, int length)
{
	int i;

	for(i = 0; i < length; ++i)
		adv7611WriteRegister(0x6c, i, edid[i]);
}

static void adv7611WriteConfig(const struct ConfigTable* table)
{
	const struct ConfigTable* p;

	for(p = table; p->address != 0; ++p)
	{
		uint8_t ack = adv7611WriteRegister(p->address, p->subaddress, p->data);
		if(!silent && !ack)
			printf("%x %x %x : %s\n", p->address, p->subaddress, p->data, ack ? "ACK" : "NACK");
	}
}

void adv7611Init()
{
	// Configure pins
	// PA9  = SCL
	// PA10 = SDA

	// Enable GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Set pins mode to alternate function
	GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
	GPIOA->MODER |= (2 << GPIO_MODER_MODER9_Pos) | (2 << GPIO_MODER_MODER10_Pos);

	// Set alternate function to I2C2
	// PA9  = I2C2_SCL = Alt Func 4
	// PA10 = I2C2_SDA = Alt Func 4
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH1 | GPIO_AFRH_AFRH2);
	GPIOA->AFR[1] |= (4 << GPIO_AFRH_AFRH1_Pos) | (4 << GPIO_AFRH_AFRH2_Pos);

	// Set pin mode to open drain
	GPIOA->OTYPER |= (1 << 9) | (1 << 10);

	// Enable I2C2 Clock
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

	// Disable I2C2 for configuration
	I2C2->CR1 &= ~I2C_CR1_PE;

	I2C2->CR1 = 0;
	I2C2->CR2 = 0;
	I2C2->ICR |= 0xffff;
	I2C2->TIMINGR = (0x01 << I2C_TIMINGR_PRESC_Pos) |
	                (0xC9 << I2C_TIMINGR_SCLL_Pos) |
	                (0x03 << I2C_TIMINGR_SCLH_Pos) |
	                (0x01 << I2C_TIMINGR_SDADEL_Pos) |
	                (0x03 << I2C_TIMINGR_SCLDEL_Pos);

	// Enable I2C2
	I2C2->CR1 |= I2C_CR1_PE;
}

void adv7611Suspend()
{
	printf("ADV7611 Suspend\n");
	adv7611ResetOn();
}

void adv7611Resume()
{
	printf("ADV7611 Resuming...");
	adv7611ResetOff();

	adv7611WriteConfig(g_configTablePreEdid);
	adv7611WriteEdid(g_edid, sizeof(g_edid));
	adv7611WriteConfig(g_configTablePostEdid);
	printf("done.\n");
}

void i2cStart()
{
	I2C2->CR2 |= I2C_CR2_START;
	while(I2C2->CR2 & I2C_CR2_START)
		;
}

void i2cStop()
{
	I2C2->CR2 |= I2C_CR2_STOP;
	while(I2C2->ISR & I2C_CR2_STOP)
		;
}

void i2cWrite(uint8_t x)
{
	while(!(I2C2->ISR & I2C_ISR_TXIS))
		;

	I2C2->TXDR = x;

	while(!(I2C2->ISR & (I2C_ISR_TXIS | I2C_ISR_TC)))
		;
}

uint8_t i2cRead()
{
	while(!(I2C2->ISR & I2C_ISR_RXNE))
		;
	return I2C2->RXDR & 0xff;
}

uint8_t adv7611ReadRegister(uint8_t addr, uint8_t subaddr)
{
	uint8_t result;

	while(I2C2->ISR & I2C_ISR_BUSY)
		;

	I2C2->CR2 &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN);
	I2C2->CR2 |= (addr << I2C_CR2_SADD_Pos) | (1 << I2C_CR2_NBYTES_Pos);
	i2cStart();
	i2cWrite(subaddr);

	I2C2->CR2 &= ~I2C_CR2_NBYTES;
	I2C2->CR2 |= 1 << I2C_CR2_NBYTES_Pos;
	I2C2->CR2 |= I2C_CR2_RD_WRN;
	i2cStart();

	result = i2cRead();

	i2cStop();

	return result;	
}

uint8_t adv7611WriteRegister(uint8_t addr, uint8_t subaddr, uint8_t value)
{
	while(I2C2->ISR & I2C_ISR_BUSY)
		;

	I2C2->ICR |= I2C_ICR_STOPCF | I2C_ICR_NACKCF;

	I2C2->CR2 &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RD_WRN);
	I2C2->CR2 |= (addr << I2C_CR2_SADD_Pos) | (2 << I2C_CR2_NBYTES_Pos);
	i2cStart();
	i2cWrite(subaddr);
	i2cWrite(value);
	i2cStop();

	if(I2C2->ISR & I2C_ISR_NACKF)
		return 0;
	else
		return 1;
}

