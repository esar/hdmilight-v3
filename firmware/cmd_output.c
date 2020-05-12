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

#include <stdint.h>
#include <string.h>
#include "printf.h"
#include "ambilight.h"


void setOutput(uint8_t output, uint16_t light, uint8_t area, uint8_t coef, uint8_t gamma, uint8_t enabled)
{
	//  15    12    9                 0
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |E| coe  | gam  | area          |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |               |               |

	uint16_t address = AMBILIGHT_BASE_ADDR_OUTPUT;
	address += (uint16_t)output * 1024;
	address += light * 2;

	if(enabled)
		enabled = 0x80;

	uint8_t data[2] = { area & 0xff, enabled | ((coef & 15) << 3) | (gamma & 7) };
	fpgaConfigWrite(address, data, sizeof(data));
}

void getOutput(uint8_t output, uint16_t light, int* area, int* coef, int* gamma, int* enabled)
{
	uint16_t address = AMBILIGHT_BASE_ADDR_OUTPUT;
	address += (uint16_t)output * 1024;
	address += light * 2;

	uint8_t data[2];
	fpgaConfigRead(address, data, sizeof(data));

	*area = data[0];
	*gamma = data[1] & 7;
	*coef = (data[1] >> 3) & 15;
	*enabled = data[1] & 0x80;
}

void cmdSetOutput(uint8_t argc, char** argv)
{
	if(argc == 7)
	{
		int area, coef, gamma, enabled;
		uint8_t output, maxOutput;
		uint8_t light, minLight, maxLight;

		area = getint(&argv[3]);
		coef = getint(&argv[4]);
		gamma = getint(&argv[5]);
		enabled = getint(&argv[6]);
		
		getrange(argv[1], &output, &maxOutput);
		getrange(argv[2], &minLight, &maxLight);
		do
		{
			light = minLight;
			do
			{
				setOutput(output, light, area, coef, gamma, enabled);
				
			} while(light++ < maxLight);
			
		} while(output++ < maxOutput);
	}
}

void cmdGetOutput(uint8_t argc, char** argv)
{
	if(argc == 3)
	{
		uint8_t output, maxOutput;
		uint8_t light, minLight, maxLight;

		getrange(argv[1], &output, &maxOutput);
		getrange(argv[2], &minLight, &maxLight);
		do
		{
			light = minLight;
			do
			{
				int area, coef, gamma, enabled;

				getOutput(output, light, &area, &coef, &gamma, &enabled);

				printf("%d: %d: %d %d %d %d\n", output, light, area, coef, gamma, enabled);
				
			} while(light++ < maxLight);
			
		} while(output++ < maxOutput);
	}
}

void cmdRstOutput(uint8_t argc, char** argv)
{
	int output;

	for(output = 0; output < 8; ++output)
	{
		int light;

		for(light = 0; light < 512; ++light)
			setOutput(output, light, light & 0xff, 0, 0, (light & 0x100) == 0);
	}
}

