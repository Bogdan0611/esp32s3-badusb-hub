#include <WiFi.h>
#include <USB.h>
#include "config.h"
#include "payloads.h"
#include "web_ui.h"

void setup() {
    Serial.begin(115200);
    Serial.setTxTimeoutMs(0); 
    delay(1000);
    
    Serial.println("[SETUP] Pornire BadUSB Hub...");

    pinMode(SAFETY_PIN, INPUT_PULLUP);

    Serial.println("[SETUP] Inițializare USB HID...");
    USB.begin();
    Keyboard.begin();
    Serial.println("[SETUP] USB HID initializat cu succes.");

    Serial.println("[SETUP] Pornire WiFi Access Point...");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);
    
    if(WiFi.softAP(WIFI_SSID, WIFI_PASS)) {
        Serial.println("[SETUP] WiFi AP pornit cu succes!");
        Serial.print("[SETUP] IP AP: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("[EROARE] WiFi AP a esuat sa porneasca!");
    }

    Serial.println("[SETUP] Pornire Web Server...");
    setupWebServer();
    Serial.println("[SETUP] Web Server pornit. Totul e gata!");
}

void loop() {
    if (isTyping) {
        if (currentPayload == "calc") payload_calc();
        else if (currentPayload == "notepad") payload_notepad();
        else if (currentPayload == "cmd") payload_cmd();
        else if (currentPayload == "custom") payload_custom();
        
        isTyping = false;
        currentPayload = "";
        customTextBuffer = ""; // Curățăm bufferul după ce am tastat
    }
}