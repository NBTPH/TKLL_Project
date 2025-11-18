#ifndef DHT20_H
#define DHT20_H

#include "global.h"  
#include "Wire.h"
#include "DHT20.h"

void enable_I2C(void);

void TaskDHT20(void *pvParameter);

#endif