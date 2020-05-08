/*

   Copyright (C) 2018 Stephen Robinson
  
   This file is part of vivepos
  
   vivepos is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.
  
   vivepos is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this code (see the file names COPING).  
   If not, see <http://www.gnu.org/licenses/>.
  
*/

#include <stdint.h>


//#define MICROSECONDS_IN_TICKS(x) ((uint16_t)((x) / 0.2))

//#define SWEEP_CENTER_IN_MICROSEC  4000
//#define SWEEP_180_IN_MICROSEC     8333
//#define SWEEP_CENTER_IN_TICKS     MICROSECONDS_IN_TICKS(SWEEP_CENTER_IN_MICROSEC)
//#define SWEEP_180_IN_TICKS        MICROSECONDS_IN_TICKS(SWEEP_180_IN_MICROSEC)

#define NUM_SENSORS          4

//#define PULSE_TYPE_SYNC      0x00
//#define PULSE_TYPE_SWEEP     0x10
//#define PULSE_TYPE_INVALID   0xf0
//#define PULSE_HAS_DATA(x)    (((x) & 0xF0) == 0)
//#define PULSE_DATA_AXIS(x)   ((x) & 1)
//#define PULSE_DATA_DATA(x)   (((x) >> 1) & 1)
//#define PULSE_DATA_SKIP(x)   (((x) >> 2) & 1)
//#define PULSE_DATA_INVALID   0xFF

//#define ANGLE_INVALID        0xFF

//#define SENSOR_TIMEOUT       1000

//#define SENSOR_STATE_IDLE    0
//#define SENSOR_STATE_RISEN   1
//#define SENSOR_STATE_FALLEN  2

#define NUM_LIGHTHOUSES      2
#define NUM_AXES             2
#define NUM_ANGLES           (NUM_LIGHTHOUSES * NUM_AXES)

#define OOTX_MAX_DATA        0x30

#define MASTER               0
#define SLAVE                1
#define AXIS0                0
#define AXIS1                1

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//typedef _Accum scalar_t;
typedef float scalar_t;

typedef scalar_t vec3_t[3];
typedef scalar_t vec4_t[4];
typedef scalar_t vec6_t[6];

typedef scalar_t mat3_3_t[9];
typedef scalar_t mat4_4_t[16];
typedef scalar_t mat4_6_t[24];
typedef scalar_t mat6_4_t[24];


typedef struct
{
	scalar_t err01, err02, err03, err12, err13, err23, total;

} solution_error_t;

typedef struct
{
	scalar_t rotationMatrix[9];
	vec3_t origin;
	solution_error_t error;

} lighthouse_t;

typedef struct
{
	lighthouse_t lighthouses[NUM_LIGHTHOUSES];
	scalar_t totalError;

} lighthouse_solution_t;

typedef struct
{
	uint16_t fw_version;
	uint32_t id;
	uint16_t fcal_0_phase;
	uint16_t fcal_1_phase;
	uint16_t fcal_0_tilt;
	uint16_t fcal_1_tilt;
	uint8_t  sys_unlock_count;
	uint8_t  hw_version;
	uint16_t fcal_0_curve;
	uint16_t fcal_1_curve;
	int8_t   accel_dir_x;
	int8_t   accel_dir_y;
	int8_t   accel_dir_z;
	uint16_t fcal_0_gibphase;
	uint16_t fcal_1_gibphase;
	uint16_t fcal_0_gibmag;
	uint16_t fcal_1_gibmag;
	uint8_t  mode_current;
	uint8_t  sys_faults;

} ootx_msg_v6_t;

typedef struct
{
	uint8_t  pos;
	uint16_t length;
	uint8_t  shiftCount;
	uint16_t shiftRegister;
	uint8_t  frameBit;
	uint8_t  validLength;
	uint8_t  data[OOTX_MAX_DATA];

} ootx_t;

typedef struct
{
	uint8_t isNew;
	uint8_t length;
	union
	{
		uint8_t data[OOTX_MAX_DATA];
		ootx_msg_v6_t msg;
	};

} ootx_msg_t;

typedef scalar_t sensor_angle_t[NUM_LIGHTHOUSES][NUM_AXES];

typedef struct
{
	volatile uint8_t  state;
	volatile uint16_t riseTime;
	volatile uint16_t fallTime;

	uint16_t angleTicks[NUM_LIGHTHOUSES][NUM_AXES];
	scalar_t angleRadians[NUM_LIGHTHOUSES][NUM_AXES];
	uint16_t startTime;
	uint16_t firstPulseTime;
	uint8_t  currentAngle;

	ootx_t ootx[NUM_LIGHTHOUSES];

} sensor_t;

typedef sensor_angle_t sensor_angle_set_t[NUM_SENSORS];

void solveLighthouse(lighthouse_solution_t* solution, sensor_angle_set_t sensorAngles, ootx_msg_t* ootx);

uint8_t calcPosition(lighthouse_t* lighthouses,
                     scalar_t angle1, scalar_t angle2, 
                     scalar_t angle3, scalar_t angle4,
                     vec3_t result, scalar_t* distance);

void calcSolutionErrorFromSensorAngles(lighthouse_solution_t* solution, 
                                       sensor_angle_set_t sensorAngles, 
                                       solution_error_t* error);
