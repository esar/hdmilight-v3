#include <stdint.h>
#include <stdarg.h>
#include <stm32f303x8.h>
#include <core_cm4.h>

#define TRUE 1
#define FALSE 0

// TX_BUF_SIZE must be power of 2
#define TX_BUF_SIZE    1024

static uint8_t g_txBuf[TX_BUF_SIZE];
static uint16_t g_txReadPos = 0;
static uint16_t g_txWritePos = 0;


static void uartTx()
{
	if(g_txReadPos != g_txWritePos)
	{
		// buffer is not empty, transmit next byte
		USART2->TDR = g_txBuf[g_txReadPos];
		g_txReadPos = (g_txReadPos + 1) & (TX_BUF_SIZE - 1);
	}
	else
	{
		// buffer is empty, turn off interrupt
		USART2->CR1 &= ~((1 << 3) | (1 << 7));
		if(USART2->ICR & (1 << 6))
			USART2->ICR |= 1 << 6;
		if(USART2->ISR & (1 << 7))
			USART2->RQR |= 1 << 4;
	}
}

int _putchar(char c)
{
	if(c == '\n')
		_putchar('\r');

	uint16_t pos = (g_txWritePos + 1) & (TX_BUF_SIZE - 1);

	// discard if tx buffer is full
	if(pos == g_txReadPos)
		return 0;

	// add data to buffer and enable interrupt to start sending
	g_txBuf[g_txWritePos] = c;
	g_txWritePos = pos;

	if(!(USART2->CR1 & (1 << 3)))
		USART2->CR1 |= (1 << 3) | (1 << 7);

	return 0;
}

int _getchar()
{
	if(USART2->ISR & USART_ISR_RXNE)
		return USART2->RDR & 0xff;
	else
		return -1;
}


void USART2_IRQHandler()
{
	if(USART2->ISR & (1 << 7))
	{
		uartTx();
	}
	else if(USART2->ISR & (1 << 5))
	{
		// RX
	}
}

void uartInit(int baud)
{
	__disable_irq();

	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Select sysclk as clock source
	RCC->CFGR3  |= 1 << 16;
	RCC->CFGR3  &= ~(1 << 17);

	// Enable USART2 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// Configure I/O pins, PA2=TX, PA3=RX
	GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
	GPIOA->MODER |= (2 << GPIO_MODER_MODER2_Pos) | (2 << GPIO_MODER_MODER3_Pos);
	
	// Configure alt function, PA2 = AF7, PA3 = AF7
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFRL2_Pos) | (7 << GPIO_AFRL_AFRL3_Pos);
	

	// Release USART2 reset
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST;

	// Disable USART2 to allow control bits to be set
	USART2->CR1 = 0;
	USART2->CR2 = 0;
	USART2->CR3 = 0;

	// Set baud rate
	USART2->BRR = SystemCoreClock / baud;

	// Clear pending interrrupts
	USART2->ICR = 0xffffffff;

	// Turn on receiver, and TX/RX interrupts
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TXEIE; // | USART_CR1_RXNEIE;

	// Enable USART2
	USART2->CR1 |= USART_CR1_UE;

	// Enable Interrupts
	EXTI->IMR |= 1 << 26;
	EXTI->EMR |= 1 << 26;

	NVIC_EnableIRQ(38);

	__enable_irq();
}
