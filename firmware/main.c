/*

   Copyright (C) 2013 Stephen Robinson
  
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

#include <inttypes.h>
#include <string.h>
#include <stm32f303x8.h>
#include <core_cm4.h>
#include "printf.h"
#include "ambilight.h"
#include "clock.h"
#include "uart.h"
#include "papix/pos_sensors.h"


// If this line is commented out, the ADV7611 will not be started
// at power on and HDMI handshaking will not occur, in which case "R"
// needs to be run from the serial console to start the ADV7611
#define AUTO_INITIALIZATION

// Uncomment to enable power on/off via physical switch.
// Requires a pull-up resistor and switch to be added.
//#define ENABLE_POWER_SWITCH

#define BELL    '\a'


uint8_t silent = 0;
volatile uint8_t g_formatChanged = 0;
volatile uint8_t g_cecMessage[16];
volatile uint8_t g_cecMessageLength;

int __errno;

/*
ISR(_VECTOR(1))
{
	g_formatChanged = 1;
	INT_CLEAR = 1;
}

ISR(_VECTOR(2))
{
	uint8_t cecRxIntState = i2cRead(0x98, 0x93);

	// if this is a CEC RX message 0 ready interupt
	if(cecRxIntState & 8)
	{
		int i;

		// copy the received message
		g_cecMessageLength = i2cRead(0x80, 0x25);
		if(g_cecMessageLength > sizeof(g_cecMessage))
			g_cecMessageLength = sizeof(g_cecMessage);
		for(i = 0; i < g_cecMessageLength; ++i)
			g_cecMessage[i] = i2cRead(0x80, 0x15 + i);

		// clear the message buffer ready for a new message
		i2cWrite(0x80, 0x2C, 0x02);

		// clear the interrupt
		i2cWrite(0x98, 0x94, 0x08);
	}
}
*/

void idle()
{
/*
#ifdef ENABLE_POWER_SWITCH
	static uint8_t debounceValue = 0;
	static uint8_t debounceTicks = 0;
#endif // ENABLE_POWER_SWITCH

	if(g_formatChanged)
	{
		changeFormat();
		g_formatChanged = 0;
	}

	if(g_cecMessageLength != 0)
	{
		cli();
		processCecMessage();
		g_cecMessageLength = 0;
		sei();
	}

#ifdef ENABLE_POWER_SWITCH
	if((PIND & (1 << GPIO_POWER_PIN)) == debounceValue)
	{
		if(debounceTicks == DEBOUNCE_TICK_COUNT)
		{
			if(debounceValue)
				powerOn();
			else
				powerOff();
			++debounceTicks;
		}
		else if(debounceTicks < DEBOUNCE_TICK_COUNT)
			++debounceTicks;
	}
	else
	{
		debounceValue = PIND & (1 << GPIO_POWER_PIN);
		debounceTicks = 0;
	}
#endif // ENABLE_POWER_SWITCH
*/
}

// Print a section of the ring buffer to stdout, wrapping around when passing the end.
// Also replacing NULLs with spaces as old commands have had spaces replaced with NULLs
// when they were being split into args
void ringBufPrint(char* buf, uint8_t pos, int len)
{
	while(len--)
	{
		if(buf[pos] == '\0')
			_putchar(' ');
		else
			_putchar(buf[pos]);
		++pos;
	}
}

void ringBufCopy(char* buf, uint8_t dst, uint8_t src, int len)
{
	while(len--)
		buf[dst++] = buf[src++];
}

void ringBufCopyReverse(char* buf, uint8_t dst, uint8_t src, int len)
{
	dst += len;
	src += len;
	while(len--)
		buf[--dst] = buf[--src];
}

#define ESCAPE_STATE_NONE    0
#define ESCAPE_STATE_BRACKET 1
#define ESCAPE_STATE_CHAR    2

#define UP    0x100
#define DOWN  0x101
#define RIGHT 0x102
#define LEFT  0x103

