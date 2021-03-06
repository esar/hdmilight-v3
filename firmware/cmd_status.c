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


void cmdGetStatus(uint8_t argc, char** argv)
{
	uint8_t status;
	fpgaConfigRead(AMBILIGHT_BASE_ADDR_STATUS, &status, sizeof(status));
	printf("status: %d\n", status);
	fpgaConfigRead(AMBILIGHT_BASE_ADDR_INTERRUPT, &status, sizeof(status));
	printf("interrupts: %02x\n", status);
}

void cmdSetStatus(uint8_t argc, char** argv)
{
	if(argc == 2)
	{
		uint16_t mask = getint(&argv[1]);
		fpgaConfigWrite(AMBILIGHT_BASE_ADDR_INTERRUPT, &mask, sizeof(mask));
		printf("ok\n");
	}
}

