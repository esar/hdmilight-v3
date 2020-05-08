#include <stdint.h>
#include <stddef.h>
#include <stm32f303xe.h>
#include <core_cm4.h>
#include "printf.h"
#include "pos_sensors.h"



#define M_PI    3.14159265358979323846264338327950288

#define PULSE_TYPE_SYNC    0x00
#define PULSE_TYPE_SWEEP   0x10
#define PULSE_TYPE_INVALID 0xf0
#define PULSE_HAS_DATA(x)  (((x) & 0xf0) == 0)
#define PULSE_DATA_AXIS(x) ((x) & 1)
#define PULSE_DATA_DATA(x) (((x) >> 1) & 1)
#define PULSE_DATA_SKIP(x) (((x) >> 2) & 1)

#define ANGLE_INVALID      0xff

#define TICKS_PER_MICROSECOND    (7000000 / 1000000)
#define MICROSECONDS_IN_TICKS(x)  ((x) * TICKS_PER_MICROSECOND)

#define SWEEP_CENTER_IN_MICROSEC  4000
#define SWEEP_180_IN_MICROSEC     8333
#define SWEEP_CENTER_IN_TICKS     MICROSECONDS_IN_TICKS(SWEEP_CENTER_IN_MICROSEC)
#define SWEEP_180_IN_TICKS        MICROSECONDS_IN_TICKS(SWEEP_180_IN_MICROSEC)

#define NUM_SENSOR_ANGLE_SETS    2

static uint8_t g_angleCount = 0;
static volatile sensor_t g_sensors[NUM_SENSORS];
static lighthouse_solution_t g_solution;
static ootx_msg_t   g_lighthouseOotx[NUM_LIGHTHOUSES];

static uint8_t g_lastReadSensorAngleSet = 1;
static uint8_t g_currentSensorAngleSet = 0;
static sensor_angle_set_t g_sensorAngles[NUM_SENSOR_ANGLE_SETS];


// decodePulseWidth returns:
//     PULSE_TYPE_SWEEP (0x10)   for short sweep pulses
//     0x00 -> 0x07              for data from valid sync pulses
//     PULSE_TYPE_INVALID (0xf0) for pulses that are too long
//
// Pulse encoding info originally from: 
// https://github.com/nairol/LighthouseRedox/blob/master/docs/Light%20Emissions.md
//
// skip | data | axis |  length uS
// -----+------+------+-----------
//    0 |    0 |    0 |       62.5
//    0 |    0 |    1 |       72.9
//    0 |    1 |    0 |       83.3
//    0 |    1 |    1 |       93.8
//    1 |    0 |    0 |      104.0
//    1 |    0 |    1 |      115.0
//    1 |    1 |    0 |      125.0
//    1 |    1 |    1 |      135.0
//
// 10.4 uS between centers
//
uint8_t decodePulseWidth(int32_t width)
{
	uint8_t result;

	width = width - (uint16_t)((62.5 - (10.4/2)) * TICKS_PER_MICROSECOND);
	if(width < 0)
		return PULSE_TYPE_SWEEP;
	result = width / (uint16_t)(10.4 * TICKS_PER_MICROSECOND);
	if(result > 7)
		result = PULSE_TYPE_INVALID;
	return result;
}

void ootxAddBit(ootx_t* ootx, uint8_t bit)
{
	if((bit & 1) && ootx->shiftRegister == 0 && ootx->frameBit == 0)
	{
		// got preamble
		ootx->pos = 0;
		ootx->length = 0;
		ootx->shiftCount = 0;
	}
	else if(ootx->shiftCount == 17)
	{
		ootx->shiftCount = 0;
		if((bit & 1) == 0)// || ootx->frameBit == 0)
			ootx->length = 0xff;  // abort

		// end of word
		if(ootx->length == 0)
		{
			ootx->length = (ootx->shiftRegister << 8) | (ootx->shiftRegister >> 8);
			ootx->length += 2;  // add checksum to length
		}
		else if(ootx->length != 0xff)
		{
			if(ootx->pos < ootx->length && ootx->pos < sizeof(ootx->data))
			{
				ootx->data[ootx->pos++] = ootx->shiftRegister >> 8;
				ootx->data[ootx->pos++] = ootx->shiftRegister & 0xFF;
			}
			else
			{
				// TODO: validate checksum
				ootx->validLength = ootx->length;
				ootx->length = 0xff;
			}
		}
	}

	ootx->frameBit = ootx->shiftRegister >> 15;
	ootx->shiftRegister = (ootx->shiftRegister << 1) | ((bit & 1));
	ootx->shiftCount = (ootx->shiftCount + 1);
}

