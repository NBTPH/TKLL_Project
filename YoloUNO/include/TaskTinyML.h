#ifndef TINYML_H
#define TINYML_H

#include <TensorFlowLite_ESP32.h>
#include "global.h"
#include "../../TinyML/TinyML.h"

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

void setupTinyML();
void TaskTinyML(void *pvParameters);

#endif