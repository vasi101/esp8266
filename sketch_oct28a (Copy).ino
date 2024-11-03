// Memory optimization settings
#define BLYNK_MAX_READBYTES 512
#define BLYNK_NO_SSL
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid = "";
const char* pass = "";

// Define relay pins and states
const uint8_t RelayPins[] = {D1, D2, D3, D4};
bool relayStates[] = {HIGH, HIGH, HIGH, HIGH};  // HIGH = OFF for active-low relays

// Create web server for manual control
ESP8266WebServer server(80);

// Timer for connection check
BlynkTimer timer;
bool isConnected = false;

// Function declarations
void handleRoot();
void handleRelay();
void checkConnection();

// Update relay state with error checking
ICACHE_FLASH_ATTR void updateRelay(uint8_t index, bool state) {
  if (index < 4) {
    relayStates[index] = state;
    digitalWrite(RelayPins[index], state);
    
    // Update Blynk if connected
    if (Blynk.connected()) {
      Blynk.virtualWrite(V0 + index, state);
    }
  }
}

// Blynk virtual pin handlers
BLYNK_WRITE(V0) { updateRelay(0, param.asInt()); }
BLYNK_WRITE(V1) { updateRelay(1, param.asInt()); }
BLYNK_WRITE(V2) { updateRelay(2, param.asInt()); }
BLYNK_WRITE(V3) { updateRelay(3, param.asInt()); }

// Reconnection handler
BLYNK_CONNECTED() {
  isConnected = true;
  for (uint8_t i = 0; i < 4; i++) {
    Blynk.virtualWrite(V0 + i, relayStates[i]);
  }
}

// Setup web server pages
ICACHE_FLASH_ATTR void setupWebPages() {
  // Root page
  server.on("/", handleRoot);
  
  // Handle relay controls
  server.on("/relay", handleRelay);
  
  server.begin();
}

// Generate control page
ICACHE_FLASH_ATTR void handleRoot() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Arial;text-align:center;margin-top:50px;}";
  html += "button{background-color:#4CAF50;color:white;padding:15px 32px;border:none;border-radius:4px;margin:4px;}</style></head>";
  html += "<body><h1>ESP8266 Relay Control</h1>";
  
  for (uint8_t i = 0; i < 4; i++) {
    html += "<p>Relay " + String(i + 1) + ": ";
    html += "<button onclick='location.href=\"/relay?id=" + String(i) + "&state=" + String(!relayStates[i]) + "\"'>";
    html += (relayStates[i] == HIGH ? "OFF" : "ON");
    html += "</button></p>";
  }
  
  html += "<p>WiFi: " + String(WiFi.RSSI()) + "dBm</p>";
  html += "<p>Blynk: " + String(isConnected ? "Connected" : "Disconnected") + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Handle relay control requests
ICACHE_FLASH_ATTR void handleRelay() {
  if (server.hasArg("id") && server.hasArg("state")) {
    uint8_t id = server.arg("id").toInt();
    bool state = server.arg("state") == "1";
    updateRelay(id, state);
  }
  handleRoot();
}

// Check connection status
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
  }
  if (!Blynk.connected()) {
    isConnected = false;
    Blynk.connect();
  }
}

void setup() {
  // Initialize relay pins
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(RelayPins[i], OUTPUT);
    digitalWrite(RelayPins[i], relayStates[i]);
  }
  
  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  // Wait up to 20 seconds for WiFi connection
  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    attempts++;
  }
  
  // Setup web server
  setupWebPages();
  
  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Setup connection check timer
  timer.setInterval(30000L, checkConnection);
}

void loop() {
  Blynk.run();
  timer.run();
  server.handleClient();
}