void sensorIrqHandler(volatile sensor_t* sensor, uint16_t riseTime, uint16_t fallTime)
{
	uint16_t width = fallTime - riseTime;
	uint8_t data = decodePulseWidth(width);


uint32_t index = ((uint32_t)sensor - (uint32_t)g_sensors) / sizeof(sensor_t);
	if(data == PULSE_TYPE_INVALID)
	{
		printf("LRG %u: %u %u %d\r\n", index, riseTime, fallTime, (int16_t)width);
		return;
	}

	if(data == PULSE_TYPE_SWEEP)
	{
//if(index == 1)
//	printf("SWP %u: %u %u %d\r\n", index, riseTime, fallTime, (int16_t)width);
		if(sensor->currentAngle == 0 && g_angleCount > NUM_SENSORS)
			g_angleCount = 1;
		else
			g_angleCount += 1;

		if(sensor->currentAngle < NUM_ANGLES)
		{
			uint16_t delta = riseTime - sensor->startTime;
			sensor->angleTicks[sensor->currentAngle >> 1][sensor->currentAngle & 1] += delta;
			sensor->angleTicks[sensor->currentAngle >> 1][sensor->currentAngle & 1] >>= 1;
			sensor->angleRadians[sensor->currentAngle >> 1][sensor->currentAngle & 1] = ((scalar_t)delta - SWEEP_CENTER_IN_TICKS) * (M_PI / SWEEP_180_IN_TICKS);
			sensor->currentAngle = ANGLE_INVALID;
		}


		if(g_angleCount == 16)
		{
			int i, j, k;
			for(i = 0; i < NUM_SENSORS; ++i)
				for(j = 0; j < NUM_LIGHTHOUSES; ++j)
					for(k = 0; k < NUM_AXES; ++k)
						g_sensorAngles[g_currentSensorAngleSet][i][j][k] = g_sensors[i].angleRadians[j][k];
			g_currentSensorAngleSet = (g_currentSensorAngleSet + 1) % NUM_SENSOR_ANGLE_SETS;
		}
	}
	else
	{
		uint16_t ticksSinceFirstPulse = riseTime - sensor->firstPulseTime;
		uint8_t lighthouse;

		if(ticksSinceFirstPulse >= MICROSECONDS_IN_TICKS(300) &&
		   ticksSinceFirstPulse <= MICROSECONDS_IN_TICKS(500))
		{
			lighthouse = 1;
			sensor->firstPulseTime = riseTime - MICROSECONDS_IN_TICKS(400);
		}
		else
		{
			// Doesn't fit timing of second lighthouse, so it's either the first
			// or we're out of sync and might as well consider it the first
			lighthouse = 0;
			sensor->firstPulseTime = riseTime;
		}

		if(!PULSE_DATA_SKIP(data))
		{
			sensor->currentAngle = (lighthouse << 1) | PULSE_DATA_AXIS(data);
			sensor->startTime = riseTime;
		}

		ootxAddBit(&sensor->ootx[lighthouse], PULSE_DATA_DATA(data));
	}
}

void TIM3_IRQHandler()
{
	if(TIM3->SR & TIM_SR_CC2IF)
		sensorIrqHandler(&g_sensors[0], TIM3->CCR1, TIM3->CCR2);
	if(TIM3->SR & TIM_SR_CC4IF)
		sensorIrqHandler(&g_sensors[1], TIM3->CCR3, TIM3->CCR4);
}

void TIM4_IRQHandler()
{
	if(TIM4->SR & TIM_SR_CC2IF)
		sensorIrqHandler(&g_sensors[3], TIM4->CCR1, TIM4->CCR2);
	if(TIM4->SR & TIM_SR_CC4IF)
		sensorIrqHandler(&g_sensors[2], TIM4->CCR3, TIM4->CCR4);
}

ootx_msg_t* posSensorsGetOotx()
{
	if(g_lighthouseOotx[0].isNew && g_lighthouseOotx[1].isNew)
		return g_lighthouseOotx;
	return NULL;
}