char pollCommand(char** argv, char maxargs)
{
	static char cmdbuf[256] = {0, 0};
	static unsigned char current = 0;
	static unsigned char history = 0;
	static unsigned char pos = 1;
	static unsigned char end = 1;
	static uint8_t escapeState = ESCAPE_STATE_NONE;

	int c = _getchar();
	if(c == -1)
		return 0;
	
	switch(escapeState)
	{
		case ESCAPE_STATE_NONE:
			if(c == 0x1B)
				escapeState = ESCAPE_STATE_BRACKET;
			break;
		case ESCAPE_STATE_BRACKET:
			if(c == '[')
				escapeState = ESCAPE_STATE_CHAR;
			else
				escapeState = ESCAPE_STATE_NONE;
			break;
		case ESCAPE_STATE_CHAR:
			switch(c)
			{
				case 'A': c = UP;    break;
				case 'B': c = DOWN;  break;
				case 'C': c = RIGHT; break;
				case 'D': c = LEFT;  break;
			}
			escapeState = ESCAPE_STATE_NONE;
			break;
	}

	if(escapeState == ESCAPE_STATE_NONE)
	{
		switch(c)
		{
			case LEFT:
			case 0x02: // left
				if(pos - 1 != current)
				{
					--pos;
					printf("\x1B[D"); // move left
				}
				else
					_putchar(BELL);
				break;
			case 0x08: // backspace
			case 0x7f:
				if(pos > 0)
				{
					// move partial line left in buffer to fill hole
					ringBufCopy(cmdbuf, pos - 1, pos, end - pos);
					--pos; --end;

					// move cursor left one place and print the partial line
					printf("\x1B[D");
					ringBufPrint(cmdbuf, pos, end - pos);

					// move the cursor left, back to the edit position
					printf(" \x1B[%dD", end - pos + 1);
				}
				else
					_putchar(BELL);
				break;
			case RIGHT:
			case 0x06: // right
				if(pos + 1 != end)
				{
					++pos;
					printf("\x1B[C"); // move right
				}
				else
					_putchar(BELL);
				break;
			case '\n':
			case '\r':
				break;
			case UP:
			case 0x10:
				if(1)
				{
					unsigned char p = current;
					unsigned char len = 0;
					unsigned char i;

					for(i = 0; i < history + 1; ++i)
					{
						len = cmdbuf[p];
printf("len: %d\n", len);
						if(len == 0)
							break;
						p -= len;
					}
					if(i == history + 1)
					{
						++history;

						// lines in the history are NULL terminated, remove the termination
						--len;

						// copy the old line into the current position in the buffer
						ringBufCopy(cmdbuf, current + 1, p + 1, len - 1);
						cmdbuf[current + len] = '\0';

						// move the cursor to the beginning of the line and print the copied line
						printf(" \x1B[%dD", pos - current);
						ringBufPrint(cmdbuf, current + 1, len - 1);

						// if the copied line is shorter then overwrite any extra characters with spaces
						if(end - (current + len) > 0)
						{
							for(i = 0; i < end - (current + len); ++i)
								_putchar(' ');
							printf("\x1B[%dD", end - (current + len));
						}
						end = current + len;
						pos = end;
					}
					else
						_putchar(BELL);
				}
				break;
			case DOWN:
			case 0x0E:
				if(history > 1)
				{
					unsigned char p = current;
					unsigned char len;
					unsigned char i;

					--history;
					for(i = 0; i < history; ++i)
					{
						len = cmdbuf[p];
						if(len == 0)
							break;
						p -= len;
					}

					// the lines in the history are NULL terminated, remove the termination
					--len;

					// copy the old line to the current position in the buffer
					ringBufCopy(cmdbuf, current + 1, p + 1, len - 1);
					cmdbuf[current + len] = '\0';

					// move the cursor to the beginning of the line and print the copied line
					printf(" \x1B[%dD", pos - current);
					ringBufPrint(cmdbuf, current + 1, len - 1);

					// if the copied line is shorter then overwrite the extra characters with spaces
					if(end - (current + len) > 0)
					{
						for(i = 0; i < end - (current + len); ++i)
							_putchar(' ');
						printf("\x1B[%dD", end - (current + len));
					}
					end = current + len;
					pos = end;
				}
				else if(history == 1)
				{
				}
				else
					_putchar(BELL);
				break;
			default:
				if(end + 1 != current)
				{
					// copy any characters after the current edit point to the right to make room
					ringBufCopyReverse(cmdbuf, pos + 1, pos, end - pos);

					// add the new character
					cmdbuf[pos] = c;

					// print the buffer from the current edit point to the end
					ringBufPrint(cmdbuf, pos, end - pos + 1);

					// move the cursor back to the edit point
					printf(" \x1B[%dD", end - pos + 1);
					++pos;
					++end;

				}
				else
					_putchar(BELL);
				break;
		}
	}

	if(c == '\n' || c == '\r')
	{
		unsigned char arg;

		// if the current line wraps around the end of the buffer then rotate the
		// buffer so that the line is all in one piece
		if(end < current)
		{
			uint8_t offset = 0x100 - current;
			ringBufCopyReverse(cmdbuf, end + offset, end, 0x100);
			pos += offset;
			end += offset;
			current += offset;
		}

		// NULL terminate the current line
		cmdbuf[end] = '\0';

		// split the line into args
		pos = current + 1;
		for(arg = 0; arg < maxargs; ++arg)
		{
			if(pos == end)
				break; 

			while((cmdbuf[pos] == ' ' || cmdbuf[pos] == '\0') && pos != end)
				cmdbuf[pos++] = '\0';

			argv[arg] = cmdbuf + pos;

			while(cmdbuf[pos] != ' ' && cmdbuf[pos] != '\0' && pos != end)
				++pos;
		}
		
		// account for added NULL termination of the line
		++end;

		// store the length of the line for history navigation
		cmdbuf[end] = end - current;

		// setup ready for the next line
		cmdbuf[end + 1] = '\0';
		current = end;
		
		history = 0;
		pos = current + 1;
		end = current + 1;

		return arg;
	}

	return 0;
}

