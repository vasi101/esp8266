// Blynk authentication details
#define BLYNK_TEMPLATE_ID "TMPL62IsoMLHl"
#define BLYNK_TEMPLATE_NAME "nodmcu"
#define BLYNK_AUTH_TOKEN "rIXhhO8I2liZrCkGe5-qdOK_8n9lsYNh"

// Include required libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "WiFi";
char pass[] = "12345678";

// Define pins for relays and switches
const int RELAY_PINS[] = {D1, D2, D3, D4};  // Relay pins
const int SWITCH_PINS[] = {D5, D6, D7, D8}; // Physical switch pins
const int WIFI_LED = LED_BUILTIN;           // Built-in LED for WiFi status

// Variables to store switch and relay states
int switchState[] = {1, 1, 1, 1};  // HIGH means no press (INPUT_PULLUP)
int lastSwitchState[] = {1, 1, 1, 1};
int relayState[] = {0, 0, 0, 0};   // Relay OFF initially

// Virtual pins for Blynk
const int VPIN_START = V1; // Start virtual pins from V1

// Timer for periodic tasks
BlynkTimer timer;

// Variables to track debouncing
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 50;  // 50ms debounce delay

void setup() {
  Serial.begin(115200);

  // Initialize relay pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], LOW); // Start with relays OFF
  }

  // Initialize switch pins as inputs with pullup resistors
  for (int i = 0; i < 4; i++) {
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);
  }

  // Initialize WiFi LED
  pinMode(WIFI_LED, OUTPUT);
  digitalWrite(WIFI_LED, HIGH); // Turn LED OFF initially (active LOW)

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN);
  timer.setInterval(1000L, checkWiFi);      // Check WiFi connection every second
  timer.setInterval(100L, checkSwitches);  // Check physical switches every 100ms

  Serial.println("Setup complete!");
}

// Sync relay states when Blynk reconnects
BLYNK_CONNECTED() {
  Serial.println("Blynk connected!");
  for (int i = 0; i < 4; i++) {
    Blynk.virtualWrite(VPIN_START + i, relayState[i]); // Sync all relays
  }
}

// Handle Blynk button presses
BLYNK_WRITE(V1) { relayControl(0, param.asInt()); }
BLYNK_WRITE(V2) { relayControl(1, param.asInt()); }
BLYNK_WRITE(V3) { relayControl(2, param.asInt()); }
BLYNK_WRITE(V4) { relayControl(3, param.asInt()); }

// Control relay and sync with Blynk
void relayControl(int relayIndex, int state) {
  relayState[relayIndex] = state;                   // Update relay state
  digitalWrite(RELAY_PINS[relayIndex], state);      // Update relay output
  Blynk.virtualWrite(VPIN_START + relayIndex, state); // Sync Blynk
  Serial.print("Relay "); Serial.print(relayIndex + 1);
  Serial.println(state == HIGH ? " ON" : " OFF");
}

// Check physical switch states with debouncing
void checkSwitches() {
  for (int i = 0; i < 4; i++) {
    int currentState = digitalRead(SWITCH_PINS[i]);

    // Debounce logic
    if (currentState != lastSwitchState[i]) {
      lastDebounceTime[i] = millis(); // Update debounce timer
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (currentState != switchState[i]) {
        switchState[i] = currentState;

        if (currentState == LOW) { // Switch pressed
          relayState[i] = !relayState[i]; // Toggle relay state
          digitalWrite(RELAY_PINS[i], relayState[i]); // Update relay
          Blynk.virtualWrite(VPIN_START + i, relayState[i]); // Sync Blynk
          Serial.print("Relay "); Serial.print(i + 1);
          Serial.println(relayState[i] == HIGH ? " ON" : " OFF");
        }
      }
    }

    lastSwitchState[i] = currentState; // Update last state
  }
}

// Check WiFi connection
void checkWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WIFI_LED, LOW); // Turn LED ON (WiFi connected)
    if (!Blynk.connected()) {
      Blynk.connect(); // Attempt to reconnect Blynk
    }
    Blynk.run(); // Handle Blynk tasks
  } else {
    digitalWrite(WIFI_LED, HIGH); // Turn LED OFF (No WiFi)
  }
}

void loop() {
  timer.run();  // Run periodic tasks
}
