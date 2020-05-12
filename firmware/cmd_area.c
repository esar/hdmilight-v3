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


void setArea(unsigned int area, 
             unsigned char xmin, unsigned char xmax,
             unsigned char ymin, unsigned char ymax,
             unsigned char shift)
{
	uint16_t address = AMBILIGHT_BASE_ADDR_AREA;
	address += (uint16_t)area * 4;

	//  31      28      24          18          12          6           0
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// | unused  | shift | ymax      | ymin      | xmax      | xmin      |
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// |                 |               |               |               |

	area <<= 2;

	uint8_t data[4];
	data[0] = (xmin & 0x3f) | (xmax << 6);
	data[1] = ((xmax & 0x3f) >> 2) | (ymin << 4);
	data[2] = ((ymin & 0x3f) >> 4) | (ymax << 2);
	data[3] = shift & 0xf;

	fpgaConfigWrite(address, data, sizeof(data));
}

void cmdSetArea(uint8_t argc, char** argv)
{
	if(argc == 7)
	{
		uint8_t index, maxIndex;
		uint8_t xmin, xmax, ymin, ymax, shift;
		
		xmin = getint(&argv[2]);
		xmax = getint(&argv[3]);
		ymin = getint(&argv[4]);
		ymax = getint(&argv[5]);
		shift = getint(&argv[6]);

		getrange(argv[1], &index, &maxIndex);
		do
		{
			setArea(index, xmin, xmax, ymin, ymax, shift);

		} while(index++ < maxIndex);
	}
}

void cmdGetArea(uint8_t argc, char** argv)
{
	if(argc == 2)
	{
		uint8_t index, maxIndex;

		getrange(argv[1], &index, &maxIndex);
		do
		{
			uint16_t address = AMBILIGHT_BASE_ADDR_AREA + ((uint16_t)index * 4);
			uint8_t data[4];
			int x;

			fpgaConfigRead(address, data, sizeof(data));

			x = data[0] & 0x3f; // xmin
			printf("%d: %d ", index, x);
			x = (data[0] >> 6) | ((data[1] & 0x0f) << 2); // xmax
			printf("%d ", x);
			x = (data[1] >> 4) | ((data[2] & 3) << 4); // ymin
			printf("%d ", x);
			x = (data[2] >> 2); // ymax
			printf("%d ", x);
			x = data[3] & 0xf; // shift
			printf("%d\n", x);

		} while(index++ < maxIndex);
	}
}

void cmdRstArea(uint8_t argc, char** argv)
{
	int i;

	for(i = 0; i < 64; ++i)
		setArea(i, i, i, 0, 8, 3);
}