int getint(char** str)
{
	char* s = *str;
	uint8_t neg = 0;
	uint16_t x = 0;

	if(s[0] == '-')
	{
		neg = 1;
		++s;
	}

	if(s[0] == '0' && s[1] == 'x')
	{
		s += 2;
		while(1)
		{
			if(*s >= '0' && *s <= '9')
				x = (x << 4) + (*s - '0');
			else if(*s >= 'A' && *s <= 'F')
				x = (x << 4) + (*s - 'A' + 10);
			else if(*s >= 'a' && *s <= 'f')
				x = (x << 4) + (*s - 'a' + 10);
			else
				break;
			++s;
		}
	}
	else
	{
		while(*s >= '0' && *s <= '9')
			x = (x * 10) + (*s++ - '0');
	}
	
	*str = s;
	return neg ? 0 - x : x;
}

void getrange(char* str, uint8_t* min, uint8_t* max)
{
	if(*str == '*')
	{
		*min = 0;
		*max = 255;
		return;
	}
	else
	{
		*min = getint(&str);
		if(*str == '-')
		{
			++str;
			*max = getint(&str);
		}
		else
			*max = *min;
	}
}

void cmdRstAll(uint8_t argc, char** argv)
{
	cmdRstI2C(argc, argv);
	cmdRstArea(argc, argv);
	cmdRstColour(argc, argv);
	cmdRstGamma(argc, argv);
	cmdRstOutput(argc, argv);
	cmdRstDelay(argc, argv);
}

const struct
{
	const char* cmd;
	void (*handler)(uint8_t argc, char** argv);
	const char* usage;

} cmds[] = 
{
	{ "GA", cmdGetArea,   "Get Area:    GA index"   },
	{ "SA", cmdSetArea,   "Set Area:    SA index xmin xmax ymin ymax shift output"   },
	{ "RA", cmdRstArea,   "Rst Area:    RA"   },
	{ "GC", cmdGetColour, "Get Colour:  GC index row" },
	{ "SC", cmdSetColour, "Set Colour:  SC index row r g b" },
	{ "RC", cmdRstColour, "Rst Colour:  RC" },
	{ "GD", cmdGetDelay,  "Get Delay:   GD"  },
	{ "SD", cmdSetDelay,  "Set Delay:   SD num_frames num_ticks smooth_ratio"  },
	{ "RD", cmdRstDelay,  "Rst Delay:   RD"  },
	{ "DF", cmdDisFormat, "Dis Format:  DF" },
	{ "EF", cmdEnaFormat, "Ena Format:  EF" },
	{ "GF", cmdGetFormat, "Get Format:  GF" },
	{ "GG", cmdGetGamma,  "Get Gamma:   GG table channel index"  },
	{ "SG", cmdSetGamma,  "Set Gamma:   SG table channel index value"  },
	{ "RG", cmdRstGamma,  "Rst Gamma:   RG"  },
	{ "GI", cmdGetI2C,    "Get I2C:     GI addr sub_addr [bit_range]"    },
	{ "SI", cmdSetI2C,    "Set I2C:     SI addr sub_addr value"    },
	{ "RI", cmdRstI2C,    "Rst I2C:     RI"    },
	{ "SK", cmdSetKeys,   "Set Keys:    SK key_code"   },
	{ "RK", cmdRstKeys,   "Rst Keys:    RK"   },
	{ "GM", cmdGetMem,    "Get Memory:  GM index"    },
	{ "GO", cmdGetOutput, "Get Output:  GO output light" },
	{ "SO", cmdSetOutput, "Set Output:  SO output light area coef gamma enable" },
	{ "RO", cmdRstOutput, "Rst Output:  RO" },
	{ "GR", cmdGetResult, "Get Result:  GR index" },
	{ "GS", cmdGetStatus, "Get Status:  GS" },
	{ "GX", cmdGetAddr,   "Get Address: GX addr count"   },
	{ "SX", cmdSetAddr,   "Set Address: SX addr byte0 [byte1] [...]"  },
	{ "R",  cmdRstAll,    "Rst All:     R"    },
};