sensor_angle_set_t* posSensorsPoll()
{
	int i;
	uint16_t now = TIM3->CNT;
	static int call = 0;

	for(i = 0; i < NUM_SENSORS; ++i)
	{
		volatile sensor_t* sensor = &g_sensors[i];
		if(now - sensor->firstPulseTime > MICROSECONDS_IN_TICKS(8333 + 200))
		{
			// No pulse for more than 8333uS so move sensor->firstPulseTime
			// on by one cycle to try and stay locked while we're out of sight
			// of the lighthouses
			sensor->firstPulseTime += MICROSECONDS_IN_TICKS(8333);
		}
	}

	uint8_t sensorIndex;
	uint8_t lighthouseIndex;

	for(sensorIndex = 0; sensorIndex < NUM_SENSORS; ++sensorIndex)
	{
		for(lighthouseIndex = 0; lighthouseIndex < NUM_LIGHTHOUSES; ++lighthouseIndex)
		{
			ootx_t* ootx = &g_sensors[sensorIndex].ootx[lighthouseIndex];

			if(ootx->validLength)
			{
				g_lighthouseOotx[lighthouseIndex].length = ootx->validLength;
				memcpy(g_lighthouseOotx[lighthouseIndex].data, 
				       ootx->data, 
				       ootx->validLength);
				g_lighthouseOotx[lighthouseIndex].isNew = 1;
				ootx->validLength = 0;
			}
		}
	}

/*
if(++call > 1000000)
{
	call = 0;
printf("solving... %u\r\n", call);
	solveLighthouse(&g_solution, (sensor_t*)g_sensors, g_lighthouseOotx);
	printf("L:%u,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", 0,
	       (float)g_solution.lighthouses[0].origin[0],
	       (float)g_solution.lighthouses[0].origin[1],
	       (float)g_solution.lighthouses[0].origin[2],
	       (float)g_solution.lighthouses[0].rotationMatrix[0],
	       (float)g_solution.lighthouses[0].rotationMatrix[1],
	       (float)g_solution.lighthouses[0].rotationMatrix[2],
	       (float)g_solution.lighthouses[0].rotationMatrix[3],
	       (float)g_solution.lighthouses[0].rotationMatrix[4],
	       (float)g_solution.lighthouses[0].rotationMatrix[5],
	       (float)g_solution.lighthouses[0].rotationMatrix[6],
	       (float)g_solution.lighthouses[0].rotationMatrix[7],
	       (float)g_solution.lighthouses[0].rotationMatrix[8]);
	printf("L:%u,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", 1,
	       (float)g_solution.lighthouses[1].origin[0],
	       (float)g_solution.lighthouses[1].origin[1],
	       (float)g_solution.lighthouses[1].origin[2],
	       (float)g_solution.lighthouses[1].rotationMatrix[0],
	       (float)g_solution.lighthouses[1].rotationMatrix[1],
	       (float)g_solution.lighthouses[1].rotationMatrix[2],
	       (float)g_solution.lighthouses[1].rotationMatrix[3],
	       (float)g_solution.lighthouses[1].rotationMatrix[4],
	       (float)g_solution.lighthouses[1].rotationMatrix[5],
	       (float)g_solution.lighthouses[1].rotationMatrix[6],
	       (float)g_solution.lighthouses[1].rotationMatrix[7],
	       (float)g_solution.lighthouses[1].rotationMatrix[8]);
	printf("E:%f,%f,%f,%f,%f,%f\r\n",
		       (float)g_solution.lighthouses[SLAVE].error.err01,
		       (float)g_solution.lighthouses[SLAVE].error.err02,
		       (float)g_solution.lighthouses[SLAVE].error.err03,
		       (float)g_solution.lighthouses[SLAVE].error.err12,
		       (float)g_solution.lighthouses[SLAVE].error.err13,
		       (float)g_solution.lighthouses[SLAVE].error.err23);
	printf("E:%f,%f,%f,%f,%f,%f\r\n",
		       (float)g_solution.lighthouses[MASTER].error.err01,
		       (float)g_solution.lighthouses[MASTER].error.err02,
		       (float)g_solution.lighthouses[MASTER].error.err03,
		       (float)g_solution.lighthouses[MASTER].error.err12,
		       (float)g_solution.lighthouses[MASTER].error.err13,
		       (float)g_solution.lighthouses[MASTER].error.err23);
printf("done.\r\n");
}
*/

	if((g_lastReadSensorAngleSet + 1) % NUM_SENSOR_ANGLE_SETS != g_currentSensorAngleSet)
	{
		g_lastReadSensorAngleSet = (g_lastReadSensorAngleSet + 1) % NUM_SENSOR_ANGLE_SETS;
		return &g_sensorAngles[g_lastReadSensorAngleSet];
	}
	else
		return NULL;
}

