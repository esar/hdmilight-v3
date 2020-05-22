/*

   Copyright (C) 2014 Stephen Robinson
  
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


void cmdDisFormat(uint8_t argc, char** argv)
{
	if(argc == 1)
		formatEnable(0);
}

void cmdEnaFormat(uint8_t argc, char** argv)
{
	if(argc == 1)
		formatEnable(1);
}

void cmdGetFormat(uint8_t argc, char** argv)
{
	if(argc == 1)
	{
		formatChange();

		uint16_t data[6];
		uint16_t currentWidth, currentHeight, currentRatio;
		char* ratioName;

		fpgaConfigRead(AMBILIGHT_BASE_ADDR_FORMAT, data, sizeof(data));
		formatGetCurrent(&currentWidth, &currentHeight, &currentRatio);
		ratioName = formatGetRatioName(currentRatio);

		printf("raw: %d %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4], data[5]);
		printf("fmt: %dx%d %s (%d)\n", currentWidth, currentHeight, 
		       ratioName ? ratioName : "unknown", currentRatio);
	}
}