void dispatchCommand(int argc, char** argv)
{
	int i;

	if(argc > 0)
	{
		if(strcmp(argv[0], "?") == 0)
		{
			for(i = 0; i < sizeof(cmds) / sizeof(*cmds); ++i)
				printf("%s\n", cmds[i].usage);
		}
		else
		{
			for(i = 0; i < sizeof(cmds) / sizeof(*cmds); ++i)
			{
				if(strcmp(argv[0], cmds[i].cmd) == 0)
				{
					if(argc == 2 && strcmp(argv[1], "?") == 0)
						printf("%s\n", cmds[i].usage);
					else
						cmds[i].handler(argc, argv);
					break;
				}
			}

			if(i >= sizeof(cmds) / sizeof(*cmds))
				printf("err\n");
		}
	}

}

int main()
{
	int i;
	sensor_angle_set_t* angles;


	char* argv[12];
	int argc;

	clockInit();

	uartInit(115200);

	// Enable LED power
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= 1 << (1 * 2);
	GPIOC->BSRR |= 1 << 1;

	printf("Starting...\r\n");
	
	// Wait a short while
	for(i = 0; i < 10000; ++i)
		asm volatile ("nop");

	i2cInit();

	spiInit();

	posSensorsInit();

#ifdef AUTO_INITIALIZATION
	//silent = 1;
	//argv[0] = "R";
	//cmdRstAll(1, argv);
	//silent = 0;
#endif

uint8_t printit=0;

	printf("Entering main loop\r\n");
	printf("\n> ");
	while(1)
	{
		if((argc = pollCommand(argv, 12)))
		{
			printf("\n");
			dispatchCommand(argc, argv);
			printf("\n> ");
printit=1;
		}

		if((angles = posSensorsPoll()))
		{
/*
printf("angles0: %f %f %f %f\n", (*angles)[0][0][0], (*angles)[0][0][1], (*angles)[0][1][0], (*angles)[0][1][1]);
printf("angles1: %f %f %f %f\n", (*angles)[1][0][0], (*angles)[1][0][1], (*angles)[1][1][0], (*angles)[1][1][1]);
printf("angles2: %f %f %f %f\n", (*angles)[2][0][0], (*angles)[2][0][1], (*angles)[2][1][0], (*angles)[2][1][1]);
printf("angles3: %f %f %f %f\n", (*angles)[3][0][0], (*angles)[3][0][1], (*angles)[3][1][0], (*angles)[3][1][1]);
printf("\n");
*/
//printf("got angles\n");
			ootx_msg_t* ootx = posSensorsGetOotx();
			if(ootx)
			{
//printf("got ootx\n");
if(printit)
{
				lighthouse_solution_t solution;
				solveLighthouse(&solution, *angles, ootx);
static int count = 0;
//if(count++ % 100 == 0)
{
solution_error_t error;

	printf("L:%u,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", 0,
	       (float)solution.lighthouses[0].origin[0],
	       (float)solution.lighthouses[0].origin[1],
	       (float)solution.lighthouses[0].origin[2],
	       (float)solution.lighthouses[0].rotationMatrix[0],
	       (float)solution.lighthouses[0].rotationMatrix[1],
	       (float)solution.lighthouses[0].rotationMatrix[2],
	       (float)solution.lighthouses[0].rotationMatrix[3],
	       (float)solution.lighthouses[0].rotationMatrix[4],
	       (float)solution.lighthouses[0].rotationMatrix[5],
	       (float)solution.lighthouses[0].rotationMatrix[6],
	       (float)solution.lighthouses[0].rotationMatrix[7],
	       (float)solution.lighthouses[0].rotationMatrix[8]);
	printf("L:%u,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", 1,
	       (float)solution.lighthouses[1].origin[0],
	       (float)solution.lighthouses[1].origin[1],
	       (float)solution.lighthouses[1].origin[2],
	       (float)solution.lighthouses[1].rotationMatrix[0],
	       (float)solution.lighthouses[1].rotationMatrix[1],
	       (float)solution.lighthouses[1].rotationMatrix[2],
	       (float)solution.lighthouses[1].rotationMatrix[3],
	       (float)solution.lighthouses[1].rotationMatrix[4],
	       (float)solution.lighthouses[1].rotationMatrix[5],
	       (float)solution.lighthouses[1].rotationMatrix[6],
	       (float)solution.lighthouses[1].rotationMatrix[7],
	       (float)solution.lighthouses[1].rotationMatrix[8]);
	calcSolutionErrorFromSensorAngles(&solution, *angles, &error);
	printf("err: %f vs %f\r\n", solution.lighthouses[0].error.total, error.total);
printit=0;
}
}
			}

		}
	}
}

