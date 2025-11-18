#include "TaskLightSleep.h"

extern "C"{
    #include "esp_pm.h"
}

void setupAutoLightSleep(){
    Serial.println("Configuring Auto Light-Sleep (Tickless Idle)...");
    esp_pm_config_esp32s3_t pm_config = {
      .max_freq_mhz = 240, 
      .min_freq_mhz = 80,   
      .light_sleep_enable = true
    };

    esp_err_t ret = esp_pm_configure(&pm_config);
  
    if (ret != ESP_OK) {
        Serial.println("Failed to configure PM!");
    } else {
        Serial.println("Auto Light-Sleep configured!");
        Serial.println("System will now sleep automatically when idle.");
    }
}