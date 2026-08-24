#include <USB.h>
#include "payloads.h"

void setup() {
    // Pornim Serial pentru debugging
    Serial.begin(115200);
    // Previne blocarea codului dacă Serial Monitorul nu e deschis pe ESP32-S3
    Serial.setTxTimeoutMs(0); 
    delay(1000);
    
    Serial.println("[SETUP] Pornire BadUSB Hub...");

    // Inițializăm USB HID
    Serial.println("[SETUP] Inițializare USB HID...");
    USB.begin();
    Keyboard.begin();
    Serial.println("[SETUP] USB HID initializat cu succes.");
    
    // --- PENTRU TESTARE HARDWARE (Commit 1) ---
    // Decomentează linia de mai jos ca să verifici că placa tastaria funcționează la 2 secunde după boot
    // executePayload("calc");
}

void loop() {
    // Sistem non-blocant pentru rularea payload-urilor
    if (isTyping) {
        if (currentPayload == "calc") payload_calc();
        else if (currentPayload == "notepad") payload_notepad();
        else if (currentPayload == "cmd") payload_cmd();
        
        isTyping = false;
        currentPayload = "";
    }
}