#ifndef PAYLOADS_H
#define PAYLOADS_H

#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>

USBHIDKeyboard Keyboard;

// Variabile de stare pentru execuția asincronă
bool isTyping = false;
String currentPayload = "";

// Helper: Simulează apăsarea Win+R (Run dialog pe Windows)
void pressWinR() {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
}

// Funcție custom pentru a scrie text fără să pierdem caractere
// ESP32-S3 e mult mai rapid decât buffer-ul OS-ului, deci punem 10ms între taste
void typeString(String text) {
    for (int i = 0; i < text.length(); i++) {
        Keyboard.write(text[i]);
        delay(10);
    }
}

// ==========================================
// PAYLOAD-URI DE BAZĂ
// ==========================================

void payload_calc() {
    pressWinR();
    delay(400);
    typeString("calc");
    delay(50);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
}

void payload_notepad() {
    pressWinR();
    delay(400);
    typeString("notepad");
    delay(50);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    delay(1500);
    typeString("Hardware level HID injection via USB hub - Demo successful.");
}

void payload_cmd() {
    pressWinR();
    delay(400);
    typeString("cmd");
    delay(50);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
}

// Dispatcher
void executePayload(String name) {
    if (isTyping) return;
    currentPayload = name;
    isTyping = true;
}

#endif