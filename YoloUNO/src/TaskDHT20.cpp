//Task to communicate and read from DHT20 sensor   

#include "TaskDHT20.h"
#include "global.h"
static QueueHandle_t tQueue = NULL;
SemaphoreHandle_t DHT20_Mutex = NULL;
DHT20_data dht20;

void enable_I2C(void){
    Wire.setPins(GPIO_NUM_11, GPIO_NUM_12);
    Wire.begin();
}

void TaskDHT20(void *pvParameter){
    DHT20 DHT20;
    memset(&dht20, 0, sizeof(dht20));

    while(!DHT20.begin()){
        Serial.println("Trying to connect");
        vTaskDelay(1000);
    }

    while (true){
        DHT20.read();
        float temp = DHT20.getTemperature();
        float humi = DHT20.getHumidity();

        if(xSemaphoreTake(DHT20_Mutex, 0)){
            dht20.temp = temp;
            dht20.humidity = humi;
            xSemaphoreGive(DHT20_Mutex);
        }
        vTaskDelay(1000);
    }
}