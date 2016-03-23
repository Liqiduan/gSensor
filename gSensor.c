#include "gSensor.h"

static SensorCtrolBlock SCBList[MAX_SENSOR_NUM];

int SensorRegister(uint8 type, uint16 id, uint8 len, void (*SensorReadP)(uint8 *), void (*SensorInitP)(void), void (*SensorSampleP)(void))
{
	int i;
	for(i=0;i<MAX_SENSOR_NUM;i++)
	{
		if( SCBList[i].state & STATE_INIT )
		{
			continue;
		}
		else
		{
			SCBList[i].type=type;
			SCBList[i].id=id;
			SCBList[i].len=len;
			SCBList[i].SensorRead=SensorReadP;
			SCBList[i].SensorInit=SensorInitP;
			SCBList[i].SensorSample=SensorSampleP;
			return 0;
		}
	}
	return -1;

}

uint32 SensorPoll(uint32 current_time, SensorDataPacket *p)
{
	int i;
	uint32 min=0xffffffff;

	for(i=0;i<MAX_SENSOR_NUM;i++)
	{
		if( ~(SCBList[i].state & STATE_INIT ))
		{
			continue;
		}

		if( SCBList[i].next_time<=current_time) 
		{
			if( (SCBList[i].state & STATE_SAMPLE) && (SCBList[i].sample_cost != 0) )
			{
				//Sample frist!
				SCBList[i].state &=~ STATE_SAMPLE;
				SCBList[i].next_time += SCBList[i].sample_cost;
				if(SCBList[i].SensorSample!=NULL)
				{	
					SCBList[i].SensorSample();
				}
				if( min > SCBList[i].next_time )
				{
					min= SCBList[i].next_time;
				}
			}
			else
			{
				//Read now!
				SCBList[i].state |= STATE_SAMPLE;
				SCBList[i].next_time += SCBList[i].period;
				if(SCBList[i].SensorRead!=NULL)
				{
					SCBList[i].SensorRead(p->data);
				}
				p->type=SCBList[i].type;
				p->id=SCBList[i].id;
				p->timestamp=current_time;
				p->len=SCBList[i].len;
				return 0; 
			}
		}
	}
	return min - current_time;
}

int SensorInit()
{
	int i;
	for(i=0; i<MAX_SENSOR_NUM; i++)
	{
		if(SCBList[i].state & STATE_INIT)
		{
			if(SCBList[i].SensorInit != NULL)
			{	
				SCBList[i].SensorInit();
			}
		}
	}
	return 0;
}

int gSensorInit()
{
	int i;
	for (i=0; i<MAX_SENSOR_NUM; i++)
	{
		SCBList[i].state=0;
		SCBList[i].len=0;
	}	
	return 0;
}

