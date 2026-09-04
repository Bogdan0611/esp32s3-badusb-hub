#ifndef CONFIG_H
#define CONFIG_H

// Credențiale Wi-Fi pentru Access Point
const char* WIFI_SSID = "BadUSB-Hub-AP";
const char* WIFI_PASS = "12345678"; // Parolă simplă, e doar pentru demo

// Pin folosit pentru un "Kill Switch" fizic (opțional)
const int SAFETY_PIN = 4; 

// IP-ul static pentru Access Point
IPAddress AP_IP(192, 168, 4, 1);
IPAddress AP_GATEWAY(192, 168, 4, 1);
IPAddress AP_SUBNET(255, 255, 255, 0);

#endif