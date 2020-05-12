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


void cmdGetResult(uint8_t argc, char** argv)
{
	if(argc == 2 || argc == 3)
	{
		uint8_t index, minIndex, maxIndex, repeat;

		if(argc == 3)
			repeat = getint(&argv[2]);
		else
			repeat = 1;

		getrange(argv[1], &minIndex, &maxIndex);

		while(repeat--)
		{
			index = minIndex;
			do
			{
				uint8_t i;
				uint8_t data;

				fpgaConfigWrite(AMBILIGHT_BASE_ADDR_RESULT, &index, sizeof(index));

				do
				{
					fpgaConfigRead(AMBILIGHT_BASE_ADDR_STATUS, &data, sizeof(data));
				} while((data & 1) == 0);

				printf("%d: ", index);
				for(i = 0; i < 12; ++i)
				{
					fpgaConfigRead(AMBILIGHT_BASE_ADDR_RESULT + 4 + i, &data, sizeof(data));
					printf("%d ", data);
				}
				printf(" \n");

			} while(index++ < maxIndex);
		}
	}
	//else
		//printf("err: GR index\n");
}
