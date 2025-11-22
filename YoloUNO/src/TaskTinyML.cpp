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
        // Prepare input data (e.g., sensor readings)
        // For a simple example, let's assume a single float input
        input->data.f[0] = dht20_LOCAL.temp;
        input->data.f[1] = dht20_LOCAL.humidity;

        // Run inference
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk)
        {
            error_reporter->Report("Invoke failed");
            return;
        }

        // Get and process output
        float result = output->data.f[0];
        String print_buffer;
        print_buffer = "Inference result, how likely the data is abnormal: " + String(result) + "\n";
        Serial.print(print_buffer);
        Serial.println();
        // Serial.println(result);

        vTaskDelay(1000);
    }
}