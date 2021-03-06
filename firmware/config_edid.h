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

static const char g_edid[] =
{
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,   // address 0x00
	0x7F, 0xFF, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,   //
	0x00, 0x18, 0x01, 0x03, 0x80, 0x73, 0x41, 0x78,   // address 0x10
	0x2A, 0x7C, 0x11, 0x9E, 0x59, 0x47, 0x9B, 0x27,   //
	0x10, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x01,   // address 0x20
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   //
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A,   // address 0x30
	0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,   //
	0x45, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x1E,   // address 0x40
	0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10,   //
	0x10, 0x3E, 0x96, 0x00, 0x04, 0x03, 0x00, 0x00,   // address 0x50
	0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48,   //
	0x44, 0x4D, 0x49, 0x20, 0x4C, 0x69, 0x67, 0x68,   // address 0x60
	0x74, 0x20, 0x32, 0x0A, 0x00, 0x00, 0x00, 0xFD,   //
	0x00, 0x3B, 0x3D, 0x0F, 0x2E, 0x08, 0x00, 0x0A,   // address 0x70
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x12,   //
	0x02, 0x03, 0x19, 0x72, 0x46, 0x90, 0x04, 0x05,   // address 0x80
	0x01, 0x02, 0x03, 0x23, 0x09, 0x07, 0x07, 0x83,   //
	0x01, 0x00, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10,   // address 0x90
	0x00, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E,   //
	0x20, 0x6E, 0x28, 0x55, 0x00, 0x10, 0x09, 0x00,   // address 0xA0
	0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71,   //
	0x1C, 0x16, 0x20, 0x58, 0x2C, 0x25, 0x00, 0x10,   // address 0xB0
	0x09, 0x00, 0x00, 0x00, 0x9E, 0xD6, 0x09, 0x80,   //
	0xA0, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x60, 0xA2,   // address 0xC0
	0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x18, 0x8C,   //
	0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10,   // address 0xD0
	0x3E, 0x96, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00,   //
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // address 0xE0
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // address 0xF0
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0,   //
};

