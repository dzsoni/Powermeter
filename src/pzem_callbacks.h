#ifndef PZEM_CALLBACKS_H
#define PZEM_CALLBACKS_H

#include <Arduino.h>
#include <MycilaPZEM.h>
#include <vector>

extern Mycila::PZEM* pzem1;
extern Mycila::PZEM* pzem2;
extern Mycila::PZEM* pzem3;



pzem1->setCallback([](const Mycila::PZEM::EventType eventType, const Mycila::PZEM::Data& data) {
  
  if(eventType == Mycila::PZEM::EventType::EVT_READ_ERROR) {
    Serial.println("L1 READ_ERROR");
    return;
  }
  if(eventType == Mycila::PZEM::EventType::EVT_READ_TIMEOUT){
    Serial.println("L1 READ_TIMEOUT");
    return;
  }
  if(eventType == Mycila::PZEM::EventType::EVT_READ){
    Serial.printf("L1 EVT_READ (SUCCESS): %f V, %f A, %f W\n", data.voltage, data.current, data.activePower);
    Serial.flush();
    return;
  }
  Serial.printf(" - %" PRIu32 "L1 EVT_UNKNOWN: %f V, %f A, %f W\n", millis(), data.voltage, data.current, data.activePower);  
  });

pzem2->setCallback([](const Mycila::PZEM::EventType eventType, const Mycila::PZEM::Data& data) {
    
    if(eventType == Mycila::PZEM::EventType::EVT_READ_ERROR) {
      Serial.println("L2 READ_ERROR");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ_TIMEOUT){
      Serial.println("L2 READ_TIMEOUT");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ){
      Serial.printf("L2 EVT_READ (SUCCESS): %f V, %f A, %f W\n", data.voltage, data.current, data.activePower);
      Serial.flush();
      return;
    }
    
    Serial.printf(" - %" PRIu32 "L2 EVT_UNKNOWN: %f V, %f A, %f W\n", millis(), data.voltage, data.current, data.activePower);
    
  });
pzem3->setCallback([](const Mycila::PZEM::EventType eventType, const Mycila::PZEM::Data& data) {
    
    if(eventType == Mycila::PZEM::EventType::EVT_READ_ERROR) {
      Serial.println("L3 READ_ERROR");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ_TIMEOUT){
      Serial.println("L3 READ_TIMEOUT");
      Serial.flush();
      return;
    }
    if(eventType == Mycila::PZEM::EventType::EVT_READ){
      Serial.printf("L3 EVT_READ (SUCCESS): %f V, %f A, %f W\n", data.voltage, data.current, data.activePower);
      Serial.flush();
      return;
    }
  
    Serial.printf(" - %" PRIu32 "L3 EVT_UNKNOWN: %f V, %f A, %f W\n", millis(), data.voltage, data.current, data.activePower);
    Serial.flush();
  });


#endif /* PZEM_CALLBACKS_H */
