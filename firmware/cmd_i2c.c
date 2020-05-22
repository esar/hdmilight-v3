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


void cmdSetI2C(uint8_t argc, char** argv)
{
	if(argc == 4)
	{
		adv7611WriteRegister(getint(&argv[1]), getint(&argv[2]), getint(&argv[3]));
		printf("OK\n");
	}
}

void cmdGetI2C(uint8_t argc, char** argv)
{
	if(argc == 3)
	{
		int val = adv7611ReadRegister(getint(&argv[1]), getint(&argv[2]));
		printf("Read: %d (0x%02x)\n", val, val);
	}
	else if(argc == 4)
	{
		uint8_t minBit, maxBit;
		int val = adv7611ReadRegister(getint(&argv[1]), getint(&argv[2]));
		
		getrange(argv[3], &minBit, &maxBit);
		if(maxBit < minBit)
		{
			uint8_t tmp = maxBit;
			maxBit = minBit;
			minBit = tmp;
		}

		val >>= minBit;
		val &= (1 << (maxBit - minBit + 1)) - 1;
		
		printf("Read: %d (0x%02x)\n", val, val);
	}
}

void cmdRstI2C(uint8_t argc, char** argv)
{
	adv7611Suspend();
	// TODO: delay before releasing reset
	adv7611Resume();
}
