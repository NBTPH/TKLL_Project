#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include <semphr.h>
// ===== One source of truth for pins =====
#define LED1_PIN 6
#define LED2_PIN 7
#define BOOT_PIN 0
#define NEO_NUM 1
#define NEO_PIN 45
#define LED_PIN 48
// ===== MQTT topics =====
#define MQTT_CMD_TOPIC     "esp32/lab5/cmd"
#define MQTT_STATUS_TOPIC  "esp32/lab5/status"
#define MQTT_SENSORS_TOPIC "esp32/lab5/sensors"
#define MQTT_WEATHER_TOPIC "esp32/lab5/weather"
// ===== Sleep =====
#define SLEEP_DURATION_SECONDS  10
#define SLEEP_DURATION_US SLEEP_DURATION_SECONDS*1000000

//global struct and semaphore for sensor value
typedef struct {
    float temp;
    float humidity;
} DHT20_data;
extern DHT20_data dht20;
extern SemaphoreHandle_t DHT20_Mutex;

#endif
