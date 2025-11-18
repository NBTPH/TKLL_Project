#ifndef ___MAIN_SERVER__
#define ___MAIN_SERVER__
#include "global.h"
#include <WiFi.h>
#include <WebServer.h>
#include "TaskDHT20.h" 

String mainPage();
String settingsPage();

void startAP();
void setupServer();
void connectToWiFi();

void TaskMainserver(void *pvParameters);

#endif