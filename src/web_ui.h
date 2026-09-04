#ifndef WEB_UI_H
#define WEB_UI_H

#include <ESPAsyncWebServer.h>
#include "config.h"
#include "payloads.h"

AsyncWebServer server(80);

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BadUSB Hub | Control Panel</title>
    <style>
        body { background-color: #0d0d0d; color: #00ff41; font-family: 'Courier New', Courier, monospace; text-align: center; margin-top: 50px; }
        .container { max-width: 650px; margin: auto; background: #141414; padding: 30px; border-radius: 0px; border: 1px solid #333; box-shadow: 0 0 15px rgba(0, 255, 65, 0.1); }
        h1 { margin-bottom: 5px; font-size: 26px; letter-spacing: 1px; text-transform: uppercase; }
        .subtitle { color: #555; font-size: 14px; margin-bottom: 35px; }
        h2 { color: #00ff41; font-size: 14px; margin-top: 30px; margin-bottom: 15px; text-transform: uppercase; letter-spacing: 2px; border-top: 1px dashed #333; padding-top: 15px; }
        .btn { background: #1a1a1a; color: #00ff41; border: 1px solid #333; padding: 12px 24px; margin: 5px; cursor: pointer; border-radius: 0px; font-size: 14px; transition: 0.2s; width: 180px; font-family: 'Courier New', Courier, monospace; }
        .btn:hover { background: #00ff41; color: #000; transform: scale(1.02); }
        .custom-input { width: 80%; background: #000; color: #00ff41; border: 1px solid #333; padding: 12px; font-family: 'Courier New', Courier, monospace; margin-bottom: 10px; }
        .status { margin-top: 30px; color: #555; font-weight: normal; font-size: 14px; min-height: 20px; text-transform: uppercase; }
        .status.active { color: #00ff41; }
    </style>
</head>
<body>
    <div class="container">
        <h1>ESP32-S3 BadUSB Hub</h1>
        <div class="subtitle">Hardware HID Injection Device</div>
        
        <h2>System Payloads</h2>
        <button class="btn" onclick="sendPayload('calc')">Run Calculator</button>
        <button class="btn" onclick="sendPayload('notepad')">Open Notepad</button>
        <button class="btn" onclick="sendPayload('cmd')">Open CMD</button>
        
        <h2>Custom Input</h2>
        <input type="text" id="customText" class="custom-input" placeholder="Type text to inject...">
        <br>
        <button class="btn" style="width: 80%; border-left: 3px solid #00ff41;" onclick="sendCustom()">Inject Custom Text</button>
        
        <div class="status" id="statusText">Status: Idle</div>
    </div>

    <script>
        function updateStatus(text, isActive) {
            let el = document.getElementById('statusText');
            el.innerText = "Status: " + text;
            el.className = "status" + (isActive ? " active" : "");
        }

        function sendPayload(name) {
            updateStatus("Executing " + name + "...", true);
            fetch('/trigger?payload=' + name)
                .then(response => response.text())
                .then(data => updateStatus(data, true))
                .catch(error => updateStatus("Connection Error", false));
        }

        function sendCustom() {
            let text = document.getElementById('customText').value;
            if(!text) {
                updateStatus("Input is empty", false);
                return;
            }
            updateStatus("Injecting custom text...", true);
            fetch('/trigger?payload=custom&text=' + encodeURIComponent(text))
                .then(response => response.text())
                .then(data => updateStatus(data, true))
                .catch(error => updateStatus("Connection Error", false));
        }
    </script>
</body>
</html>
)rawliteral";

void setupWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", INDEX_HTML);
    });

    server.on("/trigger", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("payload")) {
            String payloadName = request->getParam("payload")->value();
            
            if (payloadName == "custom" && request->hasParam("text")) {
                customTextBuffer = request->getParam("text")->value();
                executePayload("custom");
                request->send(200, "text/plain", "Custom text injected");
            } else {
                executePayload(payloadName);
                request->send(200, "text/plain", "Payload triggered: " + payloadName);
            }
        } else {
            request->send(400, "text/plain", "Bad Request: Missing payload");
        }
    });

    server.begin();
}

#endif