void posSensorsInit()
{
	int i;

	// Configure pins
	// PB5 = TIM3_CH2 = Photo-diode 0
	// PB6 = TIM4_CH1 = Photo-diode 3
	// PB7 = TIM3_CH4 = Photo-diode 1
	// PB9 = TIM4_CH4 = Photo-diode 2

	// Enable GPIOB
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Set pin modes to alternate function
	GPIOB->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | 
	                  GPIO_MODER_MODER7 | GPIO_MODER_MODER9);
	GPIOB->MODER |= (2 << GPIO_MODER_MODER5_Pos) | 
	                (2 << GPIO_MODER_MODER6_Pos) |
	                (2 << GPIO_MODER_MODER7_Pos) |
	                (2 << GPIO_MODER_MODER9_Pos);

	// Set alternate function to timer input
	// PB4 = TIM3_CH1 = Alt Func 2
	GPIOB->AFR[0] &= ~GPIO_AFRH_AFRH5;
	GPIOB->AFR[0] |= 2 << GPIO_AFRH_AFRH5_Pos;
	// PB6 = TIM4_CH1 = Alt Func 2
	GPIOB->AFR[0] &= ~GPIO_AFRH_AFRH6;
	GPIOB->AFR[0] |= 2 << GPIO_AFRH_AFRH6_Pos;
	// PB7 = TIM3_CH4 = Alt Func 10
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL7;
	GPIOB->AFR[0] |= 10 << GPIO_AFRH_AFRH7_Pos;
	// PB9 = TIM4_CH4 = Alt Func 2
	GPIOB->AFR[1] &= ~GPIO_AFRH_AFRH1;
	GPIOB->AFR[1] |= 2 << GPIO_AFRH_AFRH1_Pos;


	// Configure timer

	// Enable timer 3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	// Enable timer 4 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	// Set timer 3 channel inputs
	TIM3->CCMR1 = TIM_CCMR1_CC1S_1 |  // Channel1: TI2 (PB5)
	              TIM_CCMR1_CC2S_0 ;  // Channel2: TI2 (PB5)
	TIM3->CCMR2 = TIM_CCMR2_CC3S_1 |  // Channel3: TI4 (PB7)
	              TIM_CCMR2_CC4S_0 ;  // Channel4: TI4 (PB7)
	TIM3->CCER  = TIM_CCER_CC2P    |  // Channel2 active on falling edge
	              TIM_CCER_CC4P    |  // Channel4 active on falling edge
	              TIM_CCER_CC1E    |  // Enable channel 1
	              TIM_CCER_CC2E    |  // Enable channel 2
	              TIM_CCER_CC3E    |  // Enable channel 3
	              TIM_CCER_CC4E    ;  // Enable channel 4

	// Set timer 4 channel inputs
	TIM4->CCMR1 = TIM_CCMR1_CC1S_0 |  // Channel1: TI1 (PB6)
	              TIM_CCMR1_CC2S_1 ;  // Channel2: TI1 (PB6)
	TIM4->CCMR2 = TIM_CCMR2_CC3S_1 |  // Channel3: TI4 (PB9)
	              TIM_CCMR2_CC4S_0 ;  // Channel4: TI4 (PB9)
	TIM4->CCER  = TIM_CCER_CC2P    |  // Channel2 active on falling edge
	              TIM_CCER_CC4P    |  // Channel4 active on falling edge
	              TIM_CCER_CC1E    |  // Enable channel 1
	              TIM_CCER_CC2E    |  // Enable channel 2
	              TIM_CCER_CC3E    |  // Enable channel 3
	              TIM_CCER_CC4E    ;  // Enable channel 4

	// Enable capture interrupts for falling edges
	TIM3->DIER |= TIM_DIER_CC2IE |
	              TIM_DIER_CC4IE ;
	TIM4->DIER |= TIM_DIER_CC2IE |
	              TIM_DIER_CC4IE ;

	// Clear capture flags
	TIM3->SR &= ~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF);
	TIM4->SR &= ~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF);

	// Enable interrupt
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_EnableIRQ(TIM4_IRQn);

	// Approximately synchronise the counters
	TIM3->CNT = 0;
	TIM4->CNT = 0;

	// Set prescaler 56MHz / (7+1) = 7MHZ
	// 1 cycle (8333uS) = 58331 ticks
	TIM3->PSC = 7;
	TIM4->PSC = 7;

	// Enable timers
	TIM3->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;


	for(i = 0; i < NUM_SENSORS; ++i)
	{
		g_sensors[i].angleTicks[0][0] = 0;
		g_sensors[i].angleTicks[0][1] = 0;
		g_sensors[i].angleTicks[1][0] = 0;
		g_sensors[i].angleTicks[1][1] = 0;
		g_sensors[i].startTime = 0;
		g_sensors[i].firstPulseTime = 0;
		g_sensors[i].currentAngle = ANGLE_INVALID;
	}
}

lighthouse_solution_t* posSensorsGetLighthouseSolution()
{
	return &g_solution;
}

