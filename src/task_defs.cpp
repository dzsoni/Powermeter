#include "task_defs.h"

extern struct_hardwares sh;
extern AsyncMqttClient mqttClient;


void t1Cb_secTimer()
{
  digitalWrite(FLASH_LED, !digitalRead(FLASH_LED));
}

void t2Cb_writeMQTT()
{
  if (WiFi.isConnected())
  {
    SimpleJsonParser jsp;

     //mqtt publish
    if (sh.connected())
    {
      Serial.println(F("Try publishing to MQTT server."));
      
    }
  }
}
