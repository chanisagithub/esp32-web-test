#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "";
const char* password = "";

// Web server on port 80
WebServer server(80);

// JSON endpoint for dynamic data
void handleSystemInfo() {
    String json = "{";
    json += "\"cpuFreq\":" + String(ESP.getCpuFreqMHz()) + ",";
    json += "\"freeHeap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"uptime\":" + String(millis() / 1000) + ",";
    json += "\"wifiRSSI\":" + String(WiFi.RSSI()) + ",";
    json += "\"ipAddress\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"macAddress\":\"" + WiFi.macAddress() + "\"";
    json += "}";
    server.send(200, "application/json", json);
}

// Serve the dashboard page
void handleRoot() {
    String html = "<html><head><title>ESP32 Dashboard</title>";
    html += "<style>body { font-family: Arial; text-align: center; } ";
    html += "table { margin: auto; border-collapse: collapse; } ";
    html += "td, th { border: 1px solid #ddd; padding: 8px; } ";
    html += "th { background-color: #f4f4f4; } </style>";
    html += "<script>";
    html += "async function fetchData() {";
    html += "const response = await fetch('/system-info');";
    html += "const data = await response.json();";
    html += "document.getElementById('cpuFreq').innerText = data.cpuFreq + ' MHz';";
    html += "document.getElementById('freeHeap').innerText = data.freeHeap + ' bytes';";
    html += "document.getElementById('uptime').innerText = data.uptime + ' seconds';";
    html += "document.getElementById('wifiRSSI').innerText = data.wifiRSSI + ' dBm';";
    html += "document.getElementById('ipAddress').innerText = data.ipAddress;";
    html += "document.getElementById('macAddress').innerText = data.macAddress;";
    html += "}";
    html += "setInterval(fetchData, 500);"; // Fetch data every 1 second
    html += "window.onload = fetchData;";
    html += "</script></head><body>";
    html += "<h1>ESP32 System Dashboard</h1>";
    html += "<table>";
    html += "<tr><th>Parameter</th><th>Value</th></tr>";
    html += "<tr><td>CPU Frequency</td><td id='cpuFreq'>Loading...</td></tr>";
    html += "<tr><td>Free Heap Memory</td><td id='freeHeap'>Loading...</td></tr>";
    html += "<tr><td>Uptime</td><td id='uptime'>Loading...</td></tr>";
    html += "<tr><td>Wi-Fi Signal Strength</td><td id='wifiRSSI'>Loading...</td></tr>";
    html += "<tr><td>IP Address</td><td id='ipAddress'>Loading...</td></tr>";
    html += "<tr><td>MAC Address</td><td id='macAddress'>Loading...</td></tr>";
    html += "</table>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Connect to Wi-Fi
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Configure the web server
    server.on("/", handleRoot);
    server.on("/system-info", handleSystemInfo);

    // Start the server
    server.begin();
    Serial.println("Web server started!");
}

void loop() {
    server.handleClient();
}
