#ifndef DHT20_H
#define DHT20_H

#include "global.h"  
#include "Wire.h"
#include "DHT20.h"  

extern DHT20_data dht20;
extern SemaphoreHandle_t DHT20_Mutex;

void TaskDHT20(void *pvParameter);

#endif