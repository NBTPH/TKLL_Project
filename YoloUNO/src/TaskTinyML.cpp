#include "TaskTinyML.h"

// // Globals, for the convenience of one-shot setup.
// namespace
// {
//     tflite::ErrorReporter *error_reporter = nullptr;
//     const tflite::Model *model = nullptr;
//     tflite::MicroInterpreter *interpreter = nullptr;
//     TfLiteTensor *input = nullptr;
//     TfLiteTensor *output = nullptr;
//     constexpr int kTensorArenaSize = 8 * 1024; // Adjust size based on your model
//     uint8_t tensor_arena[kTensorArenaSize];
// } // namespace
tflite::ErrorReporter *error_reporter = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;
SemaphoreHandle_t serialMutex;

constexpr int kTensorArenaSize = 10 * 1024;  // Adjust to your model size
uint8_t tensor_arena[kTensorArenaSize];

void setupTinyML(){
    Serial.println("TensorFlow Lite Init....");
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    const tflite::Model *model = tflite::GetModel(model_arr); // g_model_data is from model_data.h
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report("Model provided is schema version %d, not equal to supported version %d.",
                               model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        error_reporter->Report("AllocateTensors() failed");
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    Serial.println("TensorFlow Lite Micro initialized on ESP32.");
    serialMutex = xSemaphoreCreateMutex(); //use mutex for printing so that other tasks don't interrupt it before it's finished printing
}

void TaskTinyML(void *pvParameters)
{

    setupTinyML();
    DHT20_data dht20_LOCAL;
    memset(&dht20_LOCAL, 0, sizeof(dht20_LOCAL));
    while (1)
    {
        DHT20_data dht20_LOCAL;
        if(xSemaphoreTake(DHT20_Mutex, 1)){
            dht20_LOCAL = dht20;
            xSemaphoreGive(DHT20_Mutex);
        }
        int input_temp = static_cast<int>(round(dht20_LOCAL.temp));
        int input_humi = static_cast<int>(round(dht20_LOCAL.humidity));

        String print_buffer;
        if (xSemaphoreTake(serialMutex, 0) == pdTRUE){
            print_buffer = "[TinyML] Input: " + String(input_temp) + "C " +  String(input_humi) + "%\n";
            Serial.print(print_buffer);
            Serial.flush();
            xSemaphoreGive(serialMutex);
        }

        input->data.f[0] = input_temp;
        input->data.f[1] = input_humi;

        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk){
            error_reporter->Report("Invoke failed");
            return;
        }

        float result = output->data.f[0];

        if (xSemaphoreTake(serialMutex, 0) == pdTRUE){
            print_buffer = "Inference result, how likely the data is abnormal: " + String(result) + "\n\n";
            Serial.print(print_buffer);
            Serial.flush();
            xSemaphoreGive(serialMutex);
        }

        vTaskDelay(1000);
    }
}