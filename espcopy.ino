// Blynk authentication details
#define BLYNK_TEMPLATE_ID "IMPL6w817Fd5m"
#define BLYNK_TEMPLATE_NAME "nodemcu"
#define BLYNK_AUTH_TOKEN "f9Q1B40pu9X3zFdtHA2V722GsQTy3fvG"

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
int switchState[] = {1, 1, 1, 1}; // HIGH means no press (INPUT_PULLUP)
int lastSwitchState[] = {1, 1, 1, 1};
int relayState[] = {0, 0, 0, 0}; // Relay OFF initially

// Virtual pins for Blynk
const int VPIN_START = V1; // Start virtual pins from V1

// Timer for periodic tasks
BlynkTimer timer;

// Variables to track debouncing
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
unsigned long debounceDelay = 50;  // 50ms debounce delay

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

  // Attempt to connect to WiFi
  WiFi.begin(ssid, pass);
  timer.setInterval(1000L, checkWiFi); // Check WiFi connection every second

  Serial.println("Setup complete!");
}

// Sync relay states when Blynk reconnects
BLYNK_CONNECTED() {
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

// Check physical switch states with non-blocking debounce
void checkPhysicalSwitches() {
  for (int i = 0; i < 4; i++) {
    int currentState = digitalRead(SWITCH_PINS[i]);

    // Check if enough time has passed to debounce
    if (currentState != lastSwitchState[i] && (millis() - lastDebounceTime[i]) > debounceDelay) {
      lastDebounceTime[i] = millis(); // Update debounce timer

      if (currentState == LOW) { // Switch pressed
        relayState[i] = !relayState[i]; // Toggle relay state
        digitalWrite(RELAY_PINS[i], relayState[i]); // Update relay
        Serial.print("Relay "); Serial.print(i + 1);
        Serial.println(relayState[i] == HIGH ? " ON" : " OFF");
      }

      lastSwitchState[i] = currentState; // Update last state
    }
  }
}

// Functionality when WiFi is connected
void withInternet() {
  Blynk.run();  // Handle Blynk connection and communication
}

// Functionality when WiFi is not connected
void withoutInternet() {
  checkPhysicalSwitches(); // Only check physical switches
}

// Check WiFi connection
void checkWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WIFI_LED, LOW); // Turn LED ON (WiFi connected)
    withInternet();
  } else {
    digitalWrite(WIFI_LED, HIGH); // Turn LED OFF (No WiFi)
    withoutInternet();
  }
}

void loop() {
  timer.run();  // Run periodic tasks
}
