// Async mód példa serialonkénti mutex-szel
#include "lib/MycilaPZEM/src/MycilaPZEM.h"

// Négy PZEM eszköz két különböző Serial porton
Mycila::PZEM pzem1, pzem2, pzem3, pzem4;

void setup() {
    Serial.begin(115200);
    
    // ✅ ASYNC MÓD: A library automatikusan olvassa őket
    pzem1.begin(Serial1, 16, 17, 0x01, true);  // Serial1, async
    pzem2.begin(Serial2, 18, 19, 0x02, true);  // Serial2, async  
    pzem3.begin(Serial1, 16, 17, 0x03, true);  // Serial1, async (shared)
    pzem4.begin(Serial2, 18, 19, 0x04, true);  // Serial2, async (shared)
    
    // Megosztott Serial portok jelzése
    pzem1.setSharedSerial(true);
    pzem3.setSharedSerial(true);
    pzem2.setSharedSerial(true);
    pzem4.setSharedSerial(true);
    
    Serial.println("=== ASYNC Serialonkénti Mutex Teszt ===");
    Serial.println("Automatikus, háttérben futó adatolvasás!");
}

void loop() {
    // ✅ Nem kell read() hívás! Az async task automatikusan csinálja
    
    // Thread-safe adatlekérés
    auto data1 = pzem1.getData();  // Serial1 eszközök
    auto data2 = pzem2.getData();  // Serial2 eszközök  
    auto data3 = pzem3.getData();  // Serial1 eszközök
    auto data4 = pzem4.getData();  // Serial2 eszközök
    
    Serial.println("\n=== ASYNC Mérési Adatok ===");
    Serial.printf("Serial1 - pzem1: %.2fV, %.3fA, %.1fW (connected: %s)\n", 
                  data1.voltage, data1.current, data1.activePower,
                  pzem1.isConnected() ? "YES" : "NO");
    Serial.printf("Serial1 - pzem3: %.2fV, %.3fA, %.1fW (connected: %s)\n", 
                  data3.voltage, data3.current, data3.activePower,
                  pzem3.isConnected() ? "YES" : "NO");
    
    Serial.printf("Serial2 - pzem2: %.2fV, %.3fA, %.1fW (connected: %s)\n", 
                  data2.voltage, data2.current, data2.activePower,
                  pzem2.isConnected() ? "YES" : "NO");
    Serial.printf("Serial2 - pzem4: %.2fV, %.3fA, %.1fW (connected: %s)\n", 
                  data4.voltage, data4.current, data4.activePower,
                  pzem4.isConnected() ? "YES" : "NO");
    
    Serial.printf("Utolsó sikeres olvasás: pzem1=%lums, pzem2=%lums\n",
                  millis() - pzem1.getTime(), millis() - pzem2.getTime());
    
    delay(2000);
}

/*
ASYNC MÓD MŰKÖDÉSE SERIALONKÉNTI MUTEX-SZEL:

_pzemTask (FreeRTOS task):
    loop {
        pzem1.read() -> getSerialMutex(Serial1) -> Lock Serial1
        pzem2.read() -> getSerialMutex(Serial2) -> Lock Serial2 (PÁRHUZAMOS!)
        pzem3.read() -> getSerialMutex(Serial1) -> Wait Serial1
        pzem4.read() -> getSerialMutex(Serial2) -> Wait Serial2
        yield()
    }

TELJESÍTMÉNY:
- RÉGI: 4 * 200ms = 800ms (szekvenciális)
- ŰJ:  2 * 200ms = 400ms (párhuzamos Serial portok)
- 50% GYORSABB!
*/