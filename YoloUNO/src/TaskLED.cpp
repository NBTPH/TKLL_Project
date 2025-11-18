#include "TaskLED.h"
#include "global.h"

float map(float x, float in_min, float in_max, float out_min, float out_max){
    if(x < in_min) return out_min;
    else if(x > in_max) return out_max;
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void TaskLED(void *pvParameter){
    pinMode(LED_PIN, OUTPUT);
    pinMode(NEO_PIN, OUTPUT);

    Adafruit_NeoPixel LED_Strip(NEO_NUM, NEO_PIN, NEO_GRB + NEO_KHZ800);
    LED_Strip.begin();
    LED_Strip.clear();
    LED_Strip.show();

    DHT20_data dht20_LOCAL;
    memset(&dht20_LOCAL, 0, sizeof(dht20_LOCAL));

    unsigned long last_tick = millis();
    float tick_count = 0;
    while(true){
        if(xSemaphoreTake(DHT20_Mutex, 1)){
            dht20_LOCAL = dht20;
            xSemaphoreGive(DHT20_Mutex);
        }

        tick_count = map(dht20_LOCAL.temp, 10, 40, 2000, 100);
        if(millis() - last_tick >= tick_count){
            uint8_t state = digitalRead(LED_PIN) == 0 ? 1 : 0;
            digitalWrite(LED_PIN, state);
            last_tick = millis();
        }

        //Low humidity means higher red and green (yellow) and less blue, high humidity means higher blue and less yellow
        uint8_t red = map(dht20_LOCAL.humidity, 0, 100, 255, 0);
        uint8_t green = red;
        uint8_t blue = map(dht20_LOCAL.humidity, 0, 100, 0, 255);

        uint32_t color = LED_Strip.Color(red, green, blue);
        LED_Strip.setPixelColor(0, color);
        LED_Strip.show();
    }
}