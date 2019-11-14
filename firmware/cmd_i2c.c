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

#include "config_hdmi.h"
#include "config_edid.h"



void cmdSetI2C(uint8_t argc, char** argv)
{
	if(argc == 4)
	{
		i2cWriteAdvRegister(getint(&argv[1]), getint(&argv[2]), getint(&argv[3]));
		printf("OK\n");
	}
}

void cmdGetI2C(uint8_t argc, char** argv)
{
	if(argc == 3)
	{
		int val = i2cReadAdvRegister(getint(&argv[1]), getint(&argv[2]));
		printf("Read: %d (0x%02x)\n", val, val);
	}
	else if(argc == 4)
	{
		uint8_t minBit, maxBit;
		int val = i2cReadAdvRegister(getint(&argv[1]), getint(&argv[2]));
		
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

void writeEdid(const char* edid, int length)
{
	int i;

	for(i = 0; i < length; ++i)
		i2cWriteAdvRegister(0x6c, i, edid[i]);
}

void writeConfig(const struct ConfigTable* table)
{
	const struct ConfigTable* p;

	for(p = table; p->address != 0; ++p)
	{
		if(!silent)
		{
			printf("%x %x %x : ", p->address, p->subaddress, p->data);
			uint8_t ack = i2cWriteAdvRegister(p->address, p->subaddress, p->data);
			printf("%s\n", ack ? "ACK" : "NACK");
		}
		else
			i2cWriteAdvRegister(p->address, p->subaddress, p->data);
	}
}

void cmdRstI2C(uint8_t argc, char** argv)
{
	writeConfig(g_configTablePreEdid);
	writeEdid(g_edid, sizeof(g_edid));
	writeConfig(g_configTablePostEdid);
}
