#ifndef __gsensor_H__
#define __gsensor_H__

#define MAX_BUF_SIZE 4
#define MAX_SENSOR_NUM 10
#define NULL 0

#include "hal_types.h"

#define STATE_INIT 0X01
#define STATE_SAMPLE 0X02

typedef struct
{
	uint8 state;
	uint8 type;
	uint16 id;
	uint8 len;
	uint32 period;
	uint32 next_time;
	uint16 sample_cost;
	void (*SensorRead)(uint8 *);
	void (*SensorSample)(void);
	void (*SensorInit)(void);
}SensorCtrolBlock;
#define SCB SensorCtrolBlock

typedef struct
{
	uint8 type;
	uint16 id;
	uint32 timestamp;
	uint8 len;
	uint8 *data;
}SensorDataPacket;
#define SDP SensorDataPacket

int SensorRegister(uint8 type, uint16 id, uint8 len, void (*SensorReadP)(uint8 *), void (*SensorInitP)(void), void (*SensorSampleP)(void));

uint32 SensorPoll(uint32 current_time, SensorDataPacket *p);

int SensorInit();
int gSensorInit();

#endif
