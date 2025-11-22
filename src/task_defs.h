#ifndef TASK_DEFS_H
#define TASK_DEFS_H


#include <WString.h>
#include <Arduino.h>
#include <struct_hardwares.h>
#include <Wire.h>
#include <AsyncMqttClient.h>
#include <board_def.h>
#include <TaskSchedulerDeclarations.h>
#include <wifiTool.h>

// Callback methods prototypes
void t1Cb_secTimer();
void t2Cb_writeMQTT();
#endif /* TASK_DEFS_H */
