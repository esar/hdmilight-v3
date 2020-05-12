#include <stdint.h>
#include <stm32f303xe.h>
#include <core_cm4.h>
#include "printf.h"
#include "spi.h"



void spiInit()
{
	// Configure pins
	// PC10 = SPI3_SCLK
	// PC11 = SPI3_MISO
	// PC12 = SPI3_MOSI
	// PA4  = SPI3_NSS

	// Enable GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Enable GPIOC
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	// Set pins mode to alternate function
	GPIOA->MODER &= ~GPIO_MODER_MODER4;
	GPIOA->MODER |= 2 << GPIO_MODER_MODER4_Pos;
	GPIOC->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12);
	GPIOC->MODER |= (2 << GPIO_MODER_MODER10_Pos) | (2 << GPIO_MODER_MODER11_Pos) | (2 << GPIO_MODER_MODER12_Pos);

	// Set alternate function to SPI3
	// PC10 = SPI3_SCLK = Alt Func 6
	// PC11 = SPI3_MISO = Alt Func 6
	// PC12 = SPI3_MOSI = Alt Func 6
	// PA4  = SPI3_NSS  = Alt Func 6
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL4;
	GPIOA->AFR[0] |= 6 << GPIO_AFRL_AFRL4_Pos;
	GPIOC->AFR[1] &= ~(GPIO_AFRH_AFRH2 | GPIO_AFRH_AFRH3 | GPIO_AFRH_AFRH4);
	GPIOC->AFR[1] |= (6 << GPIO_AFRH_AFRH2_Pos) | (6 << GPIO_AFRH_AFRH3_Pos) | (6 << GPIO_AFRH_AFRH4_Pos);

	// Set the drive strength for the pins
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4;
	GPIOA->OSPEEDR |= 2 << GPIO_OSPEEDER_OSPEEDR4_Pos;
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12);
	GPIOC->OSPEEDR |= (2 << GPIO_OSPEEDER_OSPEEDR10_Pos) | (2 << GPIO_OSPEEDER_OSPEEDR11_Pos) | (2 << GPIO_OSPEEDER_OSPEEDR12_Pos);

	// Enable pull-up on SPI3_NSS pin
	GPIOA->PUPDR |= 1 << GPIO_PUPDR_PUPDR4_Pos;

	// Enable SPI3 Clock
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;

	// Disable SPI3 for configuration and reset
	SPI3->CR1 &= ~SPI_CR1_SPE;
	RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;

	// Set clock rate, polarity and phase
	SPI3->CR1 = (4 << SPI_CR1_BR_Pos) | SPI_CR1_CPOL | SPI_CR1_CPHA;

	// Set as master
	SPI3->CR1 |= SPI_CR1_MSTR;

	// Set RX threshold to 8-bits 
	// If we don't then trying to receive an odd number of bytes never completes
	SPI3->CR2 |= SPI_CR2_SSOE | SPI_CR2_FRXTH;
}

void spiTransaction(void* txBuf1, int txBuf1Len,
                    void* txBuf2, int txBuf2Len,
                    void* rxBuf, int rxBufLen)
{
	volatile uint8_t dummy;

	SPI3->SR &= ~SPI_SR_MODF;
	SPI3->CR1 |= SPI_CR1_SPE;
	SPI3->CR1 |= SPI_CR1_MSTR;

	while(txBuf1Len--)
	{
		while(!(SPI3->SR & SPI_SR_TXE))
			;
		*(uint8_t*)&SPI3->DR = *(uint8_t*)txBuf1++;
	}

	while(txBuf2Len--)
	{
		while(!(SPI3->SR & SPI_SR_TXE))
			;
		*(uint8_t*)&SPI3->DR = *(uint8_t*)txBuf2++;
	}

	// Wait for the TX buffer to empty
	while(SPI3->SR & SPI_SR_FTLVL)
		;

	// Wait for the bus to be idle
	while(SPI3->SR & SPI_SR_BSY)
		;

	// Clear any junk from the RX buffer
	while(SPI3->SR & SPI_SR_FRLVL)
		dummy = SPI3->DR;

	// Read data
	while(rxBufLen--)
	{
		// Send dummy byte to clock the data
		while(!(SPI3->SR & SPI_SR_TXE))
			;
		*(uint8_t*)&SPI3->DR = 0;

		// Wait for the data to arrive in the RX FIFO
		while(!(SPI3->SR & SPI_SR_FRLVL))
			;

		// Read it
		*(uint8_t*)rxBuf++ = SPI3->DR;
	}

	// Follow SPI shutdown procedure, probably unnecessary...
	// Wait for TX FIFO to be empty (should be already)
	while(SPI3->SR & SPI_SR_FTLVL)
		;

	// Wait for bus to be idle (should be already)
	while(SPI3->SR & SPI_SR_BSY)
		;

	// Disable SPI
	SPI3->CR1 &= ~SPI_CR1_SPE;

	// Clear any junk from the RX FIFO
	while(SPI3->SR & SPI_SR_FRLVL)
		dummy = SPI3->DR;

	// pretend we used the dummy value to silence the gcc warning
	(void)dummy;
}

void fpgaConfigWrite(uint16_t address, void* data, uint16_t length)
{
	uint8_t command[] = { 0x02, address >> 8, address & 0xff };
	spiTransaction(command, sizeof(command), data, length, NULL, 0);
}

void fpgaConfigRead(uint16_t address, void* data, uint16_t length)
{
	uint8_t command[] = { 0x01, address >> 8, address & 0xff, 0 };
	spiTransaction(command, sizeof(command), NULL, 0, data, length);
}

void fpgaConfigLoad(uint8_t slot)
{
	// TODO: Why does this need the last three bytes (1, 1, 0) ?
	uint8_t command[] = { 0xa0, slot + 1, 1, 1, 0 };
	spiTransaction(command, sizeof(command), NULL, 0, NULL, 0);
}

uint8_t fpgaConfigStatus()
{
	uint8_t status;
	uint8_t command[] = { 0xa2, 0 };
	spiTransaction(command, sizeof(command), NULL, 0, &status, sizeof(status));
	return status;
}

void fpgaFlashRead(uint32_t address, void* data, int length)
{
	uint8_t passthru_command[] = { 0xb0, 0 };
	uint8_t command[] = { 0x03, (address >> 16) & 0xff, (address >> 8) & 0xff, address & 0xff };
	spiTransaction(passthru_command, sizeof(passthru_command), NULL, 0, NULL, 0);
	spiTransaction(command, sizeof(command), NULL, 0, data, length);
}

void fpgaFlashWrite(uint32_t address, void* data, int length)
{
}

