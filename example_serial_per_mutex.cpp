// Példa a serialonkénti mutex blokkolás működésére
// Ez a kód bemutatja, hogyan működik a párhuzamosság különböző Serial portok között

#include "lib/MycilaPZEM/src/MycilaPZEM.h"
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Két különböző PZEM eszköz különböző Serial portokon
Mycila::PZEM pzem1;  // Serial1 (GPIO 16, 17)
Mycila::PZEM pzem2;  // Serial2 (GPIO 18, 19)
Mycila::PZEM pzem3;  // Serial1 (GPIO 16, 17) - UGYANAZ mint pzem1
Mycila::PZEM pzem4;  // Serial2 (GPIO 18, 19) - UGYANAZ mint pzem2

void setup() {
    Serial.begin(115200);
    
    // Különböző Serial portok inicializálása
    pzem1.begin(Serial1, 16, 17, 0x01, false);  // Serial1, eszköz cím 0x01
    pzem2.begin(Serial2, 18, 19, 0x02, false);  // Serial2, eszköz cím 0x02
    pzem3.begin(Serial1, 16, 17, 0x03, false);  // Serial1, eszköz cím 0x03 (shared)
    pzem4.begin(Serial2, 18, 19, 0x04, false);  // Serial2, eszköz cím 0x04 (shared)
    
    // Megosztott Serial port jelzése
    pzem1.setSharedSerial(true);
    pzem3.setSharedSerial(true);
    pzem2.setSharedSerial(true);
    pzem4.setSharedSerial(true);
    
    Serial.println("=== Serialonkénti Mutex Teszt ===");
    Serial.println("pzem1 & pzem3: Serial1 (egymást blokkolják)");
    Serial.println("pzem2 & pzem4: Serial2 (egymást blokkolják)");
    Serial.println("pzem1/pzem3 VS pzem2/pzem4: PÁRHUZAMOS!");
}

// Task function for Serial1 devices
void serial1Task(void* parameter) {
    const char* taskName = (const char*)parameter;
    
    while (true) {
        unsigned long start = millis();
        
        // Ezek egymást blokkolják (ugyanaz a Serial1)
        bool success1 = pzem1.read();
        bool success3 = pzem3.read();
        
        unsigned long duration = millis() - start;
        
        Serial.printf("[%s] Serial1 read duration: %lu ms (pzem1: %s, pzem3: %s)\n", 
                     taskName, duration, 
                     success1 ? "OK" : "FAIL",
                     success3 ? "OK" : "FAIL");
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task function for Serial2 devices  
void serial2Task(void* parameter) {
    const char* taskName = (const char*)parameter;
    
    while (true) {
        unsigned long start = millis();
        
        // Ezek egymást blokkolják (ugyanaz a Serial2)
        bool success2 = pzem2.read();
        bool success4 = pzem4.read();
        
        unsigned long duration = millis() - start;
        
        Serial.printf("[%s] Serial2 read duration: %lu ms (pzem2: %s, pzem4: %s)\n", 
                     taskName, duration,
                     success2 ? "OK" : "FAIL",
                     success4 ? "OK" : "FAIL");
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void loop() {
    static bool tasksCreated = false;
    
    if (!tasksCreated) {
        // ✅ Ezek a task-ok PÁRHUZAMOSAN futhatnak!
        // Serial1Task és Serial2Task nem blokkolják egymást
        xTaskCreate(serial1Task, "Serial1Task", 4096, (void*)"TASK1", 2, NULL);
        xTaskCreate(serial2Task, "Serial2Task", 4096, (void*)"TASK2", 2, NULL);
        
        tasksCreated = true;
        Serial.println("\n=== Párhuzamos olvasás indítva ===");
        Serial.println("Figyeld meg: Serial1 és Serial2 task-ok párhuzamosan futnak!");
    }
    
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // Eredmények vizsgálata
    Serial.println("\n--- Aktuális mérési adatok ---");
    
    auto data1 = pzem1.getData();
    auto data2 = pzem2.getData();
    auto data3 = pzem3.getData();
    auto data4 = pzem4.getData();
    
    Serial.println("Serial1 eszközök (megosztva):");
    Serial.printf("  pzem1: %.2fV, %.3fA, %.1fW\n", 
                  data1.voltage, data1.current, data1.activePower);
    Serial.printf("  pzem3: %.2fV, %.3fA, %.1fW\n", 
                  data3.voltage, data3.current, data3.activePower);
    
    Serial.println("Serial2 eszközök (megosztva):");
    Serial.printf("  pzem2: %.2fV, %.3fA, %.1fW\n", 
                  data2.voltage, data2.current, data2.activePower);
    Serial.printf("  pzem4: %.2fV, %.3fA, %.1fW\n", 
                  data4.voltage, data4.current, data4.activePower);
}

/*
VÁRHATÓ EREDMÉNY:

=== RÉGI IMPLEMENTÁCIÓ (globális mutex) ===
[TASK1] Serial1 read duration: 800 ms (pzem1: OK, pzem3: OK)
[TASK2] Serial2 read duration: 800 ms (pzem2: OK, pzem4: OK)  <- VÁRAKOZOTT!

=== ÚJ IMPLEMENTÁCIÓ (serialonkénti mutex) ===
[TASK1] Serial1 read duration: 400 ms (pzem1: OK, pzem3: OK)
[TASK2] Serial2 read duration: 400 ms (pzem2: OK, pzem4: OK)  <- PÁRHUZAMOS!

A különbség: A Serial1 és Serial2 task-ok párhuzamosan futnak, 
míg az egy Serial-on belüli eszközök továbbra is egymás után.
*/