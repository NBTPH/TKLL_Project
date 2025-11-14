#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "freertos/FreeRTOS.h"
#include <semphr.h>
// ===== Shared sensor values  =====
typedef struct {
    float temp;
    float humidity;
} DHT20_data;
// ===== One source of truth for pins =====
#define LED1_PIN 2
#define LED2_PIN 3
#define BOOT_PIN 0
#define NEO_NUM 1
#define NEO_PIN 45
#define LED_PIN 48
// ===== MQTT topics =====
#define MQTT_CMD_TOPIC     "esp32/lab5/cmd"
#define MQTT_STATUS_TOPIC  "esp32/lab5/status"
#define MQTT_SENSORS_TOPIC "esp32/lab5/sensors"
#define MQTT_WEATHER_TOPIC "esp32/lab5/weather"

#endif